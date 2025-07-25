#include "SortedUniqueVectoredList.h"
#include <stdexcept> // std::out_of_range
#include <utility>   // std::exchange
#include <array>
#include <algorithm>
#include <string>
#include <vector>

//------------------------------METODY_POMOCNICZE---------------------------------

void SortedUniqueVectoredList::free() {
    Bucket* current = head;

    while (current) {
        Bucket* nextBuck = current->next;
        delete current;
        current = nextBuck;
    }
    head = tail = nullptr;
    capacity_ = size_ = bucketCount_ = 0;
}

void SortedUniqueVectoredList::move(SortedUniqueVectoredList&& other) {
    head = other.head;
    tail = other.tail;
    capacity_ = other.capacity_;
    size_ = other.size_;
    bucketCount_ = other.bucketCount_;

    other.head = nullptr;
    other.tail = nullptr;
    other.capacity_ = 0;
    other.size_ = 0;
    other.bucketCount_ = 0;
}

void SortedUniqueVectoredList::copy(const SortedUniqueVectoredList& other) {
    if (&other == this)
        return;

    capacity_ = other.capacity_;
    size_ = other.size_;
    bucketCount_ = 0;

    Bucket* current = other.head;

    while (current) {
        addBucket(*current);
        current = current->next;
    }
}

void SortedUniqueVectoredList::addBucket(const Bucket& other) {
    Bucket* newBuck = new Bucket(other);
    if (!head) {
        tail = head = newBuck;
    } else {
        newBuck->previous = tail;
        tail->next = newBuck;
        tail = newBuck;
    }
    capacity_ += Bucket::BUCKET_SIZE;
    ++bucketCount_;
}

bool SortedUniqueVectoredList::contains(const std::string& value) const {
    for (const auto& buck: *this) {
        for (size_t i = 0; i < buck.size; ++i) {
            if (buck.values[i] == value)
                return true;
        }
    }
    return false;
}

void SortedUniqueVectoredList::Bucket::removeAt(size_t index) {
    if (index >= size)
        return;
    for (size_t i = index; i + 1 < size; ++i) {
        values[i] = values[i + 1];
    }
    --size;
}

//------------------------------KONSTRUKTORY---------------------------------

SortedUniqueVectoredList::SortedUniqueVectoredList() = default;

SortedUniqueVectoredList::SortedUniqueVectoredList(const SortedUniqueVectoredList& other) {
    copy(other);
}

SortedUniqueVectoredList::SortedUniqueVectoredList(SortedUniqueVectoredList&& other) noexcept {
    this->move(std::move(other));
}

//------------------------------DESTRUKTOR---------------------------------

SortedUniqueVectoredList::~SortedUniqueVectoredList() {
    free();
}

//------------------------------OPERATORY=---------------------------------

SortedUniqueVectoredList& SortedUniqueVectoredList::operator=(SortedUniqueVectoredList&& other) noexcept {
    if (this == &other)
        return *this;
    free();
    move(std::move(other));
    return *this;
}

SortedUniqueVectoredList& SortedUniqueVectoredList::operator=(const SortedUniqueVectoredList& other) {
    copy(other);
    return *this;
}

//Z POWODU TESTOW NIE MOZE BYC ZROBIONE

// SortedUniqueVectoredList& SortedUniqueVectoredList::operator=(SortedUniqueVectoredList other) {
//     swap(other);
//     return *this;
// }
//
// void SortedUniqueVectoredList::swap(SortedUniqueVectoredList& other) noexcept {
//     std::swap(capacity_, other.capacity_);
//     std::swap(size_, other.size_);
//     std::swap(bucketCount_, other.bucketCount_);
//     std::swap(head, other.head);
//     std::swap(tail, other.tail);
// }

//------------------------------OPERATORY---------------------------------
SortedUniqueVectoredList::operator std::string() const {
    std::string res = "";
    for (auto& buck: *this)
        for (size_t i = 0; i < buck.size; ++i)
            res += buck.values[i];
    return res;
}

std::string& SortedUniqueVectoredList::operator[](size_t index) {
    if (index >= size_)
        throw std::out_of_range("Invalid index");

    size_t bucketIndex = index / Bucket::BUCKET_SIZE;
    size_t stringIndex = index % Bucket::BUCKET_SIZE;

    Bucket* current = head;
    for (size_t i = 0; i < bucketIndex; ++i) {
        current = current->next;
    }
    return current->values[stringIndex];
}

const std::string& SortedUniqueVectoredList::operator[](size_t index) const {
    if (index >= size_)
        throw std::out_of_range("Invalid index");

    size_t bucketIndex = index / Bucket::BUCKET_SIZE;
    size_t stringIndex = index % Bucket::BUCKET_SIZE;

    Bucket* current = head;
    for (size_t i = 0; i < bucketIndex; ++i) {
        current = current->next;
    }

    return current->values[stringIndex];
}

SortedUniqueVectoredList SortedUniqueVectoredList::operator-(const SortedUniqueVectoredList& other) const {
    SortedUniqueVectoredList res;
    for (const auto& buck: *this) {
        for (size_t i = 0; i < buck.size; ++i) {
            if (!other.contains(buck.values[i]))
                res.insert(buck.values[i]);
        }
    }
    return res;
}

SortedUniqueVectoredList& SortedUniqueVectoredList::operator*=(const size_t howManyTimesMultiply) {
    for (auto& buck: *this) {
        for (size_t i = 0; i < buck.size; ++i) {
            std::string newStr = "";
            newStr.reserve(buck.values[i].length() * howManyTimesMultiply);
            for (size_t j = 0; j < howManyTimesMultiply; ++j) {
                newStr += buck.values[i];
            }
            buck.values[i] = std::move(newStr);
        }
    }
    return *this;
}

std::ostream& operator<<(std::ostream& stream, const SortedUniqueVectoredList& container) {
    //explicit nie pozwala zrobic std::string str = container;
    stream << static_cast<std::string>(container);
    return stream;
}

//------------------------------INSERT---------------------------------

void SortedUniqueVectoredList::insert(const std::string& value) {
    if (contains(value))
        return;
    std::vector<std::string> all;
    all.reserve(size_ + 1);
    for (auto& buck: *this)
        for (size_t i = 0; i < buck.size; ++i)
            all.push_back(buck.values[i]);

    all.push_back(value);
    std::sort(all.begin(), all.end());

    free();
    for (auto& s: all) {
        if (!tail || tail->size == Bucket::BUCKET_SIZE)
            addBucket(Bucket(s));
        else
            tail->values[tail->size++] = s;
    }
    size_ = all.size();
    capacity_ = bucketCount_ * Bucket::BUCKET_SIZE;
}

//------------------------------ERASE---------------------------------

void SortedUniqueVectoredList::erase(const std::string& value) {
    Bucket* current = head;
    while (current) {
        for (size_t i = 0; i < current->size; ++i) {
            if (current->values[i] == value) {
                current->removeAt(i);
                --size_;
                if (current == tail && current->size == 0) {
                    if (tail != head) {
                        tail = tail->previous;
                        tail->next = nullptr;
                    } else {
                        head = tail = nullptr;
                    }
                    delete current;
                    --bucketCount_;
                }
                return;
            }
        }
        current = current->next;
    }
}
