#include "isvestis.h"
#include "isvesties_pagalb_fjos.h"
#include "strukturos_konstantos.h"
#include "klaidu_valdymas.h"
#include "ivesties_pagalb_fjos.h"

#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <list>
#include <algorithm>
#include <fstream>
#include <iomanip>

void isvestis(std::string RAS_FAILO_NUORODA, Container(Studentas) & grupe, Programos_laikai &t)
{
    if (grupe.empty())
        return;

    std::string galutinio_pasirinkimas;
    std::cout << "Ar norite rasti galutini vidurki ar galutine mediana?" << '\n'
              << "Jeigu vidurki, iveskite 'v'. Jeigu mediana, iveskite 'm'. " << '\n';
    for (;;)
    {
        try
        {
            std::string ivestis;
            std::getline(std::cin, ivestis);
            if (ivestis == "v" || ivestis == "m")
            {
                galutinio_pasirinkimas = ivestis;
                break;
            }
            throw std::invalid_argument("Ivestas netinkamas atsakymas (galimi atsakymai: v, m).");
        }
        catch (...)
        {
            ivesties_klaidos_valdymas();
        }
    }

    // dinamiškam lentelės stulpelių pavadinimų pavaizdavimui
    std::string pasirinktas_galutinis;
    if (galutinio_pasirinkimas == "v")
        pasirinktas_galutinis = "Galutinis (Vid.)";

    else if (galutinio_pasirinkimas == "m")
        pasirinktas_galutinis = "Galutinis (Med.)";

    std::string rus; // rus - rūšiavimo būdas
    std::cout << "Pasirinkite studentu rusiavimo buda:" << '\n'
              << "'vard' - pagal varda" << '\n'
              << "'pav' - pagal pavarde" << '\n'
              << "'vid' - pagal vidurki" << '\n'
              << "'med' - pagal mediana" << '\n'
              << "'ne' - nerusiuoti" << '\n';
    for (;;)
    {
        try
        {
            std::string ivestis;
            std::getline(std::cin, ivestis);
            if (ivestis == "vard" || ivestis == "pav" || ivestis == "vid" || ivestis == "med" || ivestis == "ne")
            {
                rus = ivestis;
                break;
            }
            throw std::invalid_argument("Ivestas netinkamas atsakymas (galimi atsakymai: vard, pav, vid, med, ne).");
        }
        catch (...)
        {
            ivesties_klaidos_valdymas();
        }
    }

    if (rus != "ne")
    {
        std::string tvarka;
        std::cout << "Pasirinkite studentu rusiavimo tvarka:" << '\n'
                  << "'d' - didejimo tvarka" << '\n'
                  << "'m' - mazejimo tvarka" << '\n';
        for (;;)
        {
            try
            {
                std::string ivestis;
                std::getline(std::cin, ivestis);
                if (ivestis == "d" || ivestis == "m")
                {
                    tvarka = ivestis;
                    break;
                }
                throw std::invalid_argument("Ivestas netinkamas atsakymas (galimi atsakymai: d, m).");
            }
            catch (...)
            {
                ivesties_klaidos_valdymas();
            }
        }

        auto pr = std::chrono::high_resolution_clock::now();

        if (tvarka == "d")
        {
            if (rus == "vard")
                stud_rikiavimas(grupe, pagal_varda_did); // viduj: std::sort() arba list.sort()
            else if (rus == "pav")
                stud_rikiavimas(grupe, pagal_pavarde_did);
            else if (rus == "vid")
                stud_rikiavimas(grupe, pagal_vidurki_did);
            else if (rus == "med")
                stud_rikiavimas(grupe, pagal_mediana_did);
        }
        else if (tvarka == "m")
        {
            if (rus == "vard")
                stud_rikiavimas(grupe, pagal_varda_maz);
            else if (rus == "pav")
                stud_rikiavimas(grupe, pagal_pavarde_maz);
            else if (rus == "vid")
                stud_rikiavimas(grupe, pagal_vidurki_maz);
            else if (rus == "med")
                stud_rikiavimas(grupe, pagal_mediana_maz);
        }

        auto pab = std::chrono::high_resolution_clock::now();
        t.duomenu_rikiavimas = pab - pr;
    }

    auto pr = std::chrono::high_resolution_clock::now();

    std::ofstream ras_failas = ras_failo_paruosimas(RAS_FAILO_NUORODA);
    // std::ofstream ras_failas(RAS_FAILO_NUORODA + RAS_FAILO_PAV);

    if (!ras_failas.is_open())
    {
        // MEST AR PARODYT ERRORĄ, GAL IR SU TEMPLATE KLAIDŲ VALDYMO F-JA
        std::cout << "Nepavyko atidaryti isvesties failo." << '\n';
        return;
    }

    ras_failas
        << std::left << std::setw(20) << "Vardas"
        << std::left << std::setw(25) << "Pavarde"
        << std::left << std::setw(15) << pasirinktas_galutinis
        << '\n';

    // skiriamosios linijos tarp lentelės viršutinės ir likusiųjų eilučių spausdinimas
    const int LENTELES_PLOTIS = 60;
    for (int i = 0; i < LENTELES_PLOTIS; i++)
        ras_failas << "-";
    ras_failas << '\n';

    for (const auto &A : grupe)
    {
        A.studento_spausd(ras_failas, galutinio_pasirinkimas[0]);
    }

    ras_failas.close();

    auto pab = std::chrono::high_resolution_clock::now();
    // t.duomenu_isvedimas = pab - pr;

    std::cout << '\n'
              << "Isvedimas baigtas."
              << '\n';
}

// ==========================================

void visu_stud_duomenu_generavimo_isvestis(std::string RAS_FAILO_NUORODA, Container(StudentasBeGalutiniu) & grupe, Programos_laikai &t)
{
    if (grupe.empty())
        return;

    std::ofstream ras_failas = ras_failo_paruosimas(RAS_FAILO_NUORODA);

    if (!ras_failas.is_open())
    {
        // MEST AR PARODYT ERRORĄ, GAL IR SU TEMPLATE KLAIDŲ VALDYMO F-JA
        std::cout << "Nepavyko atidaryti isvesties failo." << '\n';
        return;
    }

    // auto pr = std::chrono::high_resolution_clock::now();

    ras_failas
        << std::left << std::setw(20) << "Vardas"
        << std::left << std::setw(25) << "Pavarde";

    for (int i = 0; i < grupe.front().pazymiai.size(); i++) // kiek pažymių bus pirmame masyve, tai tiek turi būti ir visuose kituose! (pagal dab. įvesties funkcijos įgyvendinimą)
        ras_failas << std::left << std::setw(15) << "ND" + std::to_string(i + 1);
    ras_failas << std::left << std::setw(20) << "Egz." << '\n';

    for (const auto &A : grupe)
    {
        ras_failas
            << std::left << std::setw(20) << A.vardas
            << std::left << std::setw(25) << A.pavarde;
        for (const int &paz : A.pazymiai)
            ras_failas << std::left << std::setw(15) << paz;
        ras_failas << std::left << std::setw(20) << A.egzo_rezas << '\n';
    }

    ras_failas.close();

    // auto pab = std::chrono::high_resolution_clock::now();
    // t.failo_generavimo_trukme = pab - pr;

    std::cout << '\n'
              << "Isvedimas baigtas."
              << '\n';
}

// =====================================

void skirstoma_isvestis(std::string RAS_FAILO_NUORODA, Container(Studentas) & grupe, Programos_laikai &t)
{
    if (grupe.empty())
        return;

    std::string galutinio_pasirinkimas;
    std::cout << "Ar norite rasti galutini vidurki ar galutine mediana?" << '\n'
              << "Jeigu vidurki, iveskite 'v'. Jeigu mediana, iveskite 'm'. " << '\n';
    for (;;)
    {
        try
        {
            std::string ivestis;
            std::getline(std::cin, ivestis);
            if (ivestis == "v" || ivestis == "m")
            {
                galutinio_pasirinkimas = ivestis;
                break;
            }
            throw std::invalid_argument("Ivestas netinkamas atsakymas (galimi atsakymai: v, m).");
        }
        catch (...)
        {
            ivesties_klaidos_valdymas();
        }
    }

    std::string rus; // rus - rūšiavimo būdas
    std::cout << "Pasirinkite studentu rusiavimo buda:" << '\n'
              << "'vard' - pagal varda" << '\n'
              << "'pav' - pagal pavarde" << '\n'
              << "'vid' - pagal vidurki" << '\n'
              << "'med' - pagal mediana" << '\n'
              << "'ne' - nerusiuoti" << '\n';
    for (;;)
    {
        try
        {
            std::string ivestis;
            std::getline(std::cin, ivestis);
            if (ivestis == "vard" || ivestis == "pav" || ivestis == "vid" || ivestis == "med" || ivestis == "ne")
            {
                rus = ivestis;
                break;
            }
            throw std::invalid_argument("Ivestas netinkamas atsakymas (galimi atsakymai: vard, pav, vid, med, ne).");
        }
        catch (...)
        {
            ivesties_klaidos_valdymas();
        }
    }

    if (rus != "ne")
    {
        std::string tvarka;
        std::cout << "Pasirinkite studentu rusiavimo tvarka:" << '\n'
                  << "'d' - didejimo tvarka" << '\n'
                  << "'m' - mazejimo tvarka" << '\n';
        for (;;)
        {
            try
            {
                std::string ivestis;
                std::getline(std::cin, ivestis);
                if (ivestis == "d" || ivestis == "m")
                {
                    tvarka = ivestis;
                    break;
                }
                throw std::invalid_argument("Ivestas netinkamas atsakymas (galimi atsakymai: d, m).");
            }
            catch (...)
            {
                ivesties_klaidos_valdymas();
            }
        }

        // std::cout << "DUOMENU RIKIAVIMAS PRADETAS\n";
        auto pr = std::chrono::high_resolution_clock::now();

        if (tvarka == "d")
        {
            if (rus == "vard")
            {
                stud_rikiavimas(grupe, pagal_varda_did);
            }
            else if (rus == "pav")
            {
                stud_rikiavimas(grupe, pagal_pavarde_did);
            }
            else if (rus == "vid")
            {
                stud_rikiavimas(grupe, pagal_vidurki_did);
            }
            else if (rus == "med")
            {
                stud_rikiavimas(grupe, pagal_mediana_did);
            }
        }
        else if (tvarka == "m")
        {
            if (rus == "vard")
            {
                stud_rikiavimas(grupe, pagal_varda_maz);
            }
            else if (rus == "pav")
            {
                stud_rikiavimas(grupe, pagal_pavarde_maz);
            }
            else if (rus == "vid")
            {
                stud_rikiavimas(grupe, pagal_vidurki_maz);
            }
            else if (rus == "med")
            {
                stud_rikiavimas(grupe, pagal_mediana_maz);
            }
        }

        auto pab = std::chrono::high_resolution_clock::now();
        t.duomenu_rikiavimas = pab - pr;
        // t.visa_trukme += t.duomenu_rikiavimas;
        // std::cout << "DUOMENU RIKIAVIMAS BAIGTAS\n\n";
    }

    // STUDENTŲ SKIRSTYMAS

    int strategija;
    std::cout << "Iveskite studentu skirstymo strategija: ";
    natur_skaiciaus_ivestis(strategija, [](int x)
                            { return !(x == 1 || x == 2 || x == 3); });

    // std::cout << "STUDENTU SKIRSTYMAS PRADETAS\n";
    auto pr = std::chrono::high_resolution_clock::now();

    // ===== 3. STR. =====

    Container(Studentas) blogi;
    Container(Studentas) geri;

    studentu_skirstymas(strategija, galutinio_pasirinkimas, grupe, blogi, geri);

    auto pab = std::chrono::high_resolution_clock::now();
    t.studentu_skirstymas = pab - pr;
    // t.visa_trukme += t.studentu_skirstymas;
    // std::cout << "STUDENTU SKIRSTYMAS BAIGTAS\n\n";

    // std::cout << "GERU SPAUSDINIMAS PRADETAS\n";
    std::cout << "Geru studentu isvedimas:\n";
    if (!geri.empty())
        spausdinimas(RAS_FAILO_NUORODA, galutinio_pasirinkimas, geri); // išspausdina ir td grąžina mum spausdinimo trukmę (be vartotojo įvesčių)
    else
        spausdinimas(RAS_FAILO_NUORODA, galutinio_pasirinkimas, grupe);
    // std::cout << "GERU SPAUSDINIMAS BAIGTAS\n\n";

    // std::cout << "BLOGU SPAUSDINIMAS PRADETAS\n";
    std::cout << "Blogu studentu isvedimas:\n";
    spausdinimas(RAS_FAILO_NUORODA, galutinio_pasirinkimas, blogi);
    // std::cout << "BLOGU SPAUSDINIMAS BAIGTAS\n\n";

    grupe.clear();
    blogi.clear();
    geri.clear();
    // std::cout << "GRUPES KONTEINERIS ISVALYTAS\n\n";

    // t.visa_trukme += t.geru_isvedimas;
    // t.visa_trukme += t.blogu_isvedimas;

    std::cout << '\n'
              << "Isvedimas baigtas."
              << '\n';
}

std::chrono::duration<double> spausdinimas(std::string RAS_FAILO_NUORODA, std::string galutinio_pasirinkimas, Container(Studentas) & grupe)
{
    std::ofstream ras_failas = ras_failo_paruosimas(RAS_FAILO_NUORODA);
    // std::ofstream ras_failas(RAS_FAILO_NUORODA + RAS_FAILO_PAV);

    if (!ras_failas.is_open())
    {
        // MEST AR PARODYT ERRORĄ, GAL IR SU TEMPLATE KLAIDŲ VALDYMO F-JA
        std::cout << "Nepavyko atidaryti isvesties failo." << '\n';
        return std::chrono::milliseconds::zero();
    }

    auto pr = std::chrono::high_resolution_clock::now();

    // dinamiškam lentelės stulpelių pavadinimų pavaizdavimui
    std::string pasirinktas_galutinis;
    if (galutinio_pasirinkimas == "v")
        pasirinktas_galutinis = "Galutinis (Vid.)";

    else if (galutinio_pasirinkimas == "m")
        pasirinktas_galutinis = "Galutinis (Med.)";

    ras_failas
        << std::left << std::setw(20) << "Vardas"
        << std::left << std::setw(25) << "Pavarde"
        << std::left << std::setw(15) << pasirinktas_galutinis
        << '\n';

    // skiriamosios linijos tarp lentelės viršutinės ir likusiųjų eilučių spausdinimas
    const int LENTELES_PLOTIS = 60;
    for (int i = 0; i < LENTELES_PLOTIS; i++)
        ras_failas << "-";
    ras_failas << '\n';

    for (const auto &A : grupe)
    {
        A.studento_spausd(ras_failas, galutinio_pasirinkimas[0]);
    }

    ras_failas.close();

    auto pab = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> isvedimo_trukme = pab - pr;

    return isvedimo_trukme;
}

void studentu_skirstymas(int strategija, std::string galutinio_pasirinkimas, Container(Studentas) & grupe, Container(Studentas) & blogi, Container(Studentas) & geri)
{
    if (strategija == 1)
    {
        if (galutinio_pasirinkimas == "v")
        {
            while (!grupe.empty())
            {
                Studentas &stud = grupe.back();
                if (stud.rezas_vid() >= 5.0)
                    geri.push_back(std::move(stud)); // std::move(stud) — perkelia, o ne kopijuoja!
                else
                    blogi.push_back(std::move(stud));
                grupe.pop_back();
                // dekui/vektoriui (listas pats shrinkinasi)
                // if (grupe.size() % 1000000 == 0)
                //     grupe.shrink_to_fit();
            }
        }
        else if (galutinio_pasirinkimas == "m")
        {
            while (!grupe.empty())
            {
                Studentas &stud = grupe.back();
                if (stud.rezas_med() >= 5.0)
                    geri.push_back(std::move(stud));
                else
                    blogi.push_back(std::move(stud));
                grupe.pop_back();
                // dekui/vektoriui (listas pats shrinkinasi)
                // if (grupe.size() % 1000000 == 0)
                //     grupe.shrink_to_fit();
            }
        }
    }

    else if (strategija == 2)
    {
        // tikrinam ar "v" čia, išorėj, kad nereiktų tikrint per kiekvieną kartojimą
        if (galutinio_pasirinkimas == "v")
        {
            stud_rikiavimas(grupe, pagal_vidurki_maz);
            while (grupe.back().rezas_vid() < 5.0)
            {
                blogi.push_back(std::move(grupe.back()));
                grupe.pop_back();
            }
        }
        else if (galutinio_pasirinkimas == "m")
        {
            stud_rikiavimas(grupe, pagal_mediana_maz);
            while (grupe.back().rezas_med() < 5.0)
            {
                blogi.push_back(std::move(grupe.back()));
                grupe.pop_back();
            }
        }
    }
    else if (strategija == 3)
    {
        // tikrinam ar "v" čia, išorėj, kad nereiktų tikrint per kiekvieną kartojimą
        if (galutinio_pasirinkimas == "v")
        {
            // apie 4.7 s / 48 s
            // blogi.reserve(grupe.size() / 2); // VECTOR; bsk dijwina
            auto pirmas_blogu_elementas = std::stable_partition(grupe.begin(), grupe.end(), [](const Studentas &stud)
                                                                { return stud.rezas_vid() >= 5.0; });
            std::move(pirmas_blogu_elementas, grupe.end(), std::back_inserter(blogi)); // perkelia bloguosna
            grupe.erase(pirmas_blogu_elementas, grupe.end());
        }
        else if (galutinio_pasirinkimas == "m")
        {
            // blogi.reserve(grupe.size() / 2); // VECTOR; bsk dijwina
            auto pirmas_blogu_elementas = std::stable_partition(grupe.begin(), grupe.end(), [](const Studentas &stud)
                                                                { return stud.rezas_med() >= 5.0; });
            std::move(pirmas_blogu_elementas, grupe.end(), std::back_inserter(blogi)); // perkelia bloguosna
            grupe.erase(pirmas_blogu_elementas, grupe.end());
        }
    }
}