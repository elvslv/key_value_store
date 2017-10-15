#include "gtest/gtest.h"

void handler(int sig) 
{
    assert(false);

    void *array[10];
    size_t size;
  
    // get void*'s for all entries on the stack
    size = backtrace(array, 10);
  
    // print out all the frames to stderr
    fprintf(stderr, "Error: signal %d:\n", sig);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    exit(1);
}
  
  
int main(int argc, char **argv) 
{
    signal(SIGSEGV, handler);   // install our handler    
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}