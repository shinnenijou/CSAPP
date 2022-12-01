#include<cstdio>

using byte_pointer = char*;

void by_bytes(byte_pointer p, size_t n){
    for(int i = 0; i != n; ++i) printf("%.2hhx ", *p++);
    printf("\n");
}
void show_bytes(short x){
    by_bytes((byte_pointer) &x, sizeof(short));
}
void show_bytes(int x){
    by_bytes((byte_pointer) &x, sizeof(int));
}
void show_bytes(long x){
    by_bytes((byte_pointer) &x, sizeof(long));
}
void show_bytes(float x){
    by_bytes((byte_pointer) &x, sizeof(float));
}
void show_bytes(double x){
    by_bytes((byte_pointer) &x, sizeof(double));
}
void show_bytes(unsigned x){
    by_bytes((byte_pointer) &x, sizeof(unsigned));
}
void show_bytes(char x){
    by_bytes((byte_pointer) &x, sizeof(char));
}