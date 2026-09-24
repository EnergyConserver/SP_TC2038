#!/bin/env bash

RUTA=$(pwd)/vendor/MenuNcurses

g++ \
  main.cpp visualizer.cpp \
  $RUTA/MenuNcurses.cpp $RUTA/DetalleNcurses.cpp $RUTA/InputNcurses.cpp \
  -lncurses -o programa
