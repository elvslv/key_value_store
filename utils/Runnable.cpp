#include "Runnable.h"

namespace utils 
{
    Runnable::Runnable():
        isRunning(),
        thread() 
    {

    }
    Runnable::~Runnable()
    {
        try
        { 
            stop();
        } catch(...) 
        { 
            /*??*/ 
        }
    }

    void Runnable::stop()
    { 
        isRunning = false; 
        thread.join(); 
    }

    void Runnable::start()
    { 
        isRunning = true; 
        thread = std::thread(&Runnable::run, this); 
    }

    RunnableCallback::RunnableCallback(const Callback& callback):
        callback(callback)
    {
    }

    void RunnableCallback::run()
    {
        callback();
    }

    bool RunnableCallback::shouldRun()
    {
        return isRunning;
    }
}
