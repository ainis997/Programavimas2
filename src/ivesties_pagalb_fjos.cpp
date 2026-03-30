#include "ivesties_pagalb_fjos.h"
#include "klaidu_valdymas.h"

#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <list>
#include <sstream>
#include <fstream>
#include <cstdint>

// įvesties (skaitymo) failo pavadinimo gavimo funkcija
std::ifstream skait_failo_paruosimas(std::string SK_FAILO_NUORODA)
{
    std::string skait_failo_pav;
    try
    {
        std::cout << "Ivesties failu aplankalo tekstiniai failai: ";
        int ar_pavyko = system("powershell ls .\\ivesties_failai *.txt");
        if (ar_pavyko != 0)
            throw std::runtime_error("");
    }
    catch (...)
    {
        std::cout << "(Ivesties failu aplankalo tekstiniu failu nuskaityti nepavyko.)\n";
    }
    std::string pilna_failo_nuoroda; // path + file name
    std::cout << "Iveskite ivesties failo pavadinima:\n";
    for (;;)
    {
        try
        {
            std::getline(std::cin, skait_failo_pav);
            pilna_failo_nuoroda = SK_FAILO_NUORODA + skait_failo_pav;
            std::ifstream sk_failas(pilna_failo_nuoroda); // sukuria įvesties srautą ir atidaro failą
            if (!sk_failas.is_open())
                throw std::runtime_error("Ivesties failas nurodytu pavadinimu ivesties failu aplankale nerastas.");
            return sk_failas;
            break; // jeigu prieš tai bus išmesta klaida, šio kodo programa nepasieks
        }
        catch (...)
        {
            ivesties_klaidos_valdymas();
        }
    }
}

// pagalbinės vartotojo įvesties programoj funkcijos

// papild_salygu_netenkinimo_fja turi tikrinti, ar fja NEtenkina salygos
bool natur_skaiciaus_ivestis(int &sk, bool (*papild_salygu_netenkinimo_fja)(int), bool ar_ivestis_atsaukiama) // default reikšmė nullptr, jeigu papildomų sąlygų nustatymo funkcija nebus pateikta iškvietime
{
    for (;;)
    {
        try
        {
            std::string ivestis;

            if (!std::getline(std::cin, ivestis))
                throw std::invalid_argument("Ivesties srauto klaida.");

            if (ivestis.empty())
                throw std::invalid_argument("Tuscia ivestis.");

            if (ivestis == "x" && ar_ivestis_atsaukiama)
                return false;

            std::istringstream sr(ivestis);
            int temp_sk; // laikinas kintamasis
            std::string perteklius;

            if (!(sr >> temp_sk))
                throw std::invalid_argument("Netinkama ivestis.");

            if (sr >> perteklius)
                throw std::invalid_argument("Pertekline ivestis.");

            if (temp_sk <= 0 || (papild_salygu_netenkinimo_fja != nullptr && papild_salygu_netenkinimo_fja(temp_sk))) // jeigu <=0, arba: jeigu yra sąlygos funkcija, ir jeigu netenkina įvesties skaičiaus tikrinimo sąlygos (pvz., yra <0 ar >10 (tuo atveju meta true!)), tai mesti errorą
                throw std::domain_error("Netinkama ivestis.");

            sk = temp_sk; // pagaliau priskiriam skaičiui vertę
            break;
        }
        catch (...)
        {
            ivesties_klaidos_valdymas();
        }
    }
    return true;
}

// grąžina bool: jeigu false, tai po vardo/pavardės įvedimo nebebus tęsiamas apskritai studentų duomenų pildymas (reikia mišrios įvesties f-jai); jeigu true, tai atvirkščiai, nieks nesikeičia
bool vardo_pavardes_ivestis(Studentas &A, bool ar_ivestis_atsaukiama)
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
                    A.nust_varda(vardo_ivestis);
                    A.nust_pavarde(pavardes_ivestis);
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

// pagalbinės įvesties sąlygų tikrinimo funkcijos

bool ar_sk_ne_tarp_0_ir_10(int x)
{
    return x < 0 || x > 10;
}

bool ar_sk_nedidesnis_uz_0(int x)
{
    return x <= 0;
}

bool ar_sk_nedidesnis_uz_0_arba_didesnis_uz_10mil(int x)
{
    return x <= 0 || x > 10000000;
}

bool ar_sk_nera_1_2_3_4_5_6(int x)
{
    return x != 1 && x != 2 && x != 3 && x != 4 && x != 5 && x != 6;
}