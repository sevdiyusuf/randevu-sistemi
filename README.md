# Hastane Randevu Yönetim Sistemi

C dilinde geliştirilmiş, veri yapıları ve algoritmalar kullanarak hastane randevularını yöneten kapsamlı bir sistem.

## 📋 Proje Hakkında

Bu proje, bir üniversite "Veri Yapıları" dersi için geliştirilmiş final projesidir. Sistem, hastane randevularını yönetmek için çeşitli veri yapıları ve algoritmalar kullanır.

## 🎯 Özellikler

- ✅ **AVL Ağacı:** Verimli randevu saklama ve çakışma tespiti
- ✅ **Kuyruk (Queue):** Dolu doktorlar için bekleme listesi
- ✅ **Yığın (Stack):** İptal edilen randevular için geri alma sistemi
- ✅ **Min-Heap:** En erken randevuları verimli bulma
- ✅ **Sıralama:** MergeSort ve HeapSort algoritmaları
- ✅ **Binary Search:** Hızlı hasta/doktor/randevu araması
- ✅ **CSV Persistence:** Verilerin dosyaya kaydedilmesi
- ✅ **CLI Menü:** Kullanıcı dostu komut satırı arayüzü

## 🏗️ Veri Yapıları

| Veri Yapısı | Kullanım Amacı | Karmaşıklık |
|-------------|----------------|-------------|
| AVL Tree | Randevu saklama ve çakışma kontrolü | O(log n) |
| Queue | Bekleme listesi | O(1) |
| Stack | Geri alma sistemi | O(1) |
| Min-Heap | En erken randevu bulma | O(log n) |
| MergeSort | Sıralı listeleme | O(n log n) |
| HeapSort | Alternatif sıralama | O(n log n) |
| Binary Search | Hızlı arama | O(log n) |

## 📁 Proje Yapısı

```
randevu/
├── main.c                 # Ana program
├── patient.c/h           # Hasta yönetimi
├── doctor.c/h            # Doktor yönetimi
├── appointment.c/h       # Randevu yönetimi
├── avl.c/h               # AVL ağacı
├── heap.c/h              # Min-heap
├── queue.c/h             # Kuyruk
├── stack.c/h             # Yığın
├── file_io.c/h           # CSV işlemleri
├── sort_search.c/h       # Sıralama ve arama
├── menu.c/h              # Menü sistemi
├── Makefile              # Derleme dosyası
├── data/                 # CSV dosyaları
│   ├── patients.csv
│   ├── doctors.csv
│   └── appointments.csv
└── Dokümantasyon/
    ├── README.md (bu dosya)
    ├── DESIGN.md
    ├── DERLEME_TALIMATLARI.md
    ├── TEST_SENARYOLARI.md
    └── STEP*.md dosyaları
```

## 🚀 Hızlı Başlangıç

### Gereksinimler

- **C Derleyici:** GCC (MinGW-w64 Windows için)
- **Make:** Opsiyonel (elle derleme de yapılabilir)

### Windows ile Derleme

```bash
# Makefile ile
make
make run-win

# Manuel derleme
gcc -Wall -Wextra -std=c99 -g -o randevu_sistemi.exe \
    main.c patient.c doctor.c appointment.c \
    avl.c heap.c queue.c stack.c \
    file_io.c sort_search.c menu.c
```

### Linux/macOS ile Derleme

```bash
# Makefile ile
make
make run

# Manuel derleme
gcc -Wall -Wextra -std=c99 -g -o randevu_sistemi \
    main.c patient.c doctor.c appointment.c \
    avl.c heap.c queue.c stack.c \
    file_io.c sort_search.c menu.c
```

### Programı Çalıştırma

```bash
# Windows
randevu_sistemi.exe

# Linux/Mac
./randevu_sistemi
```

## 📖 Kullanım

Program başlatıldığında şu menü görünecektir:

```
========================================
  HASTANE RANDEVU YÖNETİM SİSTEMİ
========================================
1. Hasta Ekle
2. Doktor Ekle
3. Randevu Oluştur
4. Randevu İptal Et
5. İptali Geri Al (Undo)
6. Bekleme Listesini Göster
7. En Erken Randevuyu Göster
8. Tüm Randevuları Listele (Sıralı)
0. Çıkış
========================================
```

### Örnek Kullanım Senaryosu

1. **Hasta Ekle:** Menüden "1" seç, ID, ad ve yaş gir
2. **Doktor Ekle:** Menüden "2" seç, ID, ad ve bölüm gir
3. **Randevu Oluştur:** Menüden "3" seç, randevu bilgilerini gir
4. **Çakışma Kontrolü:** Sistem otomatik olarak çakışma kontrolü yapar
5. **Randevu İptal:** Menüden "4" seç, randevu ID'si gir
6. **Geri Al:** Menüden "5" seç, son iptali geri al
7. **Çıkış:** Menüden "0" seç, veriler otomatik kaydedilir

## 🧪 Test

Detaylı test senaryoları için `TEST_SENARYOLARI.md` dosyasına bakın.

### Temel Test Senaryoları

1. ✅ Hasta ve doktor ekleme
2. ✅ Randevu oluşturma
3. ✅ Çakışma kontrolü
4. ✅ Randevu iptal etme
5. ✅ Geri alma (undo)
6. ✅ En erken randevu gösterme
7. ✅ Sıralı listeleme
8. ✅ CSV yükleme/kaydetme

## 📚 Dokümantasyon

- **DESIGN.md:** Sistem tasarımı ve mimari açıklamaları
- **DERLEME_TALIMATLARI.md:** Detaylı derleme talimatları
- **TEST_SENARYOLARI.md:** Test senaryoları ve kullanım kılavuzu
- **STEP*.md:** Her adımın detaylı özeti

## 🔧 Teknik Detaylar

### Dil ve Standart
- **Dil:** C
- **Standart:** C99
- **Kütüphaneler:** Sadece stdio.h, stdlib.h, string.h

### Veri Yapıları Karmaşıklığı

| İşlem | AVL Tree | Queue | Stack | Heap |
|-------|----------|-------|-------|------|
| Insert | O(log n) | O(1) | O(1) | O(log n) |
| Delete | O(log n) | O(1) | O(1) | O(log n) |
| Search | O(log n) | - | - | O(1) peek |

### Algoritma Karmaşıklığı

| Algoritma | Zaman | Uzay |
|-----------|-------|------|
| MergeSort | O(n log n) | O(n) |
| HeapSort | O(n log n) | O(1) |
| Binary Search | O(log n) | O(1) |

## 🎓 Akademik Gereksinimler

✅ **Zorunlu Veri Yapıları:**
- AVL Tree
- Queue
- Stack
- Min-Heap
- Binary Search
- MergeSort
- HeapSort

✅ **Zorunlu Algoritmalar:**
- AVL insert/search/delete
- Çakışma tespiti
- Queue enqueue/dequeue
- Stack push/pop
- Heap insert/extract-min
- Sıralama algoritmaları
- Binary search

✅ **Kod Standartları:**
- C99 standardı
- Sadece standart kütüphaneler
- Manuel uygulama
- Modüler tasarım
- Kapsamlı açıklamalar

## 🐛 Bilinen Sınırlamalar

1. Heap'ten ID ile direkt silme zor (basitleştirilmiş uygulama)
2. Queue görüntüleme için iterator yok
3. AVL delete ID ile O(n) (ağaç start_time'a göre sıralı)

## 📝 Lisans

Bu proje akademik amaçlı geliştirilmiştir.

## 👤 Yazar

Veri Yapıları Final Projesi
Ders: Veri Yapıları ve Algoritmalar

## 🙏 Teşekkürler

Bu proje, veri yapıları ve algoritmaların pratik uygulamasını göstermek için geliştirilmiştir.

---

**Not:** Detaylı bilgi için dokümantasyon dosyalarına bakın.

