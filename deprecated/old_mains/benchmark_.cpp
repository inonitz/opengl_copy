#include "m_simd.h"
#include <chrono>
#include <windows.h>
#include <iostream>
#include <fstream>

int main()
{
    vec4 a{ 1, 2, 3, 4 };
    vec4 b{ 5, 6, 7, 8 };
    float result;
    std::ofstream benchmarks;
    benchmarks.open("src/own_builtin_prod_rel.txt");
    benchmarks << "[" << std::endl;


    for (int j = 0; j < 10000; j++) {
        auto t1 = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 1000; i++)
        {
            // test function performance here
        }
        auto t2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
        benchmarks << duration << ", " << std::endl;

        std::cout << j;
        if (j % 10 == 0) { Sleep(100); }
        std::cout << "\b \b" << "\b \b" << "\b \b" << "\b \b" << "\b \b";
    }


    benchmarks << "]";
    benchmarks.close();
    return 0;
}
