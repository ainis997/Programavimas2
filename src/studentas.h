#include <string>
#include <sstream>

#include "strukturos_konstantos.h"

class Studentas
{
private:
    std::string vardas;
    std::string pavarde;
    Container(int) pazymiai;
    int egzo_rezas;
    float rezas_vid;
    float rezas_med;

public:
    Studentas(std::string vardas, std::string pavarde) : vardas(vardas), pavarde(pavarde), egzo_rezas(0), rezas_vid(0), rezas_med(0) {}
    // gavikai / getteriai
    inline float rezas_vid() const { return rezas_vid; }
    inline float rezas_med() const { return rezas_med; }
    // nustatytojai / setteriai
    void set_egzo_rezas(int paz) { egzo_rezas = paz; }
    void apsk_vid();
    void apsk_med();
    // kt metodai
    std::istringstream &stud_skaitymas(std::istringstream &);
    void stud_spausdinimas();
};

struct Studentas
{
    Container(int) pazymiai;
    std::string vardas = "Vardas";
    std::string pavarde = "Pavarde";
    float rezas_vid = 0;
    float rezas_med = 0;
    int egzo_rezas = 0;
    void apsk_vid(); // void, nes vid. ir med. reik iš anksto apskaičiuot (o ne spausd metu), nes pagal tai reik leist rūšiuot
    void apsk_med();
};