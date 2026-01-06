# Makefile for Hospital Appointment Management System
# C99 standard, Windows/MinGW compatible

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
TARGET = randevu_sistemi
SRCDIR = .
OBJDIR = obj

# Source files
SOURCES = main.c \
          patient.c \
          doctor.c \
          appointment.c \
          avl.c \
          heap.c \
          queue.c \
          stack.c \
          file_io.c \
          sort_search.c \
          menu.c

# Object files
OBJECTS = $(SOURCES:%.c=$(OBJDIR)/%.o)

# Header files
HEADERS = patient.h \
          doctor.h \
          appointment.h \
          avl.h \
          heap.h \
          queue.h \
          stack.h \
          file_io.h \
          sort_search.h \
          menu.h

# Default target
all: $(TARGET)

# Create target executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)
	@echo "=========================================="
	@echo "  Derleme tamamlandı: $(TARGET)"
	@echo "=========================================="

# Create object directory
$(OBJDIR):
	@mkdir -p $(OBJDIR)

# Compile source files to object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS) | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	@echo "Temizleniyor..."
	@rm -rf $(OBJDIR)
	@rm -f $(TARGET)
	@rm -f $(TARGET).exe
	@echo "Temizleme tamamlandı."

# Run the program
run: $(TARGET)
	@echo "=========================================="
	@echo "  Program çalıştırılıyor..."
	@echo "=========================================="
	./$(TARGET)

# Windows specific run
run-win: $(TARGET)
	@echo "=========================================="
	@echo "  Program çalıştırılıyor..."
	@echo "=========================================="
	$(TARGET).exe

# Help target
help:
	@echo "Kullanılabilir hedefler:"
	@echo "  make          - Programı derle"
	@echo "  make clean    - Derleme dosyalarını temizle"
	@echo "  make run      - Programı çalıştır (Linux/Mac)"
	@echo "  make run-win  - Programı çalıştır (Windows)"
	@echo "  make help     - Bu yardım mesajını göster"

.PHONY: all clean run run-win help

