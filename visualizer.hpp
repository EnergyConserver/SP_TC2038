#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <string>
#include <vector>

#include <ncurses.h>
#include <string>
#include <vector>


#include "vendor/MenuNcurses/DetalleNcurses.hpp"
#include "vendor/MenuNcurses/InputNcurses.hpp"
#include "vendor/MenuNcurses/MenuNcurses.hpp"

using namespace std;

struct EstadoPrefix {
    int k;
    vector<int> sa;
    vector<int> rango;
};

struct EstadoKMP {
    int i;
    int j;
    int caracteresCoincidentes;
};

struct EstadoPalindromo {
    int centro;
    int radio;
    int inicio;
    int fin;
    string palindromo;
    string mejorPalindromo;
};

struct EstadoSubcadena {
    int posicion1;
    int posicion2;
    int longitud;
    int mejorInicio;
    int mejorLongitud;
};

string mostrarInicio();
void visualizarPrefixDoubling(
    const vector<EstadoPrefix>& estadosT1,
    const string& textoT1,
    const string& nombreT1,
    const vector<EstadoPrefix>& estadosT2,
    const string& textoT2,
    const string& nombreT2
);
void visualizarBusqueda(
    const vector<EstadoKMP>& estadosT1,
    const string& textoT1,
    const string& nombreT1,
    const vector<EstadoKMP>& estadosT2,
    const string& textoT2,
    const string& nombreT2,
    const string& patron
);
void visualizarPalindromo(
    const vector<EstadoPalindromo>& estadosT1,
    const string& textoT1,
    const string& nombreT1,
    const vector<EstadoPalindromo>& estadosT2,
    const string& textoT2,
    const string& nombreT2
);
void visualizarSubcadena(
    const vector<EstadoSubcadena>& estados,
    const string& textoT1,
    const string& textoT2,
    const string& nombreT1,
    const string& nombreT2
);
char elegirMcode();
string esperarTecla();

#endif
