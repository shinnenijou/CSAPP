#include<ctime>
#include<iostream>

inline void function_run_time(void func(void), int repeat){
    int y;
    clock_t t_start, t_end;
    t_start = clock();
    for(int i = 0; i < repeat; ++i) func();
    t_end = clock();
    clock_t time1 = t_end - t_start;
    t_start = clock();
    for(int i = 0; i < repeat; ++i) ;
    t_end = clock();
    clock_t time2 = t_end - t_start;
    std::cout << 1000 * (time1 - time2)/CLOCKS_PER_SEC << "ms" << std::endl;
}