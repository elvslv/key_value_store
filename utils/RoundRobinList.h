#pragma once

#include <mutex>
#include <list>
#include <random>
#include <unordered_map>
#include "Exceptions.h"
#include <iostream>

namespace utils
{
    template <typename T>
    class RoundRobinList
    {
    public:
        size_t size()
        {
            std::lock_guard<std::mutex> lock(elementsMutex);
            return elements.size();
        }

        bool getNextElement(T& result)
        {
            std::lock_guard<std::mutex> lock(elementsMutex);
            if (elements.empty())
            {
                return false;
            }
            
            result = *currentElement;
            advanceCurrentElement();

            return true;
        }

        bool insert(const T& element)
        {
            std::lock_guard<std::mutex> lock(elementsMutex);
            auto it = elementsMap.find(element);
            if (it != elementsMap.end())
            {
                return false;
            }

            if (elements.empty())
            {
                elements.push_front(element);
                elementsMap[element] = elements.begin();
                currentElement = elements.begin();
            }
            else
            {
                int index = std::rand() % elements.size();
                
                // optimize?
                auto elemIt = elements.begin();
                for (int i = 0; i < index; ++i, ++elemIt);
    
                elemIt = elements.insert(elemIt, element);    
                elementsMap[element] = elemIt;
            }

            return true;
        }

        bool remove(const T& element)
        {
            std::lock_guard<std::mutex> lock(elementsMutex);
            auto it = elementsMap.find(element);
            if (it == elementsMap.end())
            {
                return false;
            }

            typename std::list<T>::iterator iterToRemove = it->second;
            elementsMap.erase(element);

            if (iterToRemove == currentElement)
            {
                --currentElement;
                elements.erase(iterToRemove);                
                advanceCurrentElement();
            }
            else
            {
                elements.erase(iterToRemove);  
            }

            return true;
        }

    private:
        void advanceCurrentElement()
        {
            if (++currentElement == elements.end())
            {
                // kick of on a separate thread?
                shuffle();
            }
        }
        
        void shuffle()
        {
            std::vector< std::reference_wrapper< const T > > vec(elements.begin(), elements.end()) ;
            std::shuffle(vec.begin(), vec.end(), std::mt19937{ std::random_device{}() } ) ;
            std::list<T> shuffled_list {vec.begin(), vec.end()} ;
            elements.swap(shuffled_list) ;
            
            elementsMap.clear();
            for (auto it = elements.begin(); it != elements.end(); ++it)
            {
                elementsMap[*it] = it;
            }

            currentElement = elements.begin();
        }

        std::mutex elementsMutex;
        std::list<T> elements;
        std::unordered_map<T, typename std::list<T>::iterator> elementsMap;
        typename std::list<T>::iterator currentElement;
    };
}