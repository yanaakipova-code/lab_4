#pragma once
#include "Generator.hpp"
#include "Cardinal.hpp"
template<typename T>
class CompositeGenerator : public Generator<T> {
private:
    std::unique_ptr<Generator<T>> m_first;
    std::unique_ptr<Generator<T>> m_second;
public:
    CompositeGenerator(Generator<T>* first, Generator<T>* second): m_first{first}, m_second{second} {}
    
    T GetNext() override {
        if (m_first && m_first->HasNext()) {
            return m_first->GetNext();
        }
        return m_second->GetNext();
    }
    
    bool HasNext() const override {
        return (m_first && m_first->HasNext()) || m_second->HasNext();
    }
    
    Generator<T>* Clone() const override {
        Generator<T>* first_clone = nullptr;
        if (m_first) {
            first_clone = m_first->Clone();
        }
        Generator<T>* second_clone = m_second->Clone();
        return new CompositeGenerator<T>(first_clone, second_clone);
    }

    Cardinal GetPotentialSize() const override {
        Cardinal s1;
        if (m_first) {
            s1 = m_first->GetPotentialSize();
        } else {
            s1 = Cardinal(0);
        }
        Cardinal s2 = m_second->GetPotentialSize();
        
        if (s1.IsInfiniteNumber() || s2.IsInfiniteNumber()) {
            return Cardinal::Omega();
        }
        return Cardinal(s1.GetSize() + s2.GetSize());
    }
};