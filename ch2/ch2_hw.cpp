#include<climits>

bool is_little_endian(){/* Ex. 2.58: 
    This function is to test wether your mechine is little endian
    return 1 if is, or return 0 if not */
    int x = 1, i;
    char *p = (char*)&x;
    for(i = 0; i != sizeof(i) && *p++ != 1; ++i);
    return i == 0;
}

unsigned generate_word(unsigned x, unsigned y){/* Ex. 2.59
    Generate a new word using the lowest byte from x, and
    the remained bytes from y */
    return (x & 0xFF) + (y & ~0xFF);
}

unsigned replace_byte(unsigned x, int i, unsigned char b){/* Ex. 2.60
    replace the ith btye in unsigned number x by b
    return the new unsigned number*/
    if(i >= 0 && i < sizeof(unsigned)){//overflow check
        unsigned mask = ~(0xFF << (i << 3)), y = b << (i << 3);
        x = (x & mask) +  y;
    }
    return x;
}

bool number_check(int x){/* Ex. 2.61*/
    return !x
        || !~x
        || !((x & 0xFF) ^ 0xFF)
        || !(((x >> ((sizeof(int)-1)<<3)) & 0xFF) ^ 0);
}

bool int_shifts_are_arithmetic(){/* Ex. 2.62
    Test whether the right shift on your machine is arithmetic
    return 1 if is, or return 0 if not*/
    return !~((INT_MIN >> (sizeof(int)<<3)) - 1);
}

unsigned srl(unsigned x, int k){/* Ex. 2.63
    Perform shift arthemetically */
    unsigned xsra = (int) x >> k;
    int mask = (-1) << ((sizeof(int) << 3) - k);
    return xsra &~ mask;
}

int sra(int x, int k){/* Ex. 2.63
    Perform shift logically */
    int xsrl = (unsigned) x >> k;
    int mask1 = 1 << ((sizeof(int) << 3) - k - 1);
    int mask2 = (-1) << ((sizeof(int) << 3) - k - 1);//overflow if xsrl is minus.
    return ((mask1 ^ xsrl) + mask2) | xsrl;
}

bool any_odd_one(unsigned x){/* Ex. 2.64
    Return 1 when any odd bit of x equals 1; 0 otherwise. 
    Assume w = 32 */
    unsigned mask = 0x55555555;//0101 0101 0101 0101 0101 0101 0101 0101
    return x & mask;
}

bool odd_ones(unsigned x){/* Ex. 2.65
    return 1 when x contains an odd number of 1s; 0 otherwise. 
    Assume w = 32 */
    return true;
}