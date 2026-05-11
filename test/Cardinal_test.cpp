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

TEST_CASE("Cardinal: оператор == ") {
    SECTION("два конечных равны") {
        Cardinal a(42);
        Cardinal b(42);
        REQUIRE(a == b);
    }
    
    SECTION("два конечных не равны") {
        Cardinal a(5);
        Cardinal b(10);
        REQUIRE_FALSE(a == b);
    }
    
    SECTION("две бесконечности") {
        Cardinal w1 = Cardinal::Omega();
        Cardinal w2 = Cardinal::Omega();
        REQUIRE(w1 == w2);
    }
    
    SECTION("конечное и бесконечное") {
        Cardinal finite(5);
        Cardinal omega = Cardinal::Omega();
        REQUIRE_FALSE(finite == omega);
        REQUIRE_FALSE(omega == finite);
    }
}

TEST_CASE("Cardinal: оператор <") {
    SECTION("5 < 10") {
        Cardinal a(5);
        Cardinal b(10);
        REQUIRE(a < b);
        REQUIRE_FALSE(b < a);
    }
    
    SECTION("5 < 5") {
        Cardinal a(5);
        Cardinal b(5);
        REQUIRE_FALSE(a < b);
    }
    
    SECTION("5 < бесконечность") {
        Cardinal finite(5);
        Cardinal omega = Cardinal::Omega();
        REQUIRE(finite < omega);
    }
    
    SECTION("бесконечность < 5") {
        Cardinal finite(5);
        Cardinal omega = Cardinal::Omega();
        REQUIRE_FALSE(omega < finite);
    }
    
    SECTION("бесконечность < бесконечность") {
        Cardinal w1 = Cardinal::Omega();
        Cardinal w2 = Cardinal::Omega();
        REQUIRE_FALSE(w1 < w2);
    }
}