#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>

#include "strukturos_konstantos.h"

class Studentas
{
private:
    std::string vardas_;
    std::string pavarde_;
    Container(int) pazymiai_;
    int egzo_rezas_;
    float rezas_;
    enum class Galutinis : char
    {
        VID,
        MED
    };
    Galutinis gal_;
    // float rezas_vid;
    // float rezas_med;

public:
    Studentas(std::string vardas_, std::string pavarde_) : vardas_(vardas_), pavarde_(pavarde_), egzo_rezas_(0), rezas_(0) {}

    // gavikai / getteriai

    inline float rezas() const { return rezas_; }

    // nustatytojai / setteriai

    void set_egzo_rezas_(int paz) { egzo_rezas_ = paz; }
    void apsk_galutini()
    {
        if (gal_ == Galutinis::MED)
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
                rezas_ = *std::next(it, visu_pazymiu_sk / 2); // pastumiam iteratorių per pusę visu_pazymiu_sk — į visi_pazymiai vidurį
            }
            else
            {
                auto vid1 = std::next(it, (visu_pazymiu_sk / 2) - 1);
                auto vid2 = std::next(vid1, 1); // pastumiam iteratorių dar per vieną, kad gautume antrąją viduriniąją reikšmę
                rezas_ = (*vid1 + *vid2) / 2.0;
            }
        }
        else
        {
            if (pazymiai_.size() == 0)
                rezas_ = egzo_rezas_ * 0.6;
            int pazymiu_suma = 0;
            for (auto paz : pazymiai_)
                pazymiu_suma += paz;
            rezas_ = ((pazymiu_suma * 1.0) / (pazymiai_.size() * 1.0)) * 0.4 + (egzo_rezas_ * 0.6);
        }
    };
    // void apsk_vid();
    // void apsk_med();

    // kt metodai

    void skaityt_studenta(std::istringstream &srautas, std::string eil)
    {
        srautas.clear();  // išvalo srautą (išvalo nuo praeitos iteracijos likusį statusą); be šito, skaitytų tik pirmą eilutę iš viso failo! (neveiktų)
        srautas.str(eil); // įstato naują stringą (eilutę) srautan

        if (!(srautas >> vardas_ >> pavarde_))
            throw std::invalid_argument("Netinkamas studento duomenu pavidalas.");

        int temp; // laikinas kintamasis pažymių perdavimui
        while (srautas >> temp)
        {
            pazymiai_.push_back(std::move(temp));
        }

        if (pazymiai_.empty())
        {
            throw std::invalid_argument("Nepavyko nuskaityti studento pazymiu.");
        }
        egzo_rezas_ = pazymiai_.back(); // paskutinis elementas — egzamino rezas_
        pazymiai_.pop_back();           // ištrinam egzo rezą iš pažymių vektoriaus

        // apsk_vid();
        // apsk_med();

        apsk_galutini();
    }
    void studento_spausd(std::ofstream &ras_failas)
    {
        ras_failas
            << std::left << std::setw(20) << vardas_
            << std::left << std::setw(25) << pavarde_
            << std::setw(15) << std::fixed << std::setprecision(2) << rezas_
            << '\n';
    }
};

// struct Studentas
// {
//     Container(int) pazymiai_;
//     std::string vardas_ = "Vardas";
//     std::string pavarde_ = "pavarde_";
//     float rezas__vid = 0;
//     float rezas__med = 0;
//     int egzo_rezas__ = 0;
//     void apsk_vid(); // void, nes vid. ir med. reik iš anksto apskaičiuot (o ne spausd metu), nes pagal tai reik leist rūšiuot
//     void apsk_med();
// };