// ============ MODIFY HERE =================
#define ARGC 2
#define ARGTYPE unsigned
#define ANSWERTYPE unsigned

#define ARGPARSEFUNC strtoll   // 参数解析方法
#define ANSPARSEFUNC strtoll   // 返回值解析方法 

#define TEST_FUNC generate_word

#include "./ch2/ch2_hw.cpp"
// ==============================================
#include "unittest.h"
#include "./common/rio.cpp"

bool TestCase::run(ANSWERTYPE *output){
    ANSWERTYPE ans = TEST_FUNC(m_test_data.arg[0], m_test_data.arg[1]);
    *output = ans;
    return ans == m_test_data.anwser;
}

TestCase::TestCase(char *line, int n): m_test_data(n){
    char *cptr = line, *cptr_end;
    int cnt = 0;
    while (cnt != m_test_data.argc){
            ARGTYPE temp = ARGPARSEFUNC(cptr, &cptr_end, 0);
            if (cptr == cptr_end){
                break;
            }
            *(m_test_data.arg + cnt) = temp;
            cnt++;
            cptr = cptr_end;
    }
    ANSWERTYPE temp = ANSPARSEFUNC(cptr, &cptr_end, 0);
    if(cptr == cptr_end){
        printf("test case answer error\n");
        exit(-1);
    }
    m_test_data.anwser = temp;
}

void TestCase::print(){
    printf("arg:");
    for (int i = 0; i != m_test_data.argc; ++i){
        printf(" %d", m_test_data.arg[i]);
    }
    printf("| ");
    printf("Expected anwser: %d", m_test_data.anwser);
}

UnitTest::~UnitTest(){
    for (TestCase *tptr: m_test_cases){
        delete tptr;
    }
}

UnitTest::UnitTest(const std::string &path, const int n){
    int fd = Open(path.c_str(), O_RDONLY);
    rio_t rp;
    rio_readinitb(&rp, fd);
    char buf[1024];
    int rd;
    while((rd = rio_readlineb(&rp, buf, sizeof(buf))) > 0){
        TestCase *temp_ptr = new TestCase(buf, n);
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
            printf("| Your answer: %d\n", output);
            cnt++;
        }
    }
    printf("Correct answer: %d/%d\n", m_test_cases.size() - cnt, m_test_cases.size());
}

int main(){
    UnitTest test("test_case.txt", ARGC);
    test.run_test();
}