# Adım 5: Dosya G/Ç İşlemleri (CSV Persistence) - Özet

## ✅ Tamamlanan Uygulamalar

### CSV Dosya İşlemleri

#### 1. Hasta Dosya İşlemleri

**Fonksiyonlar:**
- ✅ `load_patients_from_csv()` - CSV'den hasta yükleme
- ✅ `save_patients_to_csv()` - CSV'ye hasta kaydetme

**CSV Formatı:**
```
id,name,age
1,Ahmet Yılmaz,35
2,Ayşe Demir,28
```

**Özellikler:**
- Header satırı otomatik algılanır ve atlanır
- Boş dosya güvenli şekilde işlenir (0 kayıt döner)
- Geçersiz satırlar atlanır
- Veri doğrulama yapılır (ID > 0, yaş 0-150 arası)

---

#### 2. Doktor Dosya İşlemleri

**Fonksiyonlar:**
- ✅ `load_doctors_from_csv()` - CSV'den doktor yükleme
- ✅ `save_doctors_to_csv()` - CSV'ye doktor kaydetme

**CSV Formatı:**
```
id,name,department
1,Dr. Zeynep Aydın,Kardiyoloji
2,Dr. Can Yıldız,Çocuk Hastalıkları
```

**Özellikler:**
- Header satırı otomatik algılanır
- Boş dosya güvenli şekilde işlenir
- Geçersiz satırlar atlanır
- Veri doğrulama yapılır

---

#### 3. Randevu Dosya İşlemleri

**Fonksiyonlar:**
- ✅ `load_appointments_from_csv()` - CSV'den randevu yükleme
- ✅ `save_appointments_to_csv()` - CSV'ye randevu kaydetme

**CSV Formatı:**
```
appointment_id,patient_id,doctor_id,start_time,end_time
1,1,1,09:00,09:30
2,2,2,10:00,10:30
```

**Özellikler:**
- Zaman formatı: HH:MM (örn: "09:30", "14:45")
- Header satırı otomatik algılanır
- Boş dosya güvenli şekilde işlenir
- Zaman aralığı doğrulaması (start_time < end_time)
- Geçerli zaman aralığı kontrolü (0-1439 dakika)

---

### Yardımcı Fonksiyonlar

#### 1. Zaman Dönüşüm Fonksiyonları

**`parse_time_string()`**
- "HH:MM" formatındaki string'i dakikaya çevirir
- Örnek: "09:30" → 570 dakika
- Hata durumunda -1 döner

**`format_time_string()`**
- Dakikayı "HH:MM" formatına çevirir
- Örnek: 570 dakika → "09:30"
- Buffer'a yazar

#### 2. CSV Parse Fonksiyonu

**`parse_csv_line()`** (internal)
- CSV satırını virgülle ayırır
- Tırnak işaretlerini destekler
- Boşlukları temizler
- Alanları diziye ayırır

---

## Güvenlik ve Hata Yönetimi

### Dosya İşlemleri
- ✅ Dosya açılamazsa güvenli şekilde 0 döner (crash yok)
- ✅ Boş dosya kontrolü yapılır
- ✅ NULL pointer kontrolleri yapılır
- ✅ Bellek tahsis hataları kontrol edilir

### Veri Doğrulama
- ✅ Hasta: ID > 0, yaş 0-150 arası
- ✅ Doktor: ID > 0
- ✅ Randevu: Tüm ID'ler > 0, zaman aralığı geçerli
- ✅ Geçersiz satırlar atlanır (hata vermez)

### Bellek Yönetimi
- ✅ malloc() ile dinamik bellek tahsisi
- ✅ NULL kontrolü yapılır
- ✅ Caller'ın bellek yönetimi sorumluluğu (free etmesi gerekir)

---

## CSV Format Detayları

### Header Satırı
- İlk satır otomatik algılanır
- "id", "ID", "appointment_id" içeriyorsa header olarak kabul edilir
- Header satırı atlanır

### Alan Ayırma
- Virgül (,) ile ayrılır
- Tırnak işareti desteklenir (virgül içeren alanlar için)
- Başındaki ve sonundaki boşluklar temizlenir

### Zaman Formatı
- **Giriş:** "HH:MM" (örn: "09:30", "14:45")
- **Çıkış:** "HH:MM" formatında
- **İç Depolama:** Dakika (0-1439)

---

## Kullanım Örnekleri

### Hasta Yükleme:
```c
Patient* patients[100];
int count = load_patients_from_csv("data/patients.csv", patients, 100);
printf("%d hasta yüklendi\n", count);
```

### Hasta Kaydetme:
```c
int saved = save_patients_to_csv("data/patients.csv", patients, count);
printf("%d hasta kaydedildi\n", saved);
```

### Randevu Yükleme:
```c
Appointment* appointments[1000];
int count = load_appointments_from_csv("data/appointments.csv", appointments, 1000);
printf("%d randevu yüklendi\n", count);
```

### Zaman Dönüşümü:
```c
int minutes = parse_time_string("09:30");  // 570 döner
char time_str[10];
format_time_string(570, time_str);  // "09:30" yazar
```

---

## Dosya Yapısı

### Oluşturulan Dosyalar:
```
data/
├── patients.csv      (Örnek hasta verileri)
├── doctors.csv       (Örnek doktor verileri)
└── appointments.csv  (Örnek randevu verileri)
```

### CSV Dosya Yolları:
- `data/patients.csv`
- `data/doctors.csv`
- `data/appointments.csv`

---

## Kod Kalitesi

- ✅ Tüm açıklamalar Türkçe
- ✅ Fonksiyon başına açıklama
- ✅ Hata durumları ele alınmış
- ✅ NULL pointer kontrolleri
- ✅ Bellek yönetimi
- ✅ Sadece stdio.h, stdlib.h, string.h kullanılmış
- ✅ C99 standardına uygun

---

## Özellikler

### Otomatik Header Algılama
- İlk satırı kontrol eder
- "id" veya "ID" içeriyorsa header olarak kabul eder
- Header satırını atlar

### Boş Dosya Güvenliği
- Dosya yoksa veya boşsa 0 döner (hata değil)
- Program crash etmez
- Güvenli başlangıç durumu

### Veri Doğrulama
- Her alan için geçerlilik kontrolü
- Geçersiz satırlar sessizce atlanır
- Program akışı bozulmaz

### Esneklik
- Header olan veya olmayan dosyalar desteklenir
- Farklı CSV formatlarına toleranslı
- Boşluk karakterleri temizlenir

---

## Akademik Gereksinimler

✅ **CSV Dosya İşlemleri:**
- Yükleme fonksiyonları ✓
- Kaydetme fonksiyonları ✓
- Boş dosya kontrolü ✓

✅ **Kütüphane Kısıtlamaları:**
- Sadece stdio.h fonksiyonları kullanıldı ✓
- Manuel CSV parsing ✓
- Hazır kütüphane kullanılmadı ✓

✅ **Güvenlik:**
- Hata durumları ele alındı ✓
- Bellek yönetimi doğru ✓
- NULL kontrolleri yapıldı ✓

---

## Sonraki Adım

**Adım 5 Tamamlandı** ✅

**Adım 6'ya Hazır:** CLI Menü Sistemi
- Metin tabanlı menü arayüzü
- Kullanıcı etkileşimi
- Tüm işlemlerin entegrasyonu

