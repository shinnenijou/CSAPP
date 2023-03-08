#include<climits>

/* Ex. 2.58: 
 * This function is to test wether your mechine is little endian
 * return 1 if is, or return 0 if not 
 */
bool is_little_endian()
{
    int x = 1, i;
    char *p = (char*)&x;
    for(i = 0; i != sizeof(i) && *p++ != 1; ++i);
    return i == 0;
}

/* Ex. 2.59
 * Generate a new word using the lowest byte from x, and
 * the remained bytes from y
 */
unsigned generate_word(unsigned x, unsigned y)
{
    return (x & 0xFF) + (y & ~0xFF);
}

/* Ex. 2.60
 * replace the ith btye in unsigned number x by b
 * return the new unsigned number
 */
unsigned replace_byte(unsigned x, int i, unsigned char b)
{
    if(i >= 0 && i < sizeof(unsigned)){//overflow check
        unsigned mask = ~(0xFF << (i << 3)), y = b << (i << 3);
        x = (x & mask) +  y;
    }
    return x;
}

/* Ex. 2.61*/
bool number_check(int x)
{
    return !x
        || !~x
        || !((x & 0xFF) ^ 0xFF)
        || !(((x >> ((sizeof(int)-1)<<3)) & 0xFF) ^ 0);
}

/* Ex. 2.62
 * Test whether the right shift on your machine is arithmetic
 * return 1 if is, or return 0 if not
 */
bool int_shifts_are_arithmetic()
{
    return !~(INT_MIN >> ((sizeof(int) << 3) - 1));
}

/* Ex. 2.63
 * Perform shift arthemetically 
 */
unsigned srl(unsigned x, int k)
{
    unsigned xsra = (int) x >> k;
    int mask = (-1) << ((sizeof(int) << 3) - k);
    return xsra &~ mask;
}

/* Ex. 2.63
 * Perform shift logically
 */
int sra(int x, int k)
{
    int xsrl = (unsigned) x >> k;
    int mask1 = 1 << ((sizeof(int) << 3) - k - 1);
    int mask2 = (-1) << ((sizeof(int) << 3) - k - 1);//overflow if xsrl is minus.
    return ((mask1 ^ xsrl) + mask2) | xsrl;
}

/* Ex. 2.64
 * Return 1 when any odd bit of x equals 1; 0 otherwise. 
 * Assume w = 32 
 */
bool any_odd_one(unsigned x)
{
    unsigned mask = 0x55555555;//0101 0101 0101 0101 0101 0101 0101 0101
    return x & mask;
}

/* Ex. 2.65
 * return 1 when x contains an odd number of 1s; 0 otherwise. 
 * Assume w = 32
 */
bool odd_ones(unsigned x)
{
    x ^= x >> 16;
    x ^= x >> 8;
    x ^= x >> 4;
    x ^= x >> 2;
    x ^= x >> 1;
    x &= 1;         /* xor all bits at rightmost bit */

    return x;
}

/* Ex. 2.68
 * Mask with least significant n bits set to 1
 * Example: n = 6 --> 0x3F, n = 17 --> 0x1FFFF
 * Assume 1 <= n <= w
 */
int lower_one_mask(int n)
{
    n = 1 << (n - 1);
    return n | (n - 1);
}

/* Ex. 2.69
 * Do rotating left shift. Assume 0 <= n < w
 * Example when x = 0x12345678 and w = 32
 *     n = 4 --> 0x23456781, n = 20 --> 0x67812345
 */
unsigned rotate_left(unsigned x, int n)
{
    return (x << n) |
           (unsigned)((unsigned long long)x >> ((sizeof(unsigned) << 3) - n));
}

/* Ex. 2.70
 * Return 1 when x can be represented as an n-bit, 2's-complement
 * number; 0 otherwise
 * Assume 1 <= n <= w
 */
int fits_bits(int x, int n)
{
    int mask = ~((1 << (n - 1)) - 1);
    return ((x & mask) ^ mask) == 0 || (x & mask) == 0;
}

/* Ex. 2.71
 * Extract byte from word. Return as signed integer
 */
typedef unsigned packed_t;
int xbyte(packed_t word, int bytenum)
{
    return (int)(word << ((3 - bytenum) << 3)) >> 24;
}