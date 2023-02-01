#ifndef unittest_H
#define unittest_H

#include <vector>
#include <string>

#define ARGC 2
#define ARGTYPE int
#define ANSWERTYPE int

struct TestData
{
    ARGTYPE arg[ARGC];
    ANSWERTYPE anwser;
};

class TestCase{

public:
    TestCase(char *);
    ~TestCase(){};
    void print();
    bool run(ANSWERTYPE *);
private:
    TestData m_test_data;
};

class UnitTest{

public:
    UnitTest(const std::string &);
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