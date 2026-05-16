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

TEST_CASE("LazySequence: Добавление элемента в конец последовательности из контейнера") {
    ArraySequence<int> data = {1, 2, 3};
    LazySequence<int, ArraySequence> seq(data);
    
    auto new_seq = seq.Append(4);
    
    REQUIRE(new_seq.GetSizeCache() == 4);
    REQUIRE(new_seq.GetFirst() == 1);
    REQUIRE(new_seq.GetLast() == 4);
}

TEST_CASE("LazySequence: Добавление элемента в конец пустой последовательности") {
    ArraySequence<int> data;
    LazySequence<int, ArraySequence> seq(data);
    
    auto new_seq = seq.Append(42);
    
    REQUIRE(new_seq.GetSizeCache() == 1);
    REQUIRE(new_seq.GetFirst() == 42);
    REQUIRE(new_seq.GetLast() == 42);
}

TEST_CASE("LazySequence: Многократное добавление в конец") {
    ArraySequence<int> data = {1, 2};
    LazySequence<int, ArraySequence> seq(data);
    
    auto seq2 = seq.Append(3);
    auto seq3 = seq2.Append(4);
    auto seq4 = seq3.Append(5);
    
    REQUIRE(seq4.GetSizeCache() == 5);
    REQUIRE(seq4.GetFirst() == 1);
    REQUIRE(seq4.GetLast() == 5);
}

TEST_CASE("LazySequence: Добавление элемента в начало последовательности из контейнера") {
    ArraySequence<int> data = {2, 3, 4};
    LazySequence<int, ArraySequence> seq(data);
    
    auto new_seq = seq.Prepend(1);
    
    REQUIRE(new_seq.GetSizeCache() == 4);
    REQUIRE(new_seq.GetFirst() == 1);
    REQUIRE(new_seq.GetLast() == 4);
}

TEST_CASE("LazySequence: Добавление элемента в начало пустой последовательности") {
    ArraySequence<int> data;
    LazySequence<int, ArraySequence> seq(data);
    
    auto new_seq = seq.Prepend(42);
    
    REQUIRE(new_seq.GetSizeCache() == 1);
    REQUIRE(new_seq.GetFirst() == 42);
    REQUIRE(new_seq.GetLast() == 42);
}

TEST_CASE("LazySequence: Многократное добавление в начало") {
    ArraySequence<int> data = {3, 4};
    LazySequence<int, ArraySequence> seq(data);
    
    auto seq2 = seq.Prepend(2);
    auto seq3 = seq2.Prepend(1);
    auto seq4 = seq3.Prepend(0);
    
    REQUIRE(seq4.GetSizeCache() == 5);
    REQUIRE(seq4.GetFirst() == 0);
    REQUIRE(seq4.GetLast() == 4);
}

TEST_CASE("LazySequence: Добавление в конец и в начало вместе") {
    ArraySequence<int> data = {20, 30};
    LazySequence<int, ArraySequence> seq(data);
    
    auto seq2 = seq.Prepend(10);
    auto seq3 = seq2.Append(40);
    
    REQUIRE(seq3.GetSizeCache() == 4);
    REQUIRE(seq3.GetFirst() == 10);
    REQUIRE(seq3.GetLast() == 40);
}

TEST_CASE("LazySequence: Добавление в конец с типом double") {
    ArraySequence<double> data = {1.5, 2.5};
    LazySequence<double, ArraySequence> seq(data);
    
    auto new_seq = seq.Append(3.5);
    
    REQUIRE(new_seq.GetSizeCache() == 3);
    REQUIRE(new_seq.GetFirst() == Approx(1.5));
    REQUIRE(new_seq.GetLast() == Approx(3.5));
}

TEST_CASE("LazySequence: Добавление в начало с типом double") {
    ArraySequence<double> data = {2.5, 3.5};
    LazySequence<double, ArraySequence> seq(data);
    
    auto new_seq = seq.Prepend(1.5);
    
    REQUIRE(new_seq.GetSizeCache() == 3);
    REQUIRE(new_seq.GetFirst() == Approx(1.5));
    REQUIRE(new_seq.GetLast() == Approx(3.5));
}

TEST_CASE("LazySequence: Добавление в конец с типом string") {
    ArraySequence<std::string> data = {"b", "c"};
    LazySequence<std::string, ArraySequence> seq(data);
    
    auto new_seq = seq.Append("d");
    
    REQUIRE(new_seq.GetSizeCache() == 3);
    REQUIRE(new_seq.GetFirst() == "b");
    REQUIRE(new_seq.GetLast() == "d");
}

TEST_CASE("LazySequence: Добавление в начало с типом string") {
    ArraySequence<std::string> data = {"b", "c"};
    LazySequence<std::string, ArraySequence> seq(data);
    
    auto new_seq = seq.Prepend("a");
    
    REQUIRE(new_seq.GetSizeCache() == 3);
    REQUIRE(new_seq.GetFirst() == "a");
    REQUIRE(new_seq.GetLast() == "c");
}

TEST_CASE("LazySequence: InsertAt в середину кеша") {
    ArraySequence<int> data = {1, 2, 3, 4, 5};
    LazySequence<int, ArraySequence> seq(data);

    auto new_seq = seq.InsertAt(100, 2);
    
    REQUIRE(new_seq.GetSizeCache() == 6);
    REQUIRE(new_seq.GetFirst() == 1);
    REQUIRE(new_seq.Get(2) == 100);
    REQUIRE(new_seq.GetLast() == 5);
}

TEST_CASE("LazySequence: InsertAt в начало кеша") {
    ArraySequence<int> data = {2, 3, 4};
    LazySequence<int, ArraySequence> seq(data);
    
    auto new_seq = seq.InsertAt(1, 0);
    
    REQUIRE(new_seq.GetSizeCache() == 4);
    REQUIRE(new_seq.GetFirst() == 1);
    REQUIRE(new_seq.GetLast() == 4);
}

TEST_CASE("LazySequence: InsertAt с неверным индексом") {
    ArraySequence<int> data = {1, 2, 3};
    LazySequence<int, ArraySequence> seq(data);
    
    REQUIRE_THROWS_AS(seq.InsertAt(100, 5), OutOfRangeException);
}

TEST_CASE("LazySequence: Concat двух конечных последовательностей") {
    ArraySequence<int> data1 = {1, 2, 3};
    ArraySequence<int> data2 = {4, 5, 6};
    LazySequence<int, ArraySequence> seq1(data1);
    LazySequence<int, ArraySequence> seq2(data2);
    
    auto result = seq1.Concat(seq2);
    
    REQUIRE(result.GetSizeSequence().GetSize() == 6);
    REQUIRE(result.GetSizeCache() == 0);
}

TEST_CASE("LazySequence: Concat с пустой последовательностью") {
    ArraySequence<int> data1 = {1, 2, 3};
    ArraySequence<int> data2;
    LazySequence<int, ArraySequence> seq1(data1);
    LazySequence<int, ArraySequence> seq2(data2);
    
    auto result = seq1.Concat(seq2);
    
    REQUIRE(result.GetSizeSequence().GetSize() == 3);
    REQUIRE(result.GetSizeCache() == 0);
}

TEST_CASE("LazySequence: Get с Cardinal индексом") {
    ArraySequence<int> data = {10, 20, 30, 40, 50};
    LazySequence<int, ArraySequence> seq(data);
    
    Cardinal idx = Cardinal(2);
    REQUIRE(seq.Get(idx) == 30);
}

TEST_CASE("LazySequence: Get с индексом 0") {
    ArraySequence<int> data = {100};
    LazySequence<int, ArraySequence> seq(data);
    
    Cardinal idx = Cardinal(0);
    REQUIRE(seq.Get(idx) == 100);
}

TEST_CASE("LazySequence: Get с бесконечным индексом") {
    ArraySequence<int> data = {1, 2, 3};
    LazySequence<int, ArraySequence> seq(data);
    
    Cardinal idx = Cardinal::Omega();
    REQUIRE_THROWS_AS(seq.Get(idx), OutOfRangeException);
}

TEST_CASE("LazySequence: Get с выходом за пределы") {
    ArraySequence<int> data = {1, 2, 3};
    LazySequence<int, ArraySequence> seq(data);
    
    Cardinal idx = Cardinal(5);
    REQUIRE_THROWS_AS(seq.Get(idx), OutOfRangeException);
}

TEST_CASE("LazySequence: Get с материализацией") {
    auto naturalFunc = [](const ArraySequence<int>& cache) -> int {
        return cache.GetLength();
    };
    ArraySequence<int> seed = {0};
    LazySequence<int, ArraySequence> seq(naturalFunc, seed);
    
    REQUIRE(seq.Get(Cardinal(1)) == 0);
    REQUIRE(seq.Get(Cardinal(2)) == 1);
    REQUIRE(seq.Get(Cardinal(3)) == 2);
    REQUIRE(seq.Get(Cardinal(4)) == 3);
    REQUIRE(seq.Get(Cardinal(5)) == 4);
}

TEST_CASE("LazySequence: Map преобразует элементы") {
    ArraySequence<int> data = {1, 2, 3, 4, 5};
    LazySequence<int, ArraySequence> seq(data);
    
    auto result = seq.Map<int>([](int x) { return x * 2; });
    
    REQUIRE(result.GetSizeSequence().GetSize() == 5);
    REQUIRE(result.Get(Cardinal(0)) == 2);
    REQUIRE(result.Get(Cardinal(2)) == 6);
    REQUIRE(result.Get(Cardinal(4)) == 10);
}

TEST_CASE("LazySequence: Map с изменением типа") {
    ArraySequence<int> data = {1, 2, 3};
    LazySequence<int, ArraySequence> seq(data);
    
    auto result = seq.Map<std::string>([](int x) { 
        return "Number: " + std::to_string(x); 
    });
    
    REQUIRE(result.GetSizeSequence().GetSize() == 3);
    REQUIRE(result.Get(Cardinal(0)) == "Number: 1");
    REQUIRE(result.Get(Cardinal(2)) == "Number: 3");
}

TEST_CASE("LazySequence: Map на бесконечной последовательности") {
    auto naturalFunc = [](const ArraySequence<int>& cache) -> int {
        return cache.GetLength();
    };
    ArraySequence<int> seed = {0};
    LazySequence<int, ArraySequence> seq(naturalFunc, seed);
    
    auto result = seq.Map<int>([](int x) { return x * x; });

    REQUIRE(result.GetSizeSequence().IsInfiniteNumber() == true);
    
    REQUIRE(result.Get(Cardinal(0)) == 0);
    REQUIRE(result.Get(Cardinal(1)) == 1);
    REQUIRE(result.Get(Cardinal(2)) == 4);
    REQUIRE(result.Get(Cardinal(3)) == 9);
    REQUIRE(result.Get(Cardinal(4)) == 16);
    REQUIRE(result.Get(Cardinal(5)) == 25);
}

TEST_CASE("LazySequence: Map на пустой последовательности") {
    ArraySequence<int> data;
    LazySequence<int, ArraySequence> seq(data);
    
    auto result = seq.Map<int>([](int x) { return x * 2; });
    
    REQUIRE(result.GetSizeSequence().GetSize() == 0);
    REQUIRE_THROWS_AS(result.Get(Cardinal(0)), OutOfRangeException);
}