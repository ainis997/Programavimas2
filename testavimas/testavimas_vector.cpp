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
        for (size_t i = 0; i < 5; ++i)
        {
            REQUIRE(v[i] == 0);
        }
    }

    SECTION("Konstruktorius, sukuriantis tam tikrą sk. tam tikros vertės elementų, veikia tinkamai")
    {
        Vector<int> v(5, 42);
        REQUIRE(v.size() == 5);
        REQUIRE(v.capacity() == 5);

        for (size_t i = 0; i < 5; ++i)
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