#include "zmogus.h"

bool Zmogus::ivest_varda_pavarde(bool ar_ivestis_atsaukiama)
{
    std::cout << "Iveskite varda ir pavarde: ";
    for (;;)
    {
        try
        {
            std::string vardas_pavarde;
            if (std::getline(std::cin, vardas_pavarde)) // jeigu sėkmingai nuskaito eilutę, tai ...
            {
                if (vardas_pavarde.empty())
                {
                    throw std::invalid_argument("Tuscia ivestis.");
                }
                if (vardas_pavarde == "x" && ar_ivestis_atsaukiama) // sąlyga, reikalinga mišrios įvesties f-jai (kad nutraukt studentų įvedimą)
                {
                    return false;
                }
                std::istringstream sr(vardas_pavarde);
                std::string vardo_ivestis, pavardes_ivestis;
                std::string perteklius;
                if (sr >> vardo_ivestis >> pavardes_ivestis) // jeigu sėkmingai nuskaitytos dvi vertės - vardo ir pavardės - tai ...
                {
                    if (sr >> perteklius) // jeigu ivesta per daug duomenu
                    {
                        throw std::invalid_argument("Pertekline ivestis.");
                    }
                    vardas_ = vardo_ivestis;
                    pavarde_ = pavardes_ivestis;
                    break;
                }
                else
                    throw std::invalid_argument("Netinkama ivestis.");
            }
            else
                throw std::invalid_argument("Netinkama ivestis.");
        }
        catch (...)
        {
            ivesties_klaidos_valdymas();
        }
    }
    return true;
}