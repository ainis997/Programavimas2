#include <iostream>
#include <vector>
#include <deque>
#include <list>
#include <algorithm>

#include "konstantos_kt_klases.h"

void Programos_laikai::spausd_laikus()
{
    std::cout << '\n'
              << "Failo duomenu nuskaitymo trukme: " << duomenu_nuskaitymas.count() << "s" << '\n'
              << "Failo duomenu surikiavimo trukme: " << duomenu_rikiavimas.count() << "s" << '\n'
              << "Studentu suskirstymo i atskirus konteinerius trukme: " << studentu_skirstymas.count() << "s" << '\n'
              //   << "Geru studentu isvedimo trukme: " << geru_isvedimas.count() << "s" << '\n'
              //   << "Blogu studentu isvedimo trukme: " << blogu_isvedimas.count() << "s" << '\n'
              //   << "Programos trukme: " << visa_trukme.count() << "s" << '\n'
              << "Programos trukme: " << visa_trukme_su_ivestim.count() << "s" << '\n'; // kol kas dabar — be įvesties
}