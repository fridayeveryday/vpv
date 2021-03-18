// ConsoleApplication3.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <time.h>
#include <iostream>
#include <intrin.h>
#include <windows.h>
#include <profileapi.h>
using namespace std;

// n & 1 => x1 because 1(10) = 000001(2) 
bool myFunction(unsigned short n) {
    cout << "x1x2x3 | x2!x3x4 | x1!x2 | x1!x2x3!x4 | x3x4";
    //             x1                x2                x3                 
    return ((((n & 2) >> 1) && ((n & 4) >> 2) && ((n & 8) >> 3))
        //       |        x2                !x3                x4            
        || (((n & 4) >> 2) && !((n & 8) >> 3) && ((n & 16) >> 4))
        //   |       x1                !x2 
        || (((n & 2) >> 1) && !((n & 4) >> 2))
        //   |       x1                !x2                x3                !x4
        || (((n & 2) >> 1) && !((n & 4) >> 2) && ((n & 8) >> 3) && !((n & 16) >> 4))
        //   |       x3                x4
        || (((n & 8) >> 3) && ((n & 16) >> 4)));
}

unsigned __int64 fibRecursive(unsigned __int64 n) {
    if (n < 2) return n;
    return fibRecursive(n - 1) + fibRecursive(n - 2);
}

void measureByClock(int n) {
    // measuring by clock
    clock_t time = clock();
    int result = fibRecursive(n);
    time -= clock(); 
    cout << "Result for " << n << " is " << result << ", time is " << (double)(time / CLOCKS_PER_SEC) << " by clock\n";
}

void measureByTSC(int n) {
    clock_t tclock = clock();
    while (clock() < tclock + 1); // ожидание конца текущего такта clock

    tclock = clock();
    unsigned long long tsc = __rdtsc();
    while (clock() < tclock + 1); // ожидание конца начавшегося такта
    //tsc = __rdtsc() – tsc;
    tsc -= __rdtsc(); // сколько тактов TSC прошло за один такт clock
    unsigned long long F1 = tsc * CLOCKS_PER_SEC; // частота процессора

    tclock = clock();
    tsc = __rdtsc();
    while (clock() < tclock + 1); // ожидание конца начавшегося такта
    //tsc = __rdtsc() – tsc;
    tsc -= __rdtsc(); // сколько тактов TSC прошло за один такт clock
    unsigned long long F2 = tsc * CLOCKS_PER_SEC; // частота процессора

    F1 = F1 < F2 ? F1 : F2;

    unsigned long long time = __rdtsc();
    int result = fibRecursive(n);
    time -= __rdtsc();
    cout << "Result for " << n << " is " << result << ", time is" << (double)(time / F1) << "by TSC\n";
}

void measureByQPC(int n) {
    LARGE_INTEGER t_start, t_finish, freqQPC, t;
    QueryPerformanceFrequency(&freqQPC); // получаем частоту
    QueryPerformanceCounter(&t_start); // засекаем время старта CODE
    int result = fibRecursive(n);
    QueryPerformanceCounter(&t_finish);   
    cout << "Result for " << n << " is " << result << ", time is" << (double)((t_finish.QuadPart - t_start.QuadPart) / freqQPC.QuadPart) << "by TSC\n";
}

int main()
{
    //int n = 10;
    //measureByClock(n);
    //measureByTSC(n);
    //measureByQPC(n);

}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
//// measuring by TSC

//clock_t tclock = clock();
//while (clock() < tclock + 1); // ожидание конца текущего такта clock
//
//tclock = clock();
//unsigned long long tsc = __rdtsc();
//while (clock() < tclock + 1); // ожидание конца начавшегося такта
////tsc = __rdtsc() – tsc;
//tsc -= __rdtsc(); // сколько тактов TSC прошло за один такт clock
//unsigned long long F1 = tsc * CLOCKS_PER_SEC; // частота процессора
//
//tclock = clock();
//tsc = __rdtsc();
//while (clock() < tclock + 1); // ожидание конца начавшегося такта
////tsc = __rdtsc() – tsc;
//tsc -= __rdtsc(); // сколько тактов TSC прошло за один такт clock
//unsigned long long F2 = tsc * CLOCKS_PER_SEC; // частота процессора
//
//F1 = F1 < F2 ? F1 : F2;
//time = clock();
//fibRecursive(10);
//time -= clock();
//cout << "Result by TSC is " << time / F1 << "\n";
