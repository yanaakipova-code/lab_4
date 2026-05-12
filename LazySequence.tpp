#include "LazySequence.hpp"
#include "RecurrentGenerator.hpp" 

template<typename T, template<typename> class Container>
LazySequence<T, Container>::LazySequence(): m_cache{} {}

template<typename T, template<typename> class Container>
LazySequence<T, Container>::LazySequence(const Container<T>& cache, std::unique_ptr<Generator<T>> generator)
: m_cache{cache}, m_generator{std::move(generator)} {}

template<typename T, template<typename> class Container>
LazySequence<T, Container>::LazySequence(const Container<T>& cache): m_cache{cache} {}

template<typename T, template<typename> class Container>
LazySequence<T, Container>::LazySequence(T* items, size_t count): m_cache{items, count} {}

template<typename T, template<typename> class Container>
LazySequence<T, Container>::LazySequence(std::function<T(const Container<T>&)> func, const Container<T>& other)
: m_generator{std::make_unique<RecurrentGenerator<T, Container>>(func, other)}, m_cache{other} {}

template<typename T, template<typename> class Container>
LazySequence<T, Container>::LazySequence(LazySequence<T, Container>& other): LazySequence(other.m_cache) {
    if (other.m_generator != nullptr) {
        m_generator.reset(other.m_generator->Clone());
    }
}

template<typename T, template<typename> class Container>
Cardinal LazySequence<T, Container>::GetSizeSequence() const{
    if (m_generator != nullptr) {
        return m_generator->GetPotentialSize();
    }
    return Cardinal(m_cache.GetLength());
}

template<typename T, template<typename> class Container>
size_t LazySequence<T, Container>::GetSizeCache() const {
    return m_cache.GetLength();
}