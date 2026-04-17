#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>

#include "ivesties_pagalb_fjos.h"
#include "isvesties_pagalb_fjos.h"
#include "konstantos_kt_klases.h"
#include "klaidu_valdymas.h"

class Studentas
{
private:
    std::string vardas_;
    std::string pavarde_;
    Container(int) pazymiai_;
    int egzo_rezas_;
    float rezas_vid_;
    float rezas_med_;
    inline static std::string galut_; // "v" / "m"  // be inline — klaida: "undefined reference to Studentas::galut_"

public:
    // nuskaitant
    Studentas() : vardas_(""), pavarde_(""), egzo_rezas_(0), rezas_vid_(0), rezas_med_(0) {}
    // surasant rankiniu budu
    Studentas(std::string vardas, std::string pavarde) : vardas_(vardas), pavarde_(pavarde), egzo_rezas_(0), rezas_vid_(0), rezas_med_(0) {}

    // destruktorius
    ~Studentas()
    {
        pazymiai_.clear();
    }
    // kopijavimo konstr [Studentas x(y);]
    Studentas(const Studentas &kitas)
        : vardas_(kitas.vardas_),
          pavarde_(kitas.pavarde_),
          pazymiai_(kitas.pazymiai_),
          egzo_rezas_(kitas.egzo_rezas_),
          rezas_vid_(kitas.rezas_vid_),
          rezas_med_(kitas.rezas_med_) {}
    // perkėlimo konstr
    Studentas(Studentas &&kitas)
        : vardas_(std::move(kitas.vardas_)),
          pavarde_(std::move(kitas.pavarde_)),
          pazymiai_(std::move(kitas.pazymiai_)),
          egzo_rezas_(kitas.egzo_rezas_), // be std::move, nes paprastiem tipam kaip int/float tai netikslinga (skirtingai nei std::string, std::vector — objektam)
          rezas_vid_(kitas.rezas_vid_),
          rezas_med_(kitas.rezas_med_)
    {
        kitas.egzo_rezas_ = 0;
        kitas.rezas_vid_ = 0;
        kitas.rezas_med_ = 0;
    }

    // perkrauti operatoriai

    // kopijavimo priskyrimo operatorius [x = y]
    Studentas &operator=(const Studentas &kitas);
    // perkėlimo priskyrimo operatorius
    Studentas &operator=(Studentas &&kitas);

    friend std::ostream &operator<<(std::ostream &os, const Studentas &stud);
    friend std::istream &operator>>(std::istream &is, Studentas &stud);

    // gavikai / getteriai

    // inline float rezas() const { return rezas_; }
    inline std::string vardas() const { return vardas_; }
    inline std::string pavarde() const { return pavarde_; }
    inline Container(int) pazymiai() const { return pazymiai_; }
    inline int egzo_rezas() const { return egzo_rezas_; }
    inline float rezas_vid() const { return rezas_vid_; }
    inline float rezas_med() const { return rezas_med_; }
    inline size_t pazymiu_sk() { return pazymiai_.size(); }

    static std::string galut() { return galut_; }

    // nustatytojai / setteriai

    void nust_varda(std::string vardas) { vardas_ = vardas; }
    void nust_pavarde(std::string pavarde) { pavarde_ = pavarde; }
    bool ivest_varda_pavarde(bool ar_ivestis_atsaukiama = false);
    void pridet_pazymi(int paz);
    void nust_egzo_reza(int rez);
    void ivest_egzo_reza();
    void apsk_vid();
    void apsk_med();

    static void nust_galutinio_tipa(std::string v_m);

    // kt metodai

    void uzpildyt_pazymius_iki_min(int min_pazymiu_sk);
};