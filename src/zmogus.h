#include <string>
#include <vector>

class Zmogus
{
private:
    std::string vardas_;
    std::string pavarde_;

public:
    virtual ~Zmogus() = 0;

    // perkrauti operatoriai

    // kopijavimo priskyrimo operatorius [x = y]
    virtual Zmogus &operator=(const Zmogus &kitas) = 0;
    // perkėlimo priskyrimo operatorius [x = std::move(y)]
    virtual Zmogus &operator=(Zmogus &&kitas) = 0;

    // friend std::ostream &operator<<(std::ostream &os, const Zmogus &stud);
    // friend std::istream &operator>>(std::istream &is, Zmogus &stud);

    // gavikai / getteriai

    inline std::string vardas() const { return vardas_; }
    inline std::string pavarde() const { return pavarde_; }

    // nustatytojai / setteriai

    void nust_varda(std::string vardas) { vardas_ = vardas; }
    void nust_pavarde(std::string pavarde) { pavarde_ = pavarde; }
    bool ivest_varda_pavarde(bool ar_ivestis_atsaukiama = false);
};