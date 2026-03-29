#include <iostream>
#include <fstream>
#include <algorithm>   // std::sort(...)
#include <type_traits> // tipui patikrint (kintamojo tipui gauti)

#include "klaidu_valdymas.h"
#include "strukturos_konstantos.h"

// ===== Išvesties failo paruošimo funkcija =====
std::ofstream ras_failo_paruosimas(std::string RAS_FAILO_NUORODA)
{
    std::string ras_failo_pav;

    std::string pilna_failo_nuoroda; // path + file name
    std::cout << "Iveskite isvesties failo pavadinima (be .txt pletinio):\n";
    for (;;)
    {
        try
        {
            std::string ivestis;
            std::getline(std::cin, ivestis);

            const std::string failam_neleidziami_simboliai = "<>:\"/\\|?*";
            if (ivestis.find_first_of(failam_neleidziami_simboliai) != std::string::npos) // jeigu įvestyje yra neleidžiamų simbolių, ...
                throw std::invalid_argument("Ivestyje yra failu pavadinimuose neleidziamu simboliu.");
            if (ivestis.back() == '.' || ivestis.back() == ' ')
                throw std::invalid_argument("Netinkama ivestis: failo pavadinimas negali baigtis tasku ar tarpu.");
            if (ivestis == "CON" || ivestis == "PRN" || ivestis == "AUX" || ivestis == "NUL" || ivestis == "COM1" || ivestis == "COM2" || ivestis == "COM3" || ivestis == "COM4" || ivestis == "COM5" || ivestis == "COM6" || ivestis == "COM7" || ivestis == "COM8" || ivestis == "COM9" || ivestis == "LPT1" || ivestis == "LPT2" || ivestis == "LPT3" || ivestis == "LPT4" || ivestis == "LPT5" || ivestis == "LPT6" || ivestis == "LPT7" || ivestis == "LPT8" || ivestis == "LPT9")
                throw std::invalid_argument("Netinkama ivestis: toks failo pavadinimas neleidziamas operacineje sistemoje.");

            ras_failo_pav = ivestis;
            pilna_failo_nuoroda = RAS_FAILO_NUORODA + ras_failo_pav + ".txt";
            std::ofstream sk_failas(pilna_failo_nuoroda); // sukuria įvesties srautą ir atidaro failą
            if (!sk_failas.is_open())
                throw std::runtime_error("Isvesties failo atidaryti nepavyko.");
            return sk_failas;
            // break; // jeigu prieš tai bus išmesta klaida, šio kodo programa nepasieks
        }
        catch (...)
        {
            ivesties_klaidos_valdymas();
        }
    }
}

// vvv PERKELTA HEADER FAILAN vvv
// // ===== Pagalbinė rikiavimą įvykdanti funkcija (rikiuoja skirtingai, priklausomai nuo to, ar rikiuojamas vector/deque, ar list) =====

// void stud_rikiavimas(Container(Studentas) & grupe, bool (*rikiavimo_taisykle)(Studentas &, Studentas &))
// {
//     // LIST
//     if constexpr (std::is_same_v<Container(Studentas), std::list<Studentas>>)
//     {
//         grupe.sort(rikiavimo_taisykle);
//     }
//     // VECTOR / DEQUE
//     else if constexpr (std::is_same_v<Container(Studentas), std::vector<Studentas>> || std::is_same_v<Container(Studentas), std::deque<Studentas>>)
//     {
//         std::sort(grupe.begin(), grupe.end(), rikiavimo_taisykle);
//     }
// }

// vvv PERKELTA HEADER FAILAN vvv
// // be nurodytos specif taisyklės, bet kokiam tipui
// template <typename T>
// void rikiavimas(Container<T> &grupe)
// {
//     // jeigu Container = std::list
//     if constexpr (std::is_same_v<Container<T>, std::list<T>>)
//         grupe.sort();
//     // jeigu Container = std::vector ar std::deque
//     else
//         std::sort(grupe.begin(), grupe.end());
// }

// ===== Studentu rikiavimo funkcijos =====

bool pagal_varda_did(Studentas &A, Studentas &B)
{
    return A.vardas < B.vardas;
}

bool pagal_varda_maz(Studentas &A, Studentas &B)
{
    return A.vardas > B.vardas;
}

bool pagal_pavarde_did(Studentas &A, Studentas &B)
{
    return A.pavarde < B.pavarde;
}

bool pagal_pavarde_maz(Studentas &A, Studentas &B)
{
    return A.pavarde > B.pavarde;
}

bool pagal_vidurki_did(Studentas &A, Studentas &B)
{
    return A.rezas_vid < B.rezas_vid;
}

bool pagal_vidurki_maz(Studentas &A, Studentas &B)
{
    return A.rezas_vid > B.rezas_vid;
}

bool pagal_mediana_did(Studentas &A, Studentas &B)
{
    return A.rezas_med < B.rezas_med;
}

bool pagal_mediana_maz(Studentas &A, Studentas &B)
{
    return A.rezas_med > B.rezas_med;
}