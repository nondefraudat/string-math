#include "stringiterator.hpp"

using namespace std;

StringIterator::StringIterator(const std::string &target) noexcept
        : targetBegin(target.cbegin()), targetEnd(target.cend()) {
    targetIterator = targetBegin;
}

StringIterator::StringIterator(const std::string::const_iterator &targetBegin,
        const std::string::const_iterator &targetEnd) noexcept
        : targetBegin(targetBegin), targetEnd(targetEnd) {
    targetIterator = this->targetBegin;
}

StringIterator StringIterator::createSubiterator() const noexcept {
    return StringIterator(targetIterator, targetEnd);
}

bool StringIterator::skipDefinition(const std::string &definition) noexcept {
    StringIterator definitionIterator(definition);
    StringIterator subiterator = createSubiterator();
    while (!definitionIterator.atEnd() && !subiterator.atEnd()) {
        if (definitionIterator.value() != subiterator.value()) {
            return false;
        }
        definitionIterator.next();
        subiterator.next();
    }
    if (!definitionIterator.atEnd()) {
        return false;
    }
    targetIterator = subiterator.targetIterator;
    return true;
}

std::string StringIterator::createSubstring() const noexcept {
    string substring("");
    for (string::const_iterator tempIterator = targetBegin;
            tempIterator != targetIterator; tempIterator++) {
        substring.push_back(*tempIterator);
    }
    return substring;
}

bool StringIterator::skipSubiterator(const StringIterator &subiterator) noexcept {
    if (subiterator.targetEnd != targetEnd) {
        return false;
    }
    targetIterator = subiterator.targetIterator;
    return true;
}

bool StringIterator::atBegin() const noexcept {
    return targetIterator == targetBegin;
}

bool StringIterator::atEnd() const noexcept
{
    return targetIterator == targetEnd;
}

char StringIterator::value() const noexcept {
    return *targetIterator;
}

void StringIterator::next() noexcept {
    if (!atEnd()) {
        targetIterator++;
    }
}

char StringIterator::valueNext() noexcept {
    if (atEnd()) {
        return '\0';
    }
    const char symbol = value();
    next();
    return symbol;
}
