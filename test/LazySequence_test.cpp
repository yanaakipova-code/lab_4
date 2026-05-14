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

TEST_CASE("LazySequence: GetFirst возвращает первый элемент из контейнера") {
    ArraySequence<int> data = {10, 20, 30, 40};
    LazySequence<int, ArraySequence> seq(data);
    
    REQUIRE(seq.GetFirst() == 10);
}

TEST_CASE("LazySequence: GetFirst на пустом контейнере выбрасывает исключение") {
    ArraySequence<int> data;
    LazySequence<int, ArraySequence> seq(data);
    
    REQUIRE_THROWS_AS(seq.GetFirst(), EmptySequenceException);
}

TEST_CASE("LazySequence: GetFirst с пустым генератором выбрасывает исключение") {
    ArraySequence<int> empty;
    auto gen = std::make_unique<SequenceGenerator<int, ArraySequence>>(empty);
    LazySequence<int, ArraySequence> seq(empty, std::move(gen));
    
    REQUIRE_THROWS_AS(seq.GetFirst(), EmptySequenceException);
}

TEST_CASE("LazySequence: GetFirst с типом double") {
    ArraySequence<double> data = {1.5, 2.5, 3.5};
    LazySequence<double, ArraySequence> seq(data);
    
    REQUIRE(seq.GetFirst() == Approx(1.5));
}

TEST_CASE("LazySequence: GetFirst с типом string") {
    ArraySequence<std::string> data = {"hello", "world"};
    LazySequence<std::string, ArraySequence> seq(data);
    
    REQUIRE(seq.GetFirst() == "hello");
}

TEST_CASE("LazySequence: GetLast возвращает последний элемент из контейнера") {
    ArraySequence<int> data = {10, 20, 30, 40, 50};
    LazySequence<int, ArraySequence> seq(data);
    
    REQUIRE(seq.GetLast() == 50);
}

TEST_CASE("LazySequence: GetLast для последовательности из одного элемента") {
    ArraySequence<int> data = {42};
    LazySequence<int, ArraySequence> seq(data);
    
    REQUIRE(seq.GetLast() == 42);
}

TEST_CASE("LazySequence: GetLast для последовательности из двух элементов") {
    ArraySequence<int> data = {7, 14};
    LazySequence<int, ArraySequence> seq(data);
    
    REQUIRE(seq.GetLast() == 14);
}

TEST_CASE("LazySequence: GetLast для пустого контейнера выбрасывает исключение") {
    ArraySequence<int> data;
    LazySequence<int, ArraySequence> seq(data);
    
    REQUIRE_THROWS_AS(seq.GetLast(), OutOfRangeException);
}

TEST_CASE("LazySequence: GetLast с типом double") {
    ArraySequence<double> data = {1.5, 2.5, 3.5, 4.5};
    LazySequence<double, ArraySequence> seq(data);
    
    REQUIRE(seq.GetLast() == Approx(4.5));
}

TEST_CASE("LazySequence: GetLast с типом string") {
    ArraySequence<std::string> data = {"a", "b", "c", "d", "e"};
    LazySequence<std::string, ArraySequence> seq(data);
    
    REQUIRE(seq.GetLast() == "e");
}

TEST_CASE("LazySequence: GetLast для большой последовательности") {
    ArraySequence<int> data;
    for (int i = 0; i < 1000; i++) {
        data.Append(i);
    }
    LazySequence<int, ArraySequence> seq(data);
    
    REQUIRE(seq.GetLast() == 999);
}

TEST_CASE("LazySequence: GetLast несколько раз возвращает один и тот же элемент") {
    ArraySequence<int> data = {1, 2, 3, 4, 5};
    LazySequence<int, ArraySequence> seq(data);
    
    REQUIRE(seq.GetLast() == 5);
    REQUIRE(seq.GetLast() == 5);
    REQUIRE(seq.GetLast() == 5);
}

TEST_CASE("LazySequence: GetSubsequence возвращает правильную подпоследовательность из контейнера") {
    ArraySequence<int> data = {10, 20, 30, 40, 50, 60, 70};
    LazySequence<int, ArraySequence> seq(data);
    
    auto* subseq = seq.GetSubsequence(2, 5);
    
    REQUIRE(subseq->GetSizeCache() == 4);
    REQUIRE(subseq->GetFirst() == 30);
    REQUIRE(subseq->GetLast() == 60);
    
    delete subseq;
}

TEST_CASE("LazySequence: GetSubsequence с start_index = 0") {
    ArraySequence<int> data = {1, 2, 3, 4, 5};
    LazySequence<int, ArraySequence> seq(data);
    
    auto* subseq = seq.GetSubsequence(0, 3);
    
    REQUIRE(subseq->GetSizeCache() == 4);
    REQUIRE(subseq->GetFirst() == 1);
    REQUIRE(subseq->GetLast() == 4);
    
    delete subseq;
}

TEST_CASE("LazySequence: GetSubsequence до последнего элемента") {
    ArraySequence<int> data = {5, 10, 15, 20, 25};
    LazySequence<int, ArraySequence> seq(data);
    
    auto* subseq = seq.GetSubsequence(1, 4);
    
    REQUIRE(subseq->GetSizeCache() == 4);
    REQUIRE(subseq->GetFirst() == 10);
    REQUIRE(subseq->GetLast() == 25);
    
    delete subseq;
}

TEST_CASE("LazySequence: GetSubsequence из одного элемента") {
    ArraySequence<int> data = {100, 200, 300, 400};
    LazySequence<int, ArraySequence> seq(data);
    
    auto* subseq = seq.GetSubsequence(2, 2);
    
    REQUIRE(subseq->GetSizeCache() == 1);
    REQUIRE(subseq->GetFirst() == 300);
    
    delete subseq;
}

TEST_CASE("LazySequence: GetSubsequence с start_index > end_index") {
    ArraySequence<int> data = {1, 2, 3, 4, 5};
    LazySequence<int, ArraySequence> seq(data);
    
    REQUIRE_THROWS_AS(seq.GetSubsequence(3, 1), InvalidArgumentException);
}

TEST_CASE("LazySequence: GetSubsequence с end_index за пределами") {
    ArraySequence<int> data = {1, 2, 3};
    LazySequence<int, ArraySequence> seq(data);
    
    REQUIRE_THROWS_AS(seq.GetSubsequence(0, 5), InvalidArgumentException);
}
// ========== 9. Тест GetSubsequence с типом double ==========
TEST_CASE("LazySequence: GetSubsequence с типом double") {
    ArraySequence<double> data = {1.5, 2.5, 3.5, 4.5, 5.5, 6.5};
    LazySequence<double, ArraySequence> seq(data);
    
    auto* subseq = seq.GetSubsequence(2, 4);
    
    REQUIRE(subseq->GetSizeCache() == 3);
    REQUIRE(subseq->GetFirst() == Approx(3.5));
    REQUIRE(subseq->GetLast() == Approx(5.5));
    
    delete subseq;
}

TEST_CASE("LazySequence: GetSubsequence для пустого контейнера") {
    ArraySequence<int> data;
    LazySequence<int, ArraySequence> seq(data);
    
    REQUIRE_THROWS_AS(seq.GetSubsequence(0, 0), InvalidArgumentException);
}
TEST_CASE("LazySequence: GetSubsequence с start_index == end_index") {
    ArraySequence<int> data = {10, 20, 30, 40, 50};
    LazySequence<int, ArraySequence> seq(data);
    
    auto* subseq = seq.GetSubsequence(3, 3);
    
    REQUIRE(subseq->GetSizeCache() == 1);
    REQUIRE(subseq->GetFirst() == 40);
    
    delete subseq;
}