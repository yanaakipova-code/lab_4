#pragma once
#include "Generator.hpp"
#include <functional>

template<typename T, typename U>
class MapGenerator : public Generator<U> {
private:
    std::unique_ptr<Generator<T>> m_source;
    std::function<U(T)> m_function;
    
public:
    MapGenerator(Generator<T>* source, std::function<U(T)> func)
        : m_source{source}, m_function{func} {}
    
    U GetNext() override {
        T value = m_source->GetNext();
        return m_function(value);
    }
    
    bool HasNext() const override {
        return m_source->HasNext();
    }
    
    Generator<U>* Clone() const override {
        return new MapGenerator<T, U>(m_source->Clone(), m_function);
    }
    
    Cardinal GetPotentialSize() const override {
        return m_source->GetPotentialSize();
    }
};