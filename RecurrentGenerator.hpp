#pragma once
#include "Cardinal.hpp"
#include "Generator.hpp"
#include "SequenceGenerator.hpp"

template<typename T, template<typename> class Container>
class RecurrentGenerator: public Generator<T> {
private:
    std::function<T(const Container<T>&)> m_func; //это функция для вычисления
    Container<T> m_cache;  //тут хранятся вычесленные элементы 
    size_t m_position; // позиция 
public:
    RecurrentGenerator();
    RecurrentGenerator(const Container<T>& other);
    RecurrentGenerator(std::function<T(const Container<T>&)> func);
    RecurrentGenerator(std::function<T(const Container<T>&)> func, const Container<T>& other);

    size_t GetSize() const;
    Cardinal GetPotentialSize() const override;
    size_t GetCacheSize() const;

    T GetNext() override;
    bool HasNext() const override;

    SequenceGenerator<T, Container> Append(T item) const;
    SequenceGenerator<T, Container> Append(Container<T>* items) const;

    SequenceGenerator<T, Container> Insert (T item) const;
    SequenceGenerator<T, Container> Insert (Container<T>* items) const;

    SequenceGenerator<T, Container> Remove (T item) const;
    SequenceGenerator<T, Container> Remove (Container<T>* items) const;
};

#include "RecurrentGenerator.tpp"