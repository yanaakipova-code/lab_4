#include "SequenceGenerator.hpp"
#include "lab2-3/Error.hpp"

template<typename T, template<typename> class Container>
SequenceGenerator<T, Container>::SequenceGenerator(): m_data{}, m_position{0} {}

template<typename T, template<typename> class Container>
SequenceGenerator<T, Container>::SequenceGenerator(const Container<T>& data): m_data{data}, m_position{0} {}

template<typename T, template<typename> class Container>
size_t SequenceGenerator<T, Container>::GetSize() const{
    return m_position;;
}

template<typename T, template<typename> class Container>
Cardinal SequenceGenerator<T, Container>::GetPotentialSize() const{
    return m_data.GetLength();
}

template<typename T, template<typename> class Container>
T SequenceGenerator<T, Container>::GetNext() {
    if (!HasNext()){
        throw OutOfRangeException("Невозможно достать следующий элемент");
    }
    return m_data[m_position++];

}

template<typename T, template<typename> class Container>
bool SequenceGenerator<T, Container>::HasNext() const{
    if(GetPotentialSize().IsInfiniteNumber()){
        return true;
    }
    return GetSize() < GetPotentialSize().GetSize();
}


