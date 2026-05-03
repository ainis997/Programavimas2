#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../src/vector.h"

#include <memory>
#include <string>
#include <iterator> // reik dėl std::distance

// naudosim destruktoriaus tikrinimui
class ObjectCounter
{
public:
    static inline int count; // be inline reiktų atskirai deklaruot išorėj šį kintamąjį
    ObjectCounter() { count++; }
    ~ObjectCounter() { count--; }
};

TEST_CASE("Vektoriaus konstruktoriai ir destruktorius veikia tinkamai")
{

    SECTION("Numatytasis konstruktorius tinkamai sukuria tuščią vektorių")
    {
        Vector<int> v;
        REQUIRE(v.size() == 0);
        REQUIRE(v.capacity() == 0);
        REQUIRE(v.empty() == true);
    }

    SECTION("Konstruktorius su pirminių elementų sk. parametru veikia tinkamai")
    {
        Vector<int> v(5);
        REQUIRE(v.size() == 5);
        REQUIRE(v.capacity() == 5);

        // int default reikšmė yra 0, kai konstruojama per uninitialized_default_construct
        for (size_t i = 0; i < 5; i++)
        {
            REQUIRE(v[i] == 0);
        }
    }

    SECTION("Konstruktorius, sukuriantis tam tikrą sk. tam tikros vertės elementų, veikia tinkamai")
    {
        Vector<int> v(5, 42);
        REQUIRE(v.size() == 5);
        REQUIRE(v.capacity() == 5);

        for (size_t i = 0; i < 5; i++)
        {
            REQUIRE(v[i] == 42);
        }
    }

    SECTION("Konstruktorius su inicializavimo sąrašu veikia tinkamai")
    {
        Vector<int> v = {10, 20, 30, 40};
        REQUIRE(v.size() == 4);
        REQUIRE(v.capacity() == 4);
        REQUIRE(v[0] == 10);
        REQUIRE(v[3] == 40);
    }

    SECTION("Kopijavimo konstruktorius veikia tinkamai")
    {
        Vector<int> pirminis = {1, 2, 3};
        Vector<int> kopija(pirminis);

        REQUIRE(kopija.size() == 3);
        REQUIRE(kopija.capacity() == 3);
        REQUIRE(kopija[0] == 1);
        REQUIRE(kopija[2] == 3);

        // pakeičiam pirminis, kad įsitikint, kad atmintis nėr bendra
        pirminis[0] = 99;
        REQUIRE(kopija[0] == 1);
    }

    SECTION("Perkėlimo konstruktorius veikia tinkamai")
    {
        Vector<int> pirminis = {7, 8, 9};
        Vector<int> ikeltas(std::move(pirminis));

        REQUIRE(ikeltas.size() == 3);
        REQUIRE(ikeltas.capacity() == 3);
        REQUIRE(ikeltas[0] == 7);
        REQUIRE(ikeltas[2] == 9);

        // patikrinam senąjį vektorių
        REQUIRE(pirminis.size() == 0);
        REQUIRE(pirminis.capacity() == 0);
        REQUIRE(pirminis.data() == nullptr);
    }

    SECTION("Destruktorius veikia tinkamai")
    {
        // atskirų elementų-objektų tikrinimas
        ObjectCounter::count = 0;
        {
            Vector<ObjectCounter> vector(3);
            REQUIRE(ObjectCounter::count == 3);
        }
        REQUIRE(ObjectCounter::count == 0);

        // viso vektoriaus tikrinimas
        std::weak_ptr<Vector<int>> ptr_stebejimui;
        {
            std::shared_ptr<Vector<int>> ptr(new Vector<int>);
            ptr_stebejimui = ptr;
            REQUIRE(!ptr_stebejimui.expired());
        }
        REQUIRE(ptr_stebejimui.expired());
    }
}

TEST_CASE("Priskyrimo operatoriai veikia teisingai")
{

    SECTION("Kopijavimo priskyrimas sukuria deep copy")
    {
        Vector<int> v1 = {1, 2, 3};
        Vector<int> v2;

        v2 = v1; // iškviečia kopijavimo priskyrimą

        REQUIRE(v2.size() == 3);
        REQUIRE(v2.capacity() == v1.capacity());
        REQUIRE(v2[0] == 1);
        REQUIRE(v2[2] == 3);

        // deep copy patikrinimas: keičiam originalą, kopija neturi keistis
        v1[0] = 99;
        REQUIRE(v2[0] == 1);
    }

    SECTION("Kopijavimo priskyrimas priskiriant pačiam sau (self-assignment)")
    {
        Vector<int> v = {4, 5, 6};
        v = v; // neturi nulūžt ar prarast duomenų
        REQUIRE(v.size() == 3);
        REQUIRE(v[1] == 5);
    }

    SECTION("Kopijavimo priskyrimas sunaikina senus elementus (nėra atminties nutekėjimo)")
    {
        ObjectCounter::count = 0;
        {
            Vector<ObjectCounter> v1(5); // sukuria 5 objektus
            Vector<ObjectCounter> v2(2); // sukuria 2 objektus
            REQUIRE(ObjectCounter::count == 7);

            // v2 senieji 2 objektai turi būti sunaikinti, ir nukopijuoti 5 nauji iš v1
            v2 = v1;
            REQUIRE(ObjectCounter::count == 10); // 5(v1) + 5(v2 kopijos)
        }
        REQUIRE(ObjectCounter::count == 0);
    }

    SECTION("Perkėlimo priskyrimas perkelia resursus ir išvalo šaltinį")
    {
        Vector<int> v1 = {7, 8, 9};
        Vector<int> v2 = {1, 2};

        v2 = std::move(v1);

        REQUIRE(v2.size() == 3);
        REQUIRE(v2[0] == 7);
        REQUIRE(v2[2] == 9);

        REQUIRE(v1.size() == 0);
        REQUIRE(v1.capacity() == 0);
        REQUIRE(v1.data() == nullptr);
    }

    SECTION("Perkėlimo priskyrimas priskiriant pačiam sau (self-assignment)")
    {
        Vector<int> v = {10, 11, 12};
        v = std::move(v);
        REQUIRE(v.size() == 3);
        REQUIRE(v[0] == 10);
    }

    SECTION("Perkėlimo priskyrimas sunaikina senus elementus")
    {
        ObjectCounter::count = 0;
        {
            Vector<ObjectCounter> v1(4);
            Vector<ObjectCounter> v2(3);
            REQUIRE(ObjectCounter::count == 7);

            // v2 senieji 3 objektai turi pradingt, v2 perima 4 objektus iš v1, v1 lieka su 0
            v2 = std::move(v1);
            REQUIRE(ObjectCounter::count == 4); // liko tik tie 4, kurie dabar yra pas v2
        }
        REQUIRE(ObjectCounter::count == 0);
    }
}

TEST_CASE("Elementų pasiekimo metodai veikia teisingai")
{

    SECTION("at() grąžina elementus ir meta klaidą")
    {
        Vector<int> v = {10, 20, 30};

        REQUIRE(v.at(0) == 10);
        REQUIRE(v.at(2) == 30);

        v.at(1) = 25;
        REQUIRE(v.at(1) == 25);

        // tikrinam išimtis
        REQUIRE_THROWS_AS(v.at(3), std::out_of_range);
        REQUIRE_THROWS_AS(v.at(100), std::out_of_range);
    }

    SECTION("at() const versija meta klaidą")
    {
        const Vector<int> v = {1, 2};
        REQUIRE(v.at(1) == 2);
        REQUIRE_THROWS_AS(v.at(2), std::out_of_range);
    }

    SECTION("operator[] grąžina teisingus elementus ir leidžia keisti")
    {
        Vector<int> v = {5, 6, 7};
        REQUIRE(v[0] == 5);

        v[2] = 77;
        REQUIRE(v[2] == 77);
    }

    SECTION("front() ir back() grąžina pirmą ir paskutinį elementus")
    {
        Vector<int> v = {100, 200, 300};

        REQUIRE(v.front() == 100);
        REQUIRE(v.back() == 300);

        v.front() = 101;
        v.back() = 301;

        REQUIRE(v[0] == 101);
        REQUIRE(v[2] == 301);
    }

    SECTION("front() ir back() const versijos veikia su const objektais")
    {
        const Vector<int> v = {9, 8, 7};
        REQUIRE(v.front() == 9);
        REQUIRE(v.back() == 7);
    }

    SECTION("data() grąžina tiesioginę rodyklę į elementų masyvą")
    {
        Vector<int> v = {42, 43, 44};
        int *ptr = v.data();

        REQUIRE(ptr != nullptr);
        REQUIRE(ptr[0] == 42);
        REQUIRE(ptr[2] == 44);

        // pakeitus per rodyklę, turi pasikeist vektoriaus turinys
        ptr[1] = 99;
        REQUIRE(v[1] == 99);
    }
}

TEST_CASE("Iteratoriai veikia tinkamai")
{

    SECTION("begin() ir end() leidžia pravaryt per elementus ir juos modifikuot")
    {
        Vector<int> v = {10, 20, 30};

        REQUIRE(*v.begin() == 10);
        // end() rodo į VIENĄ elementą už paskutiniojo, todėl atimame 1, kad gautume paskutinį
        REQUIRE(*(v.end() - 1) == 30);

        // Modifikuojame elementus naudodami iteratorius
        for (auto it = v.begin(); it != v.end(); it++)
        {
            *it += 5;
        }

        REQUIRE(v[0] == 15);
        REQUIRE(v[1] == 25);
        REQUIRE(v[2] == 35);
    }

    SECTION("const begin() ir end() veikia su const objektais (tik skaitymui)")
    {
        const Vector<int> v = {1, 2, 3};

        REQUIRE(*v.begin() == 1);
        REQUIRE(*(v.end() - 1) == 3);

        // atstumas tarp iteratorių turi atitikt vektoriaus dydį
        REQUIRE(std::distance(v.begin(), v.end()) == 3);
    }

    SECTION("cbegin() ir cend() užtikrina const iteratorių grąžinimą net ir ne-const objektams")
    {
        Vector<int> v = {4, 5, 6};

        REQUIRE(*v.cbegin() == 4);
        REQUIRE(*(v.cend() - 1) == 6);

        int suma = 0;
        for (auto it = v.cbegin(); it != v.cend(); it++)
        {
            suma += *it;
        }
        REQUIRE(suma == 15);
    }

    SECTION("rbegin() ir rend() leidžia pravaryt atvirkščia tvarka ir modifikuot")
    {
        Vector<int> v = {100, 200, 300};

        // rbegin() grąžina PASKUTINĮ elementą
        REQUIRE(*v.rbegin() == 300);
        // rend() yra vienas elementas PRIEŠ pirmąjį
        REQUIRE(*(v.rend() - 1) == 100);

        int skaitliukas = 1;
        for (auto it = v.rbegin(); it != v.rend(); it++)
        {
            *it = skaitliukas++;
        }

        // vektorius po pakeitimo turi būt {3, 2, 1}
        REQUIRE(v[0] == 3);
        REQUIRE(v[1] == 2);
        REQUIRE(v[2] == 1);
    }

    SECTION("crbegin() ir crend() veikia atvirkščiai tik skaitymui")
    {
        Vector<int> v = {7, 8, 9};

        REQUIRE(*v.crbegin() == 9);
        REQUIRE(*(v.crend() - 1) == 7);

        Vector<int> atvirksciai;
        for (auto it = v.crbegin(); it != v.crend(); it++)
        {
            atvirksciai.push_back(*it);
        }

        REQUIRE(atvirksciai.size() == 3);
        REQUIRE(atvirksciai[0] == 9);
        REQUIRE(atvirksciai[1] == 8);
        REQUIRE(atvirksciai[2] == 7);
    }
}

TEST_CASE("Talpos metodai veikia tinkamai")
{

    SECTION("empty() metodas veikia tinkamai")
    {
        Vector<int> v_tuscias;
        REQUIRE(v_tuscias.empty() == true);

        Vector<int> v_netuscias(5, 10); // 5 elementai, kurių reikšmė 10
        REQUIRE(v_netuscias.empty() == false);
    }

    SECTION("reserve() padidina talpą, bet nekeičia dydžio ar elementų")
    {
        Vector<int> v = {1, 2, 3};
        REQUIRE(v.size() == 3);
        REQUIRE(v.capacity() == 3);

        v.reserve(10);

        REQUIRE(v.size() == 3);
        REQUIRE(v.capacity() == 10);
        REQUIRE(v.empty() == false);

        // elementai turi atlikt savo vietose
        REQUIRE(v[0] == 1);
        REQUIRE(v[2] == 3);
    }

    SECTION("reserve() nieko nedaro, jei prašoma talpa mažesnė arba lygi esamai")
    {
        Vector<int> v = {1, 2, 3, 4, 5};
        v.reserve(10);
        REQUIRE(v.capacity() == 10);

        v.reserve(3);
        REQUIRE(v.capacity() == 10);

        v.reserve(10);
        REQUIRE(v.capacity() == 10);
    }

    SECTION("reserve() teisingai perkelia elementus ir atlaisvina senus (be nutekėjimų)")
    {
        ObjectCounter::count = 0;

        Vector<ObjectCounter> v(4);
        REQUIRE(ObjectCounter::count == 4);

        v.reserve(8);

        REQUIRE(ObjectCounter::count == 4);
        REQUIRE(v.capacity() == 8);
        REQUIRE(v.size() == 4);
    }

    SECTION("shrink_to_fit() sumažina talpą iki elementų skaičiaus")
    {
        Vector<int> v = {10, 20, 30};
        v.reserve(100);

        REQUIRE(v.size() == 3);
        REQUIRE(v.capacity() == 100);

        v.shrink_to_fit();

        REQUIRE(v.size() == 3);
        REQUIRE(v.capacity() == 3); // talpa turi susitraukt ligi pat size()

        // duomenys turi atlikt
        REQUIRE(v[0] == 10);
        REQUIRE(v[1] == 20);
        REQUIRE(v[2] == 30);
    }

    SECTION("shrink_to_fit() nieko nedaro, jei size() ir capacity() jau sutampa")
    {
        Vector<int> v = {5, 15, 25};
        REQUIRE(v.size() == 3);
        REQUIRE(v.capacity() == 3);

        int *senas_ptr = v.data();
        v.shrink_to_fit();

        REQUIRE(v.capacity() == 3);
        REQUIRE(v.data() == senas_ptr); // rodyklė neturi pasikeist (neturėjo būti jokių reallocationų)
    }
}

TEST_CASE("Modifikavimo metodai (clear, push_back, pop_back) veikia tinkamai")
{

    SECTION("clear() sunaikina elementus ir dydis tampa 0, bet talpa išlieka")
    {
        ObjectCounter::count = 0;
        {
            Vector<ObjectCounter> v(5);
            REQUIRE(ObjectCounter::count == 5);

            size_t sena_talpa = v.capacity();
            v.clear();

            REQUIRE(v.size() == 0);
            REQUIRE(v.capacity() == sena_talpa); // talpa neturi keistis
            REQUIRE(ObjectCounter::count == 0);  // visų objektų turi nelikt
        }
    }

    SECTION("push_back(const T& value) ir push_back(T&& value) veikia tinkamai")
    {
        Vector<int> v;
        REQUIRE(v.capacity() == 0);

        v.push_back(10); // R-value
        REQUIRE(v.size() == 1);
        REQUIRE(v.capacity() == 1);

        int val = 20;
        v.push_back(val); // L-value
        REQUIRE(v.size() == 2);
        REQUIRE(v.capacity() == 2);

        v.push_back(30);
        REQUIRE(v.size() == 3);
        REQUIRE(v.capacity() == 4);

        REQUIRE(v[0] == 10);
        REQUIRE(v[1] == 20);
        REQUIRE(v[2] == 30);
    }

    SECTION("pop_back() pašalina paskutinį elementą ir jį sunaikina")
    {
        ObjectCounter::count = 0;
        {
            Vector<ObjectCounter> v(3);
            REQUIRE(ObjectCounter::count == 3);

            v.pop_back();
            REQUIRE(v.size() == 2);
            REQUIRE(ObjectCounter::count == 2);

            v.pop_back();
            v.pop_back();
            REQUIRE(v.size() == 0);
            REQUIRE(ObjectCounter::count == 0);

            // pop_back ant tuščio vektoriaus (turi nebūt klaidos)
            v.pop_back();
            REQUIRE(v.size() == 0);
        }
    }
}

TEST_CASE("insert() metodai teisingai iterpia elementus ir valdo atminti")
{

    SECTION("insert(pos, value) įterpia elementą bet kurion vieton")
    {
        Vector<int> v = {10, 30};

        // vidury
        auto it = v.insert(v.begin() + 1, 20);
        REQUIRE(*it == 20);
        REQUIRE(v.size() == 3);
        REQUIRE(v[1] == 20); // {10, 20, 30}

        // pradžioj
        it = v.insert(v.begin(), 5);
        REQUIRE(*it == 5);
        REQUIRE(v[0] == 5); // {5, 10, 20, 30}

        // pabaigoj
        it = v.insert(v.end(), 40);
        REQUIRE(*it == 40);
        REQUIRE(v.back() == 40); // {5, 10, 20, 30, 40}
    }

    SECTION("insert(pos, count, value) veikia tinkamai, kai count = 0")
    {
        Vector<int> v = {1, 2, 3};
        v.insert(v.begin() + 1, 0, 99);
        REQUIRE(v.size() == 3);
        REQUIRE(v[1] == 2); // turi tas pats likt antroj vietoj
    }

    SECTION("insert(pos, count, value) veikia tinkamai, kai įterpiamų elementų mažiau nei pastumiamų")
    {
        Vector<int> v = {1, 5, 6};

        v.insert(v.begin() + 1, 1, 99);

        REQUIRE(v.size() == 4);
        REQUIRE(v[0] == 1);
        REQUIRE(v[1] == 99);
        REQUIRE(v[2] == 5);
        REQUIRE(v[3] == 6);
    }

    SECTION("insert(pos, count, value) veikia tinkamai, kai įterpiamų elementų daugiau/lygu nei pastumiamų")
    {
        Vector<int> v = {1, 2};

        v.insert(v.begin() + 1, 3, 7);

        REQUIRE(v.size() == 5);
        REQUIRE(v[0] == 1);
        REQUIRE(v[1] == 7);
        REQUIRE(v[2] == 7);
        REQUIRE(v[3] == 7);
        REQUIRE(v[4] == 2); // pastumtasai elementas atsiduria pačian gali
    }
}

TEST_CASE("erase() metodai veikia tinkamai")
{

    SECTION("erase(pos) ištrina vieną elementą iš vidurio")
    {
        Vector<int> v = {10, 20, 30, 40, 50};

        auto it = v.erase(v.begin() + 2); // Šaliname 30

        REQUIRE(*it == 40); // turi grąžint iteratorių kitan elementan
        REQUIRE(v.size() == 4);
        REQUIRE(v[0] == 10);
        REQUIRE(v[1] == 20);
        REQUIRE(v[2] == 40);
        REQUIRE(v[3] == 50);
    }

    SECTION("erase(pos) ištrina pirmą ir paskutinį elementus")
    {
        Vector<int> v = {1, 2, 3};

        v.erase(v.begin()); // šalinam pirmą
        REQUIRE(v.size() == 2);
        REQUIRE(v[0] == 2);

        v.erase(v.end() - 1); // šalinam paskutinį
        REQUIRE(v.size() == 1);
        REQUIRE(v[0] == 2);
    }

    SECTION("erase(first, last) ištrina elementų rėžį")
    {
        Vector<int> v = {1, 2, 3, 4, 5, 6};

        // šalinama {3, 4}
        auto it = v.erase(v.begin() + 2, v.begin() + 4);

        REQUIRE(*it == 5); // erase grąžina pirmą elementą po ištrintųjų
        REQUIRE(v.size() == 4);
        REQUIRE(v[0] == 1);
        REQUIRE(v[1] == 2);
        REQUIRE(v[2] == 5);
        REQUIRE(v[3] == 6);
    }

    SECTION("erase() tinkamai destruktina ištrintus elementus")
    {
        ObjectCounter::count = 0;
        {
            Vector<ObjectCounter> v(5);
            REQUIRE(ObjectCounter::count == 5);

            v.erase(v.begin() + 1);
            REQUIRE(ObjectCounter::count == 4);
            REQUIRE(v.size() == 4);

            v.erase(v.begin(), v.begin() + 2);
            REQUIRE(ObjectCounter::count == 2);
            REQUIRE(v.size() == 2);
        }
        REQUIRE(ObjectCounter::count == 0);
    }
}

TEST_CASE("resize() veikia tinkamai")
{

    SECTION("resize(count) sumažinant dydį sunaikina nereikalingus elementus")
    {
        ObjectCounter::count = 0;
        {
            Vector<ObjectCounter> v(5);
            size_t old_cap = v.capacity(); // Tarkim 5

            v.resize(3); // Sumažiname

            REQUIRE(v.size() == 3);
            REQUIRE(v.capacity() == old_cap);   // Talpa neturi sumažėti
            REQUIRE(ObjectCounter::count == 3); // 2 objektai turi būti sunaikinti
        }
    }

    SECTION("resize(count) padidinant dydį dapildo default reikšmių")
    {
        Vector<int> v = {1, 2};
        v.resize(5); // default int — 0

        REQUIRE(v.size() == 5);
        REQUIRE(v.capacity() >= 5);
        REQUIRE(v[0] == 1);
        REQUIRE(v[1] == 2);
        REQUIRE(v[2] == 0);
        REQUIRE(v[3] == 0);
        REQUIRE(v[4] == 0);
    }

    SECTION("resize(count, value) padidinant dydį dapildo nurodyta reikšme")
    {
        Vector<int> v = {9, 8};
        v.resize(4, 10);

        REQUIRE(v.size() == 4);
        REQUIRE(v[0] == 9);
        REQUIRE(v[1] == 8);
        REQUIRE(v[2] == 10);
        REQUIRE(v[3] == 10);
    }
}

TEST_CASE("swap() veikia tinkamai")
{

    SECTION("swap() tinkamai apsikeičia skirtingais vektoriais")
    {
        Vector<int> v1 = {1, 2, 3};

        Vector<int> v2 = {99, 88};

        int *v1_data = v1.data();
        int *v2_data = v2.data();

        v1.swap(v2);

        // tikrinam v1
        REQUIRE(v1.size() == 2);
        REQUIRE(v1.capacity() == 2);
        REQUIRE(v1[0] == 99);
        REQUIRE(v1.data() == v2_data); // rodyklės turėjo apsimainyt

        // tikrinam v2
        REQUIRE(v2.size() == 3);
        REQUIRE(v2.capacity() == 3);
        REQUIRE(v2[2] == 3);
        REQUIRE(v2.data() == v1_data);
    }
}

TEST_CASE("Vector non-member funkcijos veikia tinkamai")
{

    SECTION("operator== ir operator!= veikia tinkamai")
    {
        Vector<int> v1 = {1, 2, 3};
        Vector<int> v2 = {1, 2, 3};
        Vector<int> v3 = {1, 2, 4};
        Vector<int> v4 = {1, 2};
        Vector<int> v_empty1;
        Vector<int> v_empty2;

        REQUIRE(v1 == v2);
        REQUIRE_FALSE(v1 != v2);

        REQUIRE(v1 != v3);
        REQUIRE_FALSE(v1 == v3);

        REQUIRE(v1 != v4);

        REQUIRE(v_empty1 == v_empty2);

        REQUIRE(v1 != v_empty1);
    }

    SECTION("Non-member swap() permaino tinkamai")
    {
        Vector<int> v1 = {10, 20};
        Vector<int> v2 = {99, 88, 77};

        swap(v1, v2);

        REQUIRE(v1.size() == 3);
        REQUIRE(v1[0] == 99);

        REQUIRE(v2.size() == 2);
        REQUIRE(v2[0] == 10);
    }

    SECTION("erase(vector, value) ištrina nurodytus elementus ir grąžina jų kiekį")
    {
        Vector<int> v = {1, 2, 3, 2, 4, 2, 5};

        size_t removed_count = erase(v, 2);

        REQUIRE(removed_count == 3); // dingo 3 dvejetai
        REQUIRE(v.size() == 4);

        REQUIRE(v == Vector<int>{1, 3, 4, 5});
    }

    SECTION("erase(vector, value) nieko nedaro, kai nurodyto elemento nėra")
    {
        Vector<int> v = {10, 20, 30};

        size_t removed_count = erase(v, 99);

        REQUIRE(removed_count == 0);
        REQUIRE(v.size() == 3);
        REQUIRE(v == Vector<int>{10, 20, 30});
    }

    SECTION("erase_if(vector, pred) tinkamai ištrina elementus pagal sąlygą")
    {
        Vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8};

        size_t removed_count = erase_if(v, [](int x)
                                        { return x % 2 == 0; });

        REQUIRE(removed_count == 4); // dingo 2, 4, 6, 8
        REQUIRE(v.size() == 4);
        REQUIRE(v == Vector<int>{1, 3, 5, 7});
    }
}