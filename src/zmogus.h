#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include "klaidu_valdymas.h"

class Zmogus
{
protected:
    std::string vardas_;
    std::string pavarde_;

public:
    Zmogus() : vardas_(""), pavarde_("") {}
    // surasant rankiniu budu
    Zmogus(std::string vardas, std::string pavarde) : vardas_(vardas), pavarde_(pavarde) {}
    // kopijavimo konstr [Zmogus x(y) / Zmogus x = y]
    Zmogus(const Zmogus &kitas)
        : vardas_(kitas.vardas_),
          pavarde_(kitas.pavarde_) {}
    // perkėlimo konstr [Zmogus x(std::move(y)) / Zmogus x = std::move(y)]
    Zmogus(Zmogus &&kitas)
        : vardas_(std::move(kitas.vardas_)),
          pavarde_(std::move(kitas.pavarde_)) {}

    // destruktorius
    ~Zmogus() {};

    // gavikai / getteriai

    inline std::string vardas() const { return vardas_; }
    inline std::string pavarde() const { return pavarde_; }

    // nustatytojai / setteriai

    void nust_varda(std::string vardas) { vardas_ = vardas; }
    void nust_pavarde(std::string pavarde) { pavarde_ = pavarde; }

    virtual bool ivest_varda_pavarde(bool ar_ivestis_atsaukiama = false) = 0;
};