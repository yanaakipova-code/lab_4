#pragma once
#include <cstddef>
#include <Error.hpp>
class Cardinal {
private:
    size_t finite = 0;
    bool is_omega = false;
public:
    Cardinal() : finite(0), is_omega(false) {}
    Cardinal(size_t size) : finite(size), is_omega(false) {}

    static Cardinal Omega() {
        Cardinal c;
        c.is_omega = true;
        c.finite = 0;
        return c;
    }

    bool IsFinalNumber() const{
        if (!is_omega) return true;
        return false;
    }
    bool IsInfiniteNumber() const{
        if(is_omega) return true;
        return false;
    }
    size_t GetSize() const {
        if (is_omega) {
            throw IsInfiniteLengthException("Cardinal.hpp");
        }
        return finite;
    }
};