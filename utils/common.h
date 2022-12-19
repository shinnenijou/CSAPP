#ifndef COMMON_H

#include "show_bytes.cpp"
#include "time_test.cpp"

void by_bytes(byte_pointer p, size_t n);
void show_bytes(short x);
void show_bytes(int x);
void show_bytes(long x);
void show_bytes(float x);
void show_bytes(double x);
void show_bytes(unsigned x);
void show_bytes(char x);

void function_run_time(void func(void), int repeat);
#endif