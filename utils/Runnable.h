#pragma once

#include <condition_variable>
#include <thread>
#include <string>

namespace utils
{
    class Runnable
    {
    public:
        Runnable();
        virtual ~Runnable();

        Runnable(Runnable const&) = delete;
        Runnable& operator =(Runnable const&) = delete;

        void stop();
        void start();

        std::atomic<bool> isRunning;
        std::condition_variable_any condVar;

    protected:
        virtual void run() = 0;
        std::string str;

    private:
        std::thread thread;
    };

    class RunnableCallback: public Runnable
    {
    public:
        typedef std::function<void()> Callback;
        RunnableCallback(const Callback& callback);

        virtual void run();
    private:
        Callback callback;
    };
}
