#pragma once

#include <cstddef>
#include <algorithm>
#include <vector> // pavyzdžiui
#include <stdexcept>
#include <string>

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

    // ===== konstruktoriai

    // numatytasai konstruktorius
    Vector() : data_(nullptr), size_(0), capacity_(0) {}
    // konstruktorius n elementų vektoriaus
    Vector(size_type n) : size_(n), capacity_(n)
    {
        data_ = new T[n](); // () — daro value-initialization visiem nariam (int: 0, float: 0.0, objektam: default konstruktoriai)
    }
    // konstruktorius su visų prad. elementų užpildymu elementais x
    Vector(size_type n, const T &x) : size_(n), capacity(n)
    {
        data_ = new T[n];
        std::fill(data_, data_ + n, x); // užpildo masyvą reikšmėmis x iki size_-tojo elemento
    }
    // konstruktorius su inicializavimo sąrašu
    Vector(std::initializer_list<T> list) : size_(list.size_), capacity_(list.size_)
    {
        data_ = new T[n];
        std::copy(list.begin(), list.end(), data_);
    }
    // kopijavimo konstruktorius
    Vector(const Vector &other) : size_(other.size_), capacity_(other.size_)
    {
        data_ = new T[capacity_];
        std::copy(other.data_, other.data_ + other.size_, data_);
    }
    // perkėlimo konstruktorius
    Vector(Vector &&other) : data_(other.data_), size_(other.size_), capacity_(other.size_)
    {
        // AR TSG DESTRUKTORIUM?
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    ~Vector()
    {
        delete[] data_;
        data_ = nullptr;
        size_ = 0;
        capacity_ = 0;
    }

    // ===== priskyrimo operatoriai

    // kopijavimo priskyrimo operatorius
    Vector &operator=(const Vector &other)
    {
        if (this == &other)
            return *this;

        delete[] data_;
        size_ = other.size_;
        capacity_ = other.capacity_;

        if (other.data_) // tikrinam, kad nereiktų priskirt new T[0] ar kopijuot nullptr turinį
        {
            data_ = new T[capacity_];
            std::copy(other.data_, other.data_ + other.size_, data_);
        }
        else
        {
            data_ = nullptr;
        }

        return *this;
    }

    // perkėlimo priskyrimo operatorius
    Vector &operator=(Vector &&other)
    {
        if (this == &other)
            return *this;

        delete[] data_;

        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;

        delete[] other.data_;
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;

        return *this;
    }

    // ===== elementų pasiekimo metodai

    reference at(size_type i) // size_type — neigiamas nebus
    {
        if (i >= size_)
        {
            throw std::out_of_range("Vector::at() : Index " + std::to_string(i) + " is out of range (size is " + std::to_string(size_) + ")");
        }
        return data_[i];
    }

    const_reference at(size_type i) const // size_type — neigiamas nebus
    {
        if (i >= size_)
        {
            throw std::out_of_range("Vector::at() const : Index " + std::to_string(i) + " is out of range (size is " + std::to_string(size_) + ")");
        }
        return data_[i];
    }

    reference operator[](size_type i)
    {
        return data_[i];
    }

    const_reference operator[](size_type i) const
    {
        return data_[i];
    }

    reference front()
    {
        return data_[0];
    }

    const_reference front() const
    {
        return data_[0];
    }

    reference back()
    {
        return data_[size_ - 1];
    }

    const_reference back() const
    {
        return data_[size_ - 1];
    }

    T *data()
    {
        return data_;
    }

    // ===== iteratoriai

    iterator begin()
    {
        return data_;
    }

    const_iterator begin() const // kaipo perdengimas
    {
        return data_;
    }

    const_iterator cbegin() const
    {
        return data_;
    }

    iterator end()
    {
        return data_ + size_;
    }

    const_iterator end() const // kaipo perdengimas
    {
        return data_ + size_;
    }

    const_iterator cend() const
    {
        return data_ + size_;
    }

    reverse_iterator rbegin()
    {
        return std::reverse_iterator(data_);
    }

    const_reverse_iterator rbegin() const
    {
        return std::reverse_iterator(data_);
    }

    const_reverse_iterator crbegin() const
    {
        return std::reverse_iterator(data_);
    }

    reverse_iterator rend()
    {
        return std::reverse_iterator(data_ + size_);
    }

    const_reverse_iterator rend() const
    {
        return std::reverse_iterator(data_ + size_);
    }

    const_reverse_iterator crend() const
    {
        return std::reverse_iterator(data_ + size_);
    }

    // ===== talpa

    bool empty()
    {
        return begin() == end();
    }

    size_type size()
    {
        return size_;
    }

    void reserve(size_type new_cap)
    {
        if (new_cap <= capacity_)
            return;

        T *new_alloc = new T[new_cap];
        std::move(begin(), end(), new_alloc);
        delete[] data_;
        data_ = new_alloc;

        capacity_ = new_cap;
    }

    size_type capacity() const
    {
        return capacity_;
    }

    void shrink_to_fit()
    {
        if (capacity_ == size_)
            return;

        T *new_alloc = new T[size_];
        std::move(begin(), end(), new_alloc);
        delete[] data_;
        data_ = new_alloc;

        capacity_ = size_;
    }

    // ===== keitimo/modifikavimo metodai

    // !!!!!!!!!!!!!!!!!! ?????????????????
    void clear()
    {
        size_ = 0; // kadangi mūsų realizacija yra ne su atskiru allocatorium, o tsg su pointeriais, tai mes negalim PAPRASTAI ištrint masyvo elementų, bet tuo pačiu palikt atmintį (atminties skyrimas ir objektų gyvavimas neatskiri)
    }

    iterator insert(const_iterator pos, const T &value)
    {
        size_type idx = pos - begin();

        if (size_ == capacity_)
        {
            if (capacity_ == 0)
                reserve(1);
            else
                reserve(capacity_ * 2); // po reserve, iteratorius pos nebegalioja
        }

        for (size_type i = size_; i > idx; i--)
        {
            data_[i] = std::move(data_[i - 1]);
        }
        data_[idx] = value;
        size_++;

        return begin() + idx;
    }
    iterator insert(const_iterator pos, T &&value)
    {
        size_type idx = pos - begin();

        if (size_ == capacity_)
        {
            if (capacity_ == 0)
                reserve(1);
            else
                reserve(capacity_ * 2);
        }

        for (size_type i = size_; i > idx; i--)
        {
            data[i] = std::move(data_[i - 1]);
        }
        data[idx] = value;
        size_++;

        return begin() + idx;
    }
    iterator insert(const_iterator pos, size_type count, const T &value)
    {
        if (count == 0)
            return pos;

        size_type idx = pos - begin();

        size_type new_size = size_ + count;
        if (new_size > capacity_)
        {
            if (capacity_ == 0)
            {
                reserve(count);
            }
            else
            {
                unsigned int multiplicator = 1 + ((new_size - 1) / capacity_); // kad sužinot, kiek kartų didint capacity_, darom lubinę dalybą new_size / capacity_
                reserve(capacity_ * multiplicator);
            }
        }

        for (size_type i = size_ - 1; i >= idx; i--) // pastumiam elementus, kurie toliau nei pos, tolyn
        {
            data[i + count] = std::move(data_[i]);
        }
        for (size_type i = count - 1; i >= 0; i--) // įterpiam count skaičių elementų value prieš buvusį pos-tąjį elementą
        {
            data[idx + i] = std::move(value);
        }
        size_ = new_size;

        return begin() + idx;
    }
};