#pragma once

#include "vector.h"

#include <string>
#include <vector>
#include <deque>
#include <list>
#include <chrono>
#include <limits> // maksimaliai int reikšmei gauti

// KONTEINERIO TIPAS, KURĮ NAUDOJA VISA PROGRAMA (VECTOR / DEQUE / LIST)
#define Container(T) std::vector<T>

// kad būtų trumpiau
const auto MAX_INT = std::numeric_limits<int>::max();

class Programos_laikai
{
public:
    // failo apdorojimo laikai
    std::chrono::duration<double> duomenu_nuskaitymas;
    std::chrono::duration<double> duomenu_rikiavimas;
    std::chrono::duration<double> studentu_skirstymas;
    // std::chrono::duration<double> geru_isvedimas;
    // std::chrono::duration<double> blogu_isvedimas;
    // std::chrono::duration<double> visa_trukme = std::chrono::milliseconds::zero();
    std::chrono::duration<double> visa_trukme_su_ivestim = std::chrono::milliseconds::zero();

    // failo sukūrimo (su visais stud. duomenimis) laikai
    // std::chrono::duration<double> failo_generavimo_trukme = std::chrono::milliseconds::zero();

    // metodai
    void spausd_laikus();
};