#!/bin/bash
# GTK3 ile GUI derleme scripti
# MSYS2 MinGW64 terminalinde çalıştırılmalı

echo "GTK3 ile derleme baslatiyor..."

gcc -Wall -Wextra -std=c99 -g \
  main.c gui.c \
  patient.c doctor.c appointment.c \
  avl.c heap.c queue.c stack.c \
  file_io.c sort_search.c menu.c \
  -o randevu_sistemi.exe \
  `pkg-config --cflags --libs gtk+-3.0`

if [ $? -eq 0 ]; then
    echo ""
    echo "Derleme basarili!"
    echo ""
    echo "GUI ile calistirmak icin:"
    echo "  ./randevu_sistemi.exe --gui"
    echo ""
    echo "CLI ile calistirmak icin:"
    echo "  ./randevu_sistemi.exe"
else
    echo ""
    echo "Derleme basarisiz! GTK3 kurulu mu kontrol edin:"
    echo "  pacman -S mingw-w64-x86_64-gtk3"
fi

