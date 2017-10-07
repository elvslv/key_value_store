#include <mutex>
#include <list>
#include <unordered_map>
#include "Exceptions.h"

namespace utils
{
    template <typename T>
    class RoundRobinList
    {
    public:
        T getNextElement()
        {
            throw NotImplementedException();
        }

        bool hasElements()
        {
            throw NotImplementedException();            
        }

        bool insert(const T& element)
        {
            throw NotImplementedException();            
        }

        bool remove(const T& element)
        {
            throw NotImplementedException();
        }

    private:
        std::mutex elementsMutex;
        std::list<T> elements;
        std::unordered_map<T, typename std::list<T>::iterator> elementsMap;
        typename  std::list<T>::iterator currentElement;
    };
}