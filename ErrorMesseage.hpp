#pragma once
#include <cstddef>
#include <format>
#include <string>

template<typename T>
class Exception{
private:
    std::string m_message; 
    T m_value;
public:
    Exception(const std::string& message) : m_message(message), m_value() {}
    Exception(const std::string& message, const T& value) : m_message(message), m_value(value) {}
    virtual ~Exception() = default;
    virtual const char* what() const {
        return m_message.c_str();
    }
};

class OutOfRangeException : public Exception<size_t> {
public:
    OutOfRangeException(size_t index) : Exception<size_t>( std::format("Индекс за массивом, индекс больше размера на: {}", index), index) {}
};

class IsInfiniteLengthException: public Exception<std::string>{
public: 
    IsInfiniteLengthException(const std::string& file): Exception("Длина равна бесконечности в файле: "  + file, file) {}
};