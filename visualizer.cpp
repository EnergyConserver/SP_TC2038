#include "visualizer.hpp"
#include "vendor/MenuNcurses/MenuNcurses.hpp"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

// const string ROJO = "\033[31m";
// const string RESET = "\033[0m";

using namespace std;

string esperarTecla() {

  vector<string> opcionesTecla = {"[A] Atras", "[D] Siguiente", "[Q] Menu"};

  // menu para esperar las teclas, este era heredado del visualizador original,
  // inicialmente quería quitarlo, pero ps ya estaba así q dije pq no jaja.
  MenuNcurses menuPrincipal("Selecciona una opción para continuar.",
                            opcionesTecla);
  string tecla = menuPrincipal.handle();

  return tecla;
}

string mostrarInicio() {
  // system("cls");

  vector<string> opcionesPrincipales = {
      "1. Prefix Doubling", "2. Busqueda de mcode", "3. Palindromo mas largo",
      "4. Subcadena comun mas larga", "5. Salir"};

  // Crear menu principal.
  MenuNcurses menuPrincipal("Visualizador del proyecto", opcionesPrincipales);
  string opcion = menuPrincipal.handle();

  return opcion;
}

void visualizarPrefixDoubling(const vector<EstadoPrefix> &estadosT1,
                              const string &textoT1, const string &nombreT1,
                              const vector<EstadoPrefix> &estadosT2,
                              const string &textoT2, const string &nombreT2) {
  int transmisionActual = 0;
  int estadoActual = 0;

  // while (true) {
  const vector<EstadoPrefix> *estados;
  const string *texto;
  const string *nombre;

  if (transmisionActual == 0) {
    estados = &estadosT1;
    texto = &textoT1;
    nombre = &nombreT1;
  } else {
    estados = &estadosT2;
    texto = &textoT2;
    nombre = &nombreT2;
  }

  // system("cls");
  //  aca w
  bool enSubmenu = true;
  while (enSubmenu) {
    vector<string> opcionesSub = {
        "[X] Paso Actual", "[A] Ver estados del programa.",
        "[B] <- Paso anterior.", "[C] Siguiente paso. ->",
        "[D] Salir al menu principal."};

    MenuNcurses submenu("Prefix Doubling", opcionesSub);
    string opSub = submenu.handle();

    if (opSub == "[A] Ver estados del programa.") {
      vector<string> info = {
          "Transmision: " + *nombre,
          "Estado: " + to_string(estadoActual + 1) + " de " +
              to_string(estados->size()),

          "k = " + to_string((*estados)[estadoActual].k),

      };
      DetalleNcurses menuPrincipal("Prefix Doubling", info);
      menuPrincipal.mostrar();

    } else if (opSub == "[X] Paso Actual") {
      const vector<int> &sa = (*estados)[estadoActual].sa;
      const vector<int> &rango = (*estados)[estadoActual].rango;

      vector<string> info;
      int limite = min(13, (int)sa.size());

      for (int i = 0; i < limite; i++) {
        int posicion = sa[i];
        int primerRango = rango[posicion];
        int segundaPosicion = posicion + (*estados)[estadoActual].k;
        int segundoRango = (segundaPosicion < (int)texto->size())
                               ? rango[segundaPosicion]
                               : -1;

        string sufijo = texto->substr(posicion, 80);
        if (posicion + 80 < (int)texto->size()) {
          sufijo += "...";
        }

        string rangosStr =
            "(" + to_string(primerRango) + ", " + to_string(segundoRango) + ")";

        // Construir la línea formateada mediante stringstream
        stringstream ss;
        ss << left << setw(6) << i << setw(12) << (posicion + 1) << setw(18)
           << rangosStr << sufijo;

        info.push_back(ss.str());
      }

      if ((int)sa.size() > limite) {
        info.push_back("...");
      }

      stringstream ssTitulo;
      ssTitulo << left << setw(5) << "SA" << setw(12) << "Posicion" << setw(18)
               << "Rangos"
               << "Sufijo";

      DetalleNcurses ventanaInfo(ssTitulo.str(), info);
      ventanaInfo.mostrar();
      ventanaInfo.mostrar();
    } else if (opSub == "[C] Siguiente paso. ->") {
      // do smth.

      if (estadoActual < estados->size() - 1) {
        estadoActual++;
      } else if (transmisionActual == 0) {
        transmisionActual = 1;
        estadoActual = 0;
      } else
        break;

    } else if (opSub == "[B] <- Paso anterior.") {

      // do smth else lol.

      if (estadoActual > 0) {
        estadoActual--;
      } else if (transmisionActual == 1) {
        transmisionActual = 0;
        estadoActual = estadosT1.size() - 1;
      } else
        break;
    }

    if (opSub == "[D] Salir al menu principal." ||
        opSub == "[D] Salir al menu principal." || opSub.empty()) {
      enSubmenu = false;
    }
  }
}

void visualizarBusqueda(const vector<EstadoKMP> &estadosT1,
                        const string &textoT1, const string &nombreT1,
                        const vector<EstadoKMP> &estadosT2,
                        const string &textoT2, const string &nombreT2,
                        const string &patron) {
  int transmisionActual = 0;
  int estadoActual = 0;
  int estadosMostrados = 0;

  while (true) {
    const vector<EstadoKMP> *estados;
    const string *texto;
    const string *nombre;

    if (transmisionActual == 0) {
      estados = &estadosT1;
      texto = &textoT1;
      nombre = &nombreT1;
    } else {
      estados = &estadosT2;
      texto = &textoT2;
      nombre = &nombreT2;
    }

    if (estados->empty()) {
      if (transmisionActual == 0) {
        transmisionActual = 1;
        estadoActual = 0;
        continue;
      } else {
        break;
      }
    }

    // Se reactiva la referencia al estado actual
    const EstadoKMP &estado = (*estados)[estadoActual];

    vector<string> info = {"Archivo: " + *nombre,
                           "Patron: " + patron,
                           "Paso: " + to_string(estadoActual + 1) + " de " +
                               to_string(estados->size()),
                           "",
                           "i: " + to_string(estado.i),
                           "j: " + to_string(estado.j),
                           "Caracteres que coinciden: " +
                               to_string(estado.caracteresCoincidentes),
                           "",
                           "Texto:"};

    int inicioCoincidencia = estado.i - estado.j;
    int inicio = max(0, min(estado.i - 30, inicioCoincidencia));
    int fin = min((int)texto->size(),
                  max(estado.i + 50, inicioCoincidencia + estado.j));

    // Construcción de la línea de contexto del texto
    stringstream ssTexto;
    if (inicio < inicioCoincidencia) {
      ssTexto << texto->substr(inicio, inicioCoincidencia - inicio);
    }

    if (estado.j > 0) {
      int inicioRojo = max(inicio, inicioCoincidencia);
      int finRojo = min(fin, estado.i);
      if (inicioRojo < finRojo) {
        ssTexto << C_ROJO << texto->substr(inicioRojo, finRojo - inicioRojo)
                << C_RESET;
      }
    }

    if (estado.i < fin) {
      ssTexto << texto->substr(max(inicio, estado.i),
                               fin - max(inicio, estado.i));
    }

    info.push_back(ssTexto.str());

    // Puntero de posición en el texto
    if (estado.i >= inicio && estado.i < fin) {
      info.push_back(string(estado.i - inicio, ' ') + "^");
    }

    info.push_back("");
    info.push_back("Patron:");

    // Construcción visual del patrón
    stringstream ssPatron;
    if (estado.j > 0) {
      ssPatron << C_ROJO << patron.substr(0, estado.j) << C_RESET;
    }
    ssPatron << patron.substr(estado.j);
    info.push_back(ssPatron.str());

    if (estado.j < (int)patron.size()) {
      info.push_back(string(estado.j, ' ') + "^");
    }

    info.push_back("");

    // Diagnóstico del paso
    if (estado.j == (int)patron.size()) {
      info.push_back("{VERDE}RESULTADO: ENCONTRADO{RESET}");
    } else if (estado.i >= (int)texto->size()) {
      info.push_back("{ROJO}RESULTADO: NO ENCONTRADO{RESET}");
    } else if ((*texto)[estado.i] == patron[estado.j]) {
      info.push_back("Caracteres iguales -> avanzar i y j");
    } else if (estado.j > 0) {
      info.push_back("Caracteres diferentes -> usar LPS");
    } else {
      info.push_back("Caracteres diferentes -> avanzar i");
    }

    // Mostrar estado actual en ventana ncurses
    DetalleNcurses ventanaKMP("Busqueda KMP.", info);
    ventanaKMP.mostrar();

    string opcion = esperarTecla();

    //"[A] Atras", "[D] Siguiente", "[Q] Menu"};
    if (opcion == "[D] Siguiente") {
      if (estadoActual < (int)estados->size() - 1) {
        if (estadosMostrados < 3) {
          estadoActual++;
          estadosMostrados++;
        } else {
          int siguiente = estadoActual + 1;
          while (siguiente < (int)estados->size()) {
            const EstadoKMP &estadoSiguiente = (*estados)[siguiente];
            bool importante =
                estadoSiguiente.i < (int)texto->size() &&
                estadoSiguiente.j > 0 &&
                (*texto)[estadoSiguiente.i] != patron[estadoSiguiente.j];
            if (importante)
              break;
            siguiente++;
          }

          if (siguiente < (int)estados->size()) {
            estadoActual = siguiente;
            estadosMostrados = 0;
          } else {
            estadoActual = (int)estados->size() - 1;
          }
        }
      } else if (transmisionActual == 0) {
        transmisionActual = 1;
        estadoActual = 0;
        estadosMostrados = 0;
      } else {
        break;
      }
    } else if (opcion == "[A] Atras") {
      if (estadoActual > 0) {
        if (estadosMostrados > 0) {
          estadoActual--;
          estadosMostrados--;
        } else {
          int anterior = estadoActual - 1;
          while (anterior > 0) {
            const EstadoKMP &estadoAnterior = (*estados)[anterior];
            bool importante =
                estadoAnterior.i < (int)texto->size() && estadoAnterior.j > 0 &&
                (*texto)[estadoAnterior.i] != patron[estadoAnterior.j];
            if (importante)
              break;
            anterior--;
          }

          if (anterior > 0) {
            estadoActual = anterior;
            estadosMostrados = 3;
          } else {
            estadoActual = 0;
            estadosMostrados = 0;
          }
        }
      } else if (transmisionActual == 1) {
        transmisionActual = 0;
        estadoActual = (int)estadosT1.size() - 1;
        estadosMostrados = 0;
      } else {
        break;
      }
    } else if (opcion == "[Q] Menu") {
      break;
    }
  }
}

void visualizarPalindromo(const vector<EstadoPalindromo> &estadosT1,
                          const string &textoT1, const string &nombreT1,
                          const vector<EstadoPalindromo> &estadosT2,
                          const string &textoT2, const string &nombreT2) {
  int transmisionActual = 0;
  int estadoActual = 0;
  int estadosMostrados = 0;

  bool el_submenu_esta_corriendose = true;
  while (el_submenu_esta_corriendose) {
    const vector<EstadoPalindromo> *estados;
    const string *texto;
    const string *nombre;

    if (transmisionActual == 0) {
      estados = &estadosT1;
      texto = &textoT1;
      nombre = &nombreT1;
    } else {
      estados = &estadosT2;
      texto = &textoT2;
      nombre = &nombreT2;
    }

    if (estados->empty()) {
      if (transmisionActual == 0) {
        transmisionActual = 1;
        estadoActual = 0;
        continue;
      } else {
        break;
      }
    }

    const EstadoPalindromo &estado = (*estados)[estadoActual];

    vector<string> infoPalindromo = {"Archivo: " + *nombre,
                                     "Paso: " + to_string(estadoActual + 1) +
                                         " de " + to_string(estados->size()),
                                     "",
                                     "Centro: " + to_string(estado.centro + 1),
                                     "Radio:  " + to_string(estado.radio),
                                     "Inicio: " + to_string(estado.inicio + 1),
                                     "Fin:    " + to_string(estado.fin + 1),
                                     "",
                                     "Palindromo actual:"};

    if (estado.radio == 0) {
      infoPalindromo.push_back(string(1, (*texto)[estado.centro]));
    } else {
      infoPalindromo.push_back(estado.palindromo);
    }

    infoPalindromo.push_back("Mejor hasta ahora:");
    infoPalindromo.push_back(estado.mejorPalindromo);
    infoPalindromo.push_back("");
    infoPalindromo.push_back("Texto:");

    // Fragmento visual del texto con el palíndromo en color
    stringstream ssTexto;
    if (estado.radio == 0) {
      ssTexto << texto->substr(0, estado.centro) << C_ROJO << "["
              << (*texto)[estado.centro] << "]" << C_RESET;
      if (estado.centro + 1 < texto->size()) {
        ssTexto << texto->substr(estado.centro + 1);
      }
    } else {
      ssTexto << texto->substr(0, estado.inicio) << C_ROJO << "["
              << estado.palindromo << "]" << C_RESET;
      if (estado.fin + 1 < texto->size()) {
        ssTexto << texto->substr(estado.fin + 1);
      }
    }

    infoPalindromo.push_back(ssTexto.str());

    // Mostrar el estado en la ventana ncurses
    DetalleNcurses menuPalindromo("Palindromo Mas Largo", infoPalindromo);
    menuPalindromo.mostrar();

    // Submenu de opiciones
    // vector<string> opcionesSub = {
    //    "[D] Siguiente",
    //    "[A] Atras",
    //    "[Q] Menu"
    //};

    string opcion = esperarTecla();
    // MenuNcurses subControl("Acciones", opcionesSub);
    // string opcion = subControl.handle();

    if (opcion == "[D] Siguiente") {
      if (estadoActual < (int)estados->size() - 1) {
        if ((*estados)[estadoActual].radio > 0) {
          estadoActual++;
          estadosMostrados = 1;
        } else if (estadosMostrados < 3) {
          estadoActual++;
          estadosMostrados++;
        } else {
          int siguiente = estadoActual + 1;
          while (siguiente < (int)estados->size() &&
                 (*estados)[siguiente].radio == 0) {
            siguiente++;
          }

          if (siguiente < (int)estados->size()) {
            estadoActual = siguiente;
            estadosMostrados = 0;
          } else {
            estadoActual = (int)estados->size() - 1;
          }
        }
      } else if (transmisionActual == 0) {
        transmisionActual = 1;
        estadoActual = 0;
        estadosMostrados = 0;
      } else {
        break;
      }
    } else if (opcion == "[A] Atras") {
      if (estadoActual > 0) {
        int anterior = estadoActual - 1;
        while (anterior > 0 && (*estados)[anterior].radio == 0) {
          anterior--;
        }

        int destino = min(anterior + 3, estadoActual - 1);
        estadoActual = destino;
        estadosMostrados = 3;
      } else if (transmisionActual == 1) {
        transmisionActual = 0;
        estadoActual = (int)estadosT1.size() - 1;
        estadosMostrados = 0;
      } else {
        break;
      }
    } else if (opcion == "[Q] Menu" || opcion.empty()) {
      el_submenu_esta_corriendose = false;
    }
  }
}

void visualizarSubcadena(const vector<EstadoSubcadena> &estados,
                         const string &textoT1, const string &textoT2,
                         const string &nombreT1, const string &nombreT2) {
  if (estados.empty())
    return;

  int estadoActual = 0;
  int estadosMostrados = 0;

  bool ejecutando = true;
  while (ejecutando) {
    const EstadoSubcadena &estado = estados[estadoActual];

    vector<string> info = {"Archivo 1: " + nombreT1,
                           "Archivo 2: " + nombreT2,
                           "Paso:      " + to_string(estadoActual + 1) +
                               " de " + to_string(estados.size()),
                           "",
                           "Posicion T1: " + to_string(estado.posicion1 + 1),
                           "Posicion T2: " + to_string(estado.posicion2 + 1),
                           "Longitud:    " + to_string(estado.longitud),
                           "",
                           "Texto 1:"};

    // Construcción del fragmento para Texto 1
    if (estado.longitud > 0 && estado.posicion1 >= 0 &&
        estado.posicion1 < (int)textoT1.size()) {
      int longitud = min(80, (int)textoT1.size() - estado.posicion1);
      string fragmento = textoT1.substr(estado.posicion1, longitud);
      int resaltado = min(estado.longitud, (int)fragmento.size());

      stringstream ssT1;
      ssT1 << C_ROJO << "[" << fragmento.substr(0, resaltado) << "]" << C_RESET
           << fragmento.substr(resaltado);

      if (estado.posicion1 + 80 < (int)textoT1.size()) {
        ssT1 << "...";
      }
      info.push_back(ssT1.str());
    } else {
      info.push_back("-");
    }

    info.push_back("");
    info.push_back("Texto 2:");

    // Construcción del fragmento para Texto 2
    if (estado.longitud > 0 && estado.posicion2 >= 0 &&
        estado.posicion2 < (int)textoT2.size()) {
      int longitud = min(80, (int)textoT2.size() - estado.posicion2);
      string fragmento = textoT2.substr(estado.posicion2, longitud);
      int resaltado = min(estado.longitud, (int)fragmento.size());

      stringstream ssT2;
      ssT2 << C_ROJO << "[" << fragmento.substr(0, resaltado) << "]" << C_RESET
           << fragmento.substr(resaltado);

      if (estado.posicion2 + 80 < (int)textoT2.size()) {
        ssT2 << "...";
      }
      info.push_back(ssT2.str());
    } else {
      info.push_back("-");
    }

    info.push_back("");
    info.push_back("Mejor hasta ahora:");
    info.push_back("Inicio T1: " + to_string(estado.mejorInicio + 1));
    info.push_back("Longitud:  " + to_string(estado.mejorLongitud));

    if (estado.mejorLongitud > 0 && estado.mejorInicio >= 0 &&
        estado.mejorInicio + estado.mejorLongitud <= (int)textoT1.size()) {
      info.push_back("Subcadena: " +
                     textoT1.substr(estado.mejorInicio, estado.mejorLongitud));
    }

    // Desplegar estado actual
    DetalleNcurses ventanaSubcadena("Subcadena Comun Mas Larga", info);
    ventanaSubcadena.mostrar();

    string opcion = esperarTecla();

    if (opcion == "[D] Siguiente") {
      if (estadoActual >= (int)estados.size() - 1) {
        ejecutando = false;
      } else if (estadosMostrados < 3) {
        estadoActual++;
        estadosMostrados++;
      } else {
        int siguiente = estadoActual + 1;
        while (siguiente < (int)estados.size()) {
          bool importante = estados[siguiente].mejorLongitud >
                            estados[siguiente - 1].mejorLongitud;
          if (importante)
            break;
          siguiente++;
        }

        if (siguiente < (int)estados.size()) {
          estadoActual = siguiente;
          estadosMostrados = 0;
        } else {
          estadoActual = (int)estados.size() - 1;
        }
      }
    } else if (opcion == "[A] Atras") {
      if (estadoActual > 0) {
        if (estadosMostrados > 0) {
          estadoActual--;
          estadosMostrados--;
        } else {
          int anterior = estadoActual - 1;
          while (anterior > 0) {
            bool importante = estados[anterior].mejorLongitud >
                              estados[anterior - 1].mejorLongitud;
            if (importante)
              break;
            anterior--;
          }

          if (anterior > 0) {
            estadoActual = anterior;
            estadosMostrados = 3;
          } else {
            estadoActual = 0;
            estadosMostrados = 0;
          }
        }
      } else {
        ejecutando = false;
      }
    } else if (opcion == "[Q] Menu" || opcion.empty()) {
      ejecutando = false;
    }
  }
}
