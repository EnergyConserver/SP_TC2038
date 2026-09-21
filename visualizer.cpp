#include <iostream>
#include <iomanip>
#include "visualizer.hpp"

const string ROJO = "\033[31m";
const string RESET = "\033[0m";

using namespace std;

char esperarTecla() {
    char tecla;

    cout << "\n[A] Atras   [D] Siguiente   [Q] Menu\n";
    cout << "Opcion: ";

    cin >> tecla;
    cin.ignore();

    return tolower(tecla);
}

char mostrarInicio() {
    system("cls");

    cout << "========================================\n";
    cout << "       VISUALIZADOR DEL PROYECTO\n";
    cout << "========================================\n\n";

    cout << "1. Prefix Doubling\n";
    cout << "2. Busqueda de mcode\n";
    cout << "3. Palindromo mas largo\n";
    cout << "4. Subcadena comun mas larga\n";
    cout << "5. Salir\n";
    cout << "\nSeleccione una opcion: ";

    char opcion;
    cin >> opcion;
    cin.ignore();

    opcion = tolower(opcion);

    return opcion;
}

void visualizarPrefixDoubling(
    const vector<EstadoPrefix>& estadosT1,
    const string& textoT1,
    const string& nombreT1,
    const vector<EstadoPrefix>& estadosT2,
    const string& textoT2,
    const string& nombreT2
) {
    int transmisionActual = 0;
    int estadoActual = 0;

    while (true) {
        const vector<EstadoPrefix>* estados;
        const string* texto;
        const string* nombre;

        if (transmisionActual == 0) {
            estados = &estadosT1;
            texto = &textoT1;
            nombre = &nombreT1;
        }
        else {
            estados = &estadosT2;
            texto = &textoT2;
            nombre = &nombreT2;
        }

        system("cls");

        cout << "========================================\n";
        cout << "PREFIX DOUBLING\n";
        cout << "========================================\n";

        cout << "Transmision: " << *nombre << "\n";
        cout << "Estado: " << estadoActual + 1
             << " de " << estados->size() << "\n";
        cout << "k = " << (*estados)[estadoActual].k << "\n\n";

        const vector<int>& sa = (*estados)[estadoActual].sa;
        const vector<int>& rango = (*estados)[estadoActual].rango;

        cout << left
             << setw(6)  << "SA"
             << setw(12) << "Posicion"
             << setw(18) << "Rangos"
             << "Sufijo\n";

        cout << string(90, '-') << endl;

        int limite = min(25, (int)sa.size());

        for (int i = 0; i < limite; i++) {
            int posicion = sa[i];
            int primerRango = rango[posicion];

            int segundaPosicion = posicion + (*estados)[estadoActual].k;

            int segundoRango;

            if (segundaPosicion < texto->size()) segundoRango = rango[segundaPosicion];
            else segundoRango = -1;

            string sufijo = texto->substr(posicion, 80);

            cout << left
                 << setw(6) << i
                 << setw(12) << posicion + 1
                 << setw(18)
                 << "(" + to_string(primerRango) + ", "
                 + to_string(segundoRango) + ")"
                 << sufijo;

            if (posicion + 80 < texto->size()) cout << "...";
            cout << endl;
        }

        if (sa.size() > limite) cout << "...\n";

        char opcion = esperarTecla();

        if (opcion == 'd') {
            if (estadoActual < estados->size() - 1) {
                estadoActual++;
            } else if (transmisionActual == 0) {
                transmisionActual = 1;
                estadoActual = 0;
            } else break;
        }
        else if (opcion == 'a') {
            if (estadoActual > 0) {
                estadoActual--;
            } else if (transmisionActual == 1) {
                transmisionActual = 0;
                estadoActual = estadosT1.size() - 1;
            } else break;
        }
        else if (opcion == 'q') {
            break;
        }
    }
}

void visualizarBusqueda(
    const vector<EstadoKMP>& estadosT1,
    const string& textoT1,
    const string& nombreT1,
    const vector<EstadoKMP>& estadosT2,
    const string& textoT2,
    const string& nombreT2,
    const string& patron
) {
    int transmisionActual = 0;
    int estadoActual = 0;
    int estadosMostrados = 0;

    while (true) {

        const vector<EstadoKMP>* estados;
        const string* texto;
        const string* nombre;

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
            } else break;
        }

        system("cls");

        cout << "========================================\n";
        cout << "             BUSQUEDA KMP\n";
        cout << "========================================\n";

        cout << "Archivo: " << *nombre << "\n";
        cout << "Patron:  " << patron << "\n";
        cout << "Paso:    " << estadoActual + 1
             << " de " << estados->size() << "\n\n";

        const EstadoKMP& estado = (*estados)[estadoActual];

        cout << "i: " << estado.i << "\n";
        cout << "j: " << estado.j << "\n";
        cout << "Caracteres coincidentes: "
             << estado.caracteresCoincidentes << "\n\n";

        cout << "Texto:\n";

        int inicioCoincidencia = estado.i - estado.j;

        int inicio = max(0, min(estado.i - 30, inicioCoincidencia));

        int fin = min((int)texto->size(), max(estado.i + 50, inicioCoincidencia + estado.j));

        // Parte antes de la coincidencia
        if (inicio < inicioCoincidencia) {
            cout << texto->substr(
                inicio,
                inicioCoincidencia - inicio
            );
        }

        // Coincidencia en rojo
        if (estado.j > 0) {
            int inicioRojo = max(inicio, inicioCoincidencia);
            int finRojo = min(fin, estado.i);

            if (inicioRojo < finRojo) {
                cout << ROJO
                << texto->substr(
                    inicioRojo,
                    finRojo - inicioRojo
                )
                << RESET;
            }
        }

        // Parte después de la coincidencia
        if (estado.i < fin) {
            cout << texto->substr(
                max(inicio, estado.i),
                fin - max(inicio, estado.i)
            );
        }

        cout << "\n";

        if (estado.i >= inicio && estado.i < fin) {
            cout << string(estado.i - inicio, ' ')
                << "^\n";
        }

        cout << "\nPatron:\n";

        if (estado.j > 0) {
            cout << ROJO
                << patron.substr(0, estado.j)
                << RESET;
        }

        cout << patron.substr(estado.j);
        cout << "\n";

        if (estado.j < patron.size()) {
            cout << string(estado.j, ' ')
                << "^\n";
        }

        cout << "\n";

        if (estado.j == patron.size()) cout << "RESULTADO: ENCONTRADO\n";
        else if (estado.i >= texto->size()) cout << "RESULTADO: NO ENCONTRADO\n";
        else if ((*texto)[estado.i] == patron[estado.j]) cout << "Caracteres iguales -> avanzar i y j\n";
        else if (estado.j > 0) cout << "Caracteres diferentes -> usar LPS\n";
        else cout << "Caracteres diferentes -> avanzar i\n";
    
        char opcion = esperarTecla();

        if (opcion == 'd') {
            if (estadoActual < estados->size() - 1) {

                // Primero mostramos los primeros 3 estados.
                if (estadosMostrados < 3) {
                    estadoActual++;
                    estadosMostrados++;
                } else {

                    // Buscamos el siguiente estado importante.
                    int siguiente = estadoActual + 1;

                    while (siguiente < estados->size()) {
                        const EstadoKMP& estadoSiguiente = (*estados)[siguiente];

                        bool importante =
                            estadoSiguiente.i < texto->size() &&
                            estadoSiguiente.j > 0 &&
                            (*texto)[estadoSiguiente.i] !=
                            patron[estadoSiguiente.j];

                        if (importante) break;

                        siguiente++;
                    }

                    if (siguiente < estados->size()) {
                        estadoActual = siguiente;
                        estadosMostrados = 0;
                    } else estadoActual = estados->size() - 1;
                }
            } else if (transmisionActual == 0) {
                transmisionActual = 1;
                estadoActual = 0;
                estadosMostrados = 0;
            } else break;
        } else if (opcion == 'a') {
            if (estadoActual > 0) {

                // Si estamos dentro de los primeros 3 estados,
                // simplemente regresamos uno.
                if (estadosMostrados > 0) {
                    estadoActual--;
                    estadosMostrados--;
                } else {
                    int anterior = estadoActual - 1;

                    while (anterior > 0) {
                        const EstadoKMP& estadoAnterior = (*estados)[anterior];

                        bool importante =
                            estadoAnterior.i < texto->size() &&
                            estadoAnterior.j > 0 &&
                            (*texto)[estadoAnterior.i] !=
                            patron[estadoAnterior.j];

                        if (importante) break;
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
                estadoActual = estadosT1.size() - 1;
                estadosMostrados = 0;
            } else break;
        } else if (opcion == 'q') break;
    }
}

void visualizarPalindromo(
    const vector<EstadoPalindromo>& estadosT1,
    const string& textoT1,
    const string& nombreT1,
    const vector<EstadoPalindromo>& estadosT2,
    const string& textoT2,
    const string& nombreT2
) {
    int transmisionActual = 0;
    int estadoActual = 0;
    int estadosMostrados = 0;

    while (true) {
        const vector<EstadoPalindromo>* estados;
        const string* texto;
        const string* nombre;

        if (transmisionActual == 0) {
            estados = &estadosT1;
            texto = &textoT1;
            nombre = &nombreT1;
        }
        else {
            estados = &estadosT2;
            texto = &textoT2;
            nombre = &nombreT2;
        }

        if (estados->empty()) {
            if (transmisionActual == 0) {
                transmisionActual = 1;
                estadoActual = 0;
                continue;
            }
            else break;
        }

        system("cls");
        cout << "========================================\n";
        cout << "        PALINDROMO MAS LARGO\n";
        cout << "========================================\n";

        cout << "Archivo: " << *nombre << "\n";
        cout << "Paso:    " << estadoActual + 1 << " de " << estados->size() << "\n\n";

        const EstadoPalindromo& estado = (*estados)[estadoActual];

        cout << "Centro:  " << estado.centro + 1 << "\n";
        cout << "Radio:   " << estado.radio << "\n";
        cout << "Inicio:  " << estado.inicio + 1 << "\n";
        cout << "Fin:     " << estado.fin + 1 << "\n";

        cout << "\nPalindromo actual:\n";
        if (estado.radio == 0) cout << (*texto)[estado.centro] << "\n";
        else cout << estado.palindromo << "\n";
        
        cout << "\nMejor hasta ahora:\n";
        cout << estado.mejorPalindromo << "\n";

        cout << "\nTexto:\n";

        if (estado.radio == 0) {
            cout << texto->substr(0, estado.centro);
            cout << ROJO << "[" << (*texto)[estado.centro] << "]" << RESET;

            if (estado.centro + 1 < texto->size()) cout << texto->substr(estado.centro + 1);
        } else {
            cout << texto->substr(0, estado.inicio);
            cout << ROJO << "[" << estado.palindromo << "]" << RESET;

            if (estado.fin + 1 < texto->size()) cout << texto->substr(estado.fin + 1);
        }

        cout << "\n";
        char opcion = esperarTecla();

        if (opcion == 'd') {
            if (estadoActual < estados->size() - 1) {
    
                // Si estamos en un palíndromo,
                // comenzamos a mostrar los siguientes 3 estados.
                if ((*estados)[estadoActual].radio > 0) {
                    estadoActual++;
                    estadosMostrados = 1;
                }

                // Todavía estamos dentro de los 3 estados
                // que queremos mostrar después del palíndromo.
                else if (estadosMostrados < 3) {
                    estadoActual++;
                    estadosMostrados++;
                }

                // Ya mostramos los 3 estados.
                // Saltamos hasta el siguiente palíndromo.
                else {
                    int siguiente = estadoActual + 1;
                    while (siguiente < estados->size() &&
                    (*estados)[siguiente].radio == 0) {
                        siguiente++;
                    }

                    if (siguiente < estados->size()) {
                        estadoActual = siguiente;
                        estadosMostrados = 0;
                    }
                    else {
                        estadoActual = estados->size() - 1;
                    }
                }

            }
            else if (transmisionActual == 0) {
                transmisionActual = 1;
                estadoActual = 0;
                estadosMostrados = 0;
            }
            else {
                break;
            }
        } else if (opcion == 'a') {
            if (estadoActual > 0) {
                int anterior = estadoActual - 1;

                while (anterior > 0 &&
                (*estados)[anterior].radio == 0) {
                    anterior--;
                }

                int destino = min(
                    anterior + 3,
                    estadoActual - 1
                );

                estadoActual = destino;
                estadosMostrados = 3;
            } else if (transmisionActual == 1) {
                transmisionActual = 0;
                estadoActual = estadosT1.size() - 1;
                estadosMostrados = 0;
            } else break;
        } else if (opcion == 'q') break;
    }
}

void visualizarSubcadena(
    const vector<EstadoSubcadena>& estados,
    const string& textoT1,
    const string& textoT2,
    const string& nombreT1,
    const string& nombreT2
) {
    int estadoActual = 0;
    int estadosMostrados = 0;

    while (true) {
        system("cls");

        const EstadoSubcadena& estado = estados[estadoActual];

        cout << "========================================\n";
        cout << "       SUBCADENA COMUN MAS LARGA\n";
        cout << "========================================\n";

        cout << "Archivo 1: " << nombreT1 << "\n";
        cout << "Archivo 2: " << nombreT2 << "\n";

        cout << "Paso:      " << estadoActual + 1
             << " de " << estados.size() << "\n\n";

        cout << "Posicion T1: " << estado.posicion1 + 1 << "\n";
        cout << "Posicion T2: " << estado.posicion2 + 1 << "\n";
        cout << "Longitud:    " << estado.longitud << "\n";

        cout << "\nTexto 1:\n";

        if (estado.longitud > 0) {
            int longitud = min(
                80,
                (int)textoT1.size() - estado.posicion1
            );

            string fragmento =
                textoT1.substr(estado.posicion1, longitud);

            int resaltado = min(
                estado.longitud,
                (int)fragmento.size()
            );

            cout << ROJO << "["
                 << fragmento.substr(0, resaltado)
                 << "]" << RESET;

            cout << fragmento.substr(resaltado);

            if (estado.posicion1 + 80 < textoT1.size())
                cout << "...";
        }

        cout << "\n\nTexto 2:\n";

        if (estado.longitud > 0) {
            int longitud = min(80, (int)textoT1.size() - estado.mejorInicio);
            string fragmento = textoT1.substr(estado.mejorInicio, longitud);
            int resaltado = min(estado.mejorLongitud, (int)fragmento.size());

            cout << ROJO << "["
                 << fragmento.substr(0, resaltado)
                 << "]" << RESET;

            cout << fragmento.substr(resaltado);

            if (estado.posicion2 + 80 < textoT2.size()) cout << "...";
        }

        cout << "\n\n";

        cout << "Mejor hasta ahora:\n";

        cout << "Inicio T1: " << estado.mejorInicio + 1 << "\n";
        cout << "Longitud:  " << estado.mejorLongitud << "\n";

        if (estado.mejorLongitud > 0) {
            cout << "Subcadena: "
                 << textoT1.substr(
                        estado.mejorInicio,
                        estado.mejorLongitud
                    )
                 << "\n";
        }

        char opcion = esperarTecla();

        if (opcion == 'd') {

            if (estadoActual >= estados.size() - 1) break;

            // Todavía no hemos mostrado los 3 estados
            // posteriores al último salto.
            if (estadosMostrados < 3) {
                estadoActual++;
                estadosMostrados++;
            } else {
                // Ya mostramos +1, +2 y +3.
                // Ahora buscamos la siguiente mejora.
                int siguiente = estadoActual + 1;

                while (siguiente < estados.size()) {
                    const EstadoSubcadena& estadoSiguiente = estados[siguiente];

                    bool importante =
                        estadoSiguiente.mejorLongitud >
                        estados[siguiente-1].mejorLongitud;

                    if (importante) break;
                    siguiente++;
                }

                if (siguiente < estados.size()) {
                    // Saltamos directamente a la siguiente mejora.
                    estadoActual = siguiente;

                    // Empezamos a contar nuevamente +1, +2, +3.
                    estadosMostrados = 0;
                } else estadoActual = estados.size() - 1;
            }
        } else if (opcion == 'a') {
            if (estadoActual > 0) {
                // Si estamos dentro de los primeros 3 estados,
                // simplemente regresamos uno.
                if (estadosMostrados > 0) {
                    estadoActual--;
                    estadosMostrados--;
                } else {
                    // Buscamos la mejora anterior.
                    int anterior = estadoActual - 1;

                    while (anterior > 0) {
                        const EstadoSubcadena& estadoAnterior =
                            estados[anterior];

                        bool importante =
                            estadoAnterior.mejorLongitud >
                            estados[anterior - 1].mejorLongitud;

                        if (importante) break;
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
            } else break;
        } else if (opcion == 'q') break;
    }
}