#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define ll long long
#define ull unsigned long long//方便书写

#define MAXN 66
#define bitK 32 //压位数
#define K (1LL<<bitK) //进制
#define Int struct BigInt
#define X(i) ((i)/bitK) //第i位在w[X(i)]中
#define Y(i) ((i)%bitK) //第i位在w[X(i)]的第Y(i)位

clock_t start,end;
int _t,_c,_g,_s=-1;
char *IN,*OUT;
unsigned int _seed;//参数

//一些预定义↑

//朴素BigInt部分

struct BigInt{//无符号BigInt类，预定义为Int
    ull w[MAXN];
};

ull Mul_A;

int upper_bit(Int *num){//求最高位的1的位置（测试用）
    int pos=MAXN-1;
    while(!num->w[pos]&&pos) pos--;
    int r=bitK-1;
    while(r&&!((num->w[pos])>>r)) r--;
    return pos*bitK+r;
}

void printInt(Int *A){//以二进制形式输出一个BigInt（测试用）
    int pos=upper_bit(A);
    for(int i=pos;i>=0;i--)
        printf("%d",(A->w[X(i)])&(1<<Y(i))?1:0);
    //putchar('\n');
}

void printInt_x(Int *A){//以16进制的形式输出一个BigInt
    for(int i=31;i>=0;i--){
        printf(i==31?"0x%08llx":"%08llx",A->w[i]);
    }
}

ull find_num(char c){
    if('0'<=c&&c<='9') return c-'0';
    if('a'<=c&&c<='f') return c-'a'+10;
    if('A'<=c&&c<='F') return c-'A'+10;
    return 0;
}

void trans_x(Int *A,char ch[]){//16进制转化
    for(int i=0;i<MAXN;i++) A->w[i]=0;
    int len=0;
    int cnt=0;
    while(ch[len]) len++;
    for(int i=len-1;i>=2;i--){
        A->w[X(cnt)]+=(find_num(ch[i])<<Y(cnt));
        cnt+=4;
    }
}

void trans(Int *A,char ch[]){//将一个01字符串转化为BigInt，测试用
    int pos=0;
    for(int i=0;i<MAXN;i++) A->w[i]=0;
    while(ch[pos]!=0) pos++;
    for(int i=pos-1;i>=0;i--)
        if(ch[pos-i-1]=='1') A->w[X(i)]+=(1LL<<Y(i));
}

void Mul(Int *A,Int *B,Int *C){//朴素大整数乘法C=A*B
    for(int i=0;i<MAXN;i++) C->w[i]=0;
    for(int i=0;i<=31;i++){
        for(int j=0;j<=31;j++){
            Mul_A=A->w[i]*B->w[j];
            C->w[i+j+1]+=Mul_A>>bitK;//防止爆ull提前进一位
            C->w[i+j]+=Mul_A&(K-1);
        }
    }
    for(int i=0;i<MAXN;i++){//统一作进位
        if(C->w[i]>=K) 
            C->w[i+1]+=C->w[i]>>bitK,C->w[i]=(C->w[i])&(K-1);
    }
}

void Rsh(Int *A,int dig,Int *B){//B=A>>dig
    int digX=X(dig),digY=Y(dig);
    for(int i=0;i<MAXN;i++)//先大移X(dig)步
        B->w[i]=i+digX<MAXN?A->w[i+digX]:0;
    for(int i=0;i<MAXN;i++){//微调Y(dig)位
        if(i) B->w[i-1]+=((B->w[i])<<(bitK-digY))&(K-1);
        B->w[i]>>=digY;
    }
}

void Lsh(Int *A,int dig,Int *B){//B=A<<dig，原理同上
    int digX=X(dig),digY=Y(dig);
    for(int i=MAXN-1;i>=0;i--)
        B->w[i]=i-digX>=0?A->w[i-digX]:0;
    for(int i=MAXN-1;i>=0;i--){
        B->w[i]<<=digY;
        if(i+1<MAXN) B->w[i+1]+=(B->w[i])>>bitK;
        B->w[i]=B->w[i]&(K-1);
    }
}

int Greater(Int *A,Int *B){//判断A>=B
    for(int i=MAXN-1;i>=0;i--)
        if(A->w[i]>B->w[i]) return 1;
        else if(A->w[i]<B->w[i]) return 0;
    return 1;
}

void minus(Int *A,Int *B,Int *C){//求A-B=C(要保证A>=B)
    for(int i=0;i<MAXN;i++)
        C->w[i]=A->w[i]-B->w[i];
    for(int i=0;i<MAXN;i++){
        if(i<MAXN-1)
            while(C->w[i]&(1LL<<63))
                C->w[i]+=K,C->w[i+1]--;
    }
}

Int Mod_A;

void Brute_Mod(Int *A,Int *B,Int *C){//朴素的暴力取模A%B=C
    Lsh(B,1024,&Mod_A);
    for(int i=MAXN-1;i>=0;i--) C->w[i]=A->w[i];
    for(int i=1024;i>=0;i--){//依次右移，如果被模数大于当前模数，则减去
        if(Greater(C,&Mod_A)) minus(C,&Mod_A,C);
        Rsh(&Mod_A,1,&Mod_A);
    }
}

void num_Mul(Int *A,ull x,Int *B){//A与一个ull x相乘存入B
    for(int i=0;i<MAXN;i++) B->w[i]=A->w[i]*x;
    for(int i=0;i<MAXN;i++)
        if(B->w[i]>=K) B->w[i+1]+=(B->w[i])>>bitK,B->w[i]=(B->w[i])&(K-1);//进位
}

void plus(Int *A,Int *B,Int *C){//A+B=C
    for(int i=0;i<MAXN;i++)
        C->w[i]=A->w[i]+B->w[i];
    for(int i=0;i<MAXN;i++){//进位
        if(i<MAXN-1) C->w[i+1]+=(C->w[i])>>bitK;
        if(C->w[i]>=K) C->w[i]=(C->w[i])&(K-1);
    }
}

void num_plus(Int *A,ull x){//A=A+x
    A->w[0]+=x;
    for(int i=0;i<MAXN-1;i++){
        if(A->w[i]<K) break;
        A->w[i+1]+=(A->w[i])>>bitK;
        A->w[i]=(A->w[i])&(K-1);
    }
}

int Equal(Int *A,Int *B){//判断A==B
    for(int i=0;i<MAXN;i++){
        if(A->w[i]!=B->w[i]) return 0;
    }
    return 1;
}

//Montgomery域部分
//利用Montgomery域简化取模运算，优化a*b%n,a^b%n

ull iN;//-n^(-1) (mod K)
Int rho,InitA;
Int MontOne;//(Int)1

void MontInit(Int *N){//预处理-n^-1 (mod K)以及 rho=K^64 (mod n)
    iN=1;
    for(int i=1;i<=bitK-1;i++)//由欧拉定理求-n^-1=K-n^(2^31-1) (mod K)
        iN=iN*iN*(N->w[0]);
    iN=iN&(K-1);
    iN=K-iN;
    for(int i=0;i<MAXN;i++) rho.w[i]=0;
    rho.w[1]=1;
    for(int i=0;i<6;i++){
        Mul(&rho,&rho,&InitA);
        Brute_Mod(&InitA,N,&rho);
    }
    MontOne.w[0]=1;
}

Int MA,MB,MC;

void MontMul(Int *A,Int *B,Int *N,Int *ret){//在Montgomery域中求ret=A*B/K^32(mod n)
    for(int i=0;i<MAXN;i++) ret->w[i]=0;
    ull q=0;
    for(int i=0;i<32;i++){
        q=(A->w[0]*B->w[i]+ret->w[0])*iN;
        q=q&(K-1);
        num_Mul(A,B->w[i],&MA);
        num_Mul(N,q,&MB);//补一个数使得在模N意义下不变的同时能被K整除
        plus(ret,&MA,ret);
        plus(ret,&MB,ret);
        Rsh(ret,bitK,ret);//由于能被整除所以直接右移
    }
    if(Greater(ret,N)) minus(ret,N,ret);
}

Int A1,B1,MontD;

void Mod_Mul(Int *A,Int *B,Int *N,Int *C){//利用MontMul快速求C=A*B%N
    MontMul(A,&rho,N,&A1);
    MontMul(B,&rho,N,&B1);
    MontMul(&A1,&B1,N,&MontD);
    MontMul(&MontD,&MontOne,N,C);
}

Int MontA,MontA1,MontT1,MontT;
Int Ret;

void Mod_pow(Int *A,Int *B,Int *N,Int *C){
    MontMul(A,&rho,N,&MontA);//进入Mont域
    MontMul(&MontOne,&rho,N,&MontT);
    for(int i=0;i<=1023;i++){//按位快速幂
        if((B->w[X(i)]&(1LL<<Y(i)))){
            MontMul(&MontT,&MontA,N,&MontT1);
            for(int j=0;j<MAXN;j++) MontT.w[j]=MontT1.w[j];
        }
        MontMul(&MontA,&MontA,N,&MontA1);
        for(int j=0;j<MAXN;j++) MontA.w[j]=MontA1.w[j];
    }
    MontMul(&MontT,&MontOne,N,C);//退出Mont域
}

//基于Montgomery域实现的Miller-Rabbin部分

Int U_mr,A_mr,V_mr;
Int N1;

ull Rnd(ull L,ull R);

int MillerRabin(Int *N,int test_time){//MR板子
    if(!(N->w[0]&1)) return 0;
    MontInit(N);
    int t=0;
    for(int i=0;i<MAXN;i++) U_mr.w[i]=N->w[i],N1.w[i]=N->w[i];
    U_mr.w[0]^=1,N1.w[0]^=1;
    while(!(U_mr.w[0]&1)){
        Rsh(&U_mr,1,&U_mr);
        t++;
    }
    for(int i=0;i<test_time;i++){
        for(int j=0;j<MAXN;j++) A_mr.w[i]=0;
        A_mr.w[0]=Rnd(2,K-1);
        Mod_pow(&A_mr,&U_mr,N,&V_mr);
        if(Equal(&V_mr,&MontOne)) continue;
        int s=0;
        while(s<t){
            if(Equal(&V_mr,&N1)) break;
            Mod_Mul(&V_mr,&V_mr,N,&V_mr);
            s++;
        }
        if(s==t) return 0;
    }
    return 1;
}

//随机生成部分
//先随机生成一个1024bit的数，然后从这个数开始往后依次寻找
//用M个小质数去筛查，用一个大小为M的数组维护这个大数模小质数意义下的剩余

#define MAXP 50005

int p[MAXP],is_prime[MAXP],cntP;

ull ModL[MAXP];

int is_passed;

void find_prime(){//朴素埃氏筛
    for(int i=0;i<MAXP;i++) is_prime[i]=1;
    is_prime[0]=is_prime[1]=0;
    for(int i=2;i*i<MAXP;i++){
        if(!is_prime[i]) continue;
        for(int j=i*i;j<MAXP;j+=i){
            is_prime[j]=0;
        }
    }
    for(int i=1;i<MAXP;i++){
        if(is_prime[i]) p[++cntP]=i;
    }
}

int modify(ull add,ull mul){//维护剩余域下的增量
    int ret=1;
    for(int i=1;i<=cntP;i++){
        ModL[i]=(ModL[i]*mul%p[i]+add)%p[i];
        if(!ModL[i]) ret=0;//如果能被其中一个质数整除那一定不是质数
    }
    return ret;
}

ull ull_rnd(){//随机生成一个ull范围内的数
    return ((ull)rand()<<48)|((ull)rand()<<32)|((ull)rand()<<16)|((ull)rand());
}

ull Rnd(ull L,ull R){//[L,R]范围内的随机数
    return ull_rnd()%(R-L+1)+L;
}

void Generate_1024(Int *ret){//随机生成1024bit大数
    ret->w[31]=Rnd(1LL<<31,(1LL<<32)-1);//严格1024bit
    for(int i=30;i>=0;i--) ret->w[i]=Rnd(0,(1LL<<32)-1);
    if(!(ret->w[0]&1)) ret->w[0]^=1;//为了方便保证是奇数
    for(int i=1;i<=cntP;i++) ModL[i]=0;
    for(int i=31;i>=0;i--){//初始化剩余域
        is_passed=modify(ret->w[i],K);
    }
}

Int Num;

void V_ME_50(){
    start=clock();
    Generate_1024(&Num);//先随便生成一个1024bit的数
    while(1){
        if(is_passed){//如果都不能被小质数整除了再跑Miller Rabin
            if(MillerRabin(&Num,10)) break;
        }
        is_passed=modify(2,1);
        num_plus(&Num,2);//每2个往后找
    }
    end=clock();
    printInt_x(&Num);
}

Int CheckN;
char CheckX[10000];
Int _2,_3,_1,_0;

void V_ME_500(){//判断是否为质数
    int n;
    start=clock();
    scanf("%d",&n);
    for(int i=1;i<=n;i++){
        scanf("%s",CheckX);
        trans_x(&CheckN,CheckX);
        if(Equal(&CheckN,&_3)||Equal(&CheckN,&_2)){//特判
            printf("Yes\n");continue;
        }
        if(Equal(&CheckN,&_1)||Equal(&CheckN,&_0)){
            printf("No\n");continue;
        }
        if(MillerRabin(&CheckN,10)) printf("Yes\n");
        else printf("No\n");
    }
    end=clock();
}

int main(int argc,char *argv[]){//处理参数
    for(int i=1;i<argc;i++){
        if(argv[i][0]=='-'){
            if(argv[i][1]=='c') _c=1;
            if(argv[i][1]=='g') _g=1;
            if(argv[i][1]=='s'){
                _s=1;
                int j=2;
                while(argv[i][j]){
                    _seed=_seed*10+argv[i][j]-'0';
                    j++;
                }
            }
            if(argv[i][1]=='i'){
                IN=argv[i+1];
            }
            if(argv[i][1]=='o'){
                OUT=argv[i+1];
            }
            if(argv[i][1]=='t'){
                _t=1;
            }
        }
    }
    _1.w[0]=1,_2.w[0]=2,_3.w[0]=3;
    srand(_s==-1?time(0):_seed);
    if(IN!=NULL) freopen(IN,"r",stdin);
    if(OUT!=NULL) freopen(OUT,"w",stdout);
    if(_g){
        find_prime();
        V_ME_50();
    }else if(_c){
        V_ME_500();
    }
    if(IN!=NULL) fclose(stdin);
    if(OUT!=NULL) fclose(stdout);
    if(_t){
        freopen("time.out","w",stdout);
        printf("Runtime: %.1lf ms",(double)(end-start)/CLOCKS_PER_SEC*1000.0);
    }
}
