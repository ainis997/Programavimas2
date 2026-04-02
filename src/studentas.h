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

public:
    // nuskaitant
    Studentas() : vardas_("Vardas"), pavarde_("Pavarde"), egzo_rezas_(0), rezas_vid_(0), rezas_med_(0) {}
    // surasant rankiniu budu
    Studentas(std::string vardas, std::string pavarde) : vardas_(vardas), pavarde_(pavarde), egzo_rezas_(0), rezas_vid_(0), rezas_med_(0) {}

    ~Studentas()
    {
        pazymiai_.clear();
    }

    // gavikai / getteriai

    // inline float rezas() const { return rezas_; }
    inline std::string vardas() const { return vardas_; }
    inline std::string pavarde() const { return pavarde_; }
    inline Container(int) pazymiai() const { return pazymiai_; }
    inline int egzo_rezas() const { return egzo_rezas_; }
    inline float rezas_vid() const { return rezas_vid_; }
    inline float rezas_med() const { return rezas_med_; }
    inline size_t pazymiu_sk() { return pazymiai_.size(); }

    // nustatytojai / setteriai

    void nust_varda(std::string vardas) { vardas_ = vardas; }
    void nust_pavarde(std::string pavarde) { pavarde_ = pavarde; }
    bool ivest_varda_pavarde(bool ar_ivestis_atsaukiama = false);
    void pridet_pazymi(int paz);
    void nust_egzo_reza(int rez);
    void ivest_egzo_reza();
    void apsk_vid();
    void apsk_med();

    // kt metodai

    void uzpildyt_pazymius_iki_min(int min_pazymiu_sk);

    void skaityt_studenta(std::istringstream &srautas, std::string eil);
    void studento_spausd(std::ofstream &ras_failas, char galutinio_pasirinkimas) const; // const, tai negali pakeist klasės narių
};