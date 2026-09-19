#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "suffixArray.cpp"
using namespace std;

int main() {

    ifstream archivo1("transmission1.txt");
    ifstream archivo2("transmission2.txt");
    ifstream codigo1("mcode1.txt");
    ifstream codigo2("mcode2.txt");
    ifstream codigo3("mcode3.txt");
    string t1, t2, m1, m2, m3, linea;

    while (getline(archivo1, linea)) {
        t1 += linea;
    }

    while (getline(archivo2, linea)) {
        t2 += linea;
    }

    while (getline(codigo1, linea)) {
        m1 += linea;
    }

    while (getline(codigo2, linea)) {
        m2 += linea;
    }

    while (getline(codigo3, linea)) {
        m3 += linea;
    }

    SuffixArray suffix1(t1);
    SuffixArray suffix2(t2);
    string codigos[] = {m1, m2, m3}; 

    for (int i = 0; i < 3; i++) {
        int posicion = suffix1.buscar(codigos[i]);

        if (posicion != -1) cout << "true " << posicion + 1 << endl;
        else cout << "false" << endl;
    }

    for (int i = 0; i < 3; i++) {
        int posicion = suffix2.buscar(codigos[i]);

        if (posicion != -1) cout << "true " << posicion + 1<< endl;
        else cout << "false" << endl;
    }

    pair<int, int> resultado1 = suffix1.palindromoMasLargo();
    cout << resultado1.first + 1 << " " << resultado1.second + 1 << endl;

    pair<int, int> resultado2 = suffix2.palindromoMasLargo();
    cout << resultado2.first + 1 << " " << resultado2.second + 1 << endl;

    pair<int, int> comun = suffix1.subcadenaComunMasLarga(t2);
    if (comun.first != -1) cout << comun.first + 1 << " " << comun.second + 1 << endl;
    else cout << "-1 -1" << endl;

    return 0;
}