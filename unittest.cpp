#include "unittest.h"
#include "./common/rio.h"

// ============ MODIFY HERE =================
ANSWERTYPE test_func(ARGTYPE arg0, ARGTYPE arg1){
    return arg0 + arg1;
}

bool TestCase::run(ANSWERTYPE *output){
    ANSWERTYPE ans = test_func(m_test_data.arg[0], m_test_data.arg[1]);
    *output = ans;
    return ans == m_test_data.anwser;
}

TestCase::TestCase(char *line){
    char *cptr = line;
    int cnt = 0;
    for (char *iter = line; *iter != '\0' && cnt != ARGC; ++iter){
        if(*iter == ' '){
            *iter = '\0';
            m_test_data.arg[cnt++] = atoi(cptr);
            cptr = iter + 1;
        }
    }
    m_test_data.anwser = atoi(cptr);
}

// ==============================================

void TestCase::print(){
    printf("arg:");
    for (int i = 0; i != ARGC; ++i){
        printf(" %d", m_test_data.arg[i]);
    }
    printf("| ");
    printf("anwser: %d", m_test_data.anwser);
}

UnitTest::~UnitTest(){
    for (TestCase *tptr: m_test_cases){
        delete tptr;
    }
}

UnitTest::UnitTest(const std::string &path){
    int fd = Open(path.c_str(), O_RDONLY);
    rio_t rp;
    rio_readinitb(&rp, fd);
    char buf[1024];
    int rd;
    while((rd = rio_readlineb(&rp, buf, sizeof(buf))) > 0){
        TestCase *temp_ptr = new TestCase(buf);
        if (temp_ptr == nullptr){
            continue;
        }

        m_test_cases.push_back(temp_ptr);
    }
}

void UnitTest::print(){
    for(TestCase *ptr : m_test_cases){
        ptr->print();
    }
}

void UnitTest::run_test()
{
    int cnt = 0;
    for(TestCase *ptr : m_test_cases){
        ANSWERTYPE output;
        if(!ptr->run(&output)){
            ptr->print();
            printf("| your answer: %d\n", output);
        }
    }
}