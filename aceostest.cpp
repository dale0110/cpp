g++ -Wnon-virtual-dtor -fvisibility=hidden -fvisibility-inlines-hidden -O3 -ggdb -pthread -fno-strict-aliasing -Wall -Wextra -Wpointer-arith -pipe -D_GNU_SOURCE   -I/home/kevin/ACE_wrappers -D__ACE_INLINE__ -I../..  -c -o .obj/Test_URL_Addr.o /home/kevin/ACE_wrappers/websvcs/tests/Test_URL_Addr.cpp

g++ -Wnon-virtual-dtor -fvisibility=hidden -fvisibility-inlines-hidden -O3 -ggdb -pthread -fno-strict-aliasing -Wall -Wextra -Wpointer-arith -pipe -D_GNU_SOURCE   -I/home/kevin/ACE_wrappers -D__ACE_INLINE__ -I../..  -Wl,-E -L/home/kevin/ACE_wrappers/lib -L.  -L../../lib   -o Test_Url_Addr .obj/Test_URL_Addr.o -lwebsvcs -lACE -ldl -lrt



g++     aceostest.cpp    -o  aceostest.x -I/home/kevin/ACE_wrappers    -lACE -pthread  -L/home/kevin/ACE_wrappers/lib 





cl aceostest.cpp /EHsc /DWIN32 /W4 /D_DEBUG /MDd -ID:/tim/ume-tim-cpp/code/server/pal/server/Tim400/TimSubItems_Z102/src -ID:/compile_tools_ume/devtools/snacc_vc7/c++-lib/inc -ID:/compile_tools_ume/devtools/snacc_vc7/c++-lib/inc/additional -ID:/compile_tools_ume/devtools/snacc_vc7/include/c++ -ID:/compile_tools_ume/devtools/snacc_vc7/include/c++/additional -I. -I../ -I../include -I../export -ID:/compile_tools_ume/devtools/ace56_vc7 -ID:/compile_tools_ume/devtools/ace56_vc7/ACEXML/common -ID:/compile_tools_ume/devtools/ace56_vc7/ACEXML/parser/parser -ID:/compile_tools_ume/devtools/ace56_vc7/TAO -ID:/compile_tools_ume/devtools/ace56_vc7/TAO/tao -ID:/compile_tools_ume/devtools/ace56_vc7/TAO/tao/portableserver -ID:/compile_tools_ume/devtools/ace56_vc7/TAO/tao/strategies -ID:/compile_tools_ume/devtools/ace56_vc7/TAO/tao/valuetype -ID:/compile_tools_ume/devtools/ace56_vc7/TAO/tao/messaging -ID:/compile_tools_ume/devtools/ace56_vc7/TAO/orbsvcs -ID:/compile_tools_ume/devtools/ace56_vc7/TAO/orbsvcs/orbsvcs -ID:/compile_tools_ume/devtools/xerces/include -ID:/tim/ume-tim-cpp/code/server -ID:/tim/ume-tim-cpp/code/server/output/include -ID:/tim/ume-tim-cpp/code/server/interface/psl -ID:/tim/ume-tim-cpp/code/server/interface/pfl -ID:/tim/ume-tim-cpp/code/server/interface/pfl/cmddispatcher -ID:/tim/ume-tim-cpp/code/server/interface/pfl/dbutils -ID:/tim/ume-tim-cpp/code/server/interface/pfl/pflutils -ID:/tim/ume-tim-cpp/code/server/interface/pfl/southadapter -ID:/tim/ume-tim-cpp/code/server/interface/pfl/southdistributor -ID:/tim/ume-tim-cpp/code/server/interface/pfl/forword -ID:/tim/ume-tim-cpp/code/server/interface/pfl/timerservice -ID:/tim/ume-tim-cpp/code/server/interface/pal/palutils -ID:/tim/ume-tim-cpp/code/server/psl/context/corba/idl/lib -ID:/tim/ume-tim-cpp/code/server/psl/pslutils/export -ID:/tim/ume-tim-cpp/code/server/psl -ID:/tim/ume-tim-cpp/code/server/psl/kernel/export -ID:/tim/ume-tim-cpp/code/server/pfl/dbutils/dboperate/sybase/lib/sybase/OCS-15_0/include -ID:/tim/ume-tim-cpp/code/server/../public/qx-interface/objPublic -ID:/tim/ume-tim-cpp/code/server/../public/qx-interface/objCombine -ID:/tim/ume-tim-cpp/code/server/pfl/southdistributor/export -ID:/tim/ume-tim-cpp/code/server/interface/pal/server/resource -ID:/tim/ume-tim-cpp/code/server/interface/pal/server/tpmanager -ID:/tim/ume-tim-cpp/code/server/pal/server/Tim400/TimServer/src -ID:/tim/ume-tim-cpp/code/server/pal/server/Tim400/TimSubItems_COMM/src -ID:/compile_tools_ume/devtools/boost_1_34_1 /link D:/compile_tools_ume/devtools/ace56_vc7/lib/ACE.lib  /OUT:aceostest.x







//#include "ace/OS.h"

#include "ace/OS_NS_sys_time.h"



#include <iostream>



//#pragma comment (lib,"ACE.lib")



//using namespace ACE_OS;



using namespace std;



int main()

{

    ACE_Time_Value tv;

    tv = ACE_OS::gettimeofday();



    std::cout << "Hello World!! " << tv.sec() << " second has elapsed since EPOCH 1970 01 01 00:00:00" << std::endl;



    while(1)

    {

        if((ACE_OS::gettimeofday() - tv).sec() == 5)

        {

            printf("5 seconds has been past\n");

            exit(0);

        }

    }

}

