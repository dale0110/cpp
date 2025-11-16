/**
功能：获取windows进程详细信息
VS2002编译，X64 以管理员方式运行

输出格式
PID=60660  PPID=23092  CHILDREN=<none>           START=2025-11-16 14:02:12  CMD="D:\software\SumatraPDFPortable\SumatraPDF-prerelease-11040-64.exe" "C:\Users\10069975.A25698996\Downloads\??????????1?+????4.7???????????????????5?????? ([?]?? ????Darin Olien? [Olien?, ?? ???... (Z-Library).epub"  DIR=C:\WINDOWS\system32\   
*/
#include <windows.h>
#include <winternl.h>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>      // For _setmode
#include <fcntl.h>   // For _O_U16TEXT
#include <vector>
#include <intrin.h>
#include <map>
#include <string>

#pragma comment(lib, "ntdll.lib")

//#define DEBUG 1

#ifdef DEBUG
// 使用 Windows API GetLocalTime 来实现日志宏，避免使用非标准的 gettimeofday
#define LOGPRINT(format, ...) \
    do { \
        SYSTEMTIME st; \
        GetLocalTime(&st); \
        wprintf(L"%04d-%02d-%02d %02d:%02d:%02d.%03d %S:%d:%S() " format L"\n", \
               st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, \
               __FILE__, __LINE__, __FUNCTIONW__, ##__VA_ARGS__); \
    } while (0)

#else
#define LOGPRINT(format, ...)
#endif

// 手动补上缺失的 NT 状态码
#ifndef STATUS_INFO_LENGTH_MISMATCH
#define STATUS_INFO_LENGTH_MISMATCH ((NTSTATUS)0xC0000004)
#endif

// 通用 UNICODE_STRING 定义
template<int ptrBits>
struct UNICODE_STRING_T {
    USHORT Length;
    USHORT MaximumLength;
    typename std::conditional<ptrBits == 64, PWSTR, ULONG>::type Buffer;
};

// 手动定义 SYSTEM_PROCESS_INFORMATION 结构体，以确保 InheritedFromUniqueProcessId 成员存在
// 避免因 SDK 版本过低导致编译失败
typedef struct _MY_SYSTEM_PROCESS_INFORMATION {
    ULONG NextEntryOffset;
    ULONG NumberOfThreads;
    LARGE_INTEGER WorkingSetPrivateSize;
    ULONG HardFaultCount;
    ULONG NumberOfThreadsHighWatermark;
    ULONGLONG CycleTime;
    LARGE_INTEGER CreateTime;
    LARGE_INTEGER UserTime;
    LARGE_INTEGER KernelTime;
    UNICODE_STRING ImageName;
    KPRIORITY BasePriority;
    HANDLE UniqueProcessId;
    HANDLE InheritedFromUniqueProcessId;
} MY_SYSTEM_PROCESS_INFORMATION;

// 32 位进程参数
struct RTL_USER_PROCESS_PARAMETERS32 {
    ULONG MaximumLength;
    ULONG Length;
    ULONG Flags;
    ULONG DebugFlags;
    ULONG ConsoleHandle;
    ULONG ConsoleFlags;
    ULONG StandardInput;
    ULONG StandardOutput;
    ULONG StandardError;
    UNICODE_STRING_T<32> CurrentDirectoryPath;
    ULONG CurrentDirectoryHandle;
    UNICODE_STRING_T<32> DllPath;
    UNICODE_STRING_T<32> ImagePathName;
    UNICODE_STRING_T<32> CommandLine;
};
struct PEB32 {
    BYTE Reserved1[2];
    BYTE BeingDebugged;
    BYTE Reserved2[1];
    ULONG Reserved3[2];
    ULONG Ldr;
    ULONG ProcessParameters;  // -> RTL_USER_PROCESS_PARAMETERS32*
    ULONG Reserved4[3];
    ULONG AtlThunkSListPtr;
    ULONG Reserved5;
    ULONG Reserved6;
    ULONG Reserved7;
    ULONG Reserved8[32];
    ULONG PostProcessInitRoutine;
    ULONG Reserved9[128];
    ULONG Reserved10[1];
    ULONG SessionId;
};

// 64 位进程参数
struct RTL_USER_PROCESS_PARAMETERS64 {
    ULONG MaximumLength;
    ULONG Length;
    ULONG Flags;
    ULONG DebugFlags;
    PVOID ConsoleHandle;
    ULONG ConsoleFlags;
    PVOID StandardInput;
    PVOID StandardOutput;
    PVOID StandardError;
    UNICODE_STRING_T<64> CurrentDirectoryPath;
    PVOID CurrentDirectoryHandle;
    UNICODE_STRING_T<64> DllPath;
    UNICODE_STRING_T<64> ImagePathName;
    UNICODE_STRING_T<64> CommandLine;
};
struct PEB64 {
    BYTE Reserved1[2];
    BYTE BeingDebugged;
    BYTE Reserved2[1];
    PVOID Reserved3[2];
    PVOID Ldr;
    PVOID ProcessParameters;  // -> RTL_USER_PROCESS_PARAMETERS64*
};

// 进程信息枚举
using PNtQuerySystemInformation = NTSTATUS(WINAPI*)(ULONG, PVOID, ULONG, PULONG);
using PNtQueryInformationProcess = NTSTATUS(NTAPI*)(
    HANDLE, PROCESSINFOCLASS, PVOID, ULONG, PULONG);

// 读取字符串辅助
bool ReadUnicodeString(HANDLE hProc, PVOID remoteBuf, USHORT len, WCHAR* out, SIZE_T maxWords) {
    if (!remoteBuf || !len) return false;
    // 确保读取的长度不会超过目标缓冲区的容量
    SIZE_T bytesToRead = min((SIZE_T)len, (maxWords - 1) * sizeof(WCHAR));
    if (bytesToRead == 0) return false;
    SIZE_T got = 0;
    if (!ReadProcessMemory(hProc, remoteBuf, out, bytesToRead, &got)) return false;
    // 在读取的数据末尾手动添加 NULL 终止符，确保字符串正确结束
    out[got / sizeof(WCHAR)] = L'\0'; 
    return true;
}

// 打印单个进程信息
// 新增：提升 SeDebugPrivilege
bool EnableDebugPrivilege() {
    HANDLE hTok;
    if (!OpenProcessToken(GetCurrentProcess(),
                          TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
                          &hTok)) return false;
    TOKEN_PRIVILEGES tp{};
    LUID luid;
    if (!LookupPrivilegeValueW(nullptr, SE_DEBUG_NAME, &luid)) {
        CloseHandle(hTok); return false;
    }
    tp.PrivilegeCount           = 1;
    tp.Privileges[0].Luid       = luid;
    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    // AdjustTokenPrivileges 成功时返回非零值。
    // 即使成功，GetLastError() 也可能返回 ERROR_NOT_ALL_ASSIGNED。
    BOOL success = AdjustTokenPrivileges(hTok, FALSE, &tp, sizeof(tp), nullptr, nullptr);
    CloseHandle(hTok);
    return success && (GetLastError() == ERROR_SUCCESS);
}

// 修改：打印失败信息，并只输出有内容的条目
void PrintProcessInfo(DWORD pid, DWORD ppid, const std::vector<DWORD>& childPids, HANDLE hProc, bool isWow64) {
    PROCESS_BASIC_INFORMATION pbi{};
    ULONG retLen = 0;
    auto NtQIP = (PNtQueryInformationProcess)GetProcAddress(
        GetModuleHandleA("ntdll"), "NtQueryInformationProcess");

    if (!NT_SUCCESS(NtQIP(hProc, ProcessBasicInformation, &pbi, sizeof(pbi), &retLen))) {
        wprintf(L"PID=%lu  ERR=NtQueryInformationProcess fail\n", pid);
        return;
    }

    WCHAR cmdLine[2048] = L"";
    WCHAR curDir[2048]  = L"";

    // 在64位系统上，如果isWow64为TRUE，说明目标进程是32位的。
    if (isWow64) {
        ULONG_PTR peb32_addr = 0;
        // 1. 获取 32 位 PEB 的地址
        if (!NT_SUCCESS(NtQIP(hProc, ProcessWow64Information, &peb32_addr, sizeof(peb32_addr), &retLen))) {
            // 对于某些进程（如System），即使有权限也无法获取Wow64信息，这很正常
            wprintf(L"PID=%lu  ERR=NtQueryInformationProcess for Wow64 info failed\n", pid);
            goto Print; // 即使失败，也尝试打印已知信息
        }

        // 2. 从获取到的地址读取 PEB 内容
        PEB32 peb32{};
        if (ReadProcessMemory(hProc, (PVOID)peb32_addr, &peb32, sizeof(peb32), nullptr)) {
            // 3. 读取进程参数块
            RTL_USER_PROCESS_PARAMETERS32 params32{};
            if (peb32.ProcessParameters && ReadProcessMemory(hProc, (PVOID)peb32.ProcessParameters, &params32, sizeof(params32), nullptr)) {
                ReadUnicodeString(hProc, (PVOID)params32.CommandLine.Buffer, params32.CommandLine.Length, cmdLine, _countof(cmdLine));
                ReadUnicodeString(hProc, (PVOID)params32.CurrentDirectoryPath.Buffer, params32.CurrentDirectoryPath.Length, curDir, _countof(curDir));
                //LOGPRINT(L"32bit process cmdLine=\"%s\"  curDir=\"%s\"", cmdLine, curDir);
            } else {
                // wprintf(L"PID=%lu  ERR=ReadProcessMemory for PEB32 params failed, GetLastError=%lu\n", pid, GetLastError());
            }
        }
    }
    // 如果isWow64为FALSE，说明目标进程和本进程一样，都是64位的。
    else {
        // 64位进程的PEB地址可以直接从pbi中获取，其内部指针也是64位的
        if (!pbi.PebBaseAddress) {
            // 对于某些系统核心进程，PebBaseAddress可能为NULL
            LOGPRINT("PebBaseAddress is NULL for PID %lu", pid);
            goto Print;
        }

        PEB64 peb64{};
        if (ReadProcessMemory(hProc, pbi.PebBaseAddress, &peb64, sizeof(peb64), nullptr)) {
            RTL_USER_PROCESS_PARAMETERS64 params64{};
            if (peb64.ProcessParameters && ReadProcessMemory(hProc, peb64.ProcessParameters, &params64, sizeof(params64), nullptr)) {
                ReadUnicodeString(hProc, params64.CommandLine.Buffer, params64.CommandLine.Length, cmdLine, _countof(cmdLine));
                ReadUnicodeString(hProc, params64.CurrentDirectoryPath.Buffer, params64.CurrentDirectoryPath.Length, curDir, _countof(curDir));
            } else {
                // wprintf(L"PID=%lu  ERR=ReadProcessMemory for PEB64 params failed, GetLastError=%lu\n", pid, GetLastError());
            }
        } else {
            DWORD err = GetLastError();
            // 错误998 (ERROR_NOACCESS) 经常发生在这里，如果一个64位程序试图用PEB64结构读取一个32位进程的PEB。
            // 这暗示了IsWow64Process的判断可能在某些情况下不符合预期，但目前的逻辑是标准的。
            wprintf(L"PID=%lu  ERR=ReadProcessMemory for PEB64 failed, GetLastError=%lu\n", pid, err);
        }
    }


Print:
    // 获取并格式化进程启动时间
    FILETIME ftCreation, ftExit, ftKernel, ftUser;
    SYSTEMTIME stLocal;
    WCHAR startTimeStr[128] = L"<unknown>";
    if (GetProcessTimes(hProc, &ftCreation, &ftExit, &ftKernel, &ftUser)) {
        FILETIME ftLocal;
        if (FileTimeToLocalFileTime(&ftCreation, &ftLocal)) {
            if (FileTimeToSystemTime(&ftLocal, &stLocal)) {
                swprintf_s(startTimeStr, _countof(startTimeStr),
                           L"%04d-%02d-%02d %02d:%02d:%02d",
                           stLocal.wYear, stLocal.wMonth, stLocal.wDay,
                           stLocal.wHour, stLocal.wMinute, stLocal.wSecond);
            }
        }
    }

    // 格式化子进程ID列表
    std::wstring childrenStr;
    if (!childPids.empty()) {
        for (size_t i = 0; i < childPids.size(); ++i) {
            childrenStr += std::to_wstring(childPids[i]);
            if (i < childPids.size() - 1) {
                childrenStr += L",";
            }
        }
    } else {
        childrenStr = L"<none>";
    }

    // 只打印有内容的行
    wprintf(L"PID=%-5lu  PPID=%-5lu  CHILDREN=%-15s  START=%s  CMD=%s  DIR=%s\n", pid, ppid, childrenStr.c_str(), startTimeStr, cmdLine[0] ? cmdLine : L"<none>", curDir[0] ? curDir : L"<none>");
}

int main() {
    // 设置控制台输出模式为 UTF-16，以正确显示 Unicode 字符
    _setmode(_fileno(stdout), _O_U16TEXT);

    if (!EnableDebugPrivilege()) {
        wprintf(L"Failed to enable SeDebugPrivilege. Please run as administrator.\n");
        // 即使没有权限也继续，但会看到很多 "Access Denied" 错误
    }

    auto NtQSI = (PNtQuerySystemInformation)GetProcAddress(
        GetModuleHandleA("ntdll"), "NtQuerySystemInformation");

    NTSTATUS status;
    ULONG    bufSize = 0x4000;          // 初始 16 KB
    std::vector<BYTE> buf;

    // 循环直到长度足够
    do {
        buf.resize(bufSize);
        status = NtQSI(SystemProcessInformation, buf.data(), bufSize, &bufSize);
        if (status == STATUS_INFO_LENGTH_MISMATCH)
            bufSize *= 2;               // 指数回退，继续扩容
        else
            break;
    } while (true);

    if (!NT_SUCCESS(status)) {
        wprintf(L"NtQuerySystemInformation fail 0x%X\n", status);
        return 1;
    }

    // 1. 构建父进程到子进程列表的映射
    std::map<DWORD, std::vector<DWORD>> parentToChildrenMap;
    for (auto* p = (MY_SYSTEM_PROCESS_INFORMATION*)buf.data();; ) {
        if (p->UniqueProcessId != 0) {
            DWORD currentPid = (DWORD)(ULONG_PTR)p->UniqueProcessId;
            DWORD parentPid = (DWORD)(ULONG_PTR)p->InheritedFromUniqueProcessId;
            parentToChildrenMap[parentPid].push_back(currentPid);
        }
        if (p->NextEntryOffset == 0) {
            break;
        }
        p = (MY_SYSTEM_PROCESS_INFORMATION*)((BYTE*)p + p->NextEntryOffset);
    }


    for (auto* proc = (MY_SYSTEM_PROCESS_INFORMATION*)buf.data();; ) {
        
        //LOGPRINT(L"proc:0x%p  NextEntryOffset:0x%x", proc,proc->NextEntryOffset);
        // 只有PID不为0的才是有意义的进程（PID 0 是 System Idle Process）
        // 尽管可以处理PID 0，但它没有命令行等信息，通常可以跳过。
        if (proc->UniqueProcessId != 0) {
            DWORD pid = (DWORD)(ULONG_PTR)proc->UniqueProcessId;
            DWORD ppid = (DWORD)(ULONG_PTR)proc->InheritedFromUniqueProcessId;
            HANDLE hProc = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION | PROCESS_VM_READ,
                                       FALSE, pid);
            if (!hProc) {
                // 对于受保护的进程（如System, smss.exe等），即使有Debug权限也无法打开，这是正常的。
                // 静默跳过即可，循环会在末尾自动移动到下一个进程。
            } else {
                BOOL isWow64 = FALSE;
                IsWow64Process(hProc, &isWow64);
                // 从映射中查找子进程列表
                const auto& children = parentToChildrenMap.find(pid);
                if (children != parentToChildrenMap.end()) {
                    PrintProcessInfo(pid, ppid, children->second, hProc, isWow64);
                } else {
                    PrintProcessInfo(pid, ppid, {}, hProc, isWow64); // 没有子进程
                }
                CloseHandle(hProc);
            }
        }

        // 如果NextEntryOffset为0，说明这是最后一个条目，处理完后必须退出循环
        if (proc->NextEntryOffset == 0) 
            break;
        
        // 移动到下一个进程信息结构体
        proc = (MY_SYSTEM_PROCESS_INFORMATION*)((BYTE*)proc + proc->NextEntryOffset);
    }
    //system("pause");
    return 0;
}
