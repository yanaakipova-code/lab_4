#include "catch.hpp"
#include "../LazySequence.hpp"
#include "../SequenceGenerator.hpp"
#include "../RecurrentGenerator.hpp"
#include "lab2-3/ArraySequence.hpp"
#include <functional>


TEST_CASE("LazySequence: конструктор по умолчанию") {
    LazySequence<int, ArraySequence> seq;
    
    REQUIRE(seq.GetSizeCache() == 0);
    REQUIRE(seq.GetSizeSequence().IsFinalNumber() == true);
    REQUIRE(seq.GetSizeSequence().GetSize() == 0);
}

TEST_CASE("LazySequence: конструктор из контейнера") {
    ArraySequence<int> data = {1, 2, 3, 4, 5};
    LazySequence<int, ArraySequence> seq(data);
    
    REQUIRE(seq.GetSizeCache() == 5);
    REQUIRE(seq.GetSizeSequence().IsFinalNumber() == true);
    REQUIRE(seq.GetSizeSequence().GetSize() == 5);
}

TEST_CASE("LazySequence: конструктор из массива") {
    int arr[] = {10, 20, 30, 40};
    LazySequence<int, ArraySequence> seq(arr, 4);
    
    REQUIRE(seq.GetSizeCache() == 4);
    REQUIRE(seq.GetSizeSequence().IsFinalNumber() == true);
    REQUIRE(seq.GetSizeSequence().GetSize() == 4);
}

TEST_CASE("LazySequence: конструктор из генератора") {
    auto func = [](const ArraySequence<int>& cache) -> int {
        return 42;
    };
    ArraySequence<int> seed;
    seed.Append(0);
    
    LazySequence<int, ArraySequence> seq(func, seed);
    
    REQUIRE(seq.GetSizeCache() == 1);
    REQUIRE(seq.GetSizeSequence().IsInfiniteNumber() == true);
}

TEST_CASE("LazySequence: конструктор копирования") {
    ArraySequence<int> data = {1, 2, 3};
    LazySequence<int, ArraySequence> seq1(data);
    
    LazySequence<int, ArraySequence> seq2(seq1);
    
    REQUIRE(seq2.GetSizeCache() == 3);
    REQUIRE(seq2.GetSizeSequence().GetSize() == 3);
}

TEST_CASE("LazySequence: GetSizeSequence для конечной последовательности") {
    ArraySequence<int> data = {1, 2, 3, 4, 5, 6};
    LazySequence<int, ArraySequence> seq(data);
    
    Cardinal size = seq.GetSizeSequence();
    REQUIRE(size.IsFinalNumber() == true);
    REQUIRE(size.GetSize() == 6);
}


TEST_CASE("LazySequence: GetSizeCache для последовательности из контейнера") {
    ArraySequence<int> data = {100, 200, 300, 400, 500};
    LazySequence<int, ArraySequence> seq(data);
    
    REQUIRE(seq.GetSizeCache() == 5);
}

TEST_CASE("LazySequence: GetSizeCache для пустой последовательности") {
    LazySequence<int, ArraySequence> seq;
    
    REQUIRE(seq.GetSizeCache() == 0);
}

TEST_CASE("LazySequence: GetSizeCache для рекуррентной последовательности") {
    auto rule = [](const ArraySequence<int>& cache) -> int {
        return cache.GetLength();
    };
    ArraySequence<int> seed = {0};
    
    LazySequence<int, ArraySequence> seq(rule, seed);
    
    REQUIRE(seq.GetSizeCache() == 1);
}

TEST_CASE("LazySequence: с типом string") {
    ArraySequence<std::string> data = {"hello", "world"};
    LazySequence<std::string, ArraySequence> seq(data);
    
    REQUIRE(seq.GetSizeCache() == 2);
    REQUIRE(seq.GetSizeSequence().GetSize() == 2);
}

TEST_CASE("LazySequence: из SequenceGenerator") {
    ArraySequence<int> data = {5, 10, 15, 20};
    auto gen = std::make_unique<SequenceGenerator<int, ArraySequence>>(data);
    
    LazySequence<int, ArraySequence> seq(data, std::move(gen));
    
    REQUIRE(seq.GetSizeCache() == 4);
    REQUIRE(seq.GetSizeSequence().GetSize() == 4);
}

TEST_CASE("LazySequence: пустая последовательность") {
    LazySequence<int, ArraySequence> seq;
    
    REQUIRE(seq.GetSizeCache() == 0);
    REQUIRE(seq.GetSizeSequence().GetSize() == 0);
    REQUIRE(seq.GetSizeSequence().IsFinalNumber() == true);
}