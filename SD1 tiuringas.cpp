#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include <atomic>
#include <typeinfo>
#include <conio.h>
#include <set>

using namespace std;

struct Masina1
{
    string esamaB, naujaB;
    char esamasS, naujasS, kryptis;
};
struct Masina2
{
    string esamaB, naujaB;
    char esamasS1, esamasS2, naujasS1, naujasS2, kryptis1, kryptis2;
};

int main(int argc, char* argv[])
{
	cout << "Tiuringo masina veikia automatiskai. Norint sustabdyti programa, spauskite 'space' klavisa." << endl;
	cout << "Iveskite failo numeri: ";
    int failas, LinijuKiekis, zingsniai = 0;
    cin >> failas;
    set<string> VisosB;
    ifstream fd(argv[failas]);

    fd >> LinijuKiekis;
    if (LinijuKiekis == 1)
    {
        string juosta;
        int galvutespozicija;
        fd >> juosta >> galvutespozicija;
        galvutespozicija--;

        vector<Masina1> a;
        Masina1 temp;
        while (fd >> temp.esamaB >> temp.esamasS >> temp.naujasS >> temp.kryptis >> temp.naujaB)
        {
            a.push_back(temp);
            VisosB.insert(temp.esamaB);
        }

        fd.close();
        string busena = "0";
        bool veikimas = true;

        while (veikimas)
        {
            zingsniai++;
            if (_kbhit()) // Privestinis sustabdymas (32 yra ASCII kodas 'Space' klaviðui)
            {
                int isjungimas = _getch();
                if (isjungimas == 32) 
                {
                    cout << "Programa priverstinai sustabdyta. ";
                    break;
                }
            }
            bool KeitimasSekmingas = false;

            for (int g = 0; g < a.size(); g++) 
            {
                if (a[g].esamaB == busena && a[g].esamasS == juosta[galvutespozicija])
                {
                    busena = a[g].naujaB;
                    juosta[galvutespozicija] = a[g].naujasS;
                    if (a[g].kryptis == 'R')
                        galvutespozicija++;
                    else if (a[g].kryptis == 'L')
                        galvutespozicija--;
                    KeitimasSekmingas = true;
                    break;
                }
            }

            cout << juosta << " Dabartine busena: " << busena << " esamas simbolis: " << juosta[galvutespozicija] << endl;
            for (int j = 0; j < galvutespozicija; j++) cout << ' ';
            cout << '^' << endl;
            cout << endl;

            if (!KeitimasSekmingas) {
                if (VisosB.find(busena) == VisosB.end()) {
                    cout << "Pasiekta galine busena: " << busena << endl;
                    break;
                }
                else {
                    cout << "Nera taisykles busenai: " << busena << " bei simboliui: " << juosta[galvutespozicija] << endl;
                    break;
                }
            }
            this_thread::sleep_for(chrono::milliseconds(1));
        }
    }
    else if (LinijuKiekis == 2) {
        string juosta, juosta2;
        int galvutespozicija, galvutespozicija2;
        fd >> juosta >> juosta2 >> galvutespozicija >> galvutespozicija2;
        galvutespozicija--;
        galvutespozicija2--;

        vector<Masina2> b;
        Masina2 temp;
        while (fd >> temp.esamaB >> temp.esamasS1 >> temp.esamasS2 >> temp.naujasS1 >> temp.naujasS2 >> temp.kryptis1 >> temp.kryptis2 >> temp.naujaB) {
            b.push_back(temp);
        }

        fd.close();

        string busena = "0";
        bool veikimas = true;

        while (veikimas) {
            zingsniai++;
            if (_kbhit()) // Privestinis sustabdymas (32 yra ASCII kodas 'Space' klaviðui)
            {
                int isjungimas = _getch();
                if (isjungimas == 32)
                {
                    cout << "Programa priverstinai sustabdyta. ";
                    break;
                }
            }
            bool KeitimasSekmingas = false;

            for (int g = 0; g < b.size(); g++) {
                if (b[g].esamaB == busena && b[g].esamasS1 == juosta[galvutespozicija] && b[g].esamasS2 == juosta2[galvutespozicija2]) {
                    juosta[galvutespozicija] = b[g].naujasS1;
                    juosta2[galvutespozicija2] = b[g].naujasS2;
                    busena = b[g].naujaB;
                    if (b[g].kryptis1 == 'R')
                        galvutespozicija++;
                    else if (b[g].kryptis1 == 'L')
                        galvutespozicija--;
                    if (b[g].kryptis2 == 'R')
                        galvutespozicija2++;
                    else if (b[g].kryptis2 == 'L')
                        galvutespozicija2--;
                    KeitimasSekmingas = true;
                    break;
                }
            }

            cout << juosta << " Dabartine busena: " << busena << " esamas simbolis: " << juosta[galvutespozicija] << endl;
            for (int j = 0; j < galvutespozicija; j++) cout << ' ';
            cout << '^' << endl;
            cout << juosta2 << " Dabartine busena: " << busena << " esamas simbolis: " << juosta2[galvutespozicija2] << endl;
            for (int j = 0; j < galvutespozicija2; j++) cout << ' ';
            cout << '^' << endl;
            cout << endl;

            if (!KeitimasSekmingas) {
                cout << "Nera taisykles busenai: " << busena << " bei simboliui: " << juosta[galvutespozicija] << endl;
                cout << "Nera taisykles busenai: " << busena << " bei simboliui: " << juosta2[galvutespozicija2] << endl;
                break;
            }
            this_thread::sleep_for(chrono::milliseconds(1));
        }
    }
    cout << "Programa baigta. Is viso atlikta zingsniu: " << zingsniai << endl;
    return 0;
}
