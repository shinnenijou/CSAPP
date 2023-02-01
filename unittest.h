#ifndef unittest_H
#define unittest_H

#include <vector>
#include <string>

// ============ MODIFY HERE =================


struct TestData
{
    int argc;
    ARGTYPE *arg;
    ANSWERTYPE anwser;
    TestData(int n){ arg = (ARGTYPE *)malloc(sizeof(ARGTYPE) * 8 * n); argc = n;}
    ~TestData(){ free(arg); }
};
// ==============================================
class TestCase{

public:
    TestCase(char *, int);
    ~TestCase(){};
    void print();
    bool run(ANSWERTYPE *);
private:
    TestData m_test_data;
};

class UnitTest{

public:
    UnitTest(const std::string &, const int);
    ~UnitTest();
    void print();
    void run_test();
private:
    UnitTest();
    UnitTest(const UnitTest &);
    UnitTest & operator=(const UnitTest &);
    std::vector<TestCase*> m_test_cases;
};
#endif