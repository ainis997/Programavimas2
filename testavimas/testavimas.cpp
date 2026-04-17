#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../src/studentas.h"

#include <sstream>

TEST_CASE("Paprasti konstruktoriai")
{
    SECTION("Konstruktorius be parametrų veikia")
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

TEST_CASE("Kopijavimo ir perkėlimo konstruktoriai")
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

TEST_CASE("Kopijavimo ir perkėlimo priskyrimo operatoriai")
{
    Studentas a("Vardas1", "Pavarde1");
    a.pridet_pazymi(10);
    a.pridet_pazymi(9);
    a.pridet_pazymi(8);
    a.nust_egzo_reza(9);
    a.apsk_vid();
    a.apsk_med();

    Studentas b("Vardas2", "Pavarde2");
    a.pridet_pazymi(2);
    a.pridet_pazymi(3);
    a.pridet_pazymi(4);
    a.nust_egzo_reza(3);
    a.apsk_vid();
    a.apsk_med();

    SECTION("Kopijavimo priskyrimo operatorius veikia")
    {
        a = b;
        REQUIRE(a.vardas() == b.vardas());
        REQUIRE(a.pavarde() == b.pavarde());
        REQUIRE(a.pazymiai().size() == b.pazymiai().size());
        REQUIRE(a.egzo_rezas() == b.egzo_rezas());
        REQUIRE(a.rezas_vid() == b.rezas_vid());
        REQUIRE(a.rezas_med() == b.rezas_med());
    }

    SECTION("Perkėlimo priskyrimo operatorius veikia")
    {
        Studentas c = b;
        a = std::move(b);
        REQUIRE(a.vardas() == c.vardas());
        REQUIRE(a.pavarde() == c.pavarde());
        REQUIRE(a.pazymiai().size() == c.pazymiai().size());
        REQUIRE(a.egzo_rezas() == c.egzo_rezas());
        REQUIRE(a.rezas_vid() == c.rezas_vid());
        REQUIRE(a.rezas_med() == c.rezas_med());

        REQUIRE(b.vardas() == "");
        REQUIRE(b.pavarde() == "");
        REQUIRE(b.pazymiai().empty() == true);
        REQUIRE(b.egzo_rezas() == 0);
        REQUIRE(b.rezas_vid() == 0);
        REQUIRE(b.rezas_med() == 0);
    }
}

TEST_CASE("Išvesties/įvesties operatoriai")
{
    Studentas s("Vardas", "Pavarde");
    s.pridet_pazymi(10);
    s.pridet_pazymi(9);
    s.pridet_pazymi(8);
    s.nust_egzo_reza(9);
    s.apsk_vid();
    s.apsk_med();

    std::stringstream ss;

    SECTION("Studentas išvedamas tinkamai")
    {
        ss << s;
        std::string eilute = ss.str();
        std::istringstream is(eilute);
        std::string vardas;
        std::string pavarde;
        float galutinis;
        is >> vardas >> pavarde >> galutinis;
        REQUIRE(vardas == s.vardas());
        REQUIRE(pavarde == s.pavarde());
        REQUIRE(galutinis == s.rezas_vid());
    }

    SECTION("Studentas įvedamas tinkamai")
    {
        std::string ivestis = "Vardas Pavarde 10 9 8 9";
        ss.str(ivestis);
        Studentas t;
        ss >> t;
        REQUIRE(t.vardas() == s.vardas());
        REQUIRE(t.pavarde() == s.pavarde());
        REQUIRE(t.pazymiai().size() == s.pazymiai().size());
        REQUIRE(t.egzo_rezas() == s.egzo_rezas());
        REQUIRE(t.rezas_vid() == s.rezas_vid());
        REQUIRE(t.rezas_med() == s.rezas_med());
    }
}