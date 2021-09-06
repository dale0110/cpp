// debug_new.cpp
// compile by using: cl /EHsc /W4 /D_DEBUG /MDd debug_new.cpp

#include <cstdlib>
#include <crtdbg.h>

//需要设置宏ACE_NTRACE，默认状态是不记录TRACE信息的。
//#define ACE_NTRACE 0  //记录日志

#include "ace/Log_Msg.h"
#include "ace/svc_export.h"
#include "ace/DLL.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_time.h"

#include <fstream>  

#pragma comment (lib,"ACE.lib")



int main(int argc, char *argv[])  
{  
		//ACE::init ();
    ACE_OSTREAM_TYPE* output = new std::ofstream("acelog201.txt");  
    ACE_LOG_MSG->msg_ostream(output,1);  
    ACE_LOG_MSG->set_flags(ACE_Log_Msg::OSTREAM);  
    	
    #ACE_LOG_MSG->open(argv[0],ACE_Log_Msg::SYSLOG,ACE_TEXT("syslogTest"));
    	
    ACE_LOG_MSG->clr_flags(ACE_Log_Msg::STDERR);  
    ACE_DEBUG((LM_INFO,ACE_TEXT("%l Hello ACE Log Msg 1!\r\n"))); 
    //%N:文件名 %l行号 
    ACE_DEBUG((LM_INFO,ACE_TEXT("file:%N line:%l process:%P thread:%t time:%D Hello ACE Log Msg 2!\r\n")));  
    ACE_DEBUG((LM_TRACE,ACE_TEXT("%N %l   time:%T Hello ACE Log Msg 3!\r\n")));  
    ACE_LOG_MSG->clr_flags(ACE_Log_Msg::OSTREAM);  
    delete output;
    //ACE_DEBUG((LM_INFO,ACE_TEXT("test file %s line %d !\n",__FILE__,__LINE__))); 
    return 0;  
}
