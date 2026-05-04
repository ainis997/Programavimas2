#include <vector>
#include "../src/vector.h"

#include <chrono>
#include <iostream>

int main()
{
    // ===== std::vector vs Vector atminties perskirstymų sk. testas
    std::vector<int> v1;
    int v1_perskirstymai = 0;

    for (int i = 1; i <= 100000000; ++i)
    {
        v1.push_back(i);
        if (v1.size() == v1.capacity())
            v1_perskirstymai++;
    }

    Vector<int> v2;
    int v2_perskirstymai = 0;

    for (int i = 1; i <= 100000000; ++i)
    {
        v2.push_back(i);
        if (v2.size() == v2.capacity())
            v2_perskirstymai++;
    }

    std::cout << "std::vector perskirstymu sk.: " << v1_perskirstymai << '\n';
    std::cout << "Vector perskirstymu sk.: " << v2_perskirstymai << '\n';

    // ===== std::vector vs Vector įterpimo spartos testas
    // unsigned int sz;
    // for (;;)
    // {
    //     std::cout << "Dydis: ";
    //     std::cin >> sz; // 10000, 100000, 1000000, 10000000, 100000000
    //     std::cout << '\n';

    //     auto v1_prad = std::chrono::high_resolution_clock::now();

    //     std::vector<int> v1;

    //     for (int i = 1; i <= sz; ++i)
    //         v1.push_back(i);

    //     auto v1_pab = std::chrono::high_resolution_clock::now();

    //     auto v2_prad = std::chrono::high_resolution_clock::now();

    //     Vector<int> v2;

    //     for (int i = 1; i <= sz; ++i)
    //         v2.push_back(i);

    //     auto v2_pab = std::chrono::high_resolution_clock::now();

    //     std::chrono::duration<double> v1_laikas = v1_pab - v1_prad;
    //     std::chrono::duration<double> v2_laikas = v2_pab - v2_prad;

    //     std::cout << "std::vector laikas su " << sz << " elementu: " << v1_laikas.count() << '\n';
    //     std::cout << "Vector laikas su " << sz << " elementu: " << v2_laikas.count() << '\n';
    // }
}

// kompiliavimo komanda:
// g++ -std=c++20 testavimas/std_vector_vs_Vector.cpp -o testavimas/std_vector_vs_Vector