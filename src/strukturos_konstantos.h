#pragma once

#include <string>
#include <vector>
#include <deque>
#include <list>
#include <chrono>
#include <limits> // maksimaliai int reikšmei gauti

// KONTEINERIO TIPAS, KURĮ NAUDOJA VISA PROGRAMA (VECTOR / DEQUE / LIST)
#define Container(T) std::list<T>

// kad būtų trumpiau
const auto MAX_INT = std::numeric_limits<int>::max();

// struct Studentas
// {
//     Container(int) pazymiai;
//     std::string vardas = "Vardas";
//     std::string pavarde = "Pavarde";
//     float rezas_vid = 0;
//     float rezas_med = 0;
//     int egzo_rezas = 0;
//     void apsk_vid(); // void, nes vid. ir med. reik iš anksto apskaičiuot (o ne spausd metu), nes pagal tai reik leist rūšiuot
//     void apsk_med();
// };

// struct StudentasBeGalutiniu
// {
//     Container(int) pazymiai;
//     std::string vardas = "Vardas";
//     std::string pavarde = "Pavarde";
//     int egzo_rezas = 0;
// };

struct Programos_laikai
{
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