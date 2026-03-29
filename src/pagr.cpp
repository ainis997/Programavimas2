#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <list>
#include <chrono>
#include <fstream> // std::ifstream'ui
#include <typeinfo>

#include "strukturos_konstantos.h"
#include "ivestis.h"
#include "isvestis.h"
#include "ivesties_pagalb_fjos.h"

int main()
{
    bool ar_failas_jau_apdorotas = false;
    for (;;)
    {

        int eiga;
        std::cout << '\n'
                  << "Pasirinkite, ka norite daryti:" << '\n'
                  << "1 - ivesti duomenis is failo" << '\n'
                  << "2 - ivesti duomenis ranka" << '\n'
                  << "3 - ivesti duomenis, pazymius sugeneruoti" << '\n'
                  << "4 - sugeneruoti galutinius duomenis" << '\n'
                  << "5 - sugeneruoti visus studentu duomenis" << '\n'
                  << "6 - baigti darba" << '\n';
        natur_skaiciaus_ivestis(eiga, ar_sk_nera_1_2_3_4_5_6);

        Container(Studentas) grupe;

        const std::string SK_FAILO_NUORODA = "ivesties_failai/";   // root dir atžvilgiu
        const std::string RAS_FAILO_NUORODA = "isvesties_failai/"; // root dir atžvilgiu
        // const std::string RAS_FAILO_PAV = "studentu_isvestis.txt";

        Programos_laikai t;

        switch (eiga)
        {
        case 1:
        {
            std::cout << "Naudojamas konteineris: " << typeid(grupe).name() << std::endl;
            // atstatom/nustatom nulin laikus
            t.duomenu_nuskaitymas = t.duomenu_rikiavimas = t.studentu_skirstymas = t.visa_trukme_su_ivestim = std::chrono::milliseconds::zero();
            std::ifstream sk_failas = skait_failo_paruosimas(SK_FAILO_NUORODA);
            auto pati_pradzia = std::chrono::high_resolution_clock::now();
            failo_ivestis(sk_failas, /*SK_FAILO_NUORODA,*/ grupe, t);
            skirstoma_isvestis(RAS_FAILO_NUORODA, grupe, t);
            auto pati_pab = std::chrono::high_resolution_clock::now();
            t.visa_trukme_su_ivestim = pati_pab - pati_pradzia;

            t.spausd_laikus();
            break;
        }
        case 2:
        {
            rank_ivestis(grupe);
            isvestis(RAS_FAILO_NUORODA, grupe, t);
            break;
        }
        case 3:
        {
            misri_ivestis(grupe);
            isvestis(RAS_FAILO_NUORODA, grupe, t);
            break;
        }
        case 4:
        {
            generuota_ivestis(grupe);
            isvestis(RAS_FAILO_NUORODA, grupe, t);
            break;
        }
        case 5:
        {
            Container(StudentasBeGalutiniu) grupe;
            visu_stud_duomenu_generavimo_ivestis(grupe);
            visu_stud_duomenu_generavimo_isvestis(RAS_FAILO_NUORODA, grupe, t); // stwi mattauimai dateīnas poteikūsnas kērdan (be enwesennins)
            // std::cout << "Failo kurimo ir jo uzdarymo trukme: " << t.failo_generavimo_trukme.count() << "s" << '\n';
            break;
        }
        case 6:
        {
            return 0;
            break;
        }
        }
    }
}
