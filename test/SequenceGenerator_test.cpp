#include "catch.hpp"
#include "../SequenceGenerator.hpp"
#include "../Cardinal.hpp"
#include "lab2-3/ArraySequence.hpp"
#include "lab2-3/Error.hpp"

TEST_CASE("SequenceGenerator конструктор по умолчанию"){
    SequenceGenerator<int, ArraySequence> gen;

    REQUIRE(gen.HasNext() == false);
    REQUIRE(gen.GetSize() == 0);
    REQUIRE(gen.GetPotentialSize().GetSize() == 0);
    REQUIRE_THROWS_AS(gen.GetNext(), OutOfRangeException);
}

TEST_CASE("SequenceGenerator: конструктор из ArraySequence") {
   ArraySequence<int> seq = {10, 20, 30};

    SequenceGenerator<int, ArraySequence> gen(seq);
    
    REQUIRE(gen.HasNext() == true);
    REQUIRE(gen.GetSize() == 0);
    REQUIRE(gen.GetPotentialSize().GetSize() == 3);
}

TEST_CASE("SequenceGenerator: GetSize возвращает количество выданных элементов") {
    ArraySequence<int> seq = {1,2,3,4,5};
    
    SequenceGenerator<int, ArraySequence> gen(seq);
    
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

TEST_CASE("SequenceGenerator: GetPotentialSize возвращает общий размер") {
    ArraySequence<int> seq = {10, 20, 30};
    
    SequenceGenerator<int, ArraySequence> gen(seq);
    
    REQUIRE(gen.GetPotentialSize().GetSize() == 3);
}

TEST_CASE("SequenceGenerator: GetPotentialSize не меняется после GetNext") {
    ArraySequence<int> seq = {10, 20, 30};
    
    SequenceGenerator<int, ArraySequence> gen(seq);
    
    REQUIRE(gen.GetPotentialSize().GetSize() == 3);
    gen.GetNext();
    REQUIRE(gen.GetPotentialSize().GetSize() == 3);
    gen.GetNext();
    REQUIRE(gen.GetPotentialSize().GetSize() == 3);
    gen.GetNext();
    REQUIRE(gen.GetPotentialSize().GetSize() == 3);
}

TEST_CASE("SequenceGenerator: GetPotentialSize и GetSize возвращают разные вещи") {
    ArraySequence<int> seq = {1, 2, 3};
    
    SequenceGenerator<int, ArraySequence> gen(seq);
    
    REQUIRE(gen.GetPotentialSize().GetSize() == 3);
    REQUIRE(gen.GetSize() == 0);
    gen.GetNext();
    REQUIRE(gen.GetSize() == 1);
}

TEST_CASE("SequenceGenerator: GetNext возвращает элементы по порядку") {
    ArraySequence<int> seq = {100, 200, 300, 400};
    
    SequenceGenerator<int, ArraySequence> gen(seq);
    
    REQUIRE(gen.GetNext() == 100);
    REQUIRE(gen.GetSize() == 1);
    REQUIRE(gen.GetNext() == 200);
    REQUIRE(gen.GetSize() == 2);
    REQUIRE(gen.GetNext() == 300);
    REQUIRE(gen.GetSize() == 3);
    REQUIRE(gen.GetNext() == 400);
    REQUIRE(gen.GetSize() == 4);
}

TEST_CASE("SequenceGenerator: GetNext выбрасывает исключение после конца") {
    ArraySequence<int> seq = {1, 2};
    
    SequenceGenerator<int, ArraySequence> gen(seq);
    
    REQUIRE(gen.GetNext() == 1);
    REQUIRE(gen.GetNext() == 2);
    REQUIRE_THROWS_AS(gen.GetNext(), OutOfRangeException);
}

TEST_CASE("SequenceGenerator: GetNext для одного элемента") {
    ArraySequence<int> seq = {15};
    
    SequenceGenerator<int, ArraySequence> gen(seq);
    
    REQUIRE(gen.HasNext() == true);
    REQUIRE(gen.GetNext() == 15);
    REQUIRE(gen.HasNext() == false);
    REQUIRE_THROWS_AS(gen.GetNext(), OutOfRangeException);
}

TEST_CASE("SequenceGenerator: HasNext правильно определяет наличие элементов") {
    ArraySequence<int> seq = {10, 20, 30};
    
    SequenceGenerator<int, ArraySequence> gen(seq);
    
    REQUIRE(gen.HasNext() == true);
    gen.GetNext();
    REQUIRE(gen.HasNext() == true);
    gen.GetNext();
    REQUIRE(gen.HasNext() == true);
    gen.GetNext();
    REQUIRE(gen.HasNext() == false);
}

TEST_CASE("SequenceGenerator: HasNext не меняет состояние") {
    ArraySequence<int> seq = {10, 20, 30};
    
    SequenceGenerator<int, ArraySequence> gen(seq);
    
    REQUIRE(gen.HasNext() == true);
    REQUIRE(gen.HasNext() == true);
    REQUIRE(gen.HasNext() == true);
    REQUIRE(gen.GetSize() == 0);
}

TEST_CASE("SequenceGenerator: HasNext для пустой последовательности") {
    ArraySequence<int> seq;
    SequenceGenerator<int, ArraySequence> gen(seq);
    
    REQUIRE(gen.HasNext() == false);
    REQUIRE(gen.HasNext() == false);
}

TEST_CASE("SequenceGenerator: Append добавляет элемент в конец") {
    ArraySequence<int> seq = {1, 2, 3};
    SequenceGenerator<int, ArraySequence> gen(seq);
    
    auto new_gen = gen.Append(4);
    
    REQUIRE(gen.GetNext() == 1);
    REQUIRE(gen.GetNext() == 2);
    REQUIRE(gen.GetNext() == 3);
    REQUIRE(gen.HasNext() == false);
    
    REQUIRE(new_gen.GetNext() == 1);
    REQUIRE(new_gen.GetNext() == 2);
    REQUIRE(new_gen.GetNext() == 3);
    REQUIRE(new_gen.GetNext() == 4);
    REQUIRE(new_gen.HasNext() == false);
}

TEST_CASE("SequenceGenerator: Append к пустой последовательности") {
    ArraySequence<int> seq;
    SequenceGenerator<int, ArraySequence> gen(seq);
    
    auto new_gen = gen.Append(42);
    
    REQUIRE(gen.HasNext() == false);
    
    REQUIRE(new_gen.HasNext() == true);
    REQUIRE(new_gen.GetNext() == 42);
    REQUIRE(new_gen.HasNext() == false);
}

TEST_CASE("SequenceGenerator: несколько Append подряд") {
    ArraySequence<int> seq = {1};
    SequenceGenerator<int, ArraySequence> gen(seq);
    
    auto gen2 = gen.Append(2);
    auto gen3 = gen2.Append(3);
    auto gen4 = gen3.Append(4);
    
    REQUIRE(gen4.GetNext() == 1);
    REQUIRE(gen4.GetNext() == 2);
    REQUIRE(gen4.GetNext() == 3);
    REQUIRE(gen4.GetNext() == 4);
    REQUIRE(gen4.HasNext() == false);
    
    REQUIRE(gen3.GetNext() == 1);
    REQUIRE(gen3.GetNext() == 2);
    REQUIRE(gen3.GetNext() == 3);
    REQUIRE(gen3.HasNext() == false);
}


TEST_CASE("SequenceGenerator: Append") {
    ArraySequence<int> original = {1, 2, 3};
    ArraySequence<int> to_append = {4, 5, 6};
    
    SequenceGenerator<int, ArraySequence> gen(original);
    auto new_gen = gen.Append(&to_append);

    REQUIRE(gen.GetNext() == 1);
    REQUIRE(gen.GetNext() == 2);
    REQUIRE(gen.GetNext() == 3);
    REQUIRE(gen.HasNext() == false);
    
    REQUIRE(new_gen.GetNext() == 1);
    REQUIRE(new_gen.GetNext() == 2);
    REQUIRE(new_gen.GetNext() == 3);
    REQUIRE(new_gen.GetNext() == 4);
    REQUIRE(new_gen.GetNext() == 5);
    REQUIRE(new_gen.GetNext() == 6);
    REQUIRE(new_gen.HasNext() == false);
}

TEST_CASE("SequenceGenerator: к пустой исходной последовательности и обычной") {
    ArraySequence<int> original;
    ArraySequence<int> to_append = {100, 200, 300};
    
    SequenceGenerator<int, ArraySequence> gen(original);
    auto new_gen = gen.Append(&to_append);
    
    REQUIRE(gen.HasNext() == false);
    
    REQUIRE(new_gen.GetNext() == 100);
    REQUIRE(new_gen.GetNext() == 200);
    REQUIRE(new_gen.GetNext() == 300);
    REQUIRE(new_gen.HasNext() == false);
}

TEST_CASE("SequenceGenerator: несколько Append подряд с подпосл") {
    ArraySequence<int> start = {1, 2};
    ArraySequence<int> app1 = {3, 4};
    ArraySequence<int> app2 = {5, 6};
    
    SequenceGenerator<int, ArraySequence> gen(start);
    auto gen2 = gen.Append(&app1);
    auto gen3 = gen2.Append(&app2);
    
    REQUIRE(gen3.GetNext() == 1);
    REQUIRE(gen3.GetNext() == 2);
    REQUIRE(gen3.GetNext() == 3);
    REQUIRE(gen3.GetNext() == 4);
    REQUIRE(gen3.GetNext() == 5);
    REQUIRE(gen3.GetNext() == 6);
    REQUIRE(gen3.HasNext() == false);
}

TEST_CASE("SequenceGenerator: Insert одного элемента в начало") {
    ArraySequence<int> original = {2, 3, 4};
    SequenceGenerator<int, ArraySequence> gen(original);
    
    auto new_gen = gen.Insert(1);
    
    REQUIRE(new_gen.GetNext() == 1);
    REQUIRE(new_gen.GetNext() == 2);
    REQUIRE(new_gen.GetNext() == 3);
    REQUIRE(new_gen.GetNext() == 4);
    REQUIRE(new_gen.HasNext() == false);
}

TEST_CASE("SequenceGenerator: Insert одного элемента в пустую последовательность") {
    ArraySequence<int> original;
    SequenceGenerator<int, ArraySequence> gen(original);
    
    auto new_gen = gen.Insert(42);
    
    REQUIRE(new_gen.GetNext() == 42);
    REQUIRE(new_gen.HasNext() == false);
}

TEST_CASE("SequenceGenerator: Insert последовательности в начало") {
    ArraySequence<int> original = {4, 5, 6};
    ArraySequence<int> to_insert = {1, 2, 3};
    
    SequenceGenerator<int, ArraySequence> gen(original);
    auto new_gen = gen.Insert(&to_insert);
    
    REQUIRE(new_gen.GetNext() == 3);
    REQUIRE(new_gen.GetNext() == 2);
    REQUIRE(new_gen.GetNext() == 1);
    REQUIRE(new_gen.GetNext() == 4);
    REQUIRE(new_gen.GetNext() == 5);
    REQUIRE(new_gen.GetNext() == 6);
    REQUIRE(new_gen.HasNext() == false);
}

TEST_CASE("SequenceGenerator: Insert последовательности в пустую последовательность") {
    ArraySequence<int> original;
    ArraySequence<int> to_insert = {10, 20, 30};
    
    SequenceGenerator<int, ArraySequence> gen(original);
    auto new_gen = gen.Insert(&to_insert);
    
    REQUIRE(new_gen.GetNext() == 30);
    REQUIRE(new_gen.GetNext() == 20);
    REQUIRE(new_gen.GetNext() == 10);
    REQUIRE(new_gen.HasNext() == false);
}

TEST_CASE("SequenceGenerator: Insert не изменяет оригинальный генератор") {
    ArraySequence<int> original = {1, 2, 3};
    ArraySequence<int> to_insert = {10, 20};
    
    SequenceGenerator<int, ArraySequence> gen(original);
    auto new_gen = gen.Insert(&to_insert);

    REQUIRE(gen.GetNext() == 1);
    REQUIRE(gen.GetNext() == 2);
    REQUIRE(gen.GetNext() == 3);
    REQUIRE(gen.HasNext() == false);

    REQUIRE(new_gen.GetNext() == 20);
    REQUIRE(new_gen.GetNext() == 10);
    REQUIRE(new_gen.GetNext() == 1);
    REQUIRE(new_gen.GetNext() == 2);
    REQUIRE(new_gen.GetNext() == 3);
    REQUIRE(new_gen.HasNext() == false);
}
