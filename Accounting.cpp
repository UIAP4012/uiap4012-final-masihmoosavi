#include <iostream>
#include <windows.h>

#define green HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE) ;SetConsoleTextAttribute(hConsole,10)
#define red HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE) ;SetConsoleTextAttribute(hConsole,12)
#define blue HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE) ;SetConsoleTextAttribute(hConsole,11)
#define white HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE) ;SetConsoleTextAttribute(hConsole,7)
#define yelow HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE) ;SetConsoleTextAttribute(hConsole,6)

using namespace std;



int main()
{
    return 0;
}
