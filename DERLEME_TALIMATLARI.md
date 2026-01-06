# Derleme ve Çalıştırma Talimatları

## Gereksinimler

- **C Derleyici:** GCC (MinGW-w64 Windows için)
- **Make:** Make aracı (opsiyonel, elle derleme de yapılabilir)
- **İşletim Sistemi:** Windows, Linux veya macOS

## Windows (MinGW) ile Derleme

### Yöntem 1: Makefile Kullanarak

```bash
# Programı derle
make

# Programı çalıştır
make run-win

# Temizle
make clean
```

### Yöntem 2: Manuel Derleme

```bash
gcc -Wall -Wextra -std=c99 -g -o randevu_sistemi.exe \
    main.c \
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
```

### Programı Çalıştırma

```bash
randevu_sistemi.exe
```

## Linux/macOS ile Derleme

### Yöntem 1: Makefile Kullanarak

```bash
# Programı derle
make

# Programı çalıştır
make run

# Temizle
make clean
```

### Yöntem 2: Manuel Derleme

```bash
gcc -Wall -Wextra -std=c99 -g -o randevu_sistemi \
    main.c \
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
```

### Programı Çalıştırma

```bash
./randevu_sistemi
```

## Derleme Seçenekleri Açıklaması

- `-Wall`: Tüm uyarıları göster
- `-Wextra`: Ekstra uyarıları göster
- `-std=c99`: C99 standardını kullan
- `-g`: Hata ayıklama bilgisi ekle
- `-o randevu_sistemi`: Çıktı dosya adı

## Hata Ayıklama

Eğer derleme hatası alırsanız:

1. **Eksik başlık dosyaları:** Tüm `.h` dosyalarının mevcut olduğundan emin olun
2. **C99 desteği:** Derleyicinizin C99 standardını desteklediğinden emin olun
3. **Dosya yolları:** `data/` klasörünün mevcut olduğundan emin olun

## Test Etme

Programı çalıştırdıktan sonra:

1. Menüden seçenekleri test edin
2. Örnek veriler ekleyin
3. Randevu oluşturmayı deneyin
4. Çakışma kontrolünü test edin
5. İptal ve geri alma işlemlerini test edin

## Sorun Giderme

### "data klasörü bulunamadı" hatası
- `data/` klasörünün program ile aynı dizinde olduğundan emin olun
- CSV dosyalarının (`patients.csv`, `doctors.csv`, `appointments.csv`) mevcut olduğundan emin olun

### "undefined reference" hatası
- Tüm `.c` dosyalarının derleme komutuna eklendiğinden emin olun
- Dosya isimlerinin doğru yazıldığından emin olun

### Bellek hatası (segmentation fault)
- Programı debug modunda derleyin (`-g` flag'i ile)
- GDB veya benzeri bir debugger kullanın

