#include <time.h>
#include <vector>
#include <string>
#include <iostream>

#ifdef WIN32
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#include <windows.h>
#ifdef _DEBUG
    #define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
    // Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
    // allocations to be of _CLIENT_BLOCK type
#else
    #define DBG_NEW new
#endif

#else
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#endif


using namespace std;


struct ElemSCT
{
  public :
        string str ;
        int id ;
};

std::string gen_random(const int len) {

    std::string tmp_s;
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    srand( (unsigned) time(NULL));

    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i)
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];

    return tmp_s;

}

int getElem(vector <ElemSCT> &t)
{
        ElemSCT w1 ;

        cout<<"capacity="<<t.capacity()<<endl;

        for(int i=0; i<1000*1000; i++)
        {
                w1.str = gen_random(10) ;
                w1.id = i ;
                t.push_back(w1);
        }

        cout << t.size() <<endl ;
        cout<<"capacity="<<t.capacity()<<endl;

        return 0;
}

int test()
{
        vector <ElemSCT> t ;

        for(int i=0; i<100; i++)
        {
         getElem(t);

        cout << t.size() <<endl ;
         sleep(1);
        }




        return 0 ;
}

#ifdef WIN32
int main()
{
        HANDLE hLogFile;//▒▒▒▒▒▒־▒ļ▒▒▒▒
  hLogFile = CreateFile("memleak.log", GENERIC_WRITE, FILE_SHARE_WRITE|FILE_SHARE_READ,
              NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);//▒▒▒▒▒▒־▒ļ▒

  _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);//▒▒warn▒▒▒▒▒▒▒▒ݶ▒▒▒▒▒ļ▒▒▒ע▒▒dump▒ı▒▒漶▒▒Ϊwarning▒▒
  _CrtSetReportFile(_CRT_WARN, hLogFile);//▒▒▒▒־▒ļ▒▒▒▒▒Ϊ▒澯▒▒▒▒▒▒ļ▒

        _CrtMemState s1, s2, s3;

        _CrtMemCheckpoint( &s1 );

        test();
        int *ptr=(int *)malloc(1024);
        _CrtMemCheckpoint( &s2 );
 if ( _CrtMemDifference( &s3, &s1, &s2) )
                _CrtMemDumpStatistics( &s3 );

        cout<<"Memory leak test!"<<endl;
         _CrtDumpMemoryLeaks();
         return 0;
}

#else
int main()
{
        test();
        for(int i=0;i<1000;i++)
        {
                sleep(100);
        }
//int *ptr=(int *)malloc(1024);

}

#endif
