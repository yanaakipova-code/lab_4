#include "catch.hpp"
#include "../RecurrentGenerator.hpp"
#include "lab2-3/ArraySequence.hpp"
#include <functional>

TEST_CASE("RecurrentGenerator: конструктор по умолчанию") {
    RecurrentGenerator<int, ArraySequence> gen;
    
    REQUIRE(gen.GetSize() == 0);
    REQUIRE(gen.GetCacheSize() == 0);
    REQUIRE(gen.GetPotentialSize().IsFinalNumber() == true);
    REQUIRE(gen.GetPotentialSize().GetSize() == 0);
    REQUIRE(gen.HasNext() == false);
}

TEST_CASE("RecurrentGenerator: конструктор из контейнера") {
    ArraySequence<int> data = {10, 20, 30, 40};
    RecurrentGenerator<int, ArraySequence> gen(data);
    
    REQUIRE(gen.GetSize() == 0);
    REQUIRE(gen.GetCacheSize() == 4);
    REQUIRE(gen.GetPotentialSize().IsFinalNumber() == true);
    REQUIRE(gen.GetPotentialSize().GetSize() == 4);
    REQUIRE(gen.HasNext() == true);
}

TEST_CASE("RecurrentGenerator: конструктор из пустого контейнера") {
    ArraySequence<int> data;
    RecurrentGenerator<int, ArraySequence> gen(data);
    
    REQUIRE(gen.GetSize() == 0);
    REQUIRE(gen.GetCacheSize() == 0);
    REQUIRE(gen.GetPotentialSize().IsFinalNumber() == true);
    REQUIRE(gen.GetPotentialSize().GetSize() == 0);
    REQUIRE(gen.HasNext() == false);
}

TEST_CASE("RecurrentGenerator: конструктор с функцией") {
    auto func = [](const ArraySequence<int>& cache) -> int {
        return 42;
    };
    RecurrentGenerator<int, ArraySequence> gen(func);
    
    REQUIRE(gen.GetSize() == 0);
    REQUIRE(gen.GetCacheSize() == 0);
    REQUIRE(gen.GetPotentialSize().IsInfiniteNumber() == true);
    REQUIRE(gen.HasNext() == true);
}

TEST_CASE("RecurrentGenerator: GetSize возвращает количество выданных элементов") {
    ArraySequence<int> data = {5, 10, 15, 20, 25};
    RecurrentGenerator<int, ArraySequence> gen(data);
    
    REQUIRE(gen.GetSize() == 0);
    
    gen.GetNext();
    REQUIRE(gen.GetSize() == 1);
    
    gen.GetNext();
    REQUIRE(gen.GetSize() == 2);
    
    gen.GetNext();
    REQUIRE(gen.GetSize() == 3);
    
    gen.GetNext();
    REQUIRE(gen.GetSize() == 4);
    
    gen.GetNext();
    REQUIRE(gen.GetSize() == 5);
}

TEST_CASE("RecurrentGenerator: GetSize для пустого контейнера") {
    ArraySequence<int> data;
    RecurrentGenerator<int, ArraySequence> gen(data);
    
    REQUIRE(gen.GetSize() == 0);
}

TEST_CASE("RecurrentGenerator: GetSize для генератора с функцией") {
    auto func = [](const ArraySequence<int>& cache) -> int {
        return cache.GetLength();
    };
    RecurrentGenerator<int, ArraySequence> gen(func);
    
    REQUIRE(gen.GetSize() == 0);
    
    gen.GetNext();
    REQUIRE(gen.GetSize() == 1);
    
    gen.GetNext();
    REQUIRE(gen.GetSize() == 2);
    
    gen.GetNext();
    REQUIRE(gen.GetSize() == 3);
}

TEST_CASE("RecurrentGenerator: GetCacheSize возвращает размер кеша") {
    ArraySequence<int> data = {1, 2, 3, 4, 5};
    RecurrentGenerator<int, ArraySequence> gen(data);
    
    REQUIRE(gen.GetCacheSize() == 5);
}

TEST_CASE("RecurrentGenerator: GetCacheSize для пустого контейнера") {
    ArraySequence<int> data;
    RecurrentGenerator<int, ArraySequence> gen(data);
    
    REQUIRE(gen.GetCacheSize() == 0);
}

TEST_CASE("RecurrentGenerator: GetCacheSize для генератора с функцией") {
    auto func = [](const ArraySequence<int>& cache) -> int {
        return cache.GetLength();
    };
    RecurrentGenerator<int, ArraySequence> gen(func);
    
    REQUIRE(gen.GetCacheSize() == 0);
    
    gen.GetNext();
    REQUIRE(gen.GetCacheSize() == 1);
    
    gen.GetNext();
    REQUIRE(gen.GetCacheSize() == 2);
    
    gen.GetNext();
    REQUIRE(gen.GetCacheSize() == 3);
}

TEST_CASE("RecurrentGenerator: GetPotentialSize для конечной последовательности") {
    ArraySequence<int> data = {1, 2, 3};
    RecurrentGenerator<int, ArraySequence> gen(data);
    
    Cardinal size = gen.GetPotentialSize();
    REQUIRE(size.IsFinalNumber() == true);
    REQUIRE(size.GetSize() == 3);
}

TEST_CASE("RecurrentGenerator: GetPotentialSize для пустой конечной последовательности") {
    ArraySequence<int> data;
    RecurrentGenerator<int, ArraySequence> gen(data);
    
    Cardinal size = gen.GetPotentialSize();
    REQUIRE(size.IsFinalNumber() == true);
    REQUIRE(size.GetSize() == 0);
}

TEST_CASE("RecurrentGenerator: GetPotentialSize для бесконечной последовательности") {
    auto func = [](const ArraySequence<int>& cache) -> int {
        return 1;
    };
    RecurrentGenerator<int, ArraySequence> gen(func);
    
    Cardinal size = gen.GetPotentialSize();
    REQUIRE(size.IsInfiniteNumber() == true);
}

TEST_CASE("RecurrentGenerator: HasNext для конечной последовательности") {
    ArraySequence<int> data = {100, 200, 300};
    RecurrentGenerator<int, ArraySequence> gen(data);
    
    REQUIRE(gen.HasNext() == true);
    
    gen.GetNext();
    REQUIRE(gen.HasNext() == true);
    
    gen.GetNext();
    REQUIRE(gen.HasNext() == true);
    
    gen.GetNext();
    REQUIRE(gen.HasNext() == false);
}

TEST_CASE("RecurrentGenerator: HasNext для пустой конечной последовательности") {
    ArraySequence<int> data;
    RecurrentGenerator<int, ArraySequence> gen(data);
    
    REQUIRE(gen.HasNext() == false);
}

TEST_CASE("RecurrentGenerator: HasNext для бесконечной последовательности") {
    auto func = [](const ArraySequence<int>& cache) -> int {
        return 1;
    };
    RecurrentGenerator<int, ArraySequence> gen(func);
    
    REQUIRE(gen.HasNext() == true);
    
    for (int i = 0; i < 100; i++) {
        gen.GetNext();
        REQUIRE(gen.HasNext() == true);
    }
}

TEST_CASE("RecurrentGenerator: GetNext возвращает элементы из контейнера по порядку") {
    ArraySequence<int> data = {5, 15, 25, 35};
    RecurrentGenerator<int, ArraySequence> gen(data);
    
    REQUIRE(gen.GetNext() == 5);
    REQUIRE(gen.GetNext() == 15);
    REQUIRE(gen.GetNext() == 25);
    REQUIRE(gen.GetNext() == 35);
    REQUIRE_THROWS_AS(gen.GetNext(), OutOfRangeException);
}

TEST_CASE("RecurrentGenerator: GetNext выбрасывает исключение после последнего элемента") {
    ArraySequence<int> data = {42};
    RecurrentGenerator<int, ArraySequence> gen(data);
    
    REQUIRE(gen.GetNext() == 42);
    REQUIRE_THROWS_AS(gen.GetNext(), OutOfRangeException);
}

TEST_CASE("RecurrentGenerator: GetNext выбрасывает исключение для пустого контейнера") {
    ArraySequence<int> data;
    RecurrentGenerator<int, ArraySequence> gen(data);
    
    REQUIRE_THROWS_AS(gen.GetNext(), OutOfRangeException);
}

TEST_CASE("RecurrentGenerator: постоянный генератор с использованием функции") {
    auto constantFunc = [](const ArraySequence<int>& cache) -> int {
        return 7;
    };
    RecurrentGenerator<int, ArraySequence> gen(constantFunc);
    
    REQUIRE(gen.GetNext() == 7);
    REQUIRE(gen.GetNext() == 7);
    REQUIRE(gen.GetNext() == 7);
    REQUIRE(gen.GetNext() == 7);
    REQUIRE(gen.HasNext() == true);
}

TEST_CASE("RecurrentGenerator: с типом string - контейнер") {
    ArraySequence<std::string> data = {"hello", "world", "!"};
    RecurrentGenerator<std::string, ArraySequence> gen(data);
    
    REQUIRE(gen.GetCacheSize() == 3);
    REQUIRE(gen.GetNext() == "hello");
    REQUIRE(gen.GetNext() == "world");
    REQUIRE(gen.GetNext() == "!");
    REQUIRE_THROWS_AS(gen.GetNext(), OutOfRangeException);
}

TEST_CASE("RecurrentGenerator: GetSize и GetCacheSize для контейнера") {
    ArraySequence<int> data = {10, 20, 30, 40, 50};
    RecurrentGenerator<int, ArraySequence> gen(data);
    
    REQUIRE(gen.GetSize() == 0);
    REQUIRE(gen.GetCacheSize() == 5);
    
    gen.GetNext();
    REQUIRE(gen.GetSize() == 1);
    REQUIRE(gen.GetCacheSize() == 5);
    
    gen.GetNext();
    gen.GetNext();
    gen.GetNext();
    gen.GetNext();
    REQUIRE(gen.GetSize() == 5);
    REQUIRE(gen.GetCacheSize() == 5);
    REQUIRE(gen.HasNext() == false);
}

TEST_CASE("RecurrentGenerator: GetSize и GetCacheSize для функции") {
    auto func = [](const ArraySequence<int>& cache) -> int {
        return cache.GetLength();
    };
    RecurrentGenerator<int, ArraySequence> gen(func);
    
    REQUIRE(gen.GetSize() == 0);
    REQUIRE(gen.GetCacheSize() == 0);
    
    gen.GetNext();
    REQUIRE(gen.GetSize() == 1);
    REQUIRE(gen.GetCacheSize() == 1);
    
    gen.GetNext();
    REQUIRE(gen.GetSize() == 2);
    REQUIRE(gen.GetCacheSize() == 2);
    
    gen.GetNext();
    REQUIRE(gen.GetSize() == 3);
    REQUIRE(gen.GetCacheSize() == 3);
}


TEST_CASE("RecurrentGenerator: Append одного элемента к последовательности из контейнера") {
    ArraySequence<int> data = {10, 20, 30};
    RecurrentGenerator<int, ArraySequence> gen(data);
    
    auto new_gen = gen.Append(40);
    
    REQUIRE(gen.GetNext() == 10);
    REQUIRE(gen.GetNext() == 20);
    REQUIRE(gen.GetNext() == 30);
    REQUIRE(gen.HasNext() == false);
    
    REQUIRE(new_gen.GetNext() == 10);
    REQUIRE(new_gen.GetNext() == 20);
    REQUIRE(new_gen.GetNext() == 30);
    REQUIRE(new_gen.GetNext() == 40);
    REQUIRE_THROWS_AS(new_gen.GetNext(), OutOfRangeException);
}

TEST_CASE("RecurrentGenerator: Append к пустой рекуррентной последовательности") {
    auto func = [](const ArraySequence<int>& cache) -> int {
        return 1;
    };
    ArraySequence<int> seed;
    RecurrentGenerator<int, ArraySequence> gen(func, seed);
    
    auto new_gen = gen.Append(42);
    
    REQUIRE(new_gen.GetNext() == 42);
    REQUIRE_THROWS_AS(new_gen.GetNext(), OutOfRangeException);
}

TEST_CASE("RecurrentGenerator: Append пустой последовательности") {
    ArraySequence<int> data = {1, 2, 3};
    RecurrentGenerator<int, ArraySequence> gen(data);
    
    ArraySequence<int> empty;
    auto new_gen = gen.Append(&empty);
    
    REQUIRE(new_gen.GetNext() == 1);
    REQUIRE(new_gen.GetNext() == 2);
    REQUIRE(new_gen.GetNext() == 3);
    REQUIRE_THROWS_AS(new_gen.GetNext(), OutOfRangeException);
}

TEST_CASE("RecurrentGenerator: Append последовательности к пустой рекуррентной") {
    auto func = [](const ArraySequence<int>& cache) -> int {
        return cache.GetLength();
    };
    ArraySequence<int> seq;
    RecurrentGenerator<int, ArraySequence> gen(func, seq);
    
    ArraySequence<int> to_append = {10, 20, 30};
    auto new_gen = gen.Append(&to_append);
    
    REQUIRE(new_gen.GetNext() == 10);
    REQUIRE(new_gen.GetNext() == 20);
    REQUIRE(new_gen.GetNext() == 30);
    REQUIRE_THROWS_AS(new_gen.GetNext(), OutOfRangeException);
}

TEST_CASE("RecurrentGenerator: Append последовательности к последовательности из контейнера") {
    ArraySequence<int> data = {5, 10, 15};
    RecurrentGenerator<int, ArraySequence> gen(data);
    
    ArraySequence<int> to_append = {20, 25};
    auto new_gen = gen.Append(&to_append);
    
    REQUIRE(new_gen.GetNext() == 5);
    REQUIRE(new_gen.GetNext() == 10);
    REQUIRE(new_gen.GetNext() == 15);
    REQUIRE(new_gen.GetNext() == 20);
    REQUIRE(new_gen.GetNext() == 25);
    REQUIRE_THROWS_AS(new_gen.GetNext(), OutOfRangeException);
}

TEST_CASE("RecurrentGenerator: Insert одного элемента в начало последовательности из контейнера") {
    ArraySequence<int> data = {10, 20, 30};
    RecurrentGenerator<int, ArraySequence> gen(data);
    
    auto new_gen = gen.Insert(5);
    
    REQUIRE(gen.GetNext() == 10);
    REQUIRE(gen.GetNext() == 20);
    REQUIRE(gen.GetNext() == 30);
    REQUIRE(gen.HasNext() == false);
    
    REQUIRE(new_gen.GetNext() == 5);
    REQUIRE(new_gen.GetNext() == 10);
    REQUIRE(new_gen.GetNext() == 20);
    REQUIRE(new_gen.GetNext() == 30);
    REQUIRE_THROWS_AS(new_gen.GetNext(), OutOfRangeException);
}

TEST_CASE("RecurrentGenerator: Insert последовательности в начало последовательности из контейнера") {
    ArraySequence<int> data = {30, 40, 50};
    RecurrentGenerator<int, ArraySequence> gen(data);
    
    ArraySequence<int> to_insert = {10, 20};
    auto new_gen = gen.Insert(&to_insert);
    
    REQUIRE(new_gen.GetNext() == 20);
    REQUIRE(new_gen.GetNext() == 10);
    REQUIRE(new_gen.GetNext() == 30);
    REQUIRE(new_gen.GetNext() == 40);
    REQUIRE(new_gen.GetNext() == 50);
    REQUIRE_THROWS_AS(new_gen.GetNext(), OutOfRangeException);
}

TEST_CASE("RecurrentGenerator: Insert пустой последовательности") {
    ArraySequence<int> data = {1, 2, 3};
    RecurrentGenerator<int, ArraySequence> gen(data);
    
    ArraySequence<int> empty;
    auto new_gen = gen.Insert(&empty);
    
    REQUIRE(new_gen.GetNext() == 1);
    REQUIRE(new_gen.GetNext() == 2);
    REQUIRE(new_gen.GetNext() == 3);
    REQUIRE_THROWS_AS(new_gen.GetNext(), OutOfRangeException);
}

TEST_CASE("RecurrentGenerator: Insert с типом string") {
    ArraySequence<std::string> data = {"b", "c"};
    RecurrentGenerator<std::string, ArraySequence> gen(data);
    
    ArraySequence<std::string> to_insert = {"x", "y", "z"};
    auto new_gen = gen.Insert(&to_insert);
    
    REQUIRE(new_gen.GetNext() == "z");
    REQUIRE(new_gen.GetNext() == "y");
    REQUIRE(new_gen.GetNext() == "x");
    REQUIRE(new_gen.GetNext() == "b");
    REQUIRE(new_gen.GetNext() == "c");
    REQUIRE_THROWS_AS(new_gen.GetNext(), OutOfRangeException);
}

TEST_CASE("RecurrentGenerator: Insert не изменяет оригинальный генератор") {
    ArraySequence<int> data = {5, 10, 15};
    RecurrentGenerator<int, ArraySequence> gen(data);
    
    auto new_gen = gen.Insert(1);
    
    REQUIRE(gen.GetNext() == 5);
    REQUIRE(gen.GetNext() == 10);
    REQUIRE(gen.GetNext() == 15);
    REQUIRE(gen.HasNext() == false);
    
    REQUIRE(new_gen.GetNext() == 1);
    REQUIRE(new_gen.GetNext() == 5);
    REQUIRE(new_gen.GetNext() == 10);
    REQUIRE(new_gen.GetNext() == 15);
    REQUIRE_THROWS_AS(new_gen.GetNext(), OutOfRangeException);
}