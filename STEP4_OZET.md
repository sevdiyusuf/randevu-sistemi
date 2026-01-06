# Adım 4: Sıralama ve Arama Algoritmaları - Özet

## ✅ Tamamlanan Uygulamalar

### Sıralama Algoritmaları

#### 1. MergeSort (Birleştirme Sıralaması)

**Uygulanan Fonksiyonlar:**
- ✅ `merge_sort_appointments()` - Randevuları start_time'a göre sıralar
- ✅ `merge_sort_patients()` - Hastaları ID'ye göre sıralar
- ✅ `merge_sort_doctors()` - Doktorları ID'ye göre sıralar
- ✅ `merge_appointments()` - Yardımcı birleştirme fonksiyonu
- ✅ `merge_patients()` - Yardımcı birleştirme fonksiyonu
- ✅ `merge_doctors()` - Yardımcı birleştirme fonksiyonu

**Algoritma Özellikleri:**
- **Zaman Karmaşıklığı:** O(n log n) - her durumda garantili
- **Uzay Karmaşıklığı:** O(n) - geçici dizi gerektirir
- **Kararlılık:** Evet (stabil sıralama - eşit elemanların sırası korunur)
- **Yaklaşım:** Böl ve fethet (divide and conquer)

**Algoritma Adımları:**
1. Diziyi iki eşit yarıya böl
2. Her yarıyı özyinelemeli olarak sırala
3. İki sıralı yarıyı birleştir (merge)

**Neden MergeSort?**
- Garantili O(n log n) performans
- Stabil sıralama (eşit elemanların sırası korunur)
- Öngörülebilir performans
- Büyük veri setleri için uygun

---

#### 2. HeapSort (Yığın Sıralaması)

**Uygulanan Fonksiyonlar:**
- ✅ `heap_sort_appointments()` - Randevuları start_time'a göre sıralar
- ✅ `heapify_appointments()` - Yardımcı heapify fonksiyonu

**Algoritma Özellikleri:**
- **Zaman Karmaşıklığı:** O(n log n) - her durumda
- **Uzay Karmaşıklığı:** O(1) - yerinde sıralama (ekstra bellek gerektirmez)
- **Kararlılık:** Hayır (kararsız sıralama)
- **Yaklaşım:** Heap veri yapısı kullanarak sıralama

**Algoritma Adımları:**
1. Diziyi max-heap'e dönüştür (build heap)
2. Heap'in kökünü (en büyük eleman) al ve dizinin sonuna taşı
3. Heap boyutunu azalt
4. Yeni kök için heapify yap (heap özelliğini koru)
5. Adım 2-4'ü heap boşalana kadar tekrarla

**Neden HeapSort?**
- Yerinde sıralama (O(1) ekstra bellek)
- Garantili O(n log n) performans
- MergeSort'tan farklı bir yaklaşım gösterir
- Büyük veri setleri için bellek açısından verimli

**MergeSort vs HeapSort:**
| Özellik | MergeSort | HeapSort |
|---------|-----------|----------|
| Zaman | O(n log n) | O(n log n) |
| Uzay | O(n) | O(1) |
| Kararlılık | Evet | Hayır |
| Hız | Genelde daha hızlı | Biraz daha yavaş |
| Kullanım | Genel amaçlı | Bellek kısıtlı durumlar |

---

### Arama Algoritmaları

#### Binary Search (İkili Arama)

**Uygulanan Fonksiyonlar:**
- ✅ `binary_search_appointment_by_id()` - Randevu ID'sine göre arama
- ✅ `binary_search_patient_by_id()` - Hasta ID'sine göre arama
- ✅ `binary_search_doctor_by_id()` - Doktor ID'sine göre arama

**Algoritma Özellikleri:**
- **Zaman Karmaşıklığı:** O(log n) - her adımda arama alanı yarıya iner
- **Uzay Karmaşıklığı:** O(1) - iteratif uygulama
- **Önkoşul:** Dizi sıralı olmalıdır!

**Algoritma Adımları:**
1. Dizinin ortasındaki elemanı kontrol et
2. Eğer aranan değer ortadaki değerden küçükse, sol yarıda ara
3. Eğer aranan değer ortadaki değerden büyükse, sağ yarıda ara
4. Bulunana kadar veya arama alanı tükenene kadar tekrarla

**Neden Binary Search?**
- Çok hızlı: O(log n) vs O(n) doğrusal arama
- Büyük veri setlerinde büyük performans farkı
- Basit ve anlaşılır algoritma
- Standart arama algoritması

**Örnek:**
- 1,000,000 elemanlı dizide:
  - Doğrusal arama: en fazla 1,000,000 karşılaştırma
  - Binary search: en fazla 20 karşılaştırma

---

## Zaman Karmaşıklığı Özeti

| Algoritma | En İyi | Ortalama | En Kötü | Uzay |
|-----------|--------|----------|---------|------|
| MergeSort | O(n log n) | O(n log n) | O(n log n) | O(n) |
| HeapSort | O(n log n) | O(n log n) | O(n log n) | O(1) |
| Binary Search | O(1) | O(log n) | O(log n) | O(1) |

---

## Algoritma Karşılaştırması

### MergeSort Avantajları:
- ✅ Stabil sıralama
- ✅ Öngörülebilir performans
- ✅ Büyük veri setleri için uygun

### MergeSort Dezavantajları:
- ❌ Ekstra bellek gerektirir (O(n))
- ❌ Küçük diziler için fazla overhead

### HeapSort Avantajları:
- ✅ Yerinde sıralama (ekstra bellek yok)
- ✅ Garantili O(n log n)
- ✅ Büyük veri setleri için bellek verimli

### HeapSort Dezavantajları:
- ❌ Kararsız sıralama
- ❌ MergeSort'tan genelde daha yavaş
- ❌ Cache performansı daha kötü

---

## Kod Kalitesi

- ✅ Tüm açıklamalar Türkçe
- ✅ Her fonksiyon için zaman karmaşıklığı belirtilmiş
- ✅ Algoritma adımları açıklanmış
- ✅ Bellek yönetimi (malloc/free)
- ✅ NULL pointer kontrolleri
- ✅ Hata durumları ele alınmış
- ✅ C99 standardına uygun
- ✅ Sadece stdlib.h kullanılmış

---

## Akademik Gereksinimler

✅ **Sıralama Algoritmaları:**
- MergeSort uygulandı ✓
- HeapSort uygulandı ✓
- Her ikisi de randevular için kullanılabilir ✓

✅ **Arama Algoritmaları:**
- Binary Search uygulandı ✓
- Randevu, hasta ve doktor için kullanılabilir ✓

✅ **Karmaşıklık Analizi:**
- Tüm fonksiyonlar için zaman karmaşıklığı belirtilmiş ✓
- Uzay karmaşıklığı açıklanmış ✓

---

## Kullanım Örnekleri

### MergeSort Kullanımı:
```c
// Randevuları start_time'a göre sırala
merge_sort_appointments(appointments, 0, appointment_count - 1);
```

### HeapSort Kullanımı:
```c
// Randevuları start_time'a göre sırala (yerinde)
heap_sort_appointments(appointments, appointment_count);
```

### Binary Search Kullanımı:
```c
// Önce sırala
merge_sort_patients(patients, 0, patient_count - 1);

// Sonra ara
int index = binary_search_patient_by_id(patients, patient_count, patient_id);
if (index != -1) {
    // Hasta bulundu
}
```

---

## Sonraki Adım

**Adım 4 Tamamlandı** ✅

**Adım 5'e Hazır:** Dosya G/Ç İşlemleri (CSV Persistence)
- CSV dosyalarından veri yükleme
- CSV dosyalarına veri kaydetme
- Boş dosya kontrolü

