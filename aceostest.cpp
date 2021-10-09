g++ -Wnon-virtual-dtor -fvisibility=hidden -fvisibility-inlines-hidden -O3 -ggdb -pthread -fno-strict-aliasing -Wall -Wextra -Wpointer-arith -pipe -D_GNU_SOURCE   -I/home/kevin/ACE_wrappers -D__ACE_INLINE__ -I../..  -c -o .obj/Test_URL_Addr.o /home/kevin/ACE_wrappers/websvcs/tests/Test_URL_Addr.cpp

g++ -Wnon-virtual-dtor -fvisibility=hidden -fvisibility-inlines-hidden -O3 -ggdb -pthread -fno-strict-aliasing -Wall -Wextra -Wpointer-arith -pipe -D_GNU_SOURCE   -I/home/kevin/ACE_wrappers -D__ACE_INLINE__ -I../..  -Wl,-E -L/home/kevin/ACE_wrappers/lib -L.  -L../../lib   -o Test_Url_Addr .obj/Test_URL_Addr.o -lwebsvcs -lACE -ldl -lrt



g++     aceostest.cpp    -o  aceostest.x -I/home/kevin/ACE_wrappers    -lACE -pthread  -L/home/kevin/ACE_wrappers/lib 





set path="C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.29.30133/bin/Hostx86/x86";C:/Windows/System32

set include="C:/Program Files (x86)/Windows Kits/10/Include/10.0.19041.0/um";"C:/Program Files (x86)/Windows Kits/10/Include/10.0.19041.0/ucrt"
set lib="C:/Program Files (x86)/Windows Kits/10/Lib/10.0.19041.0/ucrt/x86";"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.19041.0/um/x86"
cl aceostest.cpp /EHsc /DWIN32 /W4 /D_DEBUG /MDd -IC:/Users/kevin/Desktop/ACE_wrappers -I"C:/Program Files (x86)/Windows Kits/10/Include/10.0.19041.0/um" -I"C:/Program Files (x86)/Windows Kits/10/Include/10.0.19041.0/ucrt" -I"C:/Program Files (x86)/Windows Kits/10/Include/10.0.19041.0/shared" -I"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.29.30133/include" /link C:/Users/kevin/Desktop/ACE_wrappers/lib/ACEd.lib "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.29.30133/lib/x86/msvcprtd.lib" "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.29.30133/lib/x86/msvcrtd.lib" "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.29.30133/lib/x86/oldnames.lib" "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.29.30133/lib/x86/vcruntimed.lib" /OUT:aceostest.x
cmd





//#include "ace/OS.h"

#include "ace/OS_NS_sys_time.h"



#include <iostream>



//#pragma comment (lib,"ACE.lib")



//using namespace ACE_OS;



using namespace std;



int main(int argc, char** argv)

{

    ACE_Time_Value tv;

    tv = ACE_OS::gettimeofday();



    std::cout << "Hello World!! " << tv.sec() << " second has elapsed since EPOCH 1970 01 01 00:00:00" << std::endl;



    while (1)

    {

        if ((ACE_OS::gettimeofday() - tv).sec() == 5)

        {

            printf("5 seconds has been past\n");

            exit(0);

        }

    }

}

