## 练习题5.1
当调用过程`xp`等于`yp`时，会在`*yp = *xp - *yp`执行后使指向的数据变成0

## 练习题5.2
过程1: $n \le 2$时是三个版本中最快的  
过程2: $3\le n \le 7$时是三个版本中最快的
过程3: $n \ge 8$时是三个版本中最快的

## 练习题5.3
| 代码 | min | max | incr | square |
| ---- | --- | --- | ---- | ------ |
| A    | 1   | 91  | 90   | 90     |
| B    | 90  | 1   | 90   | 90     |
| C    | 1   | 1   | 90   | 90     |

## 练习题5.4
1. 寄存器`%xmm0`在O1版本中单纯作为乘法计算的容器使用。而O2中则用来保存累计变量，减少了每次循环中的一次内存读取。
2. O2优化版本仍忠实实现了combine3的代码。
3. 在O1优化的版本中，略去循环开支后的两次循环间的代码
```
...
vmovsd  (%rbx), %xmm0
vmulsd  (%rdx), %xmm0, %xmm0
vmovsd  %xmm0, (%rbx)
vmovsd  (%rbx), %xmm0
vmulsd  (%rdx), %xmm0, %xmm0
vmovsd  %xmm0, (%rbx)
...
```
可以发现中间的传送指令`vmovsd  (%rbx), %xmm0`
在传送前后%xmm0的值都没有发生任何变化，因此在O2的优化版本中省去了这一步指令，并且不会导致程序有任何违背预期的行为。

## 练习题5.5
1. 内循环进行了$2n$次乘法$2n$次加法，其中$n$次加法为循环开支。
2. 内循环部分的汇编代码
```
      20: f2 0f 10 5c c7 08     movsd   8(%rdi,%rax,8), %xmm3       // load a[i]
      26: f2 0f 59 da           mulsd   %xmm2, %xmm3                // a[i] * xpwr
      2a: f2 0f 58 c3           addsd   %xmm3, %xmm0                // update result
      2e: f2 0f 59 d1           mulsd   %xmm1, %xmm2                // update xpwr
      32: 48 ff c0              incq    %rax                        // self-increase i
      35: 48 39 c6              cmpq    %rax, %rsi 
      38: 75 e6                 jne     -26 <__Z4polyPddl+0x20>
```
寄存器更新数据流如图  
[dataflow](../res/img/cp5_practice_5.7.png)
## 练习题5.7
$5\times 5$循环展开如下
```
void combine(vec_ptr v, data_t *dest)
{
    long i;
    long length = vec_length(v);
    long limit = length - 4;
    data_t *data = get_vec_start(v);
    data_t acc = IDENT

    // Combine 5 elements at a time
    for (i = 0; i < limit; i+=5){
        acc = (((((acc OP data[i]) OP data[i + 1]) OP data[i + 2]) OP data[i + 3]) OP data[i + 4]);
    }
    
    // Finish any remaining elements
    for(; i < length; ++i){
        acc = acc OP data[i];
    }
    *dest = acc;
}
```