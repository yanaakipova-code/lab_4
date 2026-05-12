#pragma once
#include "Cardinal.hpp"
#include "Generator.hpp"

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

    size_t GetSize() const;
    Cardinal GetPotentialSize() const override;
    size_t GetCacheSize() const;

    T GetNext() override;
    bool HasNext() const override;
};

#include "RecurrentGenerator.tpp"