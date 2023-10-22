#pragma once
#include <string>

class StringIterator {
public:
    explicit StringIterator(const std::string& target) noexcept;
    explicit StringIterator(const std::string::const_iterator& targetBegin,
            const std::string::const_iterator& targetEnd) noexcept;
    
    StringIterator createSubiterator() const noexcept;
    bool skipDefinition(const std::string& definition) noexcept;
    std::string createSubstring() const noexcept;
    bool skipSubiterator(const StringIterator& subiterator) noexcept;

    bool atBegin() const noexcept;
    bool atEnd() const noexcept;
    char value() const noexcept;

    void next() noexcept;
    char valueNext() noexcept;
    
private:
    std::string::const_iterator targetBegin, targetEnd;
    std::string::const_iterator targetIterator;
};
