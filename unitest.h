#ifndef UNITEST_H
#define UNITEST_H

#include "ch2/ch2_hw.h"
#include "utils/common.h"
#include <iostream>
#include <vector>
#include <initializer_list>

class TestCase
{
public:
    using param_1_type = int;
    using param_2_type = int;
    using return_type = int;

    TestCase(param_1_type x, param_2_type y, return_type z) : m_param_1(x), m_param_2(y), m_expected_return(z) {}

    typedef return_type (*fun_type)(param_1_type, param_2_type);
    bool run_test(fun_type test_func)
    {
        m_program_return = (*test_func)(m_param_1, m_param_2);
        return m_expected_return == m_program_return;
    }
    return_type get_expected_return() { return m_expected_return; }
    return_type get_program_return() { return m_program_return; }

private:
    param_1_type m_param_1;
    param_2_type m_param_2;
    return_type m_expected_return;
    return_type m_program_return;
};

class UnitTest
{
public:
    void add_testcase(TestCase *tptr) { m_testcases.push_back(tptr); }
    void run_test()
    {
        int size = m_testcases.size();
        for (int i = 0; i < size; ++i)
        {
            if (m_testcases[i]->run_test(test_func))
            {
                correct_count++;
            }
            else
            {
                std::cout << "Wrong Anwser at case " << i << std::endl
                          << "    expected return " << m_testcases[i]->get_expected_return() << std::endl
                          << "    program  return " << m_testcases[i]->get_program_return() << std::endl;
            }
        }
        std::cout << "[Result]Total TestCase " << size << ", correct " << correct_count << std::endl;
    }

private:
    std::vector<TestCase *> m_testcases;
    TestCase::fun_type test_func;
    int correct_count;

    UnitTest(TestCase::fun_type func) : m_testcases(std::vector<TestCase *>()), test_func(func), correct_count(0) {}
};

#endif