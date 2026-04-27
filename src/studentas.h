#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>

#include "zmogus.h"
#include "ivesties_pagalb_fjos.h"
#include "isvesties_pagalb_fjos.h"
#include "konstantos_kt_klases.h"
#include "klaidu_valdymas.h"

class Studentas : public Zmogus
{
private:
    /** @brief Studento namų darbų pažymių konteineris (tipas Container = std::vector). */
    Container(int) pazymiai_;
    /** @brief Egzamino rezultatas. */
    int egzo_rezas_;
    /** @brief Galutinis rezultatas, skaičiuojamas pagal vidurkį. */
    float rezas_vid_;
    /** @brief Galutinis rezultatas, skaičiuojamas pagal medianą. */
    float rezas_med_;
    /** @brief Statinis nustatymas, nurodantis galutinio rezultato skaičiavimo tipą ("v" — vidurkis, "m" — mediana). */
    inline static std::string galut_; // "v" / "m"  // be inline — klaida: "undefined reference to Studentas::galut_"

public:
    // nuskaitant
    /**
     * @brief Konstruktorius be parametrų.
     * * Sukuria tuščią studento objektą su nulinėmis reikšmėmis.
     */
    Studentas() : Zmogus(), egzo_rezas_(0), rezas_vid_(0), rezas_med_(0) {}
    // surasant rankiniu budu
    /**
     * @brief Konstruktorius su vardu ir pavarde.
     * * Sukuria studento objektą su vardu ir pavarde, o likusios reikšmės — nulinės.
     * @param vardas Studento vardas.
     * @param pavarde Studento pavardė.
     */
    Studentas(std::string vardas, std::string pavarde) : Zmogus(vardas, pavarde), egzo_rezas_(0), rezas_vid_(0), rezas_med_(0) {}

    // kopijavimo konstr [Studentas x(y) / Studentas x = y]
    /**
     * @brief Kopijavimo konstruktorius.
     * * Sukuria naują objektą nukopijuodamas visus duomenis iš kito studento.
     * @param kitas Nuoroda objekto, iš kurio kopijuojami duomenys.
     */
    Studentas(const Studentas &kitas)
        : Zmogus(kitas),
          pazymiai_(kitas.pazymiai_),
          egzo_rezas_(kitas.egzo_rezas_),
          rezas_vid_(kitas.rezas_vid_),
          rezas_med_(kitas.rezas_med_) {}
    // perkėlimo konstr [Studentas x(std::move(y)) / Studentas x = std::move(y)]
    /**
     * @brief Perkėlimo konstruktorius.
     * * Perkelia resursus iš kito objekto į naują, palikdamas senąjį tuščią.
     * @param kitas "rvalue" nuoroda į studento objektą.
     */
    Studentas(Studentas &&kitas)
        : Zmogus(std::move(kitas)),
          pazymiai_(std::move(kitas.pazymiai_)),
          egzo_rezas_(kitas.egzo_rezas_), // be std::move, nes paprastiem tipam kaip int/float tai netikslinga (skirtingai nei std::string, std::vector — objektam)
          rezas_vid_(kitas.rezas_vid_),
          rezas_med_(kitas.rezas_med_)
    {
        kitas.egzo_rezas_ = 0;
        kitas.rezas_vid_ = 0;
        kitas.rezas_med_ = 0;
    }

    // destruktorius
    /**
     * @brief Destruktorius.
     * * Išvalo pažymių konteinerį.
     */
    ~Studentas()
    {
        pazymiai_.clear();
    }

    // perkrauti operatoriai

    // kopijavimo priskyrimo operatorius [x = y]
    /**
     * @brief Kopijavimo priskyrimo operatorius.
     *
     * @param kitas Nuoroda objekto, kurio duomenys bus nukopijuoti ir priskirti.
     * @return Studentas& nuoroda į atnaujintą objektą.
     */
    Studentas &operator=(const Studentas &kitas);
    // perkėlimo priskyrimo operatorius [x = std::move(y)]
    /**
     * @brief Perkėlimo priskyrimo operatorius.
     * * Perkelia resursus iš kito studento objekto į šį.
     * Šaltinis (objektas kitas) tampa galimos, tačiau neapibrėžtos būsenos.
     * * @param kitas "rvalue" nuoroda į perkelsimą studento objektą.
     * @return Studentas& nuoroda į šį patį objektą (*this).
     */
    Studentas &operator=(Studentas &&kitas);

    /**
     * @brief Išvesties operatorius studento duomenims spausdinti.
     *
     * @param os Nuoroda į išvesties srautą.
     * @param stud Nuoroda į išvesimą studentą.
     * @return std::ostream& nuoroda į išvesties srautą (tinkamam operatoriaus veikimui).
     */
    friend std::ostream &operator<<(std::ostream &os, const Studentas &stud);
    /**
     * @brief Įvesties operatorius studento duomenims nuskaityti.
     *
     * @param is Nuoroda į įvesties srautą.
     * @param stud Nuoroda į studento objektą, į kurį bus nuskaitomi duomenys.
     * @return std::istream& Grąžinama nuoroda į įvesties srautą (tinkamam operatoriaus veikimui).
     */
    friend std::istream &operator>>(std::istream &is, Studentas &stud);

    // gavikai / getteriai

    /** @brief Grąžina pažymių konteinerį. */
    inline Container(int) pazymiai() const { return pazymiai_; }
    /** @brief Grąžina egzamino rezultatą. */
    inline int egzo_rezas() const { return egzo_rezas_; }
    /** @brief Grąžina galutinį rezultatą pagal vidurkį. */
    inline float rezas_vid() const { return rezas_vid_; }
    /** @brief Grąžina galutinį rezultatą pagal medianą. */
    inline float rezas_med() const { return rezas_med_; }
    /** @brief Grąžina esamą pažymių skaičių. */
    inline size_t pazymiu_sk() { return pazymiai_.size(); }

    /** @brief Grąžina esamą galutinio skaičiavimo tipą. */
    static std::string galut() { return galut_; }

    // nustatytojai / setteriai

    /**
     * @brief Leidžia vartotojui įvesti vardą ir pavardę rankiniu būdu.
     ** Įgyvendina tėvinės žmogaus klasės grynąją virtualiąją funkciją.
     * @param ar_ivestis_atsaukiama Ar leidžiama nutraukti studento duomenų įvedimą įvedus "x". Numatytoji reikšmė: false.
     * @return true Jei įvesta sėkmingai.
     * @return false Jei įvedimas atšauktas.
     */
    bool ivest_varda_pavarde(bool ar_ivestis_atsaukiama = false) override;

    /**
     * @brief Prideda pažymį (nuo 0 iki 10) į sąrašą.
     *
     * @param paz Pridedamas pažymys.
     */
    void pridet_pazymi(int paz);
    /**
     * @brief Nustato egzamino rezultatą.
     *
     * @param rez Egzamino rezultatas.
     */
    void nust_egzo_reza(int rez);
    /**
     * @brief Leidžia vartotojui įvesti egzamino rezultatą rankiniu būdu.
     *
     */
    void ivest_egzo_reza();
    /**
     * @brief Apskaičiuoja galutinį rezultatą naudojant aritmetinį vidurkį.
     *
     */
    void apsk_vid();
    /**
     * @brief Apskaičiuoja galutinį rezultatą naudojant medianą.
     *
     */
    void apsk_med();

    /**
     * @brief Nustato, koks galutinio rezultato skaičiavimo tipas naudosimas išvestyje.
     *
     * @param v_m "v": vidurkis, "m": mediana
     */
    static void nust_galutinio_tipa(std::string v_m);

    // kt metodai

    /**
     * @brief Užpildo pažymių sąrašą nuliais iki nurodyto kiekio.
     *
     * @param min_pazymiu_sk Mažiausias privalomas turėti pažymių skaičius.
     */
    void uzpildyt_pazymius_iki_min(int min_pazymiu_sk);
};