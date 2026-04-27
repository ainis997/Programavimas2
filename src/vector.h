#include <cstddef>
#include <algorithm>
#include <vector> // pavyzdžiui

template <typename T>
class Vector
{
public:
    // TIPAI (MEMBER TYPES)

    using value_type = T;
    // allocator — atskiro nedarysma
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    using reference = value_type &;
    using const_reference = const value_type &;

    using pointer = value_type *; // būtų allocator čia, bet jei jo nedarom, tai taip
    using const_pointer = const value_type *;

    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
    T *data_;
    size_type size_;
    size_type capacity_;

public:
    // METODAI (MEMBER FUNCTIONS)

    // konstruktoriai

    Vector() : data_(nullptr), size_(0), capacity_(0) {}
    Vector(size_type n) : size_(n), capacity_(n)
    {
        data_ = new T[n](); // () — daro value-initialization visiem nariam (int: 0, float: 0.0, objektam: default konstruktoriai)
    }
    Vector(size_type n, const T &x) : size_(n), capacity(n)
    {
        data_ = new T[n];
        std::fill(data_, data_ + n, x); // užpildo masyvą reikšmėmis x iki size_-tojo elemento
    }
    Vector(std::initializer_list<T> list) : size_(list.size()), capacity_(list.size())
    {
        data_ = new T[n];
        std::copy(list.begin(), list.end(), data_);
    }
};