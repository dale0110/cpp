@echo off


set COMPILE_TOOLS=D:/compile_tools_ume
set VC6_DIR=%COMPILE_TOOLS%/vc7
set   ACE_ROOT=%COMPILE_TOOLS%/devtools/ace56_vc7

set include=%ICAN_KERNEL_HOME%/../include;%T3_MAKE_ENV%/dset/h;%T3_MAKE_ENV%/xerces/include;%ICAN_KERNEL_HOME%/server/interface/pal/palutils;%asn1_dir%;%T3_MAKE_ENV%/aes/export;%BOOST_ROOT%;%VC6_DIR%/include;%VC6_DIR%/PlatformSDK/include;%ACE_ROOT%;
set lib=%T3_MAKE_ENV%/dset/lib;%T3_MAKE_ENV%/xerces/obj;%ICAN_KERNEL_HOME%/../lib;%asn1_dir%;%T3_MAKE_ENV%/aes/lib;%ACE_ROOT%/lib;%ACE_ROOT%/ace;%VC6_DIR%/lib;%VC6_DIR%/PlatformSDK/Lib

set VC_VERSION=vc6


set path=%COMPILE_TOOLS%\shBin\bin;%T3_MAKE_ENV%\shBin;%ROOTDIR%;%ACE_ROOT%/bin;%VC6_DIR%/bin;%VC6_DIR%/../Common7/IDE;%T3_MAKE_ENV%/dset/bin;%T3_MAKE_ENV%/dset/lib;%T3_MAKE_ENV%/dset/dlib;%T3_MAKE_ENV%/dset/h;%path%;%SNACC4J_PATH%;D:\compile_tools_ume\devtools\mksnt;

rm acetest.obj acetest.exe vc70.pch 


REM  cl -nologo /EHsc /W4 /D_DEBUG /DWIN32 -Zm1400   /MDd  /Bt /YX  /time /CGTHREADS:2 /showIncludes  dbg_mem.cpp -o dbg_mem.exe

cl /EHsc /DWIN32 /W4 /D_DEBUG /MDd -ID:/tim/ume-tim-cpp/code/server/pal/server/Tim400/TimSubItems_Z102/src -ID:/compile_tools_ume/devtools/snacc_vc7/c++-lib/inc -ID:/compile_tools_ume/devtools/snacc_vc7/c++-lib/inc/additional -ID:/compile_tools_ume/devtools/snacc_vc7/include/c++ -ID:/compile_tools_ume/devtools/snacc_vc7/include/c++/additional -I. -I../ -I../include -I../export -ID:/compile_tools_ume/devtools/ace56_vc7 -ID:/compile_tools_ume/devtools/ace56_vc7/ACEXML/common -ID:/compile_tools_ume/devtools/ace56_vc7/ACEXML/parser/parser -ID:/compile_tools_ume/devtools/ace56_vc7/TAO -ID:/compile_tools_ume/devtools/ace56_vc7/TAO/tao -ID:/compile_tools_ume/devtools/ace56_vc7/TAO/tao/portableserver -ID:/compile_tools_ume/devtools/ace56_vc7/TAO/tao/strategies -ID:/compile_tools_ume/devtools/ace56_vc7/TAO/tao/valuetype -ID:/compile_tools_ume/devtools/ace56_vc7/TAO/tao/messaging -ID:/compile_tools_ume/devtools/ace56_vc7/TAO/orbsvcs -ID:/compile_tools_ume/devtools/ace56_vc7/TAO/orbsvcs/orbsvcs -ID:/compile_tools_ume/devtools/xerces/include -ID:/tim/ume-tim-cpp/code/server -ID:/tim/ume-tim-cpp/code/server/output/include -ID:/tim/ume-tim-cpp/code/server/interface/psl -ID:/tim/ume-tim-cpp/code/server/interface/pfl -ID:/tim/ume-tim-cpp/code/server/interface/pfl/cmddispatcher -ID:/tim/ume-tim-cpp/code/server/interface/pfl/dbutils -ID:/tim/ume-tim-cpp/code/server/interface/pfl/pflutils -ID:/tim/ume-tim-cpp/code/server/interface/pfl/southadapter -ID:/tim/ume-tim-cpp/code/server/interface/pfl/southdistributor -ID:/tim/ume-tim-cpp/code/server/interface/pfl/forword -ID:/tim/ume-tim-cpp/code/server/interface/pfl/timerservice -ID:/tim/ume-tim-cpp/code/server/interface/pal/palutils -ID:/tim/ume-tim-cpp/code/server/psl/context/corba/idl/lib -ID:/tim/ume-tim-cpp/code/server/psl/pslutils/export -ID:/tim/ume-tim-cpp/code/server/psl -ID:/tim/ume-tim-cpp/code/server/psl/kernel/export -ID:/tim/ume-tim-cpp/code/server/pfl/dbutils/dboperate/sybase/lib/sybase/OCS-15_0/include -ID:/tim/ume-tim-cpp/code/server/../public/qx-interface/objPublic -ID:/tim/ume-tim-cpp/code/server/../public/qx-interface/objCombine -ID:/tim/ume-tim-cpp/code/server/pfl/southdistributor/export -ID:/tim/ume-tim-cpp/code/server/interface/pal/server/resource -ID:/tim/ume-tim-cpp/code/server/interface/pal/server/tpmanager -ID:/tim/ume-tim-cpp/code/server/pal/server/Tim400/TimServer/src -ID:/tim/ume-tim-cpp/code/server/pal/server/Tim400/TimSubItems_COMM/src -ID:/compile_tools_ume/devtools/boost_1_34_1 acetest.cpp


cmd
pause

