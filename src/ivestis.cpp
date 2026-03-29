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
#include "strukturos_konstantos.h"
#include "klaidu_valdymas.h"

void failo_ivestis(std::ifstream &sk_failas, /*const std::string SK_FAILO_NUORODA,*/ Container(Studentas) & grupe, Programos_laikai &t)
{
    // std::ifstream sk_failas = skait_failo_paruosimas(SK_FAILO_NUORODA);

    // std::cout << "FAILO NUSKAITYMAS PRADETAS\n";
    auto pr = std::chrono::high_resolution_clock::now(); // matuojam nuo čia, nes skait_failo_paruosimas turi įvesties

    std::string sk_failo_eil;

    std::istringstream srautas; // eilutės įvesties ("skaitymo") srautas
    while (std::getline(sk_failas, sk_failo_eil))
    {
        srautas.clear();           // išvalo srautą (išvalo nuo praeitos iteracijos likusį statusą); be šito, skaitytų tik pirmą eilutę iš viso failo! (neveiktų)
        srautas.str(sk_failo_eil); // įstato naują stringą (eilutę) srautan

        Studentas A;

        if (!(srautas >> A.vardas >> A.pavarde))
            continue;

        int temp; // laikinas kintamasis pažymių perdavimui
        while (srautas >> temp)
        {
            A.pazymiai.push_back(std::move(temp));
        }

        if (A.pazymiai.empty()) // jeigu pažymių vektorius gautųsi tuščias, tai pereit (continue) prie kitos iteracijos
            continue;
        A.egzo_rezas = A.pazymiai.back(); // paskutinis elementas — egzamino rezas
        A.pazymiai.pop_back();            // ištrinam egzo rezą iš pažymių vektoriaus

        A.apsk_vid();
        A.apsk_med();

        grupe.push_back(std::move(A)); // std::move(A) — perkelia, o ne kopijuoja duomenį!
        A.pazymiai.clear();
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

        bool ar_ivestis_atsaukiama = false;
        // vvv įves A.vardas ir A.pavarde
        vardo_pavardes_ivestis(A, ar_ivestis_atsaukiama); // false reiškia, kad šioje įvestyje negalima atšaukti studentų duomenų pildymo apskritai

        std::cout << "Iveskite semestro ivercius: (kai suvesite visus semestro ivercius, iveskite 'x')" << '\n';

        for (;;) // for loopas be parametrų — begalinis loopas (iš jo išeis tik jeigu vartotojas įves "x" (tuo atveju ta fja grąžins false))
        {
            int pazymys;
            bool ar_ivestis_atsaukiama = true;
            if (!natur_skaiciaus_ivestis(pazymys, ar_sk_ne_tarp_0_ir_10, ar_ivestis_atsaukiama))
                break;
            A.pazymiai.push_back(pazymys);
        }

        std::cout << "Iveskite egzamino vertinima: ";

        natur_skaiciaus_ivestis(A.egzo_rezas, ar_sk_ne_tarp_0_ir_10);

        if (A.pazymiai.size() < min_iverciu_sk)
        {
            A.pazymiai.resize(min_iverciu_sk, 0); // pridės reikiamą sk. nulių, jeigu pažymių yra mažiau nei jų privalomas minimalus sk.
        }

        A.apsk_vid();
        A.apsk_med();

        grupe.push_back(A);
        A.pazymiai.clear(); // apsauga: isvalo pazymiu vektoriu, kad kitam kartojime vektorius butu tuscias
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

        bool ar_ivestis_atsaukiama = true;                     // true reiškia, kad šioje įvestyje galima atšaukti studentų duomenų pildymo apskritai (jeigu vartotojas įves "x", )
        if (!vardo_pavardes_ivestis(A, ar_ivestis_atsaukiama)) // jeigu f-ja grąžina false, tai reikia nutraukti visą šį loopą
            break;

        for (int i = 0; i < min_iverciu_sk; i++)
        {
            A.pazymiai.push_back(pasiskirstymas_0_10(generatorius)); // sugeneruoti sk. nuo 0 iki 10
        }
        A.egzo_rezas = pasiskirstymas_0_10(generatorius);

        A.apsk_vid();
        A.apsk_med();

        grupe.push_back(A);
        A.pazymiai.clear(); // apsauga: isvalo pazymiu vektoriu, kad kitam kartojime vektorius butu tuscias
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

    // vardų generavimui
    // Container<std::string> vardai = {"Jonas", "Lina", "Lukas", "Egle", "Marius", "Migle", "Azuolas", "Aiste", "Tomas", "Ieva", "Mindaugas", "Austeja", "Vytautas", "Saule", "Rimvydas", "Gabija", "Povilas", "Lukne", "Audrius", "Ugne"};
    // Container<std::string> vyr_pavardes = {"Butkus", "Zemaitis", "Rimkus", "Simkus", "Mazeika", "Petraitis", "Braziunas", "Sukys", "Simonis", "Bareikis"};
    // Container<std::string> mot_pavardes = {"Butkute", "Zemaityte", "Rimkute", "Simkute", "Mazeikaite", "Petraityte", "Braziunaite", "Sukyte", "Simonyte", "Bareikyte"};

    for (int i = 0; i < reikiamas_studentu_sk; i++)
    {
        Studentas A;

        A.vardas = "Vardas" + std::to_string(i + 1);
        A.pavarde = "Pavarde" + std::to_string(i + 1);

        // A.vardas = vardai[rand() % 20];
        // if (A.vardas.back() == 's')
        //     A.pavarde = vyr_pavardes[rand() % 10];
        // else
        //     A.pavarde = mot_pavardes[rand() % 10];

        for (int i = 0; i < min_iverciu_sk; i++)
        {
            A.pazymiai.push_back(pasiskirstymas_0_10(generatorius)); // sugeneruoti sk. nuo 0 iki 10
        }
        A.egzo_rezas = pasiskirstymas_0_10(generatorius);

        A.apsk_vid();
        A.apsk_med();

        grupe.push_back(A);
        A.pazymiai.clear(); // apsauga: isvalo pazymiu vektoriu, kad kitam kartojime vektorius butu tuscias
    }
}

void visu_stud_duomenu_generavimo_ivestis(Container(StudentasBeGalutiniu) & grupe)
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
        StudentasBeGalutiniu A;

        A.vardas = "Vardas" + std::to_string(i + 1);
        A.pavarde = "Pavarde" + std::to_string(i + 1);

        for (int i = 0; i < min_iverciu_sk; i++)
        {
            A.pazymiai.push_back(pasiskirstymas_1_10(generatorius));
        }
        A.egzo_rezas = pasiskirstymas_1_10(generatorius);

        grupe.push_back(A);
        A.pazymiai.clear(); // apsauga: isvalo pazymiu vektoriu, kad kitam kartojime vektorius butu tuscias
    }
}