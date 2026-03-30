#pragma once

#include <algorithm>
#include <type_traits>

#include "studentas.h"
#include "strukturos_konstantos.h"

// class Studentas;    // forward declaration, vietoje #include "studentas.h", kad išvengt circular dependency

std::ofstream ras_failo_paruosimas(std::string RAS_FAILO_NUORODA);

// ===== Pagalbinė rikiavimą įvykdanti funkcija (rikiuoja skirtingai, priklausomai nuo to, ar rikiuojamas vector/deque, ar list) =====

template <typename T> // T = Studentas; be template, kompiliatorius neįvertins constexpr sąlygų ir mes klaidą, kad pvz. std::deque neturi .sort() metodo
void stud_rikiavimas(Container(T) & grupe, bool (*rikiavimo_taisykle)(T &, T &))
{
    // LIST
    if constexpr (std::is_same_v<Container(T), std::list<T>>)
    {
        grupe.sort(rikiavimo_taisykle);
    }
    // VECTOR / DEQUE
    else if constexpr (std::is_same_v<Container(T), std::vector<T>> || std::is_same_v<Container(T), std::deque<T>>)
    {
        std::sort(grupe.begin(), grupe.end(), rikiavimo_taisykle);
    }
}

// APRAŠAS PERKELTAS ČIA, NES TEMPLATE FUNKCIJA (.cpp failan įdėjus — neveikia)
// be nurodytos specif taisyklės, bet kokiam tipui
template <typename T>
void rikiavimas(Container(T) & konteineris)
{
    // LIST
    if constexpr (std::is_same_v<Container(T), std::list<T>>)
    {
        konteineris.sort();
    }
    // VECTOR / DEQUE
    else if constexpr (std::is_same_v<Container(T), std::vector<T>> || std::is_same_v<Container(T), std::deque<T>>)
    {
        std::sort(konteineris.begin(), konteineris.end());
    }
}

bool pagal_varda_did(Studentas &A, Studentas &B);
bool pagal_varda_maz(Studentas &A, Studentas &B);
bool pagal_pavarde_did(Studentas &A, Studentas &B);
bool pagal_pavarde_maz(Studentas &A, Studentas &B);
bool pagal_vidurki_did(Studentas &A, Studentas &B);
bool pagal_vidurki_maz(Studentas &A, Studentas &B);
bool pagal_mediana_did(Studentas &A, Studentas &B);
bool pagal_mediana_maz(Studentas &A, Studentas &B);