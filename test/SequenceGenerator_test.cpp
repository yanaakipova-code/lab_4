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