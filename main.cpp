#include "suffixArray.cpp"
#include "visualizer.hpp"
#include <fstream>
#include <string>
using namespace std;

int main() {
  initscr();
  cbreak();
  noecho();

  if (has_colors()) {
    start_color();
    use_default_colors(); // Fondo transparente/por defecto

    init_pair(1, COLOR_RED, -1);    // Par 1: Rojo
    init_pair(2, COLOR_GREEN, -1);  // Par 2: Verde
    init_pair(3, COLOR_YELLOW, -1); // Par 3: Amarillo
    init_pair(4, COLOR_BLUE, -1);   // Par 4: Azul
    init_pair(5, COLOR_CYAN, -1);   // Par 5: Cian
  }

  keypad(stdscr,
         TRUE); // Permite capturar teclas especiales como las flechitas de
                // arriba/abajo, etc.
  curs_set(0);  // Oculta el cursor.

  // Verificación de tamaño de terminal
  // La librería MenuNcurses utiliza por defecto un ancho fijo de 130 columnas.
  if (COLS < 130 || LINES < 20) {
    clear();
    printw("==================================================================="
           "=\n");
    printw("  AVISO: Dimension de terminal reducida\n");
    printw("  Tamano detectado: %d columnas x %d filas.\n", COLS, LINES);
    printw("  MenuNcurses fue disenado con un ancho por defecto de 130 "
           "columnas.\n");
    printw("  Para una visualizacion correcta, se recomienda maximizar la "
           "ventana.\n");
    printw("==================================================================="
           "=\n\n");
    printw("Presiona cualquier tecla para continuar...");
    refresh();
    getch();
  }

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

  bool ejecutando = true;

  while (ejecutando) {
    string opcion = mostrarInicio();

    if (opcion == "1. Prefix Doubling") {
      visualizarPrefixDoubling(
          suffix1.obtenerEstadosPrefix(), t1, "transmission1.txt",
          suffix2.obtenerEstadosPrefix(), t2, "transmission2.txt");
    } else if (opcion == "2. Busqueda de mcode") {

      vector<string> opcionesPrincipales = {
          "1. mcode1",
          "2. mcode2",
          "3. mode3",
      };

      MenuNcurses menuSeleccionarMcode("Seleccione el mcode", opcionesPrincipales);
      int indice = menuSeleccionarMcode.show(); // Retorna 0, 1 o 2 al presionar ENTER
      string texto = menuSeleccionarMcode.getOption(indice);


      //char mcode;
      //cin >> mcode;
      //cin.ignore();

      //int indice = mcode - '1';

      if (indice >= 0 && indice < 3) {
        vector<EstadoKMP> estadosT1;
        vector<EstadoKMP> estadosT2;

        int posicion1 = buscarKMP(t1, codigos[indice], estadosT1);

        int posicion2 = buscarKMP(t2, codigos[indice], estadosT2);

        visualizarBusqueda(estadosT1, t1, "transmission1.txt", estadosT2, t2,
                           "transmission2.txt", codigos[indice]);
      }
    } else if (opcion == "3. Palindromo mas largo") {
      visualizarPalindromo(
          suffix1.obtenerEstadosPalindromo(), t1, "transmission1.txt",
          suffix2.obtenerEstadosPalindromo(), t2, "transmission2.txt");
    } else if (opcion == "4. Subcadena comun mas larga") {
      visualizarSubcadena(suffix1.obtenerEstadosSubcadena(), t1, t2,
                          "transmission1.txt", "transmission2.txt");
    } else if (opcion == "5. Salir" || opcion.empty()) {
      ejecutando = false;
    }
  }

  endwin();

  return 0;
}
