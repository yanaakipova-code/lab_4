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
    SequenceGenerator(const SequenceGenerator<T, Container>& other);

    size_t GetSize() const; //m_position;
    Cardinal GetPotentialSize() const override; //m_data.GetLeght()    
    T GetNext() override;
    bool HasNext() const override;

    SequenceGenerator<T, Container> Append(T item) const;
    SequenceGenerator<T, Container> Append (Container<T>* items) const;

    SequenceGenerator<T, Container> Insert (T item) const;
    SequenceGenerator<T, Container> Insert (Container<T>* items) const;

    SequenceGenerator<T, Container> Remove (T item) const;
    SequenceGenerator<T, Container> Remove (Container<T>* items) const;

    SequenceGenerator<T, Container>& operator=(const SequenceGenerator<T, Container>& other);

    Generator<T>* Clone() const override;

};

#include "SequenceGenerator.tpp"