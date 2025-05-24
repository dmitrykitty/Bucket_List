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







SortedUniqueVectoredList::SortedUniqueVectoredList(const SortedUniqueVectoredList& source) {
    /// @todo zaimplementuj, szczegoly w pliku naglowkowym
    copy(source);
}

SortedUniqueVectoredList::SortedUniqueVectoredList(SortedUniqueVectoredList&& another) {
    /// @todo zaimplementuj, szczegoly w pliku naglowkowym
    this->move(std::move(another));
}

SortedUniqueVectoredList::~SortedUniqueVectoredList() {
    /// @todo zaimplementuj, szczegoly w pliku naglowkowym
    free();
}

SortedUniqueVectoredList& SortedUniqueVectoredList::operator=(SortedUniqueVectoredList&& another) {
    /// @todo zaimplementuj, szczegoly w pliku naglowkowym
    if (this != &another) {
        this->move(std::move(another));
    }
    return *this;
}

void SortedUniqueVectoredList::insert(const string& value) {
    /// @todo zaimplementuj, szczegoly w pliku naglowkowym
}

void SortedUniqueVectoredList::erase(const string& value) {
    /// @todo zaimplementuj, szczegoly w pliku naglowkowym (opcjonalne zadanie)
}

SortedUniqueVectoredList::operator std::string() const {
    /// @todo zaimplementuj, szczegoly w pliku naglowkowym
    return {};
}

void SortedUniqueVectoredList::allocate_new_bucket() {
    /// @todo zaimplementuj, szczegoly w pliku naglowkowym

    capacity_ += Bucket::BUCKET_SIZE;
    bucketCount_++;
}

void SortedUniqueVectoredList::free() {
    /// @todo zaimplementuj, szczegoly w pliku naglowkowym
}

void SortedUniqueVectoredList::move(SortedUniqueVectoredList&& another) {
    /// @todo zaimplementuj, szczegoly w pliku naglowkowym
}

void SortedUniqueVectoredList::copy(const SortedUniqueVectoredList& other) {
    if (&other == this)
        return;
}

bool SortedUniqueVectoredList::contains(const string& value) const {
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

string& SortedUniqueVectoredList::operator[](size_t index) {
    /// @todo zaimplementuj, szczegoly w pliku naglowkowym

    throw std::out_of_range("");
}

const string& SortedUniqueVectoredList::operator[](size_t index) const {
    /// @todo zaimplementuj, szczegoly w pliku naglowkowym

    throw std::out_of_range("");
}

SortedUniqueVectoredList& SortedUniqueVectoredList::operator=(const SortedUniqueVectoredList& another) {
    /// @todo zaimplementuj, szczegoly w pliku naglowkowym
    return *this;
}
