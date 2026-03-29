#include <iostream>

#include "klaidu_valdymas.h"

// wīrst boūwuns mazīngi panzdau powalnint šin pūnkcijan, preikalsīt wisēimans prēipalimans (preidāt tolimesne_zinute paramētran)
void ivesties_klaidos_valdymas()
{
    try
    {
        throw; // permetam klaidą vėl, kad fjoj galėtumbim apdorot
    }
    catch (const std::exception &e)
    {
        std::cerr << "Klaida: " << e.what() << '\n';
    }
    catch (...)
    {
        std::cerr << "Ivyko nezinoma klaida." << '\n';
    }
    std::cout << "Iveskite dar karta: ";
}