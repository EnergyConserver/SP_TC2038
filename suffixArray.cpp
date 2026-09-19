#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class SuffixArray {
private:
    string texto;
    vector<int> sa;
    vector<int> lcp;
    vector<int> posicionSA;
    vector<vector<int>> rangos;
public:
    SuffixArray(string);
    void construir();
    void construirLCP();
    int comparar(string, int);
    int buscar(string);
    int obtenerLCP(int, int);
    pair<int, int> palindromoMasLargo();
    pair<int, int> subcadenaComunMasLarga(string);
};

SuffixArray::SuffixArray(string texto) {
    this->texto = texto;
    construir();
    construirLCP();
}

void SuffixArray::construir() {
    int n = texto.size();
    if (n == 0) return;
    
    sa.resize(n);
    vector<int> rango(n);
    vector<int> nuevoRango(n);

    for (int i = 0; i < n; i++) {
        sa[i] = i;
        rango[i] = texto[i];
    }
    rangos.push_back(rango);

    for (int k = 1; k < n; k *= 2) {
        sort(sa.begin(), sa.end(), [this, &rango, k](int a, int b) {
            if (rango[a] != rango[b]) return rango[a] < rango[b];
            int A, B;

            if (a + k < texto.size()) A = rango[a + k];
            else A = -1;

            if (b + k < texto.size()) B = rango[b + k];
            else B = -1;

            return A < B;
        });

        nuevoRango[sa[0]] = 0;
        
        for (int i = 1; i < n; i++) {
            int anterior = sa[i-1];
            int actual = sa[i];

            int secAnterior, secActual;

            if (anterior + k < n) secAnterior = rango[anterior + k];
            else secAnterior = -1;

            if (actual + k < n) secActual = rango[actual + k];
            else secActual = -1;

            if (rango[anterior] != rango[actual] || secAnterior != secActual) {
                nuevoRango[actual] = nuevoRango[anterior] + 1;
            }
            else nuevoRango[actual] = nuevoRango[anterior];
        }

        rango = nuevoRango;
        rangos.push_back(rango);
        if (rango[sa[n-1]] == n-1) break;
    }
}

void SuffixArray::construirLCP() {
    int n = texto.size();

    lcp.resize(n);
    posicionSA.resize(n);

    for (int i = 0; i < n; i++) {
        posicionSA[sa[i]] = i;
    }

    int k = 0;

    for (int i = 0; i < n; i++) {
        int pos = posicionSA[i];

        if (pos == 0) continue;

        int j = sa[pos - 1];
        while (i + k < n && j + k < n && texto[i + k] == texto[j + k]) {
            k++;
        }

        lcp[pos] = k;
        if (k > 0) k--;
    }
}

int SuffixArray::comparar(string patron, int pos) {
    int i = 0;

    while (i < patron.size() && pos + i < texto.size()) {
        if (patron[i] < texto[pos + i]) return -1;
        if (patron[i] > texto[pos + i]) return 1;
        i++;
    }

    if (i == patron.size())
        return 0;
    return -1;
}

int SuffixArray::buscar(string patron) {
    int izquierda = 0;
    int derecha = sa.size() - 1;

    while (izquierda <= derecha) {

        int medio = (izquierda + derecha) / 2;
        int resultado = comparar(patron, sa[medio]);

        if (resultado == 0) return sa[medio];
        if (resultado < 0) derecha = medio - 1;
        else izquierda = medio + 1;
    }
    return -1;
}

int SuffixArray::obtenerLCP(int a, int b) {

    if (a == b)
        return texto.size() - a;


    int n = texto.size();
    int resultado = 0;


    for (int nivel = rangos.size() - 1;
         nivel >= 0;
         nivel--) {

        int longitud = 1 << nivel;


        if (a + resultado + longitud <= n &&
            b + resultado + longitud <= n &&
            rangos[nivel][a + resultado] ==
            rangos[nivel][b + resultado]) {

            resultado += longitud;
        }
    }


    return resultado;
}

pair<int, int> SuffixArray::palindromoMasLargo() {
    int n = texto.size();
    string reverso = texto;
    reverse(reverso.begin(), reverso.end());
    string combinado = texto + "#" + reverso;
    SuffixArray suffix(combinado);


    int mejorInicio = 0;
    int mejorFin = 0;


    // PALÍNDROMOS IMPARES
    for (int centro = 0; centro < n; centro++) {
        int posicionOriginal = centro + 1;
        int posicionReverso = n + 1 + (n - centro);

        // Comparamos ambas partes usando LCP.
        int radio = suffix.obtenerLCP(posicionOriginal, posicionReverso);

        radio = min(radio, centro);
        radio = min(radio, n - 1 - centro);

        int inicio = centro - radio;
        int fin = centro + radio;


        if (fin - inicio > mejorFin - mejorInicio) {
            mejorInicio = inicio;
            mejorFin = fin;
        }
    }


    // PALÍNDROMOS PARES
    for (int centro = 1; centro < n; centro++) {
        int posicionOriginal = centro;
        int posicionReverso = n + 1 + (n - centro);

        int radio = suffix.obtenerLCP(posicionOriginal, posicionReverso);

        radio = min(radio, centro);
        radio = min(radio, n - centro);

        int inicio = centro - radio;
        int fin = centro + radio - 1;

        if (fin - inicio > mejorFin - mejorInicio) {
            mejorInicio = inicio;
            mejorFin = fin;
        }
    }

    return {mejorInicio, mejorFin};
}

pair<int, int> SuffixArray::subcadenaComunMasLarga(string otroTexto) {
    int n1 = texto.size();
    int n2 = otroTexto.size();
    string combinado = texto + "#" + otroTexto;

    SuffixArray suffix(combinado);

    int mejorLongitud = 0;
    int mejorInicio = 0;

    for (int i = 1; i < suffix.sa.size(); i++) {

        int pos1 = suffix.sa[i - 1];
        int pos2 = suffix.sa[i];

        bool primeroEnTexto1 = pos1 < n1;
        bool segundoEnTexto1 = pos2 < n1;

        if (primeroEnTexto1 != segundoEnTexto1) {
            int longitud = suffix.lcp[i];
            int restante1, restante2;

            if (pos1 < n1) restante1 = n1 - pos1;
            else restante1 = combinado.size() - pos1 - 1;

            if (pos2 < n1) restante2 = n1 - pos2;
            else restante2 = combinado.size() - pos2 - 1;

            longitud = min(longitud, restante1);
            longitud = min(longitud, restante2);

            if (longitud > mejorLongitud) {
                mejorLongitud = longitud;

                if (primeroEnTexto1) mejorInicio = pos1;
                else mejorInicio = pos2;
            } else if (longitud == mejorLongitud && longitud > 0) {
                int inicio;

                if (primeroEnTexto1) inicio = pos1;
                else inicio = pos2;

                if (mejorInicio == -1 || inicio < mejorInicio) mejorInicio = inicio;
            }
        }
    }

    if (mejorLongitud == 0) return {-1, -1};
    int mejorFin = mejorInicio + mejorLongitud - 1;
    return {mejorInicio, mejorFin};
}