#include "RecurrentGenerator.hpp"
#include "lab2-3/Error.hpp"

template<typename T, template<typename> class Container>
RecurrentGenerator<T, Container>::RecurrentGenerator(): m_func{}, m_cache{}, m_position{0} {}

template<typename T, template<typename> class Container>
RecurrentGenerator<T, Container>::RecurrentGenerator(const Container<T>& other): m_func{}, m_cache{other}, m_position{0} {}

template<typename T, template<typename> class Container>
RecurrentGenerator<T, Container>::RecurrentGenerator(std::function<T(const Container<T>&)> func): m_func{func}, m_cache{}, m_position{0} {}

template<typename T, template<typename> class Container>
size_t RecurrentGenerator<T, Container>::GetSize() const{
    return m_position;
}

template<typename T, template<typename> class Container>
size_t RecurrentGenerator<T, Container>::GetCacheSize() const{
    return m_cache.GetLength();
}

template<typename T, template<typename> class Container>
Cardinal RecurrentGenerator<T, Container>::GetPotentialSize() const{
    if (m_func) {
        return Cardinal::Omega();
    }
    return Cardinal(m_cache.GetLength()); 
}

template<typename T, template<typename> class Container>
bool RecurrentGenerator<T, Container>::HasNext() const{
    if(GetPotentialSize().IsInfiniteNumber()){
        return true;
    }
    return GetSize() < GetPotentialSize().GetSize();
}

template<typename T, template<typename> class Container>
T RecurrentGenerator<T, Container>::GetNext() {
    if(!HasNext()){
        throw OutOfRangeException("Невозможно достать следующий элемент");
    }
    if(m_position < GetCacheSize()) {
        return m_cache[m_position++];
    }
    if (!m_func) {
        throw OutOfRangeException("Нет функции для вычисления следующего элемента");
    }

    T next = m_func(m_cache);
    m_cache.Append(next);
    m_position++;
    return next;
}