#pragma once

#include "FailureDetectorEvent.h"

namespace failure_detector
{
    class IFailureDetector 
    {
    public:
        class IObserver
        {
        public:
            virtual ~IObserver(){}
            virtual void onFailureDetectorEvent(const failure_detector::FailureDetectorEvent& failureDetectorEvent) = 0;
        };

        virtual ~IFailureDetector(){};
        virtual void start() = 0;
        virtual void stop() = 0;
        virtual void addObserver(IObserver* observer) = 0;
    };
}