#pragma once
#include "Generator.hpp"
#include "Cardinal.hpp"

template<typename T, template<typename> class Container>
class SequenceGenerator: public Generator<T>{
private:
    Container<T> m_data;
    size_t m_position; //сколько элементов я выдала
public:
    SequenceGenerator();
    SequenceGenerator(const Container<T>& data);

    size_t GetSize() const; //m_position;
    Cardinal GetPotentialSize() const override; //m_data.GetLeght()    
    T GetNext() override;
    bool HasNext() const;

    SequenceGenerator<T, Container> Append(T item) const;

};

#include "SequenceGenerator.tpp"