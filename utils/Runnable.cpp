#include "Runnable.h"
#include <iostream>
#include "utils/Utils.h"

namespace utils 
{
    Runnable::Runnable():
        isRunning(false),
        condVar(),
        str(Utils::getRandomString(8)),
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
        if (!isRunning)
        {
            return;
        }

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
