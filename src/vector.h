#pragma once

#include <cstddef>
#include <algorithm>
#include <vector> // pavyzdžiui
#include <stdexcept>
#include <string>
#include <new>    // dėl placement new
#include <memory> // dėl std::uninitialized_..., std::destroy

// klasė šabloninė, tai lieka .h

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
    /// @brief Numatytasis konstruktorius. Sukuria tuščią vektorių.
    ///
    /// Pradinė talpa ir dydis yra lygi nuliui, atmintis nėra išskiriama.
    Vector() : data_(nullptr), size_(0), capacity_(0) {}
    // konstruktorius n elementų vektoriaus
    /// @brief Sukuria nurodyto dydžio vektorių, užpildytą numatytosiomis reikšmėmis.
    ///
    /// Atmintis rezervuojama naudojant `operator new`, o objektai sukonstruojami naudojant
    /// `std::uninitialized_value_construct_n`. Tai užtikrina, kad primityvūs tipai (pvz., `int`)
    /// bus inicializuoti nuliais, o ne atminties "šiukšlėmis".
    /// @param n Pradinis vektoriaus dydis ir talpa.
    Vector(size_type n) : size_(n), capacity_(n)
    {
        data_ = static_cast<T *>(operator new(n * sizeof(T))); // paskiriam gryną neužimtą dinaminę atmintį (BE objektų konstravimo)
        std::uninitialized_value_construct_n(data_, n);        // sukonstruojam objektus default reikšmėmis paskirtoje neužimtoje atmintyje
    }
    // konstruktorius su visų prad. elementų užpildymu elementais x
    /// @brief Sukuria vektorių ir užpildo jį nurodyto objekto kopijomis.
    /// @param n Pradinis vektoriaus dydis ir talpa.
    /// @param x Reikšmė, kuria bus užpildyti visi elementai.
    Vector(size_type n, const T &x) : size_(n), capacity_(n)
    {
        data_ = static_cast<T *>(operator new(n * sizeof(T))); // paskiriam gryną neužimtą dinaminę atmintį (BE objektų konstravimo)
        std::uninitialized_fill_n(data_, n, x);                // užpildo neužimtą atmintį reikšmėmis (objektais) x iki n-tojo (size_-tojo) elemento
    }
    // konstruktorius su inicializavimo sąrašu
    /// @brief Sukuria vektorių iš inicializavimo sąrašo (initializer_list).
    /// @param list Sąrašas elementų, kurie bus nukopijuoti į naująjį vektorių.
    Vector(std::initializer_list<T> list) : size_(list.size()), capacity_(list.size())
    {
        data_ = static_cast<T *>(operator new(list.size() * sizeof(T))); // paskiriam gryną neužimtą dinaminę atmintį (BE objektų konstravimo)
        std::uninitialized_copy(list.begin(), list.end(), data_);
    }
    // kopijavimo konstruktorius
    /// @brief Kopijavimo konstruktorius. Sukuria naują vektorių kaip kito vektoriaus kopiją.
    /// @param other Vektorius, iš kurio kopijuojami elementai.
    Vector(const Vector &other) : size_(other.size_), capacity_(other.size_)
    {
        data_ = static_cast<T *>(operator new(other.size_ * sizeof(T))); // paskiriam gryną neužimtą dinaminę atmintį (BE objektų konstravimo)
        std::uninitialized_copy(other.data_, other.data_ + size_, data_);
    }
    // perkėlimo konstruktorius
    /// @brief Perkėlimo (move) konstruktorius. Perima kito vektoriaus resursus be kopijavimo.
    ///
    /// Po šios operacijos pradinis vektorius `other` tampa tuščias (galimoje, bet neapibrėžtoje būsenoje).
    /// @param other Vektorius, kurio resursai bus pasisavinti.
    Vector(Vector &&other) noexcept : data_(other.data_), size_(other.size_), capacity_(other.size_) // be noexcept neveiks perkėlimas, vyks kopijavimas tsg
    {
        // AR TSG DESTRUKTORIUM?
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    /// @brief Destruktorius. Sunaikina visus elementus ir atlaisvina atmintį.
    ///
    /// Sunaikinimas vyksta dviem etapais: pirmiausia `std::destroy` iškviečia visų objektų
    /// destruktorius, tada `operator delete` atlaisvina pačią atmintį.
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
    /// @brief Kopijavimo priskyrimo operatorius. Pakeičia esamo vektoriaus turinį kito vektoriaus kopija.
    /// @param other Vektorius, kurio elementai bus nukopijuoti.
    /// @return Nuoroda į šį (modifikuotą) vektorių.
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
    /// @brief Perkėlimo priskyrimo operatorius. Pakeičia esamo vektoriaus turinį kito vektoriaus resursais.
    ///
    /// Senieji elementai yra sunaikinami, o senoji atmintis atlaisvinama.
    /// @param other Vektorius, kurio resursai bus pasisavinti. Po operacijos jis tampa tuščias.
    /// @return Nuoroda į šį vektorių.
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

    /// @brief Saugiai pasiekia elementą nurodytu indeksu atlikdamas ribų patikrinimą.
    ///
    /// Jei nurodytas indeksas yra už vektoriaus ribų, funkcija meta `std::out_of_range` išimtį.
    /// @param i Norimo pasiekti elemento indeksas.
    /// @return Nuoroda (reference) į nurodytą elementą.
    /// @throw std::out_of_range Jei indeksas yra didesnis arba lygus vektoriaus dydžiui (`i >= size_`).
    reference at(size_type i) // size_type — neigiamas nebus
    {
        if (i >= size_)
        {
            throw std::out_of_range("Vector::at() : Index " + std::to_string(i) + " is out of range (size is " + std::to_string(size_) + ")");
        }
        return data_[i];
    }

    /// @brief Saugiai pasiekia elementą nurodytu indeksu atlikdamas ribų patikrinimą.
    ///
    /// Jei nurodytas indeksas yra už vektoriaus ribų, funkcija meta `std::out_of_range` išimtį.
    /// @param i Norimo pasiekti elemento indeksas.
    /// @return Konstantinė nuoroda (reference) į nurodytą elementą.
    /// @throw std::out_of_range Jei indeksas yra didesnis arba lygus vektoriaus dydžiui (`i >= size_`).
    const_reference at(size_type i) const // size_type — neigiamas nebus
    {
        if (i >= size_)
        {
            throw std::out_of_range("Vector::at() const : Index " + std::to_string(i) + " is out of range (size is " + std::to_string(size_) + ")");
        }
        return data_[i];
    }

    /// @brief Pasiekia elementą nurodytu indeksu be ribų patikrinimo.
    ///
    /// Šis metodas yra greitesnis nei `at()`, nes netikrina, ar indeksas yra tinkamas.
    /// Mėginimas nurodyti i >= size() lems neapibrėžtą programos elgseną.
    /// @param i Norimo pasiekti elemento indeksas.
    /// @return Nuoroda (reference) į nurodytą elementą.
    reference operator[](size_type i)
    {
        return data_[i];
    }

    /// @brief Pasiekia elementą nurodytu indeksu be ribų patikrinimo (tik skaityti).
    ///
    /// Šis metodas yra greitesnis nei `at()`, nes netikrina, ar indeksas tinkamas.
    /// @param i Norimo pasiekti elemento indeksas.
    /// @return Konstantinė nuoroda (reference) į nurodytą elementą.
    /// @warning Jei indeksas yra už vektoriaus ribų, elgsena neapibrėžta.
    const_reference operator[](size_type i) const
    {
        return data_[i];
    }

    /// @brief Grąžina nuorodą į pirmąjį vektoriaus elementą.
    /// @return Nuoroda į pirmąjį elementą.
    /// @warning Iškvietus šį metodą tuščiam vektoriui, elgsena neapibrėžta.
    reference front()
    {
        return data_[0];
    }

    /// @brief Grąžina nuorodą į pirmąjį vektoriaus elementą (tik skaityti).
    /// @return Konstantinė nuoroda į pirmąjį elementą.
    /// @warning Iškvietus šį metodą tuščiam vektoriui, elgsena neapibrėžta.
    const_reference front() const
    {
        return data_[0];
    }

    /// @brief Grąžina nuorodą į paskutinį vektoriaus elementą.
    /// @return Nuoroda į paskutinį elementą.
    /// @warning Iškvietus šį metodą tuščiam vektoriui, elgsena neapibrėžta.
    reference back()
    {
        return data_[size_ - 1];
    }

    /// @brief Grąžina nuorodą į paskutinį vektoriaus elementą (tik skaityti).
    /// @return Konstantinė nuoroda į paskutinį elementą.
    /// @warning Iškvietus šį metodą tuščiam vektoriui, elgsena neapibrėžta.
    const_reference back() const
    {
        return data_[size_ - 1];
    }

    /// @brief Grąžina tiesioginę rodyklę į vidinį elementų masyvą.
    /// @return Rodyklė į pirmąjį masyvo elementą arba `nullptr`, jei atmintis neišskirta.
    T *data()
    {
        return data_;
    }

    // ===== iteratoriai

    /// @brief Grąžina iteratorių į pirmąjį vektoriaus elementą.
    /// @return Iteratorius, rodantis į pradžią.
    iterator begin()
    {
        return data_;
    }

    /// @brief Grąžina konstantinį iteratorių į pirmąjį vektoriaus elementą.
    /// @return Konstantinis iteratorius, rodantis į pradžią.
    const_iterator begin() const // kaipo perdengimas
    {
        return data_;
    }

    /// @brief Grąžina konstantinį iteratorių į pirmąjį vektoriaus elementą.
    ///
    /// Naudojamas, kai norima užtikrinti, kad per iteratorių duomenys nebus keičiami,
    /// net jei pats vektorius nėra konstantinis.
    /// @return Konstantinis iteratorius, rodantis į pradžią.
    const_iterator cbegin() const
    {
        return data_;
    }

    /// @brief Grąžina iteratorių į vietą už paskutinio vektoriaus elemento.
    /// @return Iteratorius, rodantis į pabaigą (už paskutinio elemento).
    iterator end()
    {
        return data_ + size_;
    }

    /// @brief Grąžina konstantinį iteratorių į vietą už paskutinio vektoriaus elemento.
    /// @return Konstantinis iteratorius, rodantis į pabaigą (už paskutinio elemento).
    const_iterator end() const // kaipo perdengimas
    {
        return data_ + size_;
    }

    /// @brief Grąžina konstantinį iteratorių į vietą už paskutinio vektoriaus elemento.
    /// @return Konstantinis iteratorius, rodantis į pabaigą (už paskutinio elemento).
    const_iterator cend() const
    {
        return data_ + size_;
    }

    /// @brief Grąžina atvirkštinį iteratorių į pirmąjį atvirkštinės sekos elementą.
    /// @return Atvirkštinis iteratorius, rodantis į paskutinį elementą.
    reverse_iterator rbegin()
    {
        return std::reverse_iterator(data_ + size_);
    }

    /// @brief Grąžina konstantinį atvirkštinį iteratorių į pirmąjį atvirkštinės sekos elementą.
    /// @return Konstantinis atvirkštinis iteratorius, rodantis į paskutinį elementą.
    const_reverse_iterator rbegin() const
    {
        return std::reverse_iterator(data_ + size_);
    }

    /// @brief Grąžina konstantinį atvirkštinį iteratorių į pirmąjį atvirkštinės sekos elementą.
    /// @return Konstantinis atvirkštinis iteratorius, rodantis į paskutinį elementą.
    const_reverse_iterator crbegin() const
    {
        return std::reverse_iterator(data_ + size_);
    }

    /// @brief Grąžina atvirkštinį iteratorių į vietą už paskutinio atvirkštinės sekos elemento.
    /// @return Atvirkštinis iteratorius, rodantis į vietą prieš pirmąjį elementą.
    reverse_iterator rend()
    {
        return std::reverse_iterator(data_);
    }

    /// @brief Grąžina konstantinį atvirkštinį iteratorių į vietą už paskutinio atvirkštinės sekos elemento.
    /// @return Konstantinis atvirkštinis iteratorius, rodantis į vietą prieš pirmąjį elementą.
    const_reverse_iterator rend() const
    {
        return std::reverse_iterator(data_);
    }

    /// @brief Grąžina konstantinį atvirkštinį iteratorių į vietą už paskutinio atvirkštinės sekos elemento.
    /// @return Konstantinis atvirkštinis iteratorius, rodantis į vietą prieš pirmąjį elementą.
    const_reverse_iterator crend() const
    {
        return std::reverse_iterator(data_);
    }

    // ===== talpa

    /// @brief Patikrina, ar vektorius yra tuščias.
    ///
    /// Tikrinimas atliekamas lyginant pradžios ir pabaigos iteratorius.
    /// @return `true`, jei `begin() == end()` (t. y. vektoriuje nėra elementų: `size() == 0`), kitu atveju `false`.
    bool empty() const
    {
        return begin() == end();
    }

    /// @brief Grąžina vektoriuje esančių elementų skaičių.
    /// @return Elementų kiekis vektoriuje.
    size_type size() const
    {
        return size_;
    }

    /// @brief Padidina vektoriaus vidinę talpą (išskirtą atmintį), kad joje tilptų nurodytas elementų kiekis.
    ///
    /// Jei nurodyta talpa (`new_cap`) yra mažesnė arba lygi dabartinei talpai (`capacity_`), funkcija nieko nedaro.
    /// Jei reikia didinti talpą, išskiriama nauja atmintis, senieji objektai perkeliami į naują vietą, o senoji atmintis atlaisvinama.
    /// @param new_cap Norimas rezervuoti atminties dydis (elementų skaičiumi).
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

    /// @brief Grąžina vektoriaus talpą.
    ///
    /// Vektoriaus talpa — elementų skaičius, kurį vektorius gali sutalpinti be papildomo atminties išskyrimo.
    /// Šis skaičius visada didesnis arba lygus `size()`.
    /// @return Esama vektoriaus atminties talpa.
    size_type capacity() const
    {
        return capacity_;
    }

    /// @brief Sumažina išskirtos atminties talpą (`capacity`), kad ji sutaptų su esamu elementų kiekiu (`size`).
    ///
    /// Funkcija naudinga siekiant optimizuoti atminties naudojimą.
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

    /// @brief Ištrina visus elementus iš vektoriaus.
    ///
    /// Visiems vektoriuje esantiems objektams saugiai iškviečiami jų destruktoriai,
    /// tačiau vidinė atmintis (talpa / `capacity`) nėra atlaisvinama.
    /// Po šios operacijos `size()` tampa `0`.
    void clear()
    {
        std::destroy(begin(), end());
        size_ = 0;
    }

    /// @brief Prideda elementą (kopijuojant) į vektoriaus pabaigą.
    ///
    /// Jei dabartinis dydis (`size_`) pasiekia talpą (`capacity_`), prieš pridedant elementą
    /// automatiškai išskiriama nauja atmintis (paprastai dvigubai didesnė). Tokiu atveju
    /// visi esami iteratoriai, rodyklės į vektoriaus elementus tampa nebegaliojantys.
    /// @param value Pridedamo elemento reikšmė (nukopijuosima).
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
        size_++;
    }

    /// @brief Prideda elementą (perkėliant) į vektoriaus pabaigą.
    ///
    /// Jei dabartinis dydis (`size_`) pasiekia talpą (`capacity_`), prieš pridedant elementą
    /// automatiškai išskiriama nauja atmintis (paprastai dvigubai didesnė). Tokiu atveju
    /// visi esami iteratoriai, rodyklės ir nuorodos į vektoriaus elementus tampa nebegaliojantys.
    /// @param value Pridedamo elemento reikšmė (perkelsima). Po perkėlimo senasis elementas liks galimoje, bet neapibrėžtoje būsenoje.
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
        size_++;
    }

    /// @brief Ištrina paskutinį vektoriaus elementą.
    ///
    /// Sunaikina paskutinį objektą iškviesdamas jo destruktorių, bet neatlaisvina vidinės atminties (`capacity_` nepakinta).
    /// Jei vektorius jau yra tuščias, funkcija nieko nedaro.
    /// Po operacijos iteratoriai, rodyklės ir nuorodos į pašalintąjį elementą ir pabaigos iteratorių (`end()`) tampa nebegaliojantys.
    void pop_back()
    {
        if (size_ > 0)
        {
            std::destroy_at(data_ + size_ - 1);
            size_--;
        }
    }

    /// @brief Įterpia elemento kopiją prieš nurodytą poziciją.
    ///
    /// Visi elementai nuo iteratoriaus `pos` iki pabaigos yra pastumiami per vieną poziciją tolyn.
    /// Jei įvyksta atminties perskirstymas, visi iteratoriai ir nuorodos tampa nebegaliojantys.
    /// Kitu atveju tampa nebegaliojantys tik iteratoriai nuo `pos` iki pabaigos.
    /// @param pos Iteratorius, prieš kurį bus įterpiamas elementas.
    /// @param value Elementas, kuris bus nukopijuotas į vektorių.
    /// @return Iteratorius, rodantis į naujai įterptą elementą.
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
    /// @brief Įterpia elementą (perkeliant) prieš nurodytą poziciją.
    ///
    /// Visi elementai nuo iteratoriaus `pos` iki pabaigos yra pastumiami per vieną poziciją tolyn.
    /// Jei įvyksta atminties perskirstymas, visi iteratoriai ir nuorodos tampa nebegaliojantys.
    /// Kitu atveju tampa nebegaliojantys tik iteratoriai nuo `pos` iki pabaigos.
    /// @param pos Iteratorius, prieš kurį bus įterpiamas elementas.
    /// @param value Elementas, kuris bus perkeltas į vektorių. Po operacijos senasis perduotasis elementas liks galimoje, bet neapibrėžtoje būsenoje.
    /// @return Iteratorius, rodantis į naujai įterptą elementą.
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
    /// @brief Įterpia nurodytą kiekį elemento kopijų prieš nurodytą poziciją.
    ///
    /// Jei įvyksta atminties perskirstymas, visi iteratoriai ir nuorodos tampa nebegaliojantys.
    /// Kitu atveju tampa nebegaliojantys tik iteratoriai nuo `pos` iki pabaigos.
    /// @param pos Iteratorius, prieš kurį bus įterpiami nauji elementai.
    /// @param count Įterpiamų elementų skaičius. Jei jis `0`, funkcija nieko nedaro.
    /// @param value Elemento reikšmė, kurios kopijos bus įterptos.
    /// @return Iteratorius, nurodantis į pirmąjį naujai įterptą elementą (arba `pos`, jei `count == 0`).
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

    /// @brief Pašalina elementą nurodytoje pozicijoje.
    ///
    /// Visi elementai po nurodytos pozicijos perkeliami per vieną vietą atgal naudojant perkėlimo (move) priskyrimą.
    /// Visi iteratoriai ir nuorodos nuo ištrynimo vietos iki vektoriaus galo tampa negaliojantys.
    /// @param pos Iteratorius į elementą, kurį reikia ištrinti.
    /// @return Iteratorius į elementą, kuris sekė po ištrintojo (arba `end()`).
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

    /// @brief Pašalina elementą nurodytoje pozicijoje.
    ///
    /// Visi elementai po nurodytos pozicijos perkeliami per vieną vietą atgal naudojant perkėlimo (move) priskyrimą.
    /// Visi iteratoriai ir nuorodos nuo ištrynimo vietos iki vektoriaus galo tampa negaliojantys.
    /// @param pos Konstantinis iteratorius į elementą, kurį reikia ištrinti.
    /// @return Iteratorius į elementą, kuris sekė po ištrintojo (arba `end()`).
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

    /// @brief Pašalina elementų rėžį [first, last).
    ///
    /// Elementai po nurodyto rėžio perkeliami į atsilaisvinusią vietą.
    /// @param first Iteratorius į rėžio pradžią (įskaitant).
    /// @param last Iteratorius į rėžio pabaigą (neįskaitant).
    /// @return Iteratorius į elementą, sekantį po paskutinio pašalinto elemento.
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

    /// @brief Pašalina elementų rėžį [first, last).
    ///
    /// Elementai po nurodyto rėžio perkeliami į atsilaisvinusią vietą.
    /// @param first Konstantinis iteratorius į rėžio pradžią (įskaitant).
    /// @param last Konstantinis iteratorius į rėžio pabaigą (neįskaitant).
    /// @return Iteratorius į elementą, sekantį po paskutinio pašalinto elemento.
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

    /// @brief Pakeičia vektoriaus dydį iki nurodyto elementų skaičiaus.
    ///
    /// Jei naujas dydis mažesnis už esamą, elementai vektoriaus gale sunaikinami.
    /// Jei naujas dydis didesnis, vektorius papildomas reikšmėmis pagal "value-initialization" (pvz., skaičiai tampa 0).
    /// Jei reikia, padidinama talpa.
    /// @param count Naujas norimas vektoriaus dydis.
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
            std::uninitialized_value_construct_n(data_ + size_, count - size_);
        }
        size_ = count;
    }

    /// @brief Pakeičia vektoriaus dydį iki nurodyto elementų skaičiaus.
    ///
    /// Jei naujas dydis mažesnis už esamą, elementai vektoriaus gale sunaikinami.
    /// Jei naujas dydis didesnis, vektorius papildomas nurodytomis reikšmėmis `value`.
    /// Jei reikia, padidinama talpa.
    /// @param count Naujas norimas vektoriaus dydis.
    /// @param value Reikšmė, kuria bus užpildyti naujai sukurti elementai.
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
            std::uninitialized_fill_n(data_ + size_, count - size_, value);
        }
        size_ = count;
    }

    /// @brief Sukeičia šio vektoriaus turinį (duomenis, dydį ir talpą) su kitu vektoriu.
    ///
    /// Operacijoje tik sukeičiamos vidinės rodyklės ir kintamieji, neatliekant elementų kopijavimo ar perkėlimo.
    /// @param other Kitas vektorius, su kuriuo bus atliekamas sukeitimas.
    void swap(Vector &other) noexcept
    {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

    // ===== lyginimo operatoriai
};

/// @brief Patikrina, ar du vektoriai yra lygūs.
///
/// Vektoriai laikomi lygiais, jei jų dydžiai (`size()`) sutampa ir visi atitinkamose pozicijose esantys elementai yra lygūs.
/// @tparam T Vektoriuose saugomų elementų tipas.
/// @param lhs Kairysis vektorius.
/// @param rhs Dešinysis vektorius.
/// @return `true`, jei vektoriai visiškai sutampa, kitu atveju `false`.
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

/// @brief Patikrina, ar du vektoriai yra nelygūs.
///
/// Operatorius atvirkščias operatoriui `operator==`.
/// @tparam T Vektoriuose saugomų elementų tipas.
/// @param lhs Kairysis vektorius.
/// @param rhs Dešinysis vektorius.
/// @return `true`, jei vektoriai skiriasi dydžiu arba bent vienu elementu, kitu atveju `false`.
template <typename T>
bool operator!=(const Vector<T> &lhs, const Vector<T> &rhs)
{
    return !(lhs == rhs);
}

/// @brief Funkcija dviejų vektorių turiniui sukeisti.
///
/// Funkcija viduje taiko Vector klasės `swap(Vector &)` metodą.
/// @tparam T Vektorių elementų tipas.
/// @param lhs Pirmasis vektorius.
/// @param rhs Antrasis vektorius.
template <typename T>
void swap(Vector<T> &lhs, Vector<T> &rhs) noexcept
{
    lhs.swap(rhs);
}

/// @brief Ištrina iš vektoriaus visus elementus, lygius nurodytai reikšmei.
/// Funkcija perkelia visus paliekamus elementus į vektoriaus priekį ir tuomet ištrina likusius elementus gale.
/// @tparam T Vektoriaus elementų tipas.
/// @tparam U Ieškomos reikšmės tipas.
/// @param vector Vektorius, iš kurio šalinami elementai.
/// @param value Reikšmė, kurios visus atitikmenis reikia ištrinti.
/// @return Bendras ištrintų elementų skaičius.
template <typename T, typename U>
constexpr size_t erase(Vector<T> &vector, const U &value)
{
    auto first_removed = std::remove(vector.begin(), vector.end(), value);
    auto removed_elems_num = vector.end() - first_removed;
    vector.erase(first_removed, vector.end());
    return removed_elems_num;
}

/// @brief Ištrina iš vektoriaus visus elementus, atitinkančius nurodytą sąlygą (predikatą).
///
/// Veikimo principas toks pat kaip `erase` funkcijos, tačiau šalinami tie elementai, kuriems predikato funkcija grąžina `true`.
/// @tparam T Vektoriaus elementų tipas.
/// @tparam Pred Predikato tipas (pvz., lambda ar paprasta funkcija).
/// @param vector Vektorius, iš kurio šalinami elementai.
/// @param pred Sąlygos funkcija. Vieną argumentą priimanti funkcija, grąžinanti `bool`.
/// @return Bendras ištrintų elementų skaičius.
template <typename T, typename Pred>
constexpr size_t erase_if(Vector<T> &vector, Pred pred)
{
    auto first_removed = std::remove_if(vector.begin(), vector.end(), pred);
    auto removed_elems_num = vector.end() - first_removed;
    vector.erase(first_removed, vector.end());
    return removed_elems_num;
}