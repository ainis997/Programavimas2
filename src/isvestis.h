#pragma once

#include <string>
#include <vector>
#include <deque>
#include <list>
#include <chrono>

#include "konstantos_kt_klases.h"
class Studentas;

void isvestis(std::string RAS_FAILO_NUORODA, Container(Studentas) & grupe, Programos_laikai &t);

void skirstoma_isvestis(std::string RAS_FAILO_NUORODA, Container(Studentas) & grupe, Programos_laikai &t);
std::chrono::duration<double> spausdinimas(std::string RAS_FAILO_PAV, std::string RAS_FAILO_NUORODA, Container(Studentas) & grupe);
void studentu_skirstymas(int strategija, Container(Studentas) & grupe, Container(Studentas) & blogi, Container(Studentas) & geri);

void visu_stud_duomenu_generavimo_isvestis(std::string RAS_FAILO_NUORODA, Container(Studentas) & grupe, Programos_laikai &t);