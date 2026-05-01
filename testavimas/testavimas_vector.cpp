#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../src/vector.h"

#include <memory>
#include <string>

// naudosim destruktoriaus tikrinimui
class ObjectCounter
{
public:
    static int count;
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

TEST_CASE("Priskyrimo operatoriai veikia teisingai", "[Vector][operator=]")
{

    SECTION("Kopijavimo priskyrimas sukuria gilią kopiją (deep copy)")
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
        REQUIRE(ObjectCounter::count == 0); // visiems išėjus iš scope, viskas išvaloma
    }

    SECTION("Perkėlimo priskyrimas perkelia resursus ir išvalo šaltinį")
    {
        Vector<int> v1 = {7, 8, 9};
        Vector<int> v2 = {1, 2};

        v2 = std::move(v1); // iškviečia perkėlimo priskyrimą

        // v2 perima duomenis
        REQUIRE(v2.size() == 3);
        REQUIRE(v2[0] == 7);
        REQUIRE(v2[2] == 9);

        // v1 tampa tuščias
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

    SECTION("Perkėlimo priskyrimas sunaikina senus paskirties (target) elementus")
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

TEST_CASE("Elementų pasiekimo metodai veikia teisingai", "[Vector][access]")
{

    SECTION("at() grąžina elementus ir meta išimtį už ribų")
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

    SECTION("at() const versija meta išimtį už ribų")
    {
        const Vector<int> v = {1, 2};
        REQUIRE(v.at(1) == 2);
        REQUIRE_THROWS_AS(v.at(2), std::out_of_range);
    }

    SECTION("operator[] grąžina teisingus elementus ir leidžia modifikuoti")
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