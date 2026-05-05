# Programos naudojimo instrukcija

Programos esmė — apdoroti ar generuoti studentų duomenis.

Programa turi 6 skirtingas eigas, matomas pradiniame programos meniu:

1. Studentų duomenų įvedimas iš duomenų failo

- Naudotojas gali pasirinkti aplanke "ivesties_failai" esančius tekstinius failus (jeigu jų yra) su studentų duomenimis (vardu, pavarde, pažymiais), apdoroti failą ir išvesti rezultatus (apie apdorojimą, išvedimą žr. žemiau).

2. Studentų ir jų pažymių įvedimas ranka

- Naudotojas gali pats ranka įvesti norimą skaičių studentų ir jų pažymius, duomenys apdorojami ir išvedamas rezultatas.

3. Studentų duomenų įvedimas ranka ir jų pažymių sugeneravimas

- Naudotojas gali ranka įvesti norimą skaičių studentų ir sugeneruoti jiems norimą skaičių pažymių, duomenys apdorojami ir išvedamas rezultatas.

4. Studentų ir jų pažymių sugeneravimas

- Naudotojo pageidavimu gali būti sugeneruojamas norimas skaičius studentų su norimu skaičiumi pažymių, duomenys apdorojami ir išvedamas rezultatas.

5. Studentų ir jų pažymių išvedimas į failą

- Naudotojo pageidavimu gali būti sugeneruojamas norimas skaičius studentų su norimu skaičiumi pažymių ir duomenys išvedami į failą.

6. Programos baigimas

Duomenų apdorojimas ir išvedimas:

- Įvedus ar sugeneravus studentų ir jų pažymių duomenis, naudotojas gali pasirinkti galutinio vertinimo skaičiavimo būdą (pagal vidurkį arba medianą), pasirinkti studentų surikiavimą išvestyje (pagal vardą, pavardę, galutinį balą (vidurkį/medianą) arba nerikiuoti) ir pasirinkti išvesties failo pavadinimą. Rezultatas (lentelės pavidalo) išvedamas folderyje isvesties_failai į tekstinį failą su naudotojo pageidautu pavadinimu.

# Paleidimo instrukcija

- Atsisiųsti programos kodą (failai prisegti prie šio programos leidimo);
- Atidaryti komandinę eilutę programos aplanke;
- Įvesti komandą "make" (arba kitą jūsų turimo kūrimo įrankio komandą, pvz.: "mingw32-make", jei naudojate MinGW paketą), ją įvykdžius bus sukurtas programos paleidžiamasis failas;
  - (šiam žingsniui įvykdyti kompiuteryje turi būti įdiegtas kuris nors programų sukūrimo įrankis, pvz., MinGW-w64, MSVC)
- Įvesti "./bin/programa" arba "make run" (ar "mingw32-make run"), taip bus paleista programa (bus matomas programos pradinis meniu).

<sup>Pastaba: programos paleidimas pritaikytas Windows operacinei sistemai.</sup>

# Programos kodas

### Programos kode perdengti metodai

Programoje naudojamoje studento klasėje jos naudotojų patogumui perdengti kai kurie operatoriai:

- išvesties srauto operatorius (<<): šis operatorius pritaikytas tiesiogiai išvesti studento klasės objektus į terminalą ar į failą;
- įvesties srauto operatorius (>>): šis operatorius pritaikytas įvesti duomenis į studento objektą;
- priskyrimo operatorius (=): šis operatorius pritaikytas studento klasės kintamajam priskirti tiek kopijuote (copy assignment), tiek perkelte (move assignment).

Taip pat klasėje papildomai perdengti ir konstruktorių metodai, pridėti kopijavimo ir perkėlimo konstruktoriai.

### Sukurtos savos Vector klasės pavyzdžiai

##### 1. push_back

push_back metodas skirtas pridėti naują elementą į Vector klasės objektą. Šis metodas priima vieną argumentą — pridėtiną elementą, ir prideda jį į Vector pabaigą.

```
#include <iostream>
#include "Vector.h"

int main() 
{
    Vector<int> numbers;
    
    numbers.push_back(10);
    numbers.push_back(20);
    numbers.push_back(30);

    std::cout << "numbers[0]: " << numbers[0] << "\n";
    // numbers[0]: 10
    std::cout << "numbers[1]: " << numbers[1] << "\n";
    // numbers[1]: 20
    std::cout << "numbers[2]: " << numbers[2] << "\n";
    // numbers[2]: 30

    return 0;
}
```

##### 2. size

size metodas grąžina Vector klasės objekto elementų skaičių. Šis metodas neturi jokių argumentų ir grąžina sveikąjį skaičių, kuris nurodo, kiek elementų yra Vector objekte.

```
#include <iostream>
#include "Vector.h"

int main() 
{
    Vector<int> numbers;
    
    numbers.push_back(10);
    numbers.push_back(20);
    numbers.push_back(30);

    std::cout << "Vektoriaus dydis: " << numbers.size() << "\n";
    // Vektoriaus dydis: 3

    return 0;
}
```

##### 3. at

at metodas skirtas gauti elementą iš Vector klasės objekto pagal nurodytą indeksą. Šis metodas priima vieną argumentą — indeksą, ir grąžina elementą, esantį toje pozicijoje Vector objekte. Jeigu indeksas yra už Vector ribų, at metodas išmes klaidą.

```
#include <iostream>
#include "Vector.h"

int main() 
{
    Vector<std::string> strings = {"abc", "def", "ghi"};

    std::cout << words.at(1) << "\n";
    // def

    try
    {
        words.at(10); // tyčia bandoma pasiekti neegzistuojantį elementą
    }
    catch (const std::out_of_range& e) 
    {
        std::cout << "Klaida: " << e.what() << "\n"; 
        // Klaida: Vector::at() : Index 10 is out of range...
    }

    return 0;
}
```

##### 4. insert (vieno elemento įterpimas)

insert metodas skirtas įterpti naują elementą į Vector klasės objektą nurodytoje pozicijoje. Šis metodas priima du argumentus — iteratorių, prieš kurį norima įterpti elementą, ir patį elementą, kurį norima įterpti.

```
#include <iostream>
#include "Vector.h"

int main() 
{
    Vector<char> letters = {'A', 'C', 'D'};

    // įterpiame 'B' į antrąją poziciją
    letters.insert(letters.begin() + 1, 'B');

    std::cout << "Raidės: ";
    for (char c : letters) 
    {
        std::cout << c << " ";
    }
    // Raidės: A B C D 

    return 0;
}
```

##### 5. operator==

operator== metodas skirtas palyginti du Vector klasės objektus ir nustatyti, ar jie yra lygūs (t.y. turi tuos pačius elementus tose pačiose pozicijose). Šis operatorius priima du argumentus — du Vector objektus, ir grąžina true, jei abu Vector objektai turi tą patį elementų skaičių ir atitinkamus elementus, arba false, jei jie skiriasi.

```
#include <iostream>
#include "Vector.h"

int main() 
{
    Vector<int> v1 = {1, 2, 3};
    Vector<int> v2 = {1, 2, 3};
    Vector<int> v3 = {1, 2, 4};

    if (v1 == v2) 
    {
        std::cout << "v1 ir v2 yra lygūs.\n";
    }
    if (v1 != v3)
    {
        std::cout << "v1 ir v3 skiriasi.\n";
    }
    
    // Išvestis:
    // v1 ir v2 yra lygūs.
    // v1 ir v3 skiriasi.

    return 0;
}
```


# Programos leidimai

## v3.0

### Diegiklis

- Sukurtas programos diegiklis, leidžiantis lengvai įdiegti programą į kompiuterį.
- Diegiklis pritaikytas Windows operacinei sistemai.

### Vector klasė

- Sukurta savarankiška Vector klasė, imituojanti std::vector funkcionalumą.
- Atlikti testavimai, lyginantys Vector klasės ir std::vector veikimą. Rezultatai įtraukti į projekto README.md failą.
- Vector klasė pilnai ištestuota.

## v2.0

### Dokumentacija

- Naudojantis Doxygen įrankiu sukurta dokumentacija.
- Dokumentacija prieinama tiek HTML, tiek LaTeX, tiek PDF formatais.

## v1.5

### Klasių pertvarkymas

- Programoje sukurta abstrakti žmogus klasė, iš kurios paveldi studento klasė.
- Programos funkcionalumas išlaikytas lygiai toks pat.

## v1.2

### Metodų papildymas

- Programoje naudojama studento klasė papildyta naujais metodais: kopijavimo ir perkėlimo konstruktoriais, kopijavimo ir perkėlimo priskyrimo operatorių bei įvesties ir išvesties operatorių perdengimais.

### Testai

- Programa papildyta paruoštais kodo veikimo testais: paruošti testai šioje versijoje pridėtiems naujiesiems studento klasės metodams, skirti patikrinti jų tinkamą veikimą.

## v1.1

### Perėjimas prie klasių

- Programos kode pereita nuo struktūrų naudojimo prie klasių naudojimo.

### Testavimai

- README.md faile aprašytas programos trukmės tyrimas, kuriame lyginama programos trukmė priklausomai nuo: klasių ar struktūrų naudojimo, kompiliatoriaus optimizavimo lygmens (O1, O2, O3). Taip pat palygintas ir šių skirtingų programos versijų paleidžiamųjų (.exe) failų dydis.

## v1.0

### Programos paruošimas

- Programa labiau paruošta naudojimui. README.md faile įtrauktas programos aprašas, naudojimosi, paleidimo instrukcijos.

### Optimizavimas

- Šiek tiek pagerintas programos veikimo laikas.
- Atlikti išsamūs programos trukmės testavimai įvairiomis sąlygomis (įvairiais duomenų kiekiais, kode naudojamais konteineriais). Testavimų rezultatai įtraukti į projekto README.md failą.

## v1.0 pradinė

### Skirtingi konteineriai

- Programos kodas pritaikytas trims skirtingiems duomenų konteineriams: std::vector, std::deque, std::list.

### Optimizavimas

- Pagerintas programos veikimas: pagerinta programos sparta ir atminties naudojimas.
- Programos versijų su skirtingais konteineriais trukmės testavimas aprašytas README.md faile.

## v0.4

### Failų generavimas

- Pridėtas visų studentų duomenų (įskaitant ir paskirus pažymius) failo generavimo funkcionalumas.

### Studentų išvesties skirstymas

- Studentų duomenys išvedami į du atskirus failus, atrenkant pagal studentų galutinį įvertinimą — vidurkį/medianą (< 5.0 vienur, >= 5.0 kitur).

### Programos trukmės matavimas

- Programoje pridėti nauji programos etapų trukmės matavimai (failų kūrimo ir apdorojimo).
- Programos trukmės testavimo skirtingais krūviais aprašas pridėtas į projekto README.md failą.

## v0.3

### Pakeitimai:

- Projektas suskaidytas į atskirus savo paskirties failus;
- Programoje pridėta daugiau išimčių valdymo;
- Programos funkcijos tapo labiau struktūruotos.

## v0.2

### Failų funkcionalumas

- Pridėta galimybė duomenis nuskaityti iš pasirinkto failo.
- Įtraukta galimybė išvestyje studentus surūšiuoti pagal pasirinktą parametrą: vardą, pavardę, galutinį pažymį (vidurkio ar medianos); didėjimo ar mažėjimo tvarka.
- Pridėta programos išvestis į failą.

## v0.1

### Meniu

Pridėtas programos meniu, siūlantis 4 skirtingas programos eigas:

- rankinis visų duomenų įvedimas;
- rankinis vardų įvedimas, pažymių sugeneravimas;
- visų duomenų sugeneravimas;
- programos baigimas.

## v.pradinė

Pradinės versijos programa, kuri:

- nuskaito studentų duomenis (vardą, pavardę, pažymius, egzaminų įvertinimą);
- apskaičiuoja galutinį balą (pagal vidurkį arba medianą);
- pateikia visus reikalingus duomenis lentelėje.

# std::vector vs Vector spartos testavimai

...

##### 10 tūkst. įterpimų:

std::vector laikas su 10000 elementu: 0.000151
Vector laikas su 10000 elementu: 9.04e-05

std::vector laikas su 10000 elementu: 0.0001279
Vector laikas su 10000 elementu: 8.88e-05

std::vector laikas su 10000 elementu: 0.0001678
Vector laikas su 10000 elementu: 0.0001074

std::vector laikas su 10000 elementu: 0.0001559
Vector laikas su 10000 elementu: 9.87e-05

std::vector laikas su 10000 elementu: 0.000136
Vector laikas su 10000 elementu: 8.47e-05

##### 100 tūkst. įterpimų:

std::vector laikas su 100000 elementu: 0.0010157
Vector laikas su 100000 elementu: 0.0008471

std::vector laikas su 100000 elementu: 0.0021863
Vector laikas su 100000 elementu: 0.0017557

std::vector laikas su 100000 elementu: 0.0014561
Vector laikas su 100000 elementu: 0.0010383

std::vector laikas su 100000 elementu: 0.001109
Vector laikas su 100000 elementu: 0.0008951

std::vector laikas su 100000 elementu: 0.0010903
Vector laikas su 100000 elementu: 0.0013591

##### 1 mln. įterpimų:

std::vector laikas su 1000000 elementu: 0.0090452
Vector laikas su 1000000 elementu: 0.0072943

std::vector laikas su 1000000 elementu: 0.0082193
Vector laikas su 1000000 elementu: 0.0072184

std::vector laikas su 1000000 elementu: 0.0093086
Vector laikas su 1000000 elementu: 0.0087092

std::vector laikas su 1000000 elementu: 0.0082187
Vector laikas su 1000000 elementu: 0.0075397

std::vector laikas su 1000000 elementu: 0.0082514
Vector laikas su 1000000 elementu: 0.0081633

##### 10 mln. įterpimų:

std::vector laikas su 10000000 elementu: 0.091775
Vector laikas su 10000000 elementu: 0.0776703

std::vector laikas su 10000000 elementu: 0.0908184
Vector laikas su 10000000 elementu: 0.0761093

std::vector laikas su 10000000 elementu: 0.090597
Vector laikas su 10000000 elementu: 0.0773267

std::vector laikas su 10000000 elementu: 0.100054
Vector laikas su 10000000 elementu: 0.0769011

std::vector laikas su 10000000 elementu: 0.0886303
Vector laikas su 10000000 elementu: 0.0770143

##### 100 mln. įterpimų:

std::vector laikas su 100000000 elementu: 0.891555
Vector laikas su 100000000 elementu: 0.793463

std::vector laikas su 100000000 elementu: 0.867467
Vector laikas su 100000000 elementu: 0.762474

std::vector laikas su 100000000 elementu: 0.871052
Vector laikas su 100000000 elementu: 0.761339

std::vector laikas su 100000000 elementu: 0.866032
Vector laikas su 100000000 elementu: 0.776693

std::vector laikas su 100000000 elementu: 0.876186
Vector laikas su 100000000 elementu: 0.782664

##### Vidurkiai

| Įterpimų kiekis | std::vector laikas (s) | Vector laikas (s) |
| --------------- | ---------------------- | ----------------- |
| 10 000          | 0.00014772             | 0.00009400        |
| 100 000         | 0.00137148 s           | 0.00117906 s      |
| 1 000 000       | 0.00860864 s           | 0.00778498 s      |
| 10 000 000      | 0.09237494 s           | 0.07700434 s      |
| 100 000 000     | 0.87445840 s           | 0.77532660 s      |

#### Atminties perskirstymų sk.

Palygintas atminties perskirstymų skaičius tarp std::vector ir Vector įterpiant 100 mln. elementų.
Rezultatas:

- std::vector perskirstymų sk.: 27
- Vector perskirstymų sk.: 27

# Programos trukmės testavimai (std::vector vs Vector)

Programos trukmė išmatuota dviem programos versijoms: naudojančiai std::vector ir naudojančiai Vector (savasis std::vector įgyvendinimas). Testavimai atlikti kiekvienai versijai su 100 tūkst., 1 mln. ir 10 mln. studentų įrašų apdorojimu, kiekvieną bandymą kartojant 5 kartus. Visuose testavimuose studentų skirstymo strategija — 3.

## std::vector

##### 100 000 įrašų:

![alt text](images/image-26.png)
![alt text](images/image-30.png)
![alt text](images/image-29.png)
![alt text](images/image-28.png)
![alt text](images/image-27.png)

##### 1 000 000 įrašų:

![alt text](images/image-25.png)
![alt text](images/image-24.png)
![alt text](images/image-23.png)
![alt text](images/image-22.png)
![alt text](images/image-21.png)

##### 10 000 000 įrašų:

![alt text](images/image-16.png)
![alt text](images/image-17.png)
![alt text](images/image-18.png)
![alt text](images/image-19.png)
![alt text](images/image-20.png)

## Vector

##### 100 000 įrašų:

![alt text](images/image-15.png)
![alt text](images/image-14.png)
![alt text](images/image-13.png)
![alt text](images/image-12.png)
![alt text](images/image-11.png)

##### 1 000 000 įrašų:

![alt text](images/image-10.png)
![alt text](images/image-9.png)
![alt text](images/image-8.png)
![alt text](images/image-7.png)
![alt text](images/image-6.png)

##### 10 000 000 įrašų:

![alt text](images/image-1.png)
![alt text](images/image-2.png)
![alt text](images/image-3.png)
![alt text](images/image-4.png)
![alt text](images/image-5.png)

## Rezultatai

#### std::vector:

| Kiekis     | Nuskaitymas (vid.) | Rikiavimas (vid.) | Skirstymas (vid.) | Programa (vid.) |
| ---------- | ------------------ | ----------------- | ----------------- | --------------- |
| 100 000    | 0.32558s           | 0.02015s          | 0.02279s          | 0.49384s        |
| 1 000 000  | 3.26087s           | 0.26798s          | 0.21911s          | 5.09135s        |
| 10 000 000 | 33.32956s          | 3.28830s          | 2.14918s          | 52.67920s       |

#### Vector:

| Kiekis     | Nuskaitymas (vid.) | Rikiavimas (vid.) | Skirstymas (vid.) | Programa (vid.) |
| ---------- | ------------------ | ----------------- | ----------------- | --------------- |
| 100 000    | 0.30005s           | 0.02534s          | 0.01138s          | 0.46304s        |
| 1 000 000  | 2.94906s           | 0.33318s          | 0.11011s          | 4.67513s        |
| 10 000 000 | 29.92548s          | 4.08812s          | 1.08142s          | 49.10946s       |

#### Išvada

Bendrai Vector versija veikia greičiau nei std::vector versija. Tai lemia tiek greitesnis nuskaitymas, tiek žymiai greitesnis skirstymas. Visgi Vector rikiavimas yra šiek tiek lėtesnis nei std::vector, tačiau bendros programos trukmės rezultato tai pernelyg neįtakoja.

# Programos trukmės testavimai (klasės vs struktūros)

Programos trukmė išmatuota dviem programos versijoms: struktūrų ir klasių versijoms. Testavimai atlikti kiekvienai versijai su 3 skirtingais kompiliatoriaus optimizavimo lygmenimis (O1, O2, O3), su 100 tūkst. ir 1 mln. studentų įrašų apdorojimu, kiekvieną bandymą kartojant 5 kartus. Visuose testavimuose naudojamas konteineris — std::vector, studentų skirstymo strategija — 3.

Testavimo sistemos parametrai:

- CPU: AMD Ryzen 5 4600H 3GHz
- RAM: 16 GB
- SSD: Lexar SSD NM710 1TB

## Klasių versija

### O1

Paleidžiamojo (.exe) failo dydis: 245 KB

##### 100 tūkst. įrašų:

![alt text](images/image-204.png)
![alt text](images/image-203.png)
![alt text](images/image-202.png)
![alt text](images/image-201.png)
![alt text](images/image-200.png)

##### 1 mln. įrašų:

![alt text](images/image-209.png)
![alt text](images/image-208.png)
![alt text](images/image-207.png)
![alt text](images/image-206.png)
![alt text](images/image-205.png)

### O2

Paleidžiamojo (.exe) failo dydis: 257 KB

##### 100 tūkst. įrašų:

![alt text](images/image-214.png)
![alt text](images/image-213.png)
![alt text](images/image-212.png)
![alt text](images/image-211.png)
![alt text](images/image-210.png)

##### 1 mln. įrašų:

![alt text](images/image-219.png)
![alt text](images/image-218.png)
![alt text](images/image-217.png)
![alt text](images/image-216.png)
![alt text](images/image-215.png)

### O3

Paleidžiamojo (.exe) failo dydis: 298 KB

##### 100 tūkst. įrašų:

![alt text](images/image-224.png)
![alt text](images/image-223.png)
![alt text](images/image-222.png)
![alt text](images/image-221.png)
![alt text](images/image-220.png)

##### 1 mln. įrašų:

![alt text](images/image-229.png)
![alt text](images/image-228.png)
![alt text](images/image-227.png)
![alt text](images/image-226.png)
![alt text](images/image-225.png)

## Struktūrų versija

### O1

Paleidžiamojo (.exe) failo dydis: 255 KB

##### 100 tūkst. įrašų:

![alt text](images/image-234.png)
![alt text](images/image-233.png)
![alt text](images/image-232.png)
![alt text](images/image-231.png)
![alt text](images/image-230.png)

##### 1 mln. įrašų:

![alt text](images/image-239.png)
![alt text](images/image-238.png)
![alt text](images/image-237.png)
![alt text](images/image-236.png)
![alt text](images/image-235.png)

### O2

Paleidžiamojo (.exe) failo dydis: 250 KB

##### 100 tūkst. įrašų:

![alt text](images/image-244.png)
![alt text](images/image-243.png)
![alt text](images/image-242.png)
![alt text](images/image-241.png)
![alt text](images/image-240.png)

##### 1 mln. įrašų:

![alt text](images/image-249.png)
![alt text](images/image-248.png)
![alt text](images/image-247.png)
![alt text](images/image-246.png)
![alt text](images/image-245.png)

### O3

Paleidžiamojo (.exe) failo dydis: 300 KB

##### 100 tūkst. įrašų:

![alt text](images/image-254.png)
![alt text](images/image-253.png)
![alt text](images/image-252.png)
![alt text](images/image-251.png)
![alt text](images/image-250.png)

##### 1 mln. įrašų:

![alt text](images/image-255.png)
![alt text](images/image-256.png)
![alt text](images/image-257.png)
![alt text](images/image-258.png)
![alt text](images/image-259.png)

## Išvados

### Klasių versija:

##### Su O1:

| Failo įrašų sk.   | 100 tūkst. | 1 mln.    |
| ----------------- | ---------- | --------- |
| Nuskaitymas (s)   | 0,3708698  | 3,6539520 |
| Surikiavimas (s)  | 0,0772971  | 1,0453140 |
| Išskirstymas (s)  | 0,0401847  | 0,3584878 |
| Visa programa (s) | 0,6203628  | 6,3330980 |

##### Su O2:

| Failo įrašų sk.   | 100 tūkst. | 1 mln.    |
| ----------------- | ---------- | --------- |
| Nuskaitymas (s)   | 0,3466234  | 3,4449840 |
| Surikiavimas (s)  | 0,0712134  | 0,9483184 |
| Išskirstymas (s)  | 0,0354765  | 0,3387724 |
| Visa programa (s) | 0,5766506  | 5,9584880 |

##### Su O3:

| Failo įrašų sk.   | 100 tūkst. | 1 mln.    |
| ----------------- | ---------- | --------- |
| Nuskaitymas (s)   | 0,3460546  | 3,4596140 |
| Surikiavimas (s)  | 0,0676074  | 0,9484736 |
| Išskirstymas (s)  | 0,0385799  | 0,3410182 |
| Visa programa (s) | 0,5742380  | 5,9198040 |

### Struktūrų versija:

##### Su O1:

| Failo įrašų sk.   | 100 tūkst. | 1 mln.    |
| ----------------- | ---------- | --------- |
| Nuskaitymas (s)   | 0,3174212  | 3,1974620 |
| Surikiavimas (s)  | 0,0170127  | 0,2187234 |
| Išskirstymas (s)  | 0,0099000  | 0,0872966 |
| Visa programa (s) | 0,4694574  | 4,7772480 |

##### Su O2:

| Failo įrašų sk.   | 100 tūkst. | 1 mln.    |
| ----------------- | ---------- | --------- |
| Nuskaitymas (s)   | 0,3256272  | 3,1813160 |
| Surikiavimas (s)  | 0,0185178  | 0,2455842 |
| Išskirstymas (s)  | 0,0104346  | 0,0899841 |
| Visa programa (s) | 0,4855236  | 4,7933540 |

##### Su O3:

| Failo įrašų sk.   | 100 tūkst. | 1 mln.    |
| ----------------- | ---------- | --------- |
| Nuskaitymas (s)   | 0,3115604  | 3,1576640 |
| Surikiavimas (s)  | 0,0194823  | 0,2458984 |
| Išskirstymas (s)  | 0,0101887  | 0,0906315 |
| Visa programa (s) | 0,4686702  | 4,7470540 |

### Visos programos laikai (su .exe failų dydžiais):

| Failo įrašų sk.             | 100 tūkst. | 1 mln.    | .exe failo dydis: |
| --------------------------- | ---------- | --------- | ----------------- |
| Klasių versija su O1 (s)    | 0,6203628  | 6,3330980 | 245 KB            |
| Klasių versija su O2 (s)    | 0,5766506  | 5,9584880 | 257 KB            |
| Klasių versija su O3 (s)    | 0,5742380  | 5,9198040 | 298 KB            |
| Struktūrų versija su O1 (s) | 0,4694574  | 4,7772480 | 255 KB            |
| Struktūrų versija su O2 (s) | 0,4855236  | 4,7933540 | 250 KB            |
| Struktūrų versija su O3 (s) | 0,4686702  | 4,7470540 | 300 KB            |

### Išvada:

Klasių versijos programa veikia lėčiau nei struktūrų versija (visa programa trunka apie ketvirtadaliu, trečdaliu lėčiau). Itin didelis spartos skirtumas matomas surikiavimo, taip pat ir išskirstymo etapuose (greitis skiriasi keliais kartais).
Kompiliatoriaus optimizavimo vėliavėlės programos spartą veikė šiek tiek nevienareikšmiškai. Klasių versijoje aukštesnio optimizacijos lygmens poveikis matomas: su O2 programa veikia šiek tiek greičiau nei O1, o su O3 — šiek tiek greičiau nei O2. Visgi struktūrų versijoje aukštesnio optimizacijos lygmens poveikis nėra matomas: su O2 vėliavėle kompiliuota programa veikia šiek tiek lėčiau nei su O1 ar O3, o su O3 programa veikia panašia sparta kaip su O1 (ar šiek tiek greičiau).
Skirtingų versijų programų paleidžiamieji (.exe) failai užima šiek tiek skirtingą atminties kiekį. Klasių versijos programos failas užima panašų (šiek tiek mažesnį) kilobaitų skaičių kaip struktūrų versija. Aukštesnio optimizacijos laipsnio programų failai įprastai užima šiek tiek daugiau vietos, nors struktūrų versijos O2 versijos .exe failas užima šiek tiek mažiau vietos nei O1.
