
> # Chapter3 程序的机器级表示练习题

## 练习题3.1
> 假设下面的值存放在指明的内存地址和寄存器中:
>    | 地址  | 值   |
>    | ----- | ---- |
>    | 0x100 | 0xFF |
>    | 0x104 | 0xAB |
>    | 0x108 | 0x13 |
>    | 0x10C | 0x11 |
> 
>    | 寄存器 | 值    |
>    | ------ | ----- |
>    | %rax   | 0x100 |
>    | %rcx   | 0x1   |
>    | %rdx   | 0x3   |
>
> 填写下表，给出所有操作数的值

| 操作数          | 值        |
| --------------- | --------- |
| %rax            | **0x100** |
| 0x104           | **0x104** |
| $0x108          | **0x13**  |
| (%rax)          | **0xFF**  |
| 4(%rax)         | **0xAB**  |
| 9(%rax, %rdx)   | **0x11**  |
| 260(%rcx, %rdx) | **0x13**  |
| 0xFC(, %rcx, 4) | **0x13**  |
| (%rax, %rdx, 4) | **0x11**  |

## 练习题3.2
> 对于下面汇编代码的每一行，根据操作数，确定适当的指令后缀。（例如，`mov`可以被重写成`movb`，`movw`，`movl`或者，`movq`）
```
movl  %eax,(%rsp)
movw  (%rax),%dx
movb  $0xFF,%bl
movb  (%rsp, %rdx, 4),%dl
movq  (%rdx),%rax
movb  %dx,(%rax)
```

## 练习题3.3
> 当我们调用汇编器的时候，下面代码每一行都会产生一个错误消息。解释每一行都是哪里出了错。  

`movb  $0xF,(%ebx)`  寄存器寻址必须是64位寄存器  
`movl  %rax,(%rsp)`  指令`movl`传送4个字节但寄存器`%rax`包含8个字节数据  
`movw  (%rax),4(%rsp)`  不能从内存讲数据传送至内存  
`movb  %al,%sl`  没有名为`%sl`的寄存器  
`movq  %rax,$0x123`  目的操作数不能是直接数  
`movl  %eax,%rdx`  目的寄存器`%rdx`大小(8字节)与指令指定的大小(4字节)不匹配  
`movb  %si,8(%rbp)`  指令`movb`传送1个自己但寄存器`%si`包含2个字节数据

## 练习题3.4
> 假设变量sp和dp被声明为类型  
> `src_t *sp;`  
> `dest_t *dp;`  
> 这里src_t和dest_t是用typedef声明的数据类型。我们想使用适当的数据传送指令来实现下面的操作  
> `*dp = (dest_t) *sp;`  
> 
> 假设sp和dp的值分别存储在寄存器%rdi和%rsi中。对于表中的每个表项，给出实现指定数据传送的两条指令。
> 其中第一条指令应该从内存中读数，做适当的转换，并设置寄存器号rax 的适当部分。
> 然后，第二条指令要把%rax的适当部分写到内存。
> 在这两种情况中，寄存器的部分可以是%rax、%eax、%ax 或%a1，两者可以互不相同。
> 记住，当执行强制类型转换既涉及大小变化又涉及C语言中符号变化时，操作应该先改变大小。

| src_t         | dest_t        | 指令                 |
| ------------- | ------------- | -------------------- |
| long          | long          | `movq (%rdi),%rax`   |
|               |               | `movq %rax,(%rsi)`   |
| char          | int           | `movsbl (%rdi),%eax` |
|               |               | `movl %eax,(%rsi)`   |
| char          | unsigned      | `movsbl (%rdi),%eax` |
|               |               | `movl %eax,(%rsi)`   |
| unsigned char | long          | `movzbl (%rdi),%eax` |
|               |               | `movl %eax,(%rsi)`   |
| int           | char          | `movb (%rdi),%al`    |
|               |               | `movb %al,(%rsi)`    |
| unsigned      | unsigned char | `movb (%rdi),%al`    |
|               |               | `movb %al,(%rsi)`    |
| char          | short         | `movsbw (%rdi),%ax`  |
|               |               | `movw %ax,(%rsi)`    |

## 练习题3.5
> 已知信息如下。将一个原型为  
> `void decode1(long *xp, long *yp, long *zp);`  
> 的函数编译成汇编代码，得到如下代码
> ```
>   void decode1 (long *xp, long *yp, long *zp)
>   xp in %rdi, yp in %rsi, zp in %rdx
> decode1:
>   movq  (%rdi), %r8
>   movq  (%rsi), %rcx
>   movq  (%rdx), %rax
>   movq  %r8, (%rsi)
>   movq  %rcx, (%rdx)
>   movq  %rax, (%rdi)
>   ret
> ```
> 参数xp、v和20分别存储在对应的寄存器号rdi.grsi和grdx中。请写出等效于上面汇编代码的 decode1 的C代码。

```
void decode1(long *xp, long *yp, long *zp)
{
    long temp1 = *xp, temp2 = *yp, temp3 = *zp;
    *yp = temp1;
    *zp = temp2;
    *xp = temp3;
    return temp3;
}
```

## 练习题3.6
> 假设寄存器%rax的值为x，%rcx的值为y。填写下表，指明下面每条汇编代码指令存储在寄存器%rdx中的值：

| 表达式                         | 结果         |
| ------------------------------ | ------------ |
| `leag 6(%rax), %rdx`           | `x + 6`      |
| `leaq (%rax，%rcx), %rdx`      | `x + y`      |
| `leaq (%rax，%rcx, 4), %rdx`   | `x + 4y`     |
| `leaq 7(%rax, %rax, 8), %rdx`  | `x + 8y + 7` |
| `leaq 0xA(, %rcx, 4), %rdx`    | `4y + 10`    |
| `leaq 9 (%rax, %rcx, 2), &rdx` | `x + 2y + 9` |

## 练习题3.7
> 考虑下面的代码，我们省略了被计算的表达式：
> ```
> 1ong scale2 (1ong x, 1ong y, 1ong z) {
>     long t = _______________;
>     return t;
> }
> ```
> 用 GCC 编译实际的函数得到如下的汇编代码：
> ```
>   1ong scale2(1ong x, 1ong y, 10ng z)
>   x in %rdi, y in %rsi, z in %rdx
> scale2:
> leaq (%rdi, %rdi, 4), %rax
> leag (%rax, %rsi, 2), %rax
> leaq (%rax, %rdx, 8), %rax
> ret
> ```
> 填写出代码中缺失的表达式

```
1ong scale2 (1ong x, 1ong y, 1ong z) {
    long t = 5x + 2y + 8z;
    return t;
}
```

## 练习题3.8
> 假设下面的值存放在指明的内存地址和寄存器中:
>    | 地址  | 值   |
>    | ----- | ---- |
>    | 0x100 | 0xFF |
>    | 0x104 | 0xAB |
>    | 0x108 | 0x13 |
>    | 0x10C | 0x11 |
> 
>    | 寄存器 | 值    |
>    | ------ | ----- |
>    | %rax   | 0x100 |
>    | %rcx   | 0x1   |
>    | %rdx   | 0x3   |
>
> 填写下表，给出下面指令的效果，说明将被更新的寄存器或内存位置，以及得到的值：

| 指令                         | 目的                                     | 值    |
| ---------------------------- | ---------------------------------------- | ----- |
| `addq %rcx, (%rax)`          | 在寄存器`%rax`保存的值加上`%rcx`保存的值 | 0x100 |
| `subq %rdx, 8(%rax)`         | 从地址`%rax + 8`保存的值中减去`%rdx`的值 | 0xA8  |
| `imulq $16, (%rax, %rdx, 8)` | 将地址`(%rax + 8 * %rdx)`保存的值乘以16  | 0x110 |
| `incq 16(%rax)`              | 将地址`%rax + 16`保存的值加1             | 0x14  |
| `decq %rcx`                  | 将寄存器`%rcx`保存的值减1                | 0x0   |
| `subq %rdx, %rax`            | 从寄存器`%rax`的值中减去`%rdx`的值       | 0xFD  |

## 联系题3.9
> 假设我们想生成以下C函数的汇编代码：
> ```
> long shift_left4_rightn (long x, long n)
> {
>     x <<= 4:
>     x >>= n;
>     return x;
> }
> ```
> 下面这段汇编代码执行实际的移位，并将最后的结果放在奇存器`%rax`中。此处省略了两条关键的指令。
> 参数`x`和`n`分别存放在寄存器`%rdi`和`%rsi`中。
>
> 根据右边的注释，填出缺失的指令。请使用算术右移操作。

```
  1ong shift_left4_rightn (1ong x, 1ong n)
  x in %rdi, n in %rsi
shift_left4_rightn:
  movq  %rdi, %rax  # Get x
  salq  $4, %rax    # x <<= 4
  movl  %esi, %ecx  # Get n (4 bytes)
  sarq  %cl, %rax   # x >>= n
  ret
```

## 练习题3.10
> 下面的函数是图3-11a中函数一个变种，其中有些表达式用空格替代  
> 实现这些表达式的汇编代码如下：
> ```
>   long arith2 (long ×, long y, long z)
>   x in %rdi, y in %rsi, z in %rdx
> arith2:
>   orq   %rsi, %rdi
>   sarq  $3, %rdi
>   notq  %rdi
>   movq  %rdx, %rax
>   subq  %rdi, %rax
>   ret
> ```
> 基于这些汇编代码，填写C语言代码中缺失的部分。

```
long arith2(long x, long y, long z)
{
    long t1 = x | y;
    long t2 = t1 >> 3;
    long t3 = ~t2;
    long t4 = z - t3;
    return t4;
}
```

## 练习题3.11
> 常常可以看见以下形式的汇编代码行  
> `xorq %rdx, %rdx`  
> 但是在产生这段汇编代码的C代码中，并没有出现 EXCLUSIVE-OR 操作。  
> 1. 解释这条特殊的 EXCIUSIVE-OR 指今的效果，它实现了什么有用的操作。  
> 2. 更直接地表达这个操作的汇编代码是什么？  
> 3. 比较同样一个操作的两种不同实现的编码字节长度  

1. 该指令将`%rdx`的值与自身异或后更新`%rdx`。该指令实现了将寄存器保存的所有位重置为0。
2. `movl $0, %edx`
3. 使用异或指令只需要2个字节(指令+寄存器)，使用移动指令需要5个字节(指令+直接数+寄存器)

## 练习题3.12
> 考虑如下函数，它计算两个无符号64位数的商和余数：
> ```
> void uremdiv (unsigned long x, unsigned long y,
>               unsigned long *qp, unsigned long *rp) {
>   unsigned long q = x/y;
>   unsigned long r = x%y;
>   *qp = 0;
>   *rp =r;
> }
> ```
> 修改有符号除法的汇编代码来实现这个函数。

```
  void uremdiv (unsigned 1ong X, unsigned 1ong y
                unsigned long *qp, unsigned long *rp)
  x in %rdi, y in %rsi, ap in %rdx, rp in %rcx
uremdiv:
  movq  %rdx, %r8       # Copy qp
  movq  %rdi, %rax      # Move x to lower 8 bytes of dividend
  xorl  %rdx, %rdx      # Zero-extend to upper 8 bytes of dividend
  divq  %rsi            # Divide by y
  movq  %rax, (%r8)     # Store quitient at qp
  movq  %rdx, (%rcx)    # Store remainder at rp
  ret
```

