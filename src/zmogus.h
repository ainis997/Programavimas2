#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include "klaidu_valdymas.h"

class Zmogus
{
protected:
    /** @brief Žmogaus vardas. */
    std::string vardas_;
    /** @brief Žmogaus pavardė. */
    std::string pavarde_;

public:
    /**
     * @brief Konstruktorius be parametrų.
     * * Sukuria tuščią žmogaus objektą su nulinėmis reikšmėmis.
     */
    Zmogus() : vardas_(""), pavarde_("") {}
    /**
     * @brief Konstruktorius su vardu ir pavarde.
     * * Sukuria žmogaus objektą su vardu ir pavarde.
     * @param vardas Žmogaus vardas.
     * @param pavarde Žmogaus pavardė.
     */
    Zmogus(std::string vardas, std::string pavarde) : vardas_(vardas), pavarde_(pavarde) {}
    // kopijavimo konstr [Zmogus x(y) / Zmogus x = y]
    /**
     * @brief Kopijavimo konstruktorius.
     * * Sukuria naują objektą nukopijuodamas visus duomenis iš kito studento.
     * @param kitas Nuoroda objekto, iš kurio kopijuojami duomenys.
     */
    Zmogus(const Zmogus &kitas)
        : vardas_(kitas.vardas_),
          pavarde_(kitas.pavarde_) {}
    // perkėlimo konstr [Zmogus x(std::move(y)) / Zmogus x = std::move(y)]
    /**
     * @brief Perkėlimo konstruktorius.
     * * Perkelia resursus iš kito objekto į naują, palikdamas senąjį tuščią.
     * @param kitas "rvalue" nuoroda į žmogaus objektą.
     */
    Zmogus(Zmogus &&kitas)
        : vardas_(std::move(kitas.vardas_)),
          pavarde_(std::move(kitas.pavarde_)) {}

    /** @brief Destruktorius. */
    ~Zmogus() {};

    // gavikai / getteriai

    /** @brief Grąžina žmogaus vardą. */
    inline std::string vardas() const { return vardas_; }
    /** @brief Grąžina žmogaus pavardę. */
    inline std::string pavarde() const { return pavarde_; }

    // nustatytojai / setteriai

    /**
     * @brief Nustato žmogaus vardą.
     *
     * @param vardas Žmogaus vardas.
     */
    void nust_varda(std::string vardas) { vardas_ = vardas; }
    /**
     * @brief Nustato žmogaus pavardę.
     *
     * @param vardas Žmogaus pavardė.
     */
    void nust_pavarde(std::string pavarde) { pavarde_ = pavarde; }

    /**
     * @brief Grynoji virtualioji funkcija, leisianti vartotojui įvesti išvestinių klasių objektų vardą ir pavardę rankiniu būdu.
     * @param ar_ivestis_atsaukiama Ar leidžiama nutraukti studento duomenų įvedimą įvedus "x". Numatytoji reikšmė: false.
     * @return true Jei įvesta sėkmingai.
     * @return false Jei įvedimas atšauktas.
     */
    virtual bool ivest_varda_pavarde(bool ar_ivestis_atsaukiama = false) = 0;
};