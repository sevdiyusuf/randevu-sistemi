*** powershelde çalıştırmak için :.\run_gui.ps1

*** mingw de çalıştırmak için : ./randevu_sistemi.exe --gui

 *derlemek içinchmod +x build_gui.sh
./build_gui.sh

(2.derleme yolu : gcc -Wall -Wextra -std=c99 -g \
  main.c gui.c \
  patient.c doctor.c appointment.c \
  avl.c heap.c queue.c stack.c \
  file_io.c sort_search.c menu.c \
  -o randevu_sistemi.exe \
  `pkg-config --cflags --libs gtk+-3.0`)
  
mingw işlemleri için önce cd /c/ders/C/randevu