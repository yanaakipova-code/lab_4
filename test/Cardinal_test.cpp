#include "catch.hpp"
#include "../Cardinal.hpp"
#include <string>

TEST_CASE("Cardinal: конструктор по умолчанию") {
    Cardinal c;
    
    REQUIRE(c.IsFinalNumber() == true);
    REQUIRE(c.IsInfiniteNumber() == false);
    REQUIRE(c.GetSize() == 0);
}

TEST_CASE("Cardinal: конструктор от size_t (конечное число)") {
    Cardinal c1(5);
    Cardinal c2(100);
    Cardinal c3(0);
    
    REQUIRE(c1.IsFinalNumber() == true);
    REQUIRE(c1.IsInfiniteNumber() == false);
    REQUIRE(c1.GetSize() == 5);
    
    REQUIRE(c2.GetSize() == 100);
    REQUIRE(c3.GetSize() == 0);
}

TEST_CASE("Cardinal: бесконечность") {
    Cardinal omega1 = Cardinal::Omega();
    REQUIRE(omega1.IsFinalNumber() == false);
    REQUIRE(omega1.IsInfiniteNumber() == true);
}


TEST_CASE("Cardinal: GetSize для конечных чисел") {
    Cardinal c1( 42);
    Cardinal c2( 7);
    
    REQUIRE(c1.GetSize() == 42);
    REQUIRE(c2.GetSize() == 7);
}

TEST_CASE("Cardinal: GetSize для бесконечности выбрасывает исключение") {
    Cardinal omega = Cardinal::Omega();
    
    REQUIRE_THROWS_AS(omega.GetSize(), IsInfiniteLengthException);
}

TEST_CASE("Cardinal: IsFinalNumber и IsInfiniteNumber") {
    Cardinal finite(10);
    Cardinal infinite = Cardinal::Omega(); 
    
    REQUIRE(finite.IsFinalNumber() == true);
    REQUIRE(finite.IsInfiniteNumber() == false);
    
    REQUIRE(infinite.IsFinalNumber() == false);
    REQUIRE(infinite.IsInfiniteNumber() == true);
}