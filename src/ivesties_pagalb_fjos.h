#pragma once

#include "strukturos_konstantos.h"

// įvesties (skaitymo) failo pavadinimo gavimo funkcija
std::ifstream skait_failo_paruosimas(std::string SK_FAILO_NUORODA);

// pagalbinės vartotojo įvesties programoj funkcijos
bool vardo_pavardes_ivestis(Studentas &A, bool ar_ivestis_atsaukiama);
bool natur_skaiciaus_ivestis(int &sk, bool (*papild_salygu_fja)(int) = nullptr, bool ar_ivestis_atsaukiama = false);

// pagalbinės įvesties sąlygų tikrinimo funkcijos
bool ar_sk_nedidesnis_uz_0(int x);
bool ar_sk_nedidesnis_uz_0_arba_didesnis_uz_10mil(int x);
bool ar_sk_ne_tarp_0_ir_10(int x);
bool ar_sk_nera_1_2_3_4_5_6(int x);