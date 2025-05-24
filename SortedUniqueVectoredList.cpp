#include <stdexcept> // std::out_of_range
#include <utility>   // std::exchange
#include <array>
#include <limits>
#include <algorithm>
#include <string>
#include <stdexcept>

#if __has_include("../SortedUniqueVectoredList.h")
    #include "../SortedUniqueVectoredList.h"
#elif __has_include("SortedUniqueVectoredList.h")
#include "SortedUniqueVectoredList.h"
#else
    #error "File 'SortedUniqueVectoredList.h' not found!"
#endif


/** class SortedUniqueVectoredList::Bucket
 * @param size ilosc elementow w kubelku, tworzac pusty ma byc 0
 * @param values elementy kubelka, jako tablica statyczna
 * @param BUCKET_SIZE ilosc elementow w statycznej tablicy
 * @param bucketCount_ ilosc kubelkow
 * @param next wskaznik na nastepny @ref Bucket, a jesli takiego nie ma na nullptr
 * @param previous wskaznik na poprzedni @ref Bucket, a jesli takiego nie ma na nullptr
 * @note jest to klasa zrobiona przy pomocy [idiomu PIMPL](https://en.cppreference.com/w/cpp/language/pimpl),
 *       ktory polega na tym, ze w klasie zewnetrznej jest jedynie deklaracja klasy wewnetrznej, ktora jest zaimplementowana w pliku zrodlowym **/

void SortedUniqueVectoredList::Bucket::removeAt(size_t index) {
    if (index >= size)
        return;

    for (size_t i = index; i + 1 < size; ++i) {
        values[i] = values[i + 1];
    }
    --size;
}

SortedUniqueVectoredList::SortedUniqueVectoredList(const SortedUniqueVectoredList& source)
    : size_(source.size_), capacity_(source.capacity_) {
    Bucket* current = source.head;

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


SortedUniqueVectoredList::SortedUniqueVectoredList(SortedUniqueVectoredList&& other) noexcept {
    this->move(std::move(other));
}

SortedUniqueVectoredList::~SortedUniqueVectoredList() {
    free();
}

SortedUniqueVectoredList& SortedUniqueVectoredList::operator=(SortedUniqueVectoredList&& another) {
    /// @todo zaimplementuj, szczegoly w pliku naglowkowym
    if (this != &another) {
        this->move(std::move(another));
    }
    return *this;
}

void SortedUniqueVectoredList::insert(const std::string& value) {
    for (auto& buck: *this)
        for (size_t i = 0; i < buck.size; ++i)
            if (buck.values[i] == value)
                return;
    if (!tail || tail->size >= Bucket::BUCKET_SIZE) {
        Bucket* newBuck = new Bucket(value);
        addBucket(*newBuck);
    } else {
        tail->values[tail->size++] = value;
    }
    ++size_;
}

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


SortedUniqueVectoredList::operator std::string() const {
    std::string res = "";
    for (auto& buck: *this)
        for (size_t i = 0; i < buck.size; ++i)
            res += buck.values[i];
    return res;
}

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
    other.head = nullptr;

    tail = other.tail;
    other.tail = nullptr;

    capacity_ = other.capacity_;
    size_ = other.size_;
    bucketCount_ = other.bucketCount_;
    other.size_ = other.capacity_ = other.bucketCount_ = 0;
}

void SortedUniqueVectoredList::copy(const SortedUniqueVectoredList& other) {
    if (&other == this)
        return;
}

bool SortedUniqueVectoredList::contains(const std::string& value) const {
    /// @todo zaimplementuj, szczegoly w pliku naglowkowym
    return false;
}

SortedUniqueVectoredList SortedUniqueVectoredList::operator-(const SortedUniqueVectoredList& another) const {
    /// @todo zaimplementuj, szczegoly w pliku naglowkowym
    SortedUniqueVectoredList ret;

    return ret;
}

SortedUniqueVectoredList& SortedUniqueVectoredList::operator*=(const size_t howManyTimesMultiply) {
    /// @todo zaimplementuj, szczegoly w pliku naglowkowym
    return *this;
}

std::string& SortedUniqueVectoredList::operator[](size_t index) {
    /// @todo zaimplementuj, szczegoly w pliku naglowkowym

    throw std::out_of_range("");
}

const std::string& SortedUniqueVectoredList::operator[](size_t index) const {
    /// @todo zaimplementuj, szczegoly w pliku naglowkowym

    throw std::out_of_range("");
}

SortedUniqueVectoredList& SortedUniqueVectoredList::operator=(SortedUniqueVectoredList other) {
    swap(other);
    return *this;
}

void SortedUniqueVectoredList::swap(SortedUniqueVectoredList& other) {
    std::swap(capacity_, other.capacity_);
    std::swap(size_, other.size_);
    std::swap(bucketCount_, other.bucketCount_);
    std::swap(head, other.head);
    std::swap(tail, other.tail);
}

