#pragma once

#include <cstddef>
#include <algorithm>
#include <vector> // pavyzdžiui
#include <stdexcept>
#include <string>
#include <new>    // dėl placement new
#include <memory> // dėl std::uninitialized_..., std::destroy

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
        data_ = static_cast<T *>(operator new(n * sizeof(T))); // paskiriam gryną neužimtą dinaminę atmintį (BE objektų konstravimo)
        std::uninitialized_default_construct_n(data_, n);      // sukonstruojam objektus default reikšmėmis paskirtoje neužimtoje atmintyje
    }
    // konstruktorius su visų prad. elementų užpildymu elementais x
    Vector(size_type n, const T &x) : size_(n), capacity_(n)
    {
        data_ = static_cast<T *>(operator new(n * sizeof(T))); // paskiriam gryną neužimtą dinaminę atmintį (BE objektų konstravimo)
        std::uninitialized_fill_n(data_, n, x);                // užpildo neužimtą atmintį reikšmėmis (objektais) x iki n-tojo (size_-tojo) elemento
    }
    // konstruktorius su inicializavimo sąrašu
    Vector(std::initializer_list<T> list) : size_(list.size()), capacity_(list.size())
    {
        data_ = static_cast<T *>(operator new(list.size() * sizeof(T))); // paskiriam gryną neužimtą dinaminę atmintį (BE objektų konstravimo)
        std::uninitialized_copy(list.begin(), list.end(), data_);
    }
    // kopijavimo konstruktorius
    Vector(const Vector &other) : size_(other.size_), capacity_(other.size_)
    {
        data_ = static_cast<T *>(operator new(other.size_ * sizeof(T))); // paskiriam gryną neužimtą dinaminę atmintį (BE objektų konstravimo)
        std::uninitialized_copy(other.data_, other.data_ + size_, data_);
    }
    // perkėlimo konstruktorius
    Vector(Vector &&other) noexcept : data_(other.data_), size_(other.size_), capacity_(other.size_) // be noexcept neveiks perkėlimas, vyks kopijavimas tsg
    {
        // AR TSG DESTRUKTORIUM?
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    ~Vector()
    {
        std::destroy(data_, data_ + size_); // iškviečia visų masyvo data_ objektų destruktorius
        size_ = 0;
        capacity_ = 0;
        operator delete(data_); // atlaisvina data_ atmintį
        data_ = nullptr;
    }

    // ===== priskyrimo operatoriai

    // kopijavimo priskyrimo operatorius
    Vector &operator=(const Vector &other)
    {
        if (this == &other)
            return *this;

        T *temp;

        if (other.data_) // tikrinam, kad nereiktų priskirt new T[0] ar kopijuot nullptr turinį
        {
            temp = static_cast<T *>(operator new(other.capacity_ * sizeof(T)));
            std::uninitialized_copy(other.data_, other.data_ + other.size_, temp);
        }
        else
        {
            temp = nullptr;
        }

        std::destroy(data_, data_ + size_); // operator delete pats nekviečia destruktorių, dėl to turim juos išsikviest patys prieš laisvinant atmintį
        operator delete(data_);             // ištrinam PO atminties (galimo) priskyrimo, kad jeigu netyčia nepavyktų jos paskirt, duomenys nepradingtų
        data_ = temp;
        size_ = other.size_;
        capacity_ = other.capacity_;

        return *this;
    }

    // perkėlimo priskyrimo operatorius
    Vector &operator=(Vector &&other) noexcept // be noexcept neveiks perkėlimas, vyks kopijavimas tsg
    {
        if (this == &other)
            return *this;

        std::destroy(data_, data_ + size_);
        operator delete(data_);

        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;

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

    bool empty() const
    {
        return begin() == end();
    }

    size_type size() const
    {
        return size_;
    }

    void reserve(size_type new_cap)
    {
        if (new_cap <= capacity_)
            return; // taip elgiasi std::vector::reserve — jei prašoma rezervuoti mažiau nei dab. capacity_, tai funkcija nieko nedaro

        T *new_alloc = static_cast<T *>(operator new(new_cap * sizeof(T)));
        std::uninitialized_move(begin(), end(), new_alloc);

        std::destroy(begin(), end()); // net ir po move, objektai nėra sunaikinti
        operator delete(data_);

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

        T *new_alloc = static_cast<T *>(operator new(size_ * sizeof(T)));
        std::uninitialized_move(begin(), end(), new_alloc);

        std::destroy(begin(), end());
        operator delete(data_);

        data_ = new_alloc;
        capacity_ = size_;
    }

    // ===== keitimo/modifikavimo metodai

    void clear()
    {
        std::destroy(begin(), end());
        size_ = 0;
    }

    void push_back(const T &value)
    {
        if (size_ == capacity_)
        {
            if (capacity_ == 0)
                reserve(1);
            else
                reserve(capacity_ * 2);
        }

        std::construct_at(data_ + size_, value);
    }

    void push_back(T &&value)
    {
        if (size_ == capacity_)
        {
            if (capacity_ == 0)
                reserve(1);
            else
                reserve(capacity_ * 2);
        }

        std::construct_at(data_ + size_, std::move(value));
    }

    void pop_back()
    {
        if (size_ > 0)
        {
            std::destroy_at(data_ + size_ - 1);
            size_--;
        }
    }

    iterator insert(const_iterator pos, const T &value) // value — const, tai negalėsim std::move(value)
    {
        size_type idx = pos - begin();

        if (size_ == capacity_)
        {
            if (capacity_ == 0)
                reserve(1);
            else
                reserve(capacity_ * 2); // po reserve, iteratorius pos nebegalioja
        }

        if (idx == size_)
        {
            std::construct_at(data_ + size_, value);
        }
        else
        {
            std::construct_at(data_ + size_, std::move(data_[size_ - 1])); // paskutinis elementas vienintelis turi būti perkeltas nenaudojamon atmintin, taigi čia padarom atskirai
            std::move_backward(data_ + idx, data_ + size_ - 1, data_ + size_);
            data_[idx] = value;
        }
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
                reserve(capacity_ * 2); // po reserve, iteratorius pos nebegalioja
        }

        if (idx == size_)
        {
            std::construct_at(data_ + size_, std::move(value));
        }
        else
        {
            std::construct_at(data_ + size_, std::move(data_[size_ - 1])); // paskutinis elementas vienintelis turi būti perkeltas nenaudojamon atmintin, taigi čia padarom atskirai
            std::move_backward(data_ + idx, data_ + size_ - 1, data_ + size_);
            data_[idx] = std::move(value);
        }
        size_++;

        return begin() + idx;
    }
    iterator insert(const_iterator pos, size_type count, const T &value)
    {
        size_type idx = pos - begin();

        if (count == 0)
            return begin() + idx; // pos — const, tai gal neleis grąžint jo tiesiogiai (grąžina ne const)

        size_type new_size = size_ + count;
        if (new_size > capacity_)
        {
            if (capacity_ == 0)
            {
                reserve(count);
            }
            else
            {
                size_type new_cap = capacity_;
                while (new_cap < new_size)
                    new_cap *= 2;
                reserve(new_cap);
            }
        }

        if (idx == size_)
        {
            std::uninitialized_fill_n(data_ + size_, count, value); // naudojam tai, o ne push_back(), kad išvengt nebereikalingų capacity_ patikrų (push_back kas kart tai tikrina, bet mes gi jau patikrinom)
        }
        else
        {
            if (size_ - idx > count) // jei įterpsimų elementų sk. yra mažesnis už pastumsimų elementų skaičių (pvz. prieš 3-ąjį nuo galo elementą įterpiant 1-2 elementus)
            {
                // čia taip perkeliam tuos elementus, kurie atsidurs DAR NELIESTOJ atminty
                std::uninitialized_move(data_ + size_ - count, data_ + size_, data_ + size_);
                // čia taip perkeliam tuos elementus, kurie atsidurs JAU LIESTOJ atminty
                std::move_backward(data_ + idx, data_ + size_ - count, data_ + size_);
                // čia pagaliau įterpiam count skaičių elementų value prieš buvusį idx-tąjį elementą (buvusį adresu pos)
                std::fill(data_ + idx, data_ + idx + count, value);
            }
            else // jei įterpsimų elementų sk. yra lygus/didesnis už pastumsimų elementų skaičių (pvz., prieš 3-ąjį nuo galo elementą įterpiant 3+ elementus)
            {
                // čia taip įterpiam tuos elementus value, kurie atsidurs DAR NELIESTOJ atminty
                std::uninitialized_fill_n(data_ + size_, idx + count - size_, value); // jeigu size_ - idx == count, tai čia niekas neįvyks
                // čia taip perkeliam perkeltinus elementus (dar nelieston atmintin)
                std::uninitialized_move(data_ + idx, data_ + size_, data_ + idx + count);
                // čia taip įterpiam tuos elementus value, kurie atsidurs JAU LIESTOJ atminty
                std::fill(data_ + idx, data_ + size_, value);
            }
        }

        size_ = new_size;

        return begin() + idx;
    }

    iterator erase(iterator pos)
    {
        size_type idx = pos - begin();
        for (size_type i = idx; i < size_ - 1; i++)
        {
            data_[i] = std::move(data_[i + 1]);
        }
        std::destroy_at(data_ + size_ - 1); // ištrinam paskutinį elementą (dabar jis dubliuojas su priešpaskutiniuoju)
        size_--;
        return begin() + idx; // == pos
    }

    iterator erase(const_iterator pos)
    {
        size_type idx = pos - begin();
        for (size_type i = idx; i < size_ - 1; i++)
        {
            data_[i] = std::move(data_[i + 1]);
        }
        std::destroy_at(data_ + size_ - 1); // ištrinam paskutinį elementą (dabar jis dubliuojas su priešpaskutiniuoju)
        size_--;
        return begin() + idx; // == pos
    }

    iterator erase(iterator first, iterator last)
    {
        size_type first_idx = first - begin();
        size_type count = last - first;

        if (count == 0)
            return begin() + first_idx; // == first

        for (size_type i = first_idx; i < size_ - count; i++)
        {
            data_[i] = std::move(data_[i + count]);
        }
        std::destroy(data_ + (size_ - count), data_ + size_); // ištrinam paskutinius count elementų (dbr jie dubliuojas, nebereikalingi)

        size_ -= count;

        return begin() + first_idx;
    }

    iterator erase(const_iterator first, const_iterator last)
    {
        size_type first_idx = first - begin();
        size_type count = last - first;

        if (count == 0)
            return begin() + first_idx; // == first

        for (size_type i = first_idx; i < size_ - count; i++)
        {
            data_[i] = std::move(data_[i + count]);
        }
        std::destroy(data_ + (size_ - count), data_ + size_); // ištrinam paskutinius count elementų (dbr jie dubliuojas, nebereikalingi)

        size_ -= count;

        return begin() + first_idx;
    }

    void resize(size_type count)
    {
        if (count == size_)
        {
            return;
        }
        if (count < size_)
        {
            std::destroy(data_ + count, data_ + size_);
        }
        else
        {
            if (capacity_ < count)
            {
                size_type new_cap = capacity_;
                while (new_cap < count)
                    new_cap *= 2;
                reserve(new_cap);
            }
            std::uninitialized_default_construct_n(data_ + size_, count);
        }
        size_ = count;
    }

    void resize(size_type count, const T &value)
    {
        if (count == size_)
        {
            return;
        }
        if (count < size_)
        {
            std::destroy(data_ + count, data_ + size_);
        }
        else
        {
            if (capacity_ < count)
            {
                size_type new_cap = capacity_;
                while (new_cap < count)
                    new_cap *= 2;
                reserve(new_cap);
            }
            std::uninitialized_fill_n(data_ + size_, count, value);
        }
        size_ = count;
    }

    void swap(Vector &other) noexcept
    {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

    // ===== lyginimo operatoriai
};

template <typename T>
bool operator==(const Vector<T> &lhs, const Vector<T> &rhs)
{
    if (lhs.size() != rhs.size())
        return false;
    for (size_t i = 0; i < lhs.size(); i++)
    {
        if (lhs[i] != rhs[i])
            return false;
    }
    return true;
}

template <typename T>
bool operator!=(const Vector<T> &lhs, const Vector<T> &rhs)
{
    return !(lhs == rhs);
}

template <typename T>
void swap(Vector<T> &lhs, Vector<T> &rhs) noexcept
{
    lhs.swap(rhs);
}

template <typename T, typename U>
constexpr size_t erase(Vector<T> &vector, const U &value)
{
    auto first_removed = std::remove(vector.begin(), vector.end(), value);
    auto removed_elems_num = vector.end() - first_removed;
    vector.erase(first_removed, vector.end());
    return removed_elems_num;
}

template <typename T, typename Pred>
constexpr size_t erase_if(Vector<T> &vector, Pred pred)
{
    auto first_removed = std::remove_if(vector.begin(), vector.end(), pred);
    auto removed_elems_num = vector.end() - first_removed;
    vector.erase(first_removed, vector.end());
    return removed_elems_num;
}