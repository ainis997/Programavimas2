#include "studentas.h"

// pārdīgtie operātoŗi

// kopijavimo priskyrimo operatorius [x = y]
Studentas &Studentas::operator=(const Studentas &kitas)
{
    if (this != &kitas)
    {
        vardas_ = kitas.vardas_;
        pavarde_ = kitas.pavarde_;
        pazymiai_ = kitas.pazymiai_;
        egzo_rezas_ = kitas.egzo_rezas_;
        rezas_vid_ = kitas.rezas_vid_;
        rezas_med_ = kitas.rezas_med_;
    }
    return *this;
}

// perkėlimo priskyrimo operatorius
Studentas &Studentas::operator=(Studentas &&kitas)
{
    if (this == &kitas)
        return *this;

    vardas_ = std::move(kitas.vardas_);
    pavarde_ = std::move(kitas.pavarde_);
    pazymiai_ = std::move(kitas.pazymiai_);
    egzo_rezas_ = kitas.egzo_rezas_;
    rezas_vid_ = kitas.rezas_vid_;
    rezas_med_ = kitas.rezas_med_;

    kitas.egzo_rezas_ = 0;
    kitas.rezas_vid_ = 0;
    kitas.rezas_med_ = 0;

    return *this;
}

std::ostream &operator<<(std::ostream &os, const Studentas &stud)
{
    if (Studentas::galut() == "m")
    {
        os
            << std::left << std::setw(20) << stud.vardas_
            << std::left << std::setw(25) << stud.pavarde_
            << std::setw(15) << std::fixed << std::setprecision(2) << stud.rezas_med_;
        return os;
    }
    else // "v"
    {
        os
            << std::left << std::setw(20) << stud.vardas_
            << std::left << std::setw(25) << stud.pavarde_
            << std::setw(15) << std::fixed << std::setprecision(2) << stud.rezas_vid_;
        return os;
    }
}

std::istream &operator>>(std::istream &is, Studentas &stud)
{
    if (!(is >> stud.vardas_ >> stud.pavarde_))
        throw std::invalid_argument("Netinkamas studento duomenu pavidalas.");

    int temp; // laikinas kintamasis pažymių perdavimui
    while (is >> temp)
    {
        stud.pazymiai_.push_back(std::move(temp));
    }

    if (stud.pazymiai_.empty())
    {
        throw std::invalid_argument("Nepavyko nuskaityti studento pazymiu.");
    }

    stud.egzo_rezas_ = stud.pazymiai_.back();
    stud.pazymiai_.pop_back();

    stud.apsk_vid();
    stud.apsk_med();

    return is;
}

void Studentas::pridet_pazymi(int paz)
{
    if (paz >= 0 && paz <= 10)
    {
        pazymiai_.push_back(paz);
    }
}

void Studentas::nust_egzo_reza(int rez)
{
    egzo_rezas_ = rez;
}

void Studentas::ivest_egzo_reza()
{
    int paz;
    natur_skaiciaus_ivestis(paz, ar_sk_ne_tarp_0_ir_10);
    egzo_rezas_ = paz;
}

void Studentas::nust_galutinio_tipa(std::string v_m)
{
    if (v_m == "m")
        galut_ = "m";
    else
        galut_ = "v";
}

void Studentas::apsk_vid()
{
    if (pazymiai_.size() == 0)
        rezas_vid_ = egzo_rezas_ * 0.6;
    int pazymiu_suma = 0;
    for (auto paz : pazymiai_)
        pazymiu_suma += paz;
    rezas_vid_ = ((pazymiu_suma * 1.0) / (pazymiai_.size() * 1.0)) * 0.4 + (egzo_rezas_ * 0.6);
}

void Studentas::apsk_med()
{
    if (pazymiai_.size() == 0)
        return;
    Container(int) visi_pazymiai = pazymiai_;
    visi_pazymiai.push_back(egzo_rezas_);
    rikiavimas(visi_pazymiai); // sort(..) surikiuoja visi_pazymiai vektorių did. tvarka
    int visu_pazymiu_sk = visi_pazymiai.size();
    // vidurinės(-ių) reikšmės(-ių) radimas UNIVERSALIU būdu (tinka ir vector, ir deque, ir list)
    auto it = visi_pazymiai.begin();
    if (visu_pazymiu_sk % 2 != 0)
    {
        rezas_med_ = *std::next(it, visu_pazymiu_sk / 2); // pastumiam iteratorių per pusę visu_pazymiu_sk — į visi_pazymiai vidurį
    }
    else
    {
        auto vid1 = std::next(it, (visu_pazymiu_sk / 2) - 1);
        auto vid2 = std::next(vid1, 1); // pastumiam iteratorių dar per vieną, kad gautume antrąją viduriniąją reikšmę
        rezas_med_ = (*vid1 + *vid2) / 2.0;
    }
}

void Studentas::uzpildyt_pazymius_iki_min(int min_pazymiu_sk)
{
    if (pazymiai_.size() < min_pazymiu_sk)
    {
        pazymiai_.resize(min_pazymiu_sk, 0);
    }
}