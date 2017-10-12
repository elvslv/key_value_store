#include <mutex>
#include <list>
#include <random>
#include <unordered_map>
#include "Exceptions.h"

namespace utils
{
    template <typename T>
    class RoundRobinList
    {
    public:
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

            int index = std::rand() % elements.size();
            
            // optimize?
            auto elemIt = elements.begin();
            for (int i = 0; i < index; ++i, ++elemIt);

            elements.insert(elemIt, element);
            elementsMap[element] = elemIt;

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

            if (it == currentElement)
            {
                --currentElement;
                elements.remove(it);
                advanceCurrentElement();
            }
            else
            {
                elements.remove(it);                
            }
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