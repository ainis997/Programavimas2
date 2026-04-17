#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../src/studentas.h"

TEST_CASE("Konstruktorių veikimas", "[konstruktorius]")
{
    SECTION("Paprastas konstruktorius veikia")
    {
        Studentas s;
        REQUIRE(s.vardas() == "");
        REQUIRE(s.pavarde() == "");
        REQUIRE(s.egzo_rezas() == 0);
        REQUIRE(s.pazymiai().empty() == true);
        REQUIRE(s.rezas_vid() == 0);
        REQUIRE(s.rezas_med() == 0);
    }

    SECTION("Konstruktorius su parametrais veikia")
    {
        Studentas s("Vardas", "Pavarde");
        REQUIRE(s.vardas() == "Vardas");
        REQUIRE(s.pavarde() == "Pavarde");
        REQUIRE(s.pazymiai().empty() == true);
        REQUIRE(s.egzo_rezas() == 0);
        REQUIRE(s.rezas_vid() == 0);
        REQUIRE(s.rezas_med() == 0);
    }
}

TEST_CASE("Kopijavimo ir perkėlimo konstruktoriai", "[bruh]")
{
    Studentas s("Vardas", "Pavarde");
    s.pridet_pazymi(10);
    s.pridet_pazymi(9);
    s.pridet_pazymi(8);
    s.nust_egzo_reza(9);
    s.apsk_vid();
    s.apsk_med();

    SECTION("Kopijavimo konstruktorius veikia")
    {
        Studentas t(s);
        REQUIRE(t.vardas() == s.vardas());
        REQUIRE(t.pavarde() == s.pavarde());
        REQUIRE(t.pazymiai().size() == s.pazymiai().size());
        REQUIRE(t.egzo_rezas() == s.egzo_rezas());
        REQUIRE(t.rezas_vid() == s.rezas_vid());
        REQUIRE(t.rezas_med() == s.rezas_med());
    }

    SECTION("Perkėlimo konstruktorius veikia")
    {
        Studentas s2(s);
        Studentas t(std::move(s));
        REQUIRE(t.vardas() == s2.vardas());
        REQUIRE(t.pavarde() == s2.pavarde());
        REQUIRE(t.pazymiai().size() == s2.pazymiai().size());
        REQUIRE(t.egzo_rezas() == s2.egzo_rezas());
        REQUIRE(t.rezas_vid() == s2.rezas_vid());
        REQUIRE(t.rezas_med() == s2.rezas_med());

        REQUIRE(s.vardas() == "");
        REQUIRE(s.pavarde() == "");
        REQUIRE(s.pazymiai().empty() == true);
        REQUIRE(s.egzo_rezas() == 0);
        REQUIRE(s.rezas_vid() == 0);
        REQUIRE(s.rezas_med() == 0);
    }
}