#pragma once

#include <thread>

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

    protected:
        virtual void run() = 0;
        std::atomic<bool> isRunning;

    private:
        std::thread thread;
    };

    class RunnableCallback: public Runnable
    {
    public:
        typedef std::function<void()> Callback;
        RunnableCallback(const Callback& callback);

        virtual void run();
        bool shouldRun();
    private:
        Callback callback;
    };
}
