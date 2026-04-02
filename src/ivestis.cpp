#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <list>
#include <algorithm>
#include <iomanip>
#include <limits>  // maksimaliai int reikšmei gauti
#include <cstdlib> // atsitiktiniam skaičiam
#include <sstream> // std::string streamui
#include <chrono>  // laiko fiksavimui
#include <fstream> // failam
#include <random>

#include "ivestis.h"
#include "ivesties_pagalb_fjos.h"
#include "konstantos_kt_klases.h"
#include "klaidu_valdymas.h"
#include "studentas.h"

void failo_ivestis(std::ifstream &sk_failas, /*const std::string SK_FAILO_NUORODA,*/ Container(Studentas) & grupe, Programos_laikai &t)
{
    // std::ifstream sk_failas = skait_failo_paruosimas(SK_FAILO_NUORODA);

    // std::cout << "FAILO NUSKAITYMAS PRADETAS\n";
    auto pr = std::chrono::high_resolution_clock::now(); // matuojam nuo čia, nes skait_failo_paruosimas turi įvesties

    std::string sk_failo_eil;

    std::istringstream srautas; // eilutės įvesties ("skaitymo") srautas
    while (std::getline(sk_failas, sk_failo_eil))
    {
        Studentas A;
        try
        {
            A.skaityt_studenta(srautas, sk_failo_eil);
        }
        catch (...)
        {
            continue;
        }
        grupe.push_back(std::move(A)); // std::move(A) — perkelia, o ne kopijuoja duomenį!
    }

    sk_failas.close();
    auto pab = std::chrono::high_resolution_clock::now();
    t.duomenu_nuskaitymas = pab - pr;
    // t.visa_trukme += t.duomenu_nuskaitymas;
    // std::cout << "FAILO NUSKAITYMAS BAIGTAS\n\n";
}

void rank_ivestis(Container(Studentas) & grupe)
{
    int min_iverciu_sk;
    std::cout << "Iveskite, kiek studentai privalo tureti iverciu: ";
    natur_skaiciaus_ivestis(min_iverciu_sk, ar_sk_nedidesnis_uz_0); // false reiškia, kad ivestis neatsaukiama

    for (int i = 0;; i++)
    {
        if (i > 0)
        {
            std::string arDarVestiStudenta;
            std::string ivestis;
            std::cout << "Ar norite suvesti dar vieno studento duomenis?" << '\n'
                      << "t - taip, n - ne\n";
            for (;;)
            {
                try
                {
                    std::getline(std::cin, ivestis);
                    if (ivestis == "t" || ivestis == "n")
                    {
                        arDarVestiStudenta = ivestis;
                        break;
                    }
                    throw std::invalid_argument("Ivestas netinkamas atsakymas (galimi atsakymai: t, n).");
                }
                catch (...)
                {
                    ivesties_klaidos_valdymas();
                }
            }
            if (arDarVestiStudenta == "n")
                break;
        }

        Studentas A;

        A.ivest_varda_pavarde();

        std::cout << "Iveskite semestro ivercius: (kai suvesite visus semestro ivercius, iveskite 'x')" << '\n';

        for (;;) // for loopas be parametrų — begalinis loopas (iš jo išeis tik jeigu vartotojas įves "x" (tuo atveju ta fja grąžins false))
        {
            int pazymys;
            bool ar_ivestis_atsaukiama = true;
            if (!natur_skaiciaus_ivestis(pazymys, ar_sk_ne_tarp_0_ir_10, ar_ivestis_atsaukiama))
                break;
            A.pridet_pazymi(pazymys);
        }

        std::cout << "Iveskite egzamino vertinima: ";
        int egz_temp;
        natur_skaiciaus_ivestis(egz_temp, ar_sk_ne_tarp_0_ir_10);
        A.nust_egzo_reza(egz_temp);

        A.uzpildyt_pazymius_iki_min(min_iverciu_sk);

        A.apsk_vid();
        A.apsk_med();

        grupe.push_back(A);
    }
}

// ================

void misri_ivestis(Container(Studentas) & grupe)
{
    std::random_device sekla;
    std::mt19937 generatorius(sekla());
    std::uniform_int_distribution pasiskirstymas_0_10(0, 10);

    int min_iverciu_sk = 0;
    std::string ivestis3;
    std::cout << "Iveskite, kiek studentai privalo tureti iverciu: ";
    natur_skaiciaus_ivestis(min_iverciu_sk); // perduodam kintamojo *referencą*

    std::cout << "Iveskite studentu duomenis. Kai ivesite visus studentus, iveskite 'x'." << '\n';

    for (int i = 0;; i++)
    {
        Studentas A;

        bool ar_ivestis_atsaukiama = true;                 // true reiškia, kad šioje įvestyje galima atšaukti studentų duomenų pildymo apskritai (jeigu vartotojas įves "x", )
        if (!A.ivest_varda_pavarde(ar_ivestis_atsaukiama)) // jeigu f-ja grąžina false, tai reikia nutraukti visą šį loopą
            break;

        for (int i = 0; i < min_iverciu_sk; i++)
        {
            A.pridet_pazymi(pasiskirstymas_0_10(generatorius));
        }
        A.nust_egzo_reza(pasiskirstymas_0_10(generatorius));

        A.apsk_vid();
        A.apsk_med();

        grupe.push_back(A);
    }
}

void generuota_ivestis(Container(Studentas) & grupe)
{
    std::random_device sekla;
    std::mt19937 generatorius(sekla());
    std::uniform_int_distribution pasiskirstymas_0_10(0, 10);

    int min_iverciu_sk = 0;
    std::string ivestis1;
    std::cout << "Iveskite, kiek studentai privalo tureti iverciu: ";
    natur_skaiciaus_ivestis(min_iverciu_sk, ar_sk_nedidesnis_uz_0_arba_didesnis_uz_10mil); // perduodam kintamojo *referencą*

    int reikiamas_studentu_sk = 0;
    std::string ivestis2;
    std::cout << "Iveskite, kiek norite sugeneruoti studentu: ";
    natur_skaiciaus_ivestis(reikiamas_studentu_sk, ar_sk_nedidesnis_uz_0_arba_didesnis_uz_10mil); // perduodam kintamojo *referencą*

    for (int i = 0; i < reikiamas_studentu_sk; i++)
    {
        Studentas A;

        A.nust_varda("Vardas" + std::to_string(i + 1));
        A.nust_pavarde("Pavarde" + std::to_string(i + 1));

        for (int i = 0; i < min_iverciu_sk; i++)
        {
            A.pridet_pazymi(pasiskirstymas_0_10(generatorius)); // sugeneruoti sk. nuo 0 iki 10
        }
        A.nust_egzo_reza(pasiskirstymas_0_10(generatorius));

        A.apsk_vid();
        A.apsk_med();

        grupe.push_back(A);
    }
}

void visu_stud_duomenu_generavimo_ivestis(Container(Studentas) & grupe)
{
    std::random_device sekla;
    std::mt19937 generatorius(sekla());
    std::uniform_int_distribution pasiskirstymas_1_10(1, 10);

    int min_iverciu_sk = 0;
    std::string ivestis1;
    std::cout << "Iveskite, kiek studentai privalo tureti iverciu: ";
    natur_skaiciaus_ivestis(min_iverciu_sk, ar_sk_nedidesnis_uz_0_arba_didesnis_uz_10mil); // perduodam kintamojo *referencą*

    int reikiamas_studentu_sk = 0;
    std::string ivestis2;
    std::cout << "Iveskite, kiek norite sugeneruoti studentu: ";
    natur_skaiciaus_ivestis(reikiamas_studentu_sk, ar_sk_nedidesnis_uz_0_arba_didesnis_uz_10mil); // perduodam kintamojo *referencą*

    for (int i = 0; i < reikiamas_studentu_sk; i++)
    {
        Studentas A;

        A.nust_varda("Vardas" + std::to_string(i + 1));
        A.nust_pavarde("Pavarde" + std::to_string(i + 1));

        for (int i = 0; i < min_iverciu_sk; i++)
        {
            A.pridet_pazymi(pasiskirstymas_1_10(generatorius));
        }
        A.nust_egzo_reza(pasiskirstymas_1_10(generatorius));

        grupe.push_back(A);
    }
}