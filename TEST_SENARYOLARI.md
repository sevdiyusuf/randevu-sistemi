# Test Senaryoları ve Kullanım Kılavuzu

## Test Senaryoları

### Senaryo 1: Temel Hasta ve Doktor Ekleme

**Adımlar:**
1. Programı başlat
2. Menüden "1" seç (Hasta Ekle)
3. ID: 10, Ad: "Test Hasta", Yaş: 30 gir
4. Menüden "2" seç (Doktor Ekle)
5. ID: 20, Ad: "Dr. Test", Bölüm: "Test Bölümü" gir

**Beklenen Sonuç:**
- Hasta ve doktor başarıyla eklendi mesajı görünmeli
- Bilgiler doğru şekilde gösterilmeli

---

### Senaryo 2: Randevu Oluşturma (Başarılı)

**Önkoşul:** Senaryo 1 tamamlanmış olmalı

**Adımlar:**
1. Menüden "3" seç (Randevu Oluştur)
2. Randevu ID: 100
3. Hasta ID: 10 (önceki senaryodan)
4. Doktor ID: 20 (önceki senaryodan)
5. Başlangıç: 10:00
6. Bitiş: 10:30

**Beklenen Sonuç:**
- Randevu başarıyla oluşturuldu mesajı
- Randevu bilgileri gösterilmeli

---

### Senaryo 3: Çakışan Randevu (Bekleme Listesine Ekleme)

**Önkoşul:** Senaryo 2 tamamlanmış olmalı

**Adımlar:**
1. Menüden "3" seç (Randevu Oluştur)
2. Randevu ID: 101
3. Hasta ID: 10
4. Doktor ID: 20 (aynı doktor)
5. Başlangıç: 10:15 (önceki randevu ile çakışıyor)
6. Bitiş: 10:45

**Beklenen Sonuç:**
- "Uyarı: Bu zaman dilimi için doktorun başka bir randevusu var" mesajı
- "Randevu bekleme listesine eklendi" mesajı
- Randevu bekleme listesine eklenmeli

---

### Senaryo 4: Randevu İptal Etme ve Geri Alma

**Önkoşul:** Senaryo 2 tamamlanmış olmalı

**Adımlar:**
1. Menüden "4" seç (Randevu İptal Et)
2. Randevu ID: 100 gir
3. Menüden "5" seç (İptali Geri Al)

**Beklenen Sonuç:**
- İptal başarı mesajı
- Geri alma başarı mesajı
- Randevu tekrar sistemde olmalı

---

### Senaryo 5: En Erken Randevuyu Gösterme

**Önkoşul:** En az 2 randevu oluşturulmuş olmalı

**Adımlar:**
1. Birkaç farklı saatte randevu oluştur
2. Menüden "7" seç (En Erken Randevuyu Göster)

**Beklenen Sonuç:**
- En erken başlangıç saatine sahip randevu gösterilmeli
- Min-heap doğru çalışmalı

---

### Senaryo 6: Randevuları Sıralı Listeleme

**Önkoşul:** En az 3 randevu oluşturulmuş olmalı

**Adımlar:**
1. Farklı saatlerde randevular oluştur
2. Menüden "8" seç (Tüm Randevuları Listele)

**Beklenen Sonuç:**
- Randevular başlangıç saatine göre sıralı gösterilmeli
- MergeSort algoritması doğru çalışmalı

---

### Senaryo 7: Bekleme Listesini Görüntüleme

**Önkoşul:** Senaryo 3 tamamlanmış olmalı (çakışan randevu)

**Adımlar:**
1. Menüden "6" seç (Bekleme Listesini Göster)

**Beklenen Sonuç:**
- Bekleme listesindeki randevu sayısı gösterilmeli
- Queue doğru çalışmalı

---

### Senaryo 8: Veri Kalıcılığı (CSV)

**Adımlar:**
1. Hasta, doktor ve randevu ekle
2. Programdan çık (0 seç)
3. Programı tekrar başlat

**Beklenen Sonuç:**
- Önceki veriler CSV'den yüklenmeli
- Veriler kaybolmamalı

---

## Edge Case Testleri

### Test 1: Geçersiz ID Girdisi
- Negatif ID gir
- Çok büyük ID gir
- Mevcut olmayan hasta/doktor ID'si gir

**Beklenen:** Hata mesajı gösterilmeli

### Test 2: Geçersiz Zaman Formatı
- "25:00" gibi geçersiz saat gir
- "09:60" gibi geçersiz dakika gir
- Yanlış format gir (örn: "9:30" yerine "09:30")

**Beklenen:** Hata mesajı ve doğru format açıklaması

### Test 3: Boş Liste İşlemleri
- Randevu yokken iptal etmeyi dene
- Stack boşken geri almayı dene
- Heap boşken en erken randevuyu görmeyi dene

**Beklenen:** Uygun mesaj gösterilmeli, crash olmamalı

### Test 4: Maksimum Kapasite
- Maksimum hasta/doktor/randevu sayısına ulaş
- Daha fazla eklemeyi dene

**Beklenen:** Maksimum kapasite uyarısı

### Test 5: Aynı ID ile Tekrar Ekleme
- Aynı ID'ye sahip hasta/doktor/randevu eklemeyi dene

**Beklenen:** ID zaten kullanılıyor hatası

---

## Performans Testleri

### Test 1: Büyük Veri Seti
- 100 hasta ekle
- 50 doktor ekle
- 500 randevu oluştur

**Beklenen:** Sistem yavaşlamadan çalışmalı

### Test 2: Çakışma Kontrolü Performansı
- Aynı doktor için 100 randevu oluştur
- Çakışma kontrolü hızlı çalışmalı

**Beklenen:** AVL ağacı O(log n) performans göstermeli

### Test 3: Sıralama Performansı
- 1000 randevu oluştur
- Sıralı listeleme yap

**Beklenen:** MergeSort O(n log n) performans göstermeli

---

## Entegrasyon Testleri

### Test 1: Tüm Veri Yapıları Birlikte
1. AVL ağacına randevu ekle
2. Heap'e ekle
3. Çakışma kontrolü yap
4. İptal et ve stack'e ekle
5. Geri al

**Beklenen:** Tüm veri yapıları tutarlı çalışmalı

### Test 2: CSV Yükleme ve Kaydetme
1. Veri ekle
2. Çık ve kaydet
3. Tekrar başlat ve yükle
4. Verilerin doğru yüklendiğini kontrol et

**Beklenen:** Veriler kaybolmadan yüklenmeli

---

## Hata Senaryoları

### Senaryo 1: Dosya Bulunamadı
- CSV dosyalarını sil veya taşı
- Programı başlat

**Beklenen:** Program crash etmemeli, boş veri ile başlamalı

### Senaryo 2: Geçersiz CSV Formatı
- CSV dosyasını boz
- Programı başlat

**Beklenen:** Geçersiz satırlar atlanmalı, program çalışmaya devam etmeli

### Senaryo 3: Bellek Hatası Simülasyonu
- Çok büyük veri seti ekle
- Malloc başarısız olabilir

**Beklenen:** Hata mesajı gösterilmeli, program crash etmemeli

---

## Test Checklist

- [ ] Hasta ekleme çalışıyor
- [ ] Doktor ekleme çalışıyor
- [ ] Randevu oluşturma çalışıyor
- [ ] Çakışma kontrolü çalışıyor
- [ ] Bekleme listesi çalışıyor
- [ ] Randevu iptal etme çalışıyor
- [ ] Geri alma çalışıyor
- [ ] En erken randevu gösterimi çalışıyor
- [ ] Sıralı listeleme çalışıyor
- [ ] CSV yükleme çalışıyor
- [ ] CSV kaydetme çalışıyor
- [ ] Hata durumları ele alınıyor
- [ ] Edge case'ler test edildi
- [ ] Performans kabul edilebilir

---

## Test Raporu Şablonu

```
Test Tarihi: [Tarih]
Test Edilen: [Fonksiyon/Modül]
Test Senaryosu: [Senaryo adı]
Sonuç: [Başarılı/Başarısız]
Notlar: [Varsa]
```

