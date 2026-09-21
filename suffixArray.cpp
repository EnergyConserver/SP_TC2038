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
    void ordenarCounting(vector<int>& sa, vector<int>& rango, int k);
public:
    SuffixArray(string);
    void construir();
    void construirLCP();
    int obtenerLCP(int, int);
    pair<int, int> palindromoMasLargo();
    pair<int, int> subcadenaComunMasLarga(string);
};

SuffixArray::SuffixArray(string texto) {
    this->texto = texto;
    construir();
    construirLCP();
}

//Construye el Suffix Array usando Prefix Doubling.
void SuffixArray::construir() {
    int n = texto.size();
    if (n == 0) return;
    
    sa.resize(n);
    vector<int> rango(n);
    vector<int> nuevoRango(n);

    //Cada posición comienza con el rango de su carácter.
    for (int i = 0; i < n; i++) {
        sa[i] = i;
        rango[i] = texto[i];
    }
    rangos.push_back(rango);

    //Duplica la longitud de comparación en cada iteración.
    for (int k = 1; k < n; k *= 2) {
        //Ordena los sufijos según sus dos bloques de rango.
        ordenarCounting(sa, rango, k);

        nuevoRango[sa[0]] = 0;
        
        //Asigna nuevos rangos según el orden obtenido.
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
        //Termina cuando todos los sufijos tienen un rango diferente.
        if (rango[sa[n-1]] == n-1) break;
    }
}

void SuffixArray::ordenarCounting(
    vector<int>& sa,
    vector<int>& rango,
    int k
) {
    int n = sa.size();

    vector<int> temporal(n);
    vector<int> contador(n + 1, 0);

    // Primera pasada: segundo rango
    for (int i = 0; i < n; i++) {
        int posicion = sa[i] + k;
        int segundo;

        if (posicion < n) segundo = rango[posicion] + 1;
        else segundo = 0;
        contador[segundo]++;
    }

    // Convierte las cantidades en posiciones acumuladas
    for (int i = 1; i <= n; i++) contador[i] += contador[i - 1];

    // Ordenamiento estable por segundo rango
    for (int i = n - 1; i >= 0; i--) {
        int posicion = sa[i] + k;
        int segundo;

        if (posicion < n) segundo = rango[posicion] + 1;
        else segundo = 0;

        temporal[--contador[segundo]] = sa[i];
    }

    // Reinicia los contadores
    fill(contador.begin(), contador.end(), 0);

    // Segunda pasada: primer rango
    for (int i = 0; i < n; i++) {
        int primero = rango[temporal[i]] + 1;
        contador[primero]++;
    }

    for (int i = 1; i <= n; i++) contador[i] += contador[i - 1];

    // Ordenamiento estable por primer rango
    for (int i = n - 1; i >= 0; i--) {
        int posicion = temporal[i];
        int primero = rango[posicion] + 1;

        sa[--contador[primero]] = posicion;
    }
}

//Construye el arreglo LCP usando el algoritmo de Kasai.
void SuffixArray::construirLCP() {
    int n = texto.size();

    lcp.resize(n);
    posicionSA.resize(n);

    //Guarda la posición de cada sufijo dentro del Suffix Array.
    for (int i = 0; i < n; i++) {
        posicionSA[sa[i]] = i;
    }

    int k = 0;

    for (int i = 0; i < n; i++) {
        int pos = posicionSA[i];

        if (pos == 0) continue;

        //Compara el sufijo con el anterior en el Suffix Array.
        int j = sa[pos - 1];
        while (i + k < n && j + k < n && texto[i + k] == texto[j + k]) {
            k++;
        }

        lcp[pos] = k;
        if (k > 0) k--;
    }
}

//Compara un patrón con un sufijo desde una posición determinada.
vector<int> construirLPS(string patron) {
    int m = patron.size();
    vector<int> lps(m, 0);

    int longitud = 0;
    int i = 1;

    while (i < m) {
        if (patron[i] == patron[longitud]) {
            longitud++;
            lps[i] = longitud;
            i++;
        } else {
            if (longitud != 0) longitud = lps[longitud - 1];
            else {
                lps[i] = 0;
                i++;
            }
        }
    }
    return lps;
}

int buscarKMP(string texto, string patron) {
    vector<int> lps = construirLPS(patron);

    int i = 0;
    int j = 0;
    while (i < texto.size()) {
        if (texto[i] == patron[j]) {
            i++;
            j++;
            if (j == patron.size()) return i - j;
        } else {
            if (j != 0) j = lps[j - 1];
            else i++;
        }
    }
    return -1;
}

//Calcula el LCP entre dos sufijos usando los rangos construidos.s
int SuffixArray::obtenerLCP(int a, int b) {
    if (a == b)
        return texto.size() - a;


    int n = texto.size();
    int resultado = 0;

    //Avanza por los niveles más grandes posibles.
    for (int nivel = rangos.size() - 1; nivel >= 0; nivel--) {
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

//Encuentra el palíndromo más largo del texto.
pair<int, int> SuffixArray::palindromoMasLargo() {
    int n = texto.size();
    string reverso = texto;
    reverse(reverso.begin(), reverso.end());
    
    //Combina el texto con su reverso para comparar ambas direcciones.
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

//Encuentra la subcadena común más larga entre dos textos.
pair<int, int> SuffixArray::subcadenaComunMasLarga(string otroTexto) {
    int n1 = texto.size();
    string combinado = texto + "#" + otroTexto;

    //Une ambos textos para construir un solo Suffix Array.
    SuffixArray suffix(combinado);

    int mejorLongitud = 0;
    int mejorInicio = 0;

    //Busca sufijos consecutivos que pertenezcan a textos diferentes.
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

            //Evita que el separador '#' sea parte de la subcadena.
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

                //En empate, conserva la aparición más a la izquierda.
                if (mejorInicio == -1 || inicio < mejorInicio) mejorInicio = inicio;
            }
        }
    }

    if (mejorLongitud == 0) return {-1, -1};
    int mejorFin = mejorInicio + mejorLongitud - 1;
    return {mejorInicio, mejorFin};
}