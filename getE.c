#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gmp.h>

#define DEBUG_PRINT(format, ...)                                                                                              \
    do                                                                                                                        \
    {                                                                                                                         \
        struct timeval tv;                                                                                                    \
        struct tm *tm;                                                                                                        \
        char time_str[32];                                                                                                    \
        gettimeofday(&tv, NULL);                                                                                              \
        tm = localtime(&tv.tv_sec);                                                                                           \
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tm);                                                        \
        printf("%s.%03ld %s:%d:%s() " format "\n", time_str, tv.tv_usec / 1000, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
    } while (0)


long long NPlus(long long n){
    long long np=1;
    long long i=0;
    for(i=1; i<=n; i++){
        np*=i;
    }
    DEBUG_PRINT("np:%lld\n",np);
    return np;
}

double calculateE(long long n){
    long double  e=1.0;
    long long np=1ll;
    long long i=0ll;
    for(i=1; i<=n; i++){
        np*=i;
        e+=1.0/np;
        DEBUG_PRINT("i:%lld e:%.128Lf\n",i,e);
    }

    return e;
}


// 计算阶乘的倒数 1/n!
void factorial_inverse(mpf_t result, unsigned long n) {
    mpf_t fact;
    mpf_init_set_ui(fact, 1); // fact = 1
    
    for (unsigned long i = 1; i <= n; ++i) {
        mpf_mul_ui(fact, fact, i); // fact *= i
    }
    
    mpf_ui_div(result, 1, fact); // result = 1 / fact
    mpf_clear(fact);
}

// 计算自然常数 e 到指定精度
void calculate_e(mpf_t e, unsigned int precision) {
    mpf_t term;
    mpf_init(term);

    mpf_set_ui(e, 1); // e = 1, 加上第一个项 1/0!

    for (unsigned int i = 1; i < precision; i++) {
        factorial_inverse(term, i); // 计算每一项 1/i!
        mpf_add(e, e, term); // e += term
    }

    mpf_clear(term);
}

int main1(int argc,char *argv[]){
    long long n;
    long double e=0;
    if(argc<2)
    {
        DEBUG_PRINT("argc <2 ,please input target n:");
        return -1; 
    }
    DEBUG_PRINT("argv[1]:%s\n",argv[1]);
    n=atoi(argv[1]);

    DEBUG_PRINT("n:%lld\n",n);

    e=calculateE(n);
    e=0.00000000000000000000000000000000000000000000000000000023;

    printf("e:%.128Lf\n",e);
}

int main(int argc,char *argv[]) {

    long long precision;
    precision=atoi(argv[1]);

    DEBUG_PRINT("n:%lld\n",precision);
    
    mpf_set_default_prec((precision + 10) * 3.32193); // 设置精度（每位大约需要3.32193 bits）

    mpf_t e;
    mpf_init(e);

    calculate_e(e, precision); // 计算 e

    // 输出 e
    gmp_printf("%.1000Ff\n", e);

    mpf_clear(e);

    return 0;
}
