#include "SequenceGenerator.hpp"
#include "lab2-3/Error.hpp"

template<typename T, template<typename> class Container>
SequenceGenerator<T, Container>::SequenceGenerator(): m_data{}, m_position{0} {}

template<typename T, template<typename> class Container>
SequenceGenerator<T, Container>::SequenceGenerator(const Container<T>& data): m_data{data}, m_position{0} {}

template<typename T, template<typename> class Container>
SequenceGenerator<T, Container>::SequenceGenerator(const SequenceGenerator<T, Container>& other): m_data(other.m_data), m_position(other.m_position) {}

template<typename T, template<typename> class Container>
size_t SequenceGenerator<T, Container>::GetSize() const{
    return m_position;
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

template<typename T, template<typename> class Container>
SequenceGenerator<T, Container> SequenceGenerator<T, Container>::Append(T item) const{
    Container<T> new_data = m_data;
    new_data.Append(item);
    return SequenceGenerator<T, Container>(new_data);
}

template<typename T, template<typename> class Container>
SequenceGenerator<T, Container> SequenceGenerator<T, Container>::Append(Container<T>* items) const{
    Container<T> new_data = m_data;
    for(auto& i: *items){
        new_data.Append(i);
    }
    return SequenceGenerator<T, Container>(new_data);
}

template<typename T, template<typename> class Container>
SequenceGenerator<T, Container> SequenceGenerator<T, Container>::Insert(T item) const{
    Container<T> new_data = m_data;
    new_data.Prepend(item);
    return SequenceGenerator<T, Container>(new_data);
}

template<typename T, template<typename> class Container>
SequenceGenerator<T, Container> SequenceGenerator<T, Container>::Insert(Container<T>* items) const{
    Container<T> new_data = m_data;
    for(auto& i: *items){
        new_data.Prepend(i);
    }

    return SequenceGenerator<T, Container>(new_data);
}

template<typename T, template<typename> class Container>
SequenceGenerator<T, Container> SequenceGenerator<T, Container>::Remove(T item) const {
    Container<T> new_data;
    bool removed = false;
    
    for (size_t i = 0; i < m_data.GetLength(); i++) {
        if (!removed && m_data[i] == item) {
            removed = true;
        } else {
            new_data.Append(m_data[i]);
        }
    }
    
    if (!removed) {
        throw OutOfRangeException("Элемент не найден");
    }
    
    return SequenceGenerator<T, Container>(new_data);
}

template<typename T, template<typename> class Container>
SequenceGenerator<T, Container> SequenceGenerator<T, Container>::Remove(Container<T>* items) const {
    SequenceGenerator<T, Container> result(*this); 
    
    for (const auto& item : *items) {
        result = result.Remove(item);
    }
    
    return result;
}

template<typename T, template<typename> class Container>
SequenceGenerator<T, Container>& SequenceGenerator<T, Container>::operator=(const SequenceGenerator<T, Container>& other) {
    if (this != &other) {
        m_data = other.m_data;
        m_position = other.m_position;
    }
    return *this;
}

template<typename T, template<typename> class Container>
Generator<T>* SequenceGenerator<T, Container>::Clone() const {
    SequenceGenerator<T, Container>* copy = new SequenceGenerator<T, Container>(*this);
    return copy;
}