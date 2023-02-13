// 该脚本用于将src.cpp代码编译为汇编语言
// 本脚本需要输入一个或两个参数
// 第一个参数为可选参数，指定编译的优化等级，默认为-Og
// 第二个参数为需要编译的文件, 默认为src_to_show_asm, 可以将源码直接复制进对应文件进行编译
#include<string>
#include<cstdlib>
#include<iostream>

std::string get_name(std::string &s){
    int i;
    for(i = 0; s[i] != '.' && i != s.size(); ++i);
    return std::string(s.c_str(), i);
}

int main(int argc, char **argv){
    if(argc > 3) return -1;

    std::string filename = std::string("src_to_show_asm.cpp");
    std::string clang("clang -c -std=c++17 ");

    if (argc >= 2){
        filename = std::string(argv[argc - 1]);
    }

    std::string bifile_name(get_name(filename) + ".o");

    std::string objdump("objdump -d " + bifile_name);
    std::string del("rm " + bifile_name);
    std::string O(argc>2?argv[argc-2]:"-Og");
    clang += O + " " + filename;
    system(clang.c_str());
    system(objdump.c_str());
    system(del.c_str());
    return 0;
}