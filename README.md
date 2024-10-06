# 使用方法

### 质数判断

```C1.exe -c -i file.in -o file.out``` 执行素数判断，分别以```file.in```与```file.out```输入输出。

```file.in``` 输入格式：

第一行一个整数 $n$，表明样例组数；

接下来 $n$ 行，每一行输入一个十六进制的大数。请以 ```0x``` 开头，并保证输入格式正确（16进制下大小写任意）。

```file.out``` 输出格式：

输出为 $n$ 行，每一行为 ```Yes``` 或 ```No``` 表明是否为质数。

### 生成质数

```C1.exe -g -o file.out``` 生成大质数

将生成 ```file.out``` 文件保存结果。

```file.out``` 输出格式：

输出为 $1$ 行，为以 ```0x``` 开头的一个16进制1024bit质数，字母均为小写。

### 可选参数

```-sNUM``` 表明程序将以 ```NUM``` 作为随机数种子，需保证 ```NUM``` 在 unsigned int 范围内。如果不加该参数将以 ```time(0)``` 为本次运行的种子。

```-t``` 表明程序将输出运行时长到文件 ```time.out``` 中。在判断质数时为结束输入到完成输出的总用时，在生成质数时为开始生成到完成生成（输出结果前）的总用时。

注：若没有指定io文件那么将在控制台中进行输入/输出。

### 使用样例

```C1.exe -g -o prime.out -s114514 -t```

```C1.exe -c -i in.in -o out.out -s114514 -t```

# 程序说明

核心函数如下：

```void Mul(Int *A,Int *B,Int *C)``` 暴力大整数乘法

```void Brute_Mod(Int *A,Int *B,Int *C)``` 朴素暴力取模

```void MontMul(Int *A,Int *B,Int *N,Int *ret)``` 在蒙特马利域中求乘法

```void Mod_Mul(Int *A,Int *B,Int *N,Int *C))``` 快速模乘

```void Mod_pow(Int *A,Int *B,Int *N,Int *C)``` 快速模幂

```int MillerRabin(Int *N,int test_time)``` MillerRabin素性判断

```void V_ME_50()``` 生成质数

```void V_ME_500()``` 判断是否为质数

其余函数功能在注释中均有详尽说明。
