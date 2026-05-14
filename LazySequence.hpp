#pragma once
#include "Generator.hpp"
#include "Cardinal.hpp"
#include <memory>
#include <functional>

template<typename T, template<typename> class Container>
class LazySequence{
private:
    std::unique_ptr<Generator<T>> m_generator;
    Container<T> m_cache;
public:
    LazySequence();
    LazySequence(const Container<T>& cache, std::unique_ptr<Generator<T>> generator);
    LazySequence(const Container<T>& cache);
    LazySequence(T* items, size_t count);
    LazySequence(std::function<T(const Container<T>&)> func, const Container<T>& other);
    LazySequence(LazySequence<T, Container>& other);

    Cardinal GetSizeSequence() const;
    size_t GetSizeCache() const;

    T GetFirst();
    T GetLast();
    LazySequence<T, Container>* GetSubsequence(size_t start_index, size_t end_index);

    LazySequence<T, Container> Append(T item) const;
    LazySequence<T, Container> Prepend(T item) const;
};

#include "LazySequence.tpp"

/*
будет свой класс generator - из него берем размер  - интерфейс, от него наследуются генератор для фибоначи или конес=чных чисел
свой класс Cardinal - будет хранить либо size_t либо бесконечность 
*/