#include "Runnable.h"

namespace utils 
{
    Runnable::Runnable():
        isRunning(false),
        condVar(),
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
        condVar.notify_all();
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
}
