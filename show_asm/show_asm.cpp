// 该脚本用于将src.cpp代码编译为汇编语言
// 该脚本编译后已复制到/usr/bin，可以在终端直接调用
// 本脚本需要输入一个或两个参数
// 第一个参数为可选参数，指定编译的优化等级，默认为-Og
// 第二个参数为需要编译的文件
#include<string>
#include<cstdlib>

std::string get_name(char *s){
    int i;
    for(i = 0; s[i] != '.' && s[i] != '\0'; ++i);
    return std::string(s, i);
}

int main(int argc, char **argv){
    if(argc < 2 || argc > 3) return -1;
    std::string clang("clang -c -std=c++17 ");
    std::string bifile_name(get_name(argv[argc - 1]) + ".o");
    std::string objdump("objdump -d " + bifile_name);
    std::string del("rm " + bifile_name);
    std::string O(argc>2?argv[argc-2]:"-Og");
    clang += O + " " + std::string(argv[argc-1]);
    system(clang.c_str());
    system(objdump.c_str());
    system(del.c_str());
    return 0;
}