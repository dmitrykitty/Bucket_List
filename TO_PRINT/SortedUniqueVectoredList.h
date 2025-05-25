#ifndef SORTEDUNIQUEVECTOREDLIST_H
#define SORTEDUNIQUEVECTOREDLIST_H

#include <array>
#include <cstddef>   // std::size_t
#include <string>


class SortedUniqueVectoredList {
    //------------------------------BUCKET---------------------------------

    struct Bucket {
        constexpr static size_t BUCKET_SIZE = 10;
        std::array<std::string, BUCKET_SIZE> values;
        size_t size{};
        Bucket* next = nullptr;
        Bucket* previous = nullptr;

        Bucket(const Bucket& other): values(other.values), size(other.size) {}
        Bucket(const std::string& str): size(1) {
            values[0] = str;
        }

        Bucket() = default;
        void removeAt(size_t index);
    };
    //------------------------------PRIVATE_FIELDS---------------------------------

    Bucket *head = nullptr, *tail = nullptr;
    size_t bucketCount_{}, size_{}, capacity_{};

public:
    //------------------------------ITERATOR---------------------------------
    class Iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using pointer = Bucket*;
        using reference = Bucket&;
        using value = Bucket;
        using difference_type = std::ptrdiff_t; //typ dla roznicy dwoch wskaznikow
        //aktualnie tylko dla RandomAccess i wyzej

    private:
        Bucket* buck_ptr{};
    public:
        Iterator() = default;
        Iterator(Bucket* ptr): buck_ptr(ptr) {}
        //copy konstruktor, destruktor, operator= -> default

        reference operator*() const { return *buck_ptr; }
        pointer operator->() const { return buck_ptr; };

        Iterator& operator++() {
            buck_ptr = buck_ptr->next;
            return *this;
        }

        Iterator& operator--() {
            buck_ptr = buck_ptr->previous;
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            buck_ptr = buck_ptr->next;
            return temp;
        }

        Iterator operator--(int) {
            Iterator temp = *this;
            buck_ptr = buck_ptr->previous;
            return temp;
        }

        bool operator==(const Iterator& other) const { return buck_ptr == other.buck_ptr; }
        bool operator!=(const Iterator& other) const { return buck_ptr != other.buck_ptr; }
    };

    Iterator begin() const { return {head}; }
    Iterator end() const { return {nullptr}; }

protected:
    //------------------------------METODY_POMOCNICZE---------------------------------
    void addBucket(const Bucket& other);
    void free();
    void move(SortedUniqueVectoredList&& another);
    void swap(SortedUniqueVectoredList& other) noexcept;
    void copy(const SortedUniqueVectoredList& other);
    bool contains(const std::string& value) const;

public:
    //------------------------------CSTR, DSTR---------------------------------

    SortedUniqueVectoredList();;
    SortedUniqueVectoredList(const SortedUniqueVectoredList& source);
    SortedUniqueVectoredList(SortedUniqueVectoredList&& source) noexcept;
    ~SortedUniqueVectoredList();

    //------------------------------GETTERS---------------------------------

    auto size() const { return size_; }
    auto capacity() const { return capacity_; }
    auto bucket_count() const { return bucketCount_; }

    //------------------------------OPERATORS---------------------------------

    SortedUniqueVectoredList& operator=(const SortedUniqueVectoredList& another);
    SortedUniqueVectoredList& operator=(SortedUniqueVectoredList&& another) noexcept;
    std::string& operator[](std::size_t index);
    const std::string& operator[](std::size_t index) const;
    SortedUniqueVectoredList operator-(const SortedUniqueVectoredList& another) const;
    SortedUniqueVectoredList& operator*=(std::size_t howManyTimesMultiply);
    explicit operator std::string() const;
    friend std::ostream& operator<<(std::ostream& stream, const SortedUniqueVectoredList& container);

    //------------------------------ERASE, INSERT---------------------------------

    void insert(const std::string& value);
    void erase(const std::string& value);
};
#endif // SORTED_UNIQUE_VECTORED_LIST_H
