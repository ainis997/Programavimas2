#pragma once

#include <string>
#include <vector>
#include <deque>
#include <list>

#include "konstantos_kt_klases.h"
class Studentas;

void failo_ivestis(std::ifstream &sk_failas, /*const std::string SK_FAILO_NUORODA,*/ Container(Studentas) & grupe, Programos_laikai &t);
void rank_ivestis(Container(Studentas) & grupe);
void misri_ivestis(Container(Studentas) & grupe);
void generuota_ivestis(Container(Studentas) & grupe);
void visu_stud_duomenu_generavimo_ivestis(Container(Studentas) & grupe);
// void generuota_skirstoma_pazymiu_ivestis(std::vector<Studentas> &geri, std::vector<Studentas> &blogi);