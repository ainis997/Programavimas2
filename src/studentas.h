#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>

#include "strukturos_konstantos.h"
#include "klaidu_valdymas.h"
#include "ivesties_pagalb_fjos.h"
#include "isvesties_pagalb_fjos.h"

class Studentas
{
private:
    std::string vardas_;
    std::string pavarde_;
    Container(int) pazymiai_;
    int egzo_rezas_;
    // float rezas_;
    // enum class Galutinis : char
    // {
    //     VID,
    //     MED
    // };
    // Galutinis gal_;
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
    inline float rezas_vid() const { return rezas_vid_; }
    inline float rezas_med() const { return rezas_med_; }

    // nustatytojai / setteriai

    void nust_varda(std::string vardas) { vardas_ = vardas; }
    void nust_pavarde(std::string pavarde) { pavarde_ = pavarde; }
    bool ivest_varda_pavarde(bool ar_ivestis_atsaukiama = false)
    {
        std::cout << "Iveskite varda ir pavarde: ";
        for (;;)
        {
            try
            {
                std::string vardas_pavarde;
                if (std::getline(std::cin, vardas_pavarde)) // jeigu sėkmingai nuskaito eilutę, tai ...
                {
                    if (vardas_pavarde.empty())
                    {
                        throw std::invalid_argument("Tuscia ivestis.");
                    }
                    if (vardas_pavarde == "x" && ar_ivestis_atsaukiama) // sąlyga, reikalinga mišrios įvesties f-jai (kad nutraukt studentų įvedimą)
                    {
                        return false;
                    }
                    std::istringstream sr(vardas_pavarde);
                    std::string vardo_ivestis, pavardes_ivestis;
                    std::string perteklius;
                    if (sr >> vardo_ivestis >> pavardes_ivestis) // jeigu sėkmingai nuskaitytos dvi vertės - vardo ir pavardės - tai ...
                    {
                        if (sr >> perteklius) // jeigu ivesta per daug duomenu
                        {
                            throw std::invalid_argument("Pertekline ivestis.");
                        }
                        vardas_ = vardo_ivestis;
                        pavarde_ = pavardes_ivestis;
                        break;
                    }
                    else
                        throw std::invalid_argument("Netinkama ivestis.");
                }
                else
                    throw std::invalid_argument("Netinkama ivestis.");
            }
            catch (...)
            {
                ivesties_klaidos_valdymas();
            }
        }
        return true;
    }
    void pridet_pazymi(int paz)
    {
        if (paz >= 0 && paz <= 10)
        {
            pazymiai_.push_back(paz);
        }
    }
    void nust_egzo_reza(int rez)
    {
        egzo_rezas_ = rez;
    }
    void ivest_egzo_reza()
    {
        int paz;
        natur_skaiciaus_ivestis(paz, ar_sk_ne_tarp_0_ir_10);
        egzo_rezas_ = paz;
    }
    // void apsk_galutini()
    // {
    //     if (gal_ == Galutinis::MED)
    //     {
    //         if (pazymiai_.size() == 0)
    //             return;
    //         Container(int) visi_pazymiai = pazymiai_;
    //         visi_pazymiai.push_back(egzo_rezas_);
    //         rikiavimas(visi_pazymiai); // sort(..) surikiuoja visi_pazymiai vektorių did. tvarka
    //         int visu_pazymiu_sk = visi_pazymiai.size();
    //         // vidurinės(-ių) reikšmės(-ių) radimas UNIVERSALIU būdu (tinka ir vector, ir deque, ir list)
    //         auto it = visi_pazymiai.begin();
    //         if (visu_pazymiu_sk % 2 != 0)
    //         {
    //             rezas_ = *std::next(it, visu_pazymiu_sk / 2); // pastumiam iteratorių per pusę visu_pazymiu_sk — į visi_pazymiai vidurį
    //         }
    //         else
    //         {
    //             auto vid1 = std::next(it, (visu_pazymiu_sk / 2) - 1);
    //             auto vid2 = std::next(vid1, 1); // pastumiam iteratorių dar per vieną, kad gautume antrąją viduriniąją reikšmę
    //             rezas_ = (*vid1 + *vid2) / 2.0;
    //         }
    //     }
    //     else
    //     {
    //         if (pazymiai_.size() == 0)
    //             rezas_ = egzo_rezas_ * 0.6;
    //         int pazymiu_suma = 0;
    //         for (auto paz : pazymiai_)
    //             pazymiu_suma += paz;
    //         rezas_ = ((pazymiu_suma * 1.0) / (pazymiai_.size() * 1.0)) * 0.4 + (egzo_rezas_ * 0.6);
    //     }
    // };
    void apsk_vid()
    {
        if (pazymiai_.size() == 0)
            rezas_vid_ = egzo_rezas_ * 0.6;
        int pazymiu_suma = 0;
        for (auto paz : pazymiai_)
            pazymiu_suma += paz;
        rezas_vid_ = ((pazymiu_suma * 1.0) / (pazymiai_.size() * 1.0)) * 0.4 + (egzo_rezas_ * 0.6);
    };
    void apsk_med()
    {
        if (pazymiai_.size() == 0)
            return;
        Container(int) visi_pazymiai = pazymiai_;
        visi_pazymiai.push_back(egzo_rezas_);
        rikiavimas(visi_pazymiai); // sort(..) surikiuoja visi_pazymiai vektorių did. tvarka
        int visu_pazymiu_sk = visi_pazymiai.size();
        // vidurinės(-ių) reikšmės(-ių) radimas UNIVERSALIU būdu (tinka ir vector, ir deque, ir list)
        auto it = visi_pazymiai.begin();
        if (visu_pazymiu_sk % 2 != 0)
        {
            rezas_med_ = *std::next(it, visu_pazymiu_sk / 2); // pastumiam iteratorių per pusę visu_pazymiu_sk — į visi_pazymiai vidurį
        }
        else
        {
            auto vid1 = std::next(it, (visu_pazymiu_sk / 2) - 1);
            auto vid2 = std::next(vid1, 1); // pastumiam iteratorių dar per vieną, kad gautume antrąją viduriniąją reikšmę
            rezas_med_ = (*vid1 + *vid2) / 2.0;
        }
    };

    // kt metodai

    void uzpildyt_pazymius_iki_min(int min_pazymiu_sk)
    {
        if (pazymiai_.size() < min_pazymiu_sk)
        {
            pazymiai_.resize(min_pazymiu_sk, 0);
        }
    }

    void skaityt_studenta(std::istringstream &srautas, std::string eil)
    {
        srautas.clear();  // išvalo srautą (išvalo nuo praeitos iteracijos likusį statusą); be šito, skaitytų tik pirmą eilutę iš viso failo! (neveiktų)
        srautas.str(eil); // įstato naują stringą (eilutę) srautan

        if (!(srautas >> vardas_ >> pavarde_))
            throw std::invalid_argument("Netinkamas studento duomenu pavidalas.");

        int temp; // laikinas kintamasis pažymių perdavimui
        while (srautas >> temp)
        {
            pazymiai_.push_back(std::move(temp));
        }

        if (pazymiai_.empty())
        {
            throw std::invalid_argument("Nepavyko nuskaityti studento pazymiu.");
        }
        egzo_rezas_ = pazymiai_.back(); // paskutinis elementas — egzamino rezas_
        pazymiai_.pop_back();           // ištrinam egzo rezą iš pažymių vektoriaus

        apsk_vid();
        apsk_med();

        // apsk_galutini();
    }
    void studento_spausd(std::ofstream &ras_failas, char galutinio_pasirinkimas) const
    {
        if (galutinio_pasirinkimas == 'm')
        {
            ras_failas
                << std::left << std::setw(20) << vardas_
                << std::left << std::setw(25) << pavarde_
                << std::setw(15) << std::fixed << std::setprecision(2) << rezas_med_
                << '\n';
        }
        else
        {
            ras_failas
                << std::left << std::setw(20) << vardas_
                << std::left << std::setw(25) << pavarde_
                << std::setw(15) << std::fixed << std::setprecision(2) << rezas_vid_
                << '\n';
        }
    }
};

// struct Studentas
// {
//     Container(int) pazymiai_;
//     std::string vardas_ = "Vardas";
//     std::string pavarde_ = "pavarde_";
//     float rezas__vid = 0;
//     float rezas__med = 0;
//     int egzo_rezas__ = 0;
//     void apsk_vid(); // void, nes vid. ir med. reik iš anksto apskaičiuot (o ne spausd metu), nes pagal tai reik leist rūšiuot
//     void apsk_med();
// };