#include "catch_amalgamated.hpp"
#include "catch_amalgamated.cpp"

#include <climits>
#include <cmath>

/* ------------------------- Chapter 2 ---------------------------- */
int saturating_add(int x, int y);

TEST_CASE( "Addition that asturates to TMin or TMax" ,"[.][chapter2]" ) {
    REQUIRE( saturating_add(1, 1) == 2 );
    REQUIRE( saturating_add(INT_MAX - 1, 1) == INT_MAX );
    REQUIRE( saturating_add(INT_MAX - 1, 2) == INT_MAX );
    REQUIRE( saturating_add(INT_MAX, 1) == INT_MAX );
    REQUIRE( saturating_add(INT_MAX, INT_MAX) == INT_MAX );
    REQUIRE( saturating_add(INT_MAX, -1) == INT_MAX - 1 );

    REQUIRE( saturating_add(-1, -1) == -2);
    REQUIRE( saturating_add(INT_MIN + 1, -1) == INT_MIN );
    REQUIRE( saturating_add(INT_MIN + 1, -2) == INT_MIN );
    REQUIRE( saturating_add(INT_MIN, -1) == INT_MIN );
    REQUIRE( saturating_add(INT_MIN, INT_MIN) == INT_MIN );
    REQUIRE( saturating_add(INT_MIN, 1) == INT_MIN + 1 );

    REQUIRE( saturating_add(INT_MIN, INT_MAX) ==  -1 );
    REQUIRE( saturating_add(0, 0) ==  0 );

}

int tsub_ok(int, int);

TEST_CASE( "Determine whether arguments can be subtracted without overflow" ,"[.][chapter2]" ) {
    REQUIRE( tsub_ok(1, 1) == 1);
    REQUIRE( tsub_ok(0x80000001, 0) == 1);
    REQUIRE( tsub_ok(0x80000001, 1) == 1);
    REQUIRE( tsub_ok(0x80000001, 2) == 0);
    REQUIRE( tsub_ok(0x80000001, INT_MIN) == 1);
    REQUIRE( tsub_ok(0, INT_MIN) == 0);
    REQUIRE( tsub_ok(-1, INT_MIN) == 1);
    REQUIRE( tsub_ok(INT_MAX - 1, 0) == 1);
    REQUIRE( tsub_ok(INT_MAX - 1, -1) == 1);
    REQUIRE( tsub_ok(INT_MAX - 1, -2) == 0);
}

int signed_high_prod(int x, int y)
{
    long prod = (long)x * y;
    prod >>= (sizeof(int) << 3);
    return (int)prod;
}

unsigned test_unsigned_high_prod(unsigned x, unsigned y)
{
    unsigned long prod = (unsigned long)x * y;
    prod >>= (sizeof(unsigned) << 3);
    return (unsigned)prod;
}

unsigned unsigned_high_prod(unsigned x, unsigned y);

TEST_CASE( "Calculate the high w bit of the production of unsigned int x and unsigned int y" ,"[.][chapter2]" ) {
    REQUIRE( unsigned_high_prod(INT_MAX, INT_MAX) == test_unsigned_high_prod(INT_MAX, INT_MAX) );
    REQUIRE( unsigned_high_prod(0, INT_MAX) == test_unsigned_high_prod(0, INT_MAX) );
    REQUIRE( unsigned_high_prod(INT_MAX / 2, INT_MAX) == test_unsigned_high_prod(INT_MAX / 2, INT_MAX) );
    REQUIRE( unsigned_high_prod(INT_MAX / 2, INT_MAX / 2) == test_unsigned_high_prod(INT_MAX / 2, INT_MAX / 2) );
    REQUIRE( unsigned_high_prod(1, INT_MAX) == test_unsigned_high_prod(1, INT_MAX) );
    REQUIRE( unsigned_high_prod(INT_MAX - 1, INT_MAX) == test_unsigned_high_prod(INT_MAX - 1, INT_MAX) );
}

int divide_power2(int x, int k);

TEST_CASE( "Divide by power 2. Assume 0 <= k < w -1" ,"[.][chapter2]" ) {
    REQUIRE( divide_power2(INT_MAX, 0) == (INT_MAX/(1 << 0)));
    REQUIRE( divide_power2(INT_MAX, 1) == (INT_MAX/(1 << 1)));
    REQUIRE( divide_power2(INT_MAX, 10) == (INT_MAX/(1 << 10)));
    REQUIRE( divide_power2(INT_MAX, 20) == (INT_MAX/(1 << 20)));
    REQUIRE( divide_power2(INT_MAX, 30) == (INT_MAX/(1 << 30)));
    REQUIRE( divide_power2(INT_MIN + 1, 0) == ((INT_MIN + 1)/(1 << 0)));
    REQUIRE( divide_power2(INT_MIN + 1, 1) == ((INT_MIN + 1)/(1 << 1)));
    REQUIRE( divide_power2(INT_MIN + 1, 10) == ((INT_MIN + 1)/(1 << 10)));
    REQUIRE( divide_power2(INT_MIN + 1, 20) == ((INT_MIN + 1)/(1 << 20)));
    REQUIRE( divide_power2(INT_MIN + 1, 30) == ((INT_MIN + 1)/(1 << 30)));
}

int mul3div4(int x);

TEST_CASE( "Calculate 3 * x/4. May overflow when 3 * x" ,"[.][chapter2]" ) {
    REQUIRE( mul3div4(0) == (3 * 0 / 4));
    REQUIRE( mul3div4(1) == (3 * 1 / 4));
    REQUIRE( mul3div4(2) == (3 * 2 / 4));
    REQUIRE( mul3div4(3) == (3 * 3 / 4));
    REQUIRE( mul3div4(4) == (3 * 4 / 4));
    REQUIRE( mul3div4(5) == (3 * 5 / 4));
    REQUIRE( mul3div4(6) == (3 * 6 / 4));
    REQUIRE( mul3div4(7) == (3 * 7 / 4));
    REQUIRE( mul3div4(INT_MAX) == (3 * INT_MAX / 4));
    REQUIRE( mul3div4(-1) == (3 * -1 / 4));
    REQUIRE( mul3div4(-2) == (3 * -2 / 4));
    REQUIRE( mul3div4(-3) == (3 * -3 / 4));
    REQUIRE( mul3div4(-4) == (3 * -4 / 4));
    REQUIRE( mul3div4(-5) == (3 * -5 / 4));
    REQUIRE( mul3div4(-6) == (3 * -6 / 4));
    REQUIRE( mul3div4(-7) == (3 * -7 / 4));
    REQUIRE( mul3div4(INT_MIN) == (3 * INT_MIN / 4));
}

int threefourths(int x);

TEST_CASE( "Calculate 3 * x/4. Never overflow" ,"[.][chapter2]" ) {
    REQUIRE( threefourths(0) == (3 * 0 / 4));
    REQUIRE( threefourths(1) == (3 * 1 / 4));
    REQUIRE( threefourths(2) == (3 * 2 / 4));
    REQUIRE( threefourths(3) == (3 * 3 / 4));
    REQUIRE( threefourths(4) == (3 * 4 / 4));
    REQUIRE( threefourths(5) == (3 * 5 / 4));
    REQUIRE( threefourths(6) == (3 * 6 / 4));
    REQUIRE( threefourths(7) == (3 * 7 / 4));
    REQUIRE( threefourths(INT_MAX) == (int)(3 * (long)INT_MAX / 4));
    REQUIRE( threefourths(-1) == (3 * -1 / 4));
    REQUIRE( threefourths(-2) == (3 * -2 / 4));
    REQUIRE( threefourths(-3) == (3 * -3 / 4));
    REQUIRE( threefourths(-4) == (3 * -4 / 4));
    REQUIRE( threefourths(-5) == (3 * -5 / 4));
    REQUIRE( threefourths(-6) == (3 * -6 / 4));
    REQUIRE( threefourths(-7) == (3 * -7 / 4));
    REQUIRE( threefourths(INT_MIN) == (int)(3 * (long)INT_MIN / 4));
}

/* floating-point part */
/* Access bit-level representation floating-point number*/
typedef unsigned float_bits;

/* Convert float to float_bits in bit-level */
float_bits float_f2b(float f)
{
    float_bits ret = *((float_bits*)&f); 
    return ret;
}

/* Convert float_bits to float in bit-level */
float float_b2f(float_bits fb)
{
    float ret = *((float*)&fb);
    return ret;
}

/*-------------------------------------------*/

float_bits float_negate(float_bits f);

int run_float_negate_test()
{
    for (unsigned b = 0; b <= 0x7f800000; b++)
    {
        if (float_b2f(float_negate(b)) != - float_b2f(b))
        {
            return b;
        }
    }

    for (unsigned b = 0x80000000; b <= 0x8f800000; b++)
    {
        if (float_b2f(float_negate(b)) != - float_b2f(b))
        {
            return b;
        }
    }

    return 0;
}

TEST_CASE( "Compute -f. If f is NaN, then return f." ,"[.][chapter2]" ) {
    REQUIRE(run_float_negate_test() == 0);
}

float_bits float_absval(float_bits f);

int run_float_absval_test()
{
    for (unsigned b = 0; b <= 0x7f800000; b++)
    {
        if (float_b2f(float_absval(b)) != std::abs(float_b2f(b)))
        {
            return b;
        }
    }

    for (unsigned b = 0x80000000; b <= 0x8f800000; b++)
    {
        if (float_b2f(float_absval(b)) != std::abs(float_b2f(b)))
        {
            return b;
        }
    }

    return 0;
}

TEST_CASE( "Compute |f|. If f is NaN, then return f." ,"[.][chapter2]" ) {
    REQUIRE(run_float_absval_test() == 0);
}

float_bits float_twice(float_bits f);

int run_float_twice_test()
{
    for (unsigned b = 0; b <= 0x7f800000; b++)
    {
        if (float_b2f(float_twice(b)) != 2.0f * float_b2f(b))
        {
            return b;
        }
    }

    for (unsigned b = 0x80000000; b <= 0x8f800000; b++)
    {
        if (float_b2f(float_twice(b)) != 2.0f * float_b2f(b))
        {
            return b;
        }
    }

    return 0;
}

TEST_CASE( "Compute 2.0 * f. If f is NaN, then return f." ,"[.][chapter2]" ) {
    REQUIRE(run_float_twice_test() == 0);
}


float_bits float_half(float_bits f);

int run_float_half_test()
{
    for (unsigned b = 0; b <= 0x7f800000; b++)
    {
        if (float_b2f(float_half(b)) != 0.5f * float_b2f(b))
        {
            return b;
        }
    }

    for (unsigned b = 0x80000000; b <= 0x8f800000; b++)
    {
        if (float_b2f(float_half(b)) != 0.5f * float_b2f(b))
        {
            return b;
        }
    }

    return 0;
}

TEST_CASE( "Compute 0.5 * f. If f is NaN, then return f." ,"[.][chapter2]" ) {
    REQUIRE(run_float_half_test() == 0);
}

int float_f2i(float_bits f);

int run_float_f2i_test()
{
    for (unsigned b = 0; b <= 0x7f800000; b++)
    {
        if (float_f2i(b) != (int)float_b2f(b))
        {
            return b;
        }
    }

    for (unsigned b = 0x80000000; b <= 0x8f800000; b++)
    {
        if (float_f2i(b) != (int)float_b2f(b))
        {
            return b;
        }
    }

    return 0;
}

TEST_CASE( "Compute (int)f. If conversion causes overflow or f is Nan, return 0x80000000." ,"[.][chapter2]" ) {
    REQUIRE(run_float_f2i_test() == 0);
}

float_bits float_i2f(int i);

int run_float_i2f_test()
{
    int b = INT_MIN;
    do
    {
        float f1 = float_b2f(float_i2f(b));
        float f2 = (float)b;
        
        if ( f1 != f2)
        {
            return b;
        }

        b++;
    }while(b > INT_MIN);

    return 0;
}

TEST_CASE( "Compute (float) i." ,"[.][chapter2]" ) {
    REQUIRE(run_float_i2f_test() == 0);
}

/* ------------------------------------------------------ */
