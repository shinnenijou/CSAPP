# phase_1
`phase_1`函数反汇编其中有如下部分
```
  400ee4:	be 00 24 40 00       	mov    $0x402400,%esi
  400ee9:	e8 4a 04 00 00       	call   401338 <strings_not_equal>
```
`phase_1`被调用后没有改变%rdi的值, 所以可以确认传入函数`strings_not_equal`的两个参数即为比较的字符串, 其中一个为标准输入, 另一个为从内存`0x402400`开始的常量字符串. 在array section中找到这个字符串即可.  