#include "LazySequence.hpp"
#include "RecurrentGenerator.hpp"
#include "CompositeGenerator.hpp"

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

template<typename T, template<typename> class Container>
T LazySequence<T, Container>::GetFirst(){
    if(GetSizeCache() == 0){
        if(m_generator == nullptr){
            throw EmptySequenceException("последовательность пуста");
        }
        if(!m_generator->HasNext()){
            throw EmptySequenceException("первый элемент невозможно инициализировать");
        }
        m_cache.Append(m_generator->GetNext());
    }
    return m_cache[0];
}

template<typename T, template<typename> class Container>
T LazySequence<T, Container>::GetLast() {
    if (GetSizeSequence().IsInfiniteNumber()) {
        throw InvalidArgumentException("последовательность бесконечна");
    }
    size_t last_index = GetSizeSequence().GetSize() - 1;
    if (last_index < GetSizeCache()) {
        return m_cache[last_index];
    }
    for (size_t i = GetSizeCache(); i <= last_index; i++) {
        if (!m_generator || !m_generator->HasNext()) {
            throw OutOfRangeException("Недостаточно элементов");
        }
        m_cache.Append(m_generator->GetNext());
    }
    return m_cache[last_index];
}

template<typename T, template<typename> class Container>
T LazySequence<T, Container>::Get(Cardinal index) {
    if (index.IsInfiniteNumber()) {
        throw OutOfRangeException("бесконечное число не может быть выдано");
    }
    size_t idx = index.GetSize();
    Cardinal total_size = GetSizeSequence();
    if (!total_size.IsInfiniteNumber() && idx >= total_size.GetSize()) {
        throw OutOfRangeException("Индекс выходит за пределы последовательности");
    }
    for (size_t i = GetSizeCache(); i <= idx; i++) {
        if (!m_generator || !m_generator->HasNext()) {
            throw OutOfRangeException("Недостаточно элементов");
        }
        m_cache.Append(m_generator->GetNext());
    }
    
    return m_cache[idx];
}


template<typename T, template<typename> class Container>
LazySequence<T, Container>* LazySequence<T, Container>::GetSubsequence(size_t start_index, size_t end_index){
    if (start_index > end_index) {
        throw InvalidArgumentException("start_index не может быть больше end_index");
    }
    Cardinal size = GetSizeSequence();
    if (!size.IsInfiniteNumber() && end_index >= size.GetSize()) {
        throw InvalidArgumentException("end_index выходит за пределы последовательности");
    }
    Container<T> new_cache;
    for(size_t i = GetSizeCache(); i < end_index; i++){
        if(!m_generator || !m_generator->HasNext()){
            throw InvalidArgumentException("столько элементов невозможно инициализировать");
        }
        m_cache.Append(m_generator->GetNext());
    }
    for(size_t i = start_index; i <= end_index; i++){
        new_cache.Append(m_cache[i]);
    }
    return new LazySequence<T, Container>(new_cache);
}

template<typename T, template<typename> class Container>
LazySequence<T, Container> LazySequence<T, Container>::Append(T item) const{
    Container<T> new_cache = m_cache;
    new_cache.Append(item);
    
    std::unique_ptr<Generator<T>> new_generator;
    if (m_generator) {
        new_generator = std::unique_ptr<Generator<T>>(m_generator->Clone());
    }
    
    return LazySequence<T, Container>(new_cache,std::move(new_generator));
}

template<typename T, template<typename> class Container>
LazySequence<T, Container> LazySequence<T, Container>::Prepend(T item) const{
    Container<T> new_cache = m_cache;
    new_cache.Prepend(item);
    
    std::unique_ptr<Generator<T>> new_generator;
    if (m_generator) {
        new_generator = std::unique_ptr<Generator<T>>(m_generator->Clone());
    }
    
    return LazySequence<T, Container>(new_cache,std::move(new_generator));
}

template<typename T, template<typename> class Container>
LazySequence<T, Container> LazySequence<T, Container>::InsertAt(T item, size_t index) const{
    if(GetSizeSequence().IsFinalNumber() && index > GetSizeSequence().GetSize()){
        throw OutOfRangeException("Индекс выходит за рамки последовательности");
    }
    Container<T> new_cache = m_cache;
    if(GetSizeCache() < index){
        new_cache.Append(item);
    }else{
        new_cache.InsertAt(item, index);
    }
    std::unique_ptr<Generator<T>> new_generator;
    if (m_generator) {
        new_generator = std::unique_ptr<Generator<T>>(m_generator->Clone());
    }
    return LazySequence<T, Container>(new_cache, std::move(new_generator));
}

template<typename T, template<typename> class Container>
LazySequence<T, Container> LazySequence<T, Container>::Concat(LazySequence<T, Container>& list) const {
    if (GetSizeSequence().IsInfiniteNumber() || list.GetSizeSequence().IsInfiniteNumber()) {
        throw IsInfiniteLengthException("Длина равна бесконечности");
    }
    
    std::unique_ptr<Generator<T>> first_gen;
    std::unique_ptr<Generator<T>> second_gen;
    
    if (m_generator) {
        first_gen = std::unique_ptr<Generator<T>>(m_generator->Clone());
    } else {
        first_gen = std::unique_ptr<Generator<T>>(new SequenceGenerator<T, Container>(m_cache));
    }
    
    if (list.m_generator) {
        second_gen = std::unique_ptr<Generator<T>>(list.m_generator->Clone());
    } else {
        second_gen = std::unique_ptr<Generator<T>>(new SequenceGenerator<T, Container>(list.m_cache));
    }
    auto composite = new CompositeGenerator<T>(first_gen.release(), second_gen.release());
    
    Container<T> new_cache;
    Cardinal new_length = Cardinal(GetSizeSequence().GetSize() + list.GetSizeSequence().GetSize());
    
    return LazySequence<T, Container>(new_cache, std::unique_ptr<Generator<T>>(composite));
}