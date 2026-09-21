#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "suffixArray.cpp"
#include "visualizer.hpp"
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

    SuffixArray suffix1(t1, "transmission1.txt");
    SuffixArray suffix2(t2, "transmission2.txt");
    string codigos[] = {m1, m2, m3};

    pair<int, int> resultado1 = suffix1.palindromoMasLargo();
    pair<int, int> resultado2 = suffix2.palindromoMasLargo();
    pair<int, int> comun = suffix1.subcadenaComunMasLarga(t2);

    while (true) {
        char opcion = mostrarInicio();

        if (opcion == '1') {
            visualizarPrefixDoubling(
                suffix1.obtenerEstadosPrefix(),
                t1,
                "transmission1.txt",
                suffix2.obtenerEstadosPrefix(),
                t2,
                "transmission2.txt"
            );
        } else if (opcion == '2') {
            cout << "\nSeleccione el mcode:\n";
            cout << "1. mcode1\n";
            cout << "2. mcode2\n";
            cout << "3. mcode3\n";
            cout << "\nOpcion: ";

            char mcode;
            cin >> mcode;
            cin.ignore();

            int indice = mcode - '1';

            if (indice >= 0 && indice < 3) {
                vector<EstadoKMP> estadosT1;
                vector<EstadoKMP> estadosT2;

                int posicion1 = buscarKMP(
                    t1,
                    codigos[indice],
                    estadosT1
                );

                int posicion2 = buscarKMP(
                    t2,
                    codigos[indice],
                    estadosT2
                );

                visualizarBusqueda(
                    estadosT1,
                    t1,
                    "transmission1.txt",
                    estadosT2,
                    t2,
                    "transmission2.txt",
                    codigos[indice]
                );
            }
        } else if (opcion == '3') {
            visualizarPalindromo(
                suffix1.obtenerEstadosPalindromo(),
                t1,
                "transmission1.txt",
                suffix2.obtenerEstadosPalindromo(),
                t2,
                "transmission2.txt"
            );
        } else if (opcion == '4') {
            visualizarSubcadena(
                suffix1.obtenerEstadosSubcadena(),
                t1,
                t2,
                "transmission1.txt",
                "transmission2.txt"
            );
        } else if (opcion == '5') break;
    }
    return 0;
}