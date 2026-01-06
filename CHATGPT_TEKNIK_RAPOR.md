# Hastane Randevu Yönetim Sistemi - ChatGPT Teknik Rapor
## Proje Hakkında Her Şey

Bu rapor, ChatGPT'nin bu projeyi tamamen anlayabilmesi ve proje hakkında sorulara cevap verebilmesi, kod önerileri yapabilmesi ve hata ayıklayabilmesi için hazırlanmıştır.

---

## 1. PROJE GENEL BAKIŞ

### 1.1. Proje Tanımı
C99 standardında yazılmış, hastane randevu yönetim sistemi. Hem CLI hem de GTK3 tabanlı GUI desteği sunar. Tüm veri yapıları ve algoritmalar manuel olarak implement edilmiştir.

### 1.2. Temel Özellikler
- **Randevu Yönetimi:** Hasta-doktor randevu oluşturma, iptal etme, geri alma
- **Çakışma Kontrolü:** AVL Tree kullanarak aynı doktor için zaman çakışması tespiti
- **Bekleme Listesi:** Çakışan randevular için Queue (FIFO) yapısı
- **Undo Sistemi:** İptal edilen randevuları geri alma için Stack (LIFO) yapısı
- **En Erken Randevu:** Min-Heap ile O(1) zamanda en erken randevuyu bulma
- **Sıralama:** MergeSort ve HeapSort algoritmaları
- **Arama:** Binary Search algoritması
- **Veri Kalıcılığı:** CSV dosyaları ile veri saklama
- **Kullanıcı Arayüzü:** CLI menü sistemi ve GTK3 GUI

### 1.3. Teknoloji Stack
- **Dil:** C99
- **Kütüphaneler:** stdio.h, stdlib.h, string.h, gtk/gtk.h (GUI için)
- **Derleyici:** GCC (MSYS2 MinGW64)
- **GUI Framework:** GTK3
- **Veri Formatı:** CSV

---

## 2. MİMARİ VE TASARIM

### 2.1. Mimari Katmanlar

```
┌─────────────────────────────────────┐
│   Presentation Layer                │
│   - GUI (gui.c/h)                   │
│   - CLI Menu (menu.c/h)             │
└──────────────┬──────────────────────┘
               │
┌──────────────▼──────────────────────┐
│   Application Layer                 │
│   - Main (main.c)                   │
│   - Business Logic                  │
└──────────────┬──────────────────────┘
               │
┌──────────────▼──────────────────────┐
│   Data Structures Layer             │
│   - AVL Tree (avl.c/h)              │
│   - Min-Heap (heap.c/h)             │
│   - Queue (queue.c/h)               │
│   - Stack (stack.c/h)               │
└──────────────┬──────────────────────┘
               │
┌──────────────▼──────────────────────┐
│   Data Models Layer                 │
│   - Patient (patient.c/h)           │
│   - Doctor (doctor.c/h)             │
│   - Appointment (appointment.c/h)   │
└──────────────┬──────────────────────┘
               │
┌──────────────▼──────────────────────┐
│   Persistence Layer                 │
│   - File I/O (file_io.c/h)          │
│   - CSV Format                      │
└─────────────────────────────────────┘
```

### 2.2. Veri Yapıları İlişkisi

```
Appointment Array (Ana Veri Deposu)
    │
    ├──→ AVL Tree (avl_tree)
    │   └── Key: start_time
    │   └── Purpose: Zaman bazlı arama, çakışma kontrolü
    │   └── Complexity: O(log n) insert/search/delete
    │
    ├──→ Min-Heap (heap)
    │   └── Key: start_time (minimum)
    │   └── Purpose: En erken randevuyu bulma
    │   └── Complexity: O(1) peek, O(log n) insert/extract
    │
    ├──→ Queue (queue)
    │   └── Purpose: Çakışan randevular için bekleme listesi
    │   └── Order: FIFO (First In First Out)
    │   └── Complexity: O(1) enqueue/dequeue
    │
    └──→ Stack (stack)
        └── Purpose: İptal edilen randevuları geri alma
        └── Order: LIFO (Last In First Out)
        └── Complexity: O(1) push/pop
```

### 2.3. Veri Akışı

**Randevu Oluşturma:**
```
User Input → GUI/CLI → create_appointment()
    ↓
Validation (patient/doctor exists, time valid)
    ↓
avl_has_conflict() → Conflict?
    ├── YES → queue_enqueue() (Bekleme listesi)
    └── NO → avl_insert() + heap_insert() + array append
```

**Randevu İptal:**
```
User Input → avl_delete() → Remove from array
    ↓
stack_push() (Undo için sakla)
```

**Undo İşlemi:**
```
User Input → stack_pop() → Get cancelled appointment
    ↓
avl_has_conflict() → Conflict?
    ├── YES → stack_push() (Geri ekle, başarısız)
    └── NO → avl_insert() + heap_insert() + array append
```

---

## 3. DOSYA YAPISI VE MODÜLLER

### 3.1. Dosya Organizasyonu

```
randevu_sistemi/
├── main.c                    # Ana giriş noktası, veri yapıları başlatma
├── patient.c/h              # Hasta veri modeli ve yönetimi
├── doctor.c/h               # Doktor veri modeli ve yönetimi
├── appointment.c/h          # Randevu veri modeli ve yönetimi
├── avl.c/h                  # AVL Tree implementasyonu
├── heap.c/h                 # Min-Heap implementasyonu
├── queue.c/h                # Queue (FIFO) implementasyonu
├── stack.c/h                # Stack (LIFO) implementasyonu
├── sort_search.c/h          # MergeSort, HeapSort, Binary Search
├── file_io.c/h              # CSV dosya işlemleri
├── menu.c/h                 # CLI menü sistemi
├── gui.c/h                  # GTK3 GUI implementasyonu
└── data/                    # CSV veri dosyaları
    ├── patients.csv
    ├── doctors.csv
    └── appointments.csv
```

### 3.2. Modül Bağımlılıkları

```
main.c
├── patient.h
├── doctor.h
├── appointment.h
├── avl.h (depends on appointment.h)
├── heap.h (depends on appointment.h)
├── queue.h (depends on appointment.h)
├── stack.h (depends on appointment.h)
├── file_io.h (depends on patient.h, doctor.h, appointment.h)
├── sort_search.h (depends on appointment.h, patient.h, doctor.h)
├── menu.h (depends on all above)
└── gui.h (depends on all above + gtk/gtk.h)
```

---

## 4. VERİ MODELLERİ

### 4.1. Patient (Hasta)

**Yapı Tanımı:**
```c
typedef struct {
    int id;              // Unique patient identifier (1-999999)
    char name[100];      // Patient's full name
    int age;             // Patient's age (0-150)
} Patient;
```

**Fonksiyonlar:**
- `Patient* create_patient(int id, const char* name, int age)` - Yeni hasta oluşturur
- `void free_patient(Patient* patient)` - Belleği serbest bırakır
- `void display_patient(const Patient* patient)` - Hasta bilgilerini yazdırır
- `int validate_patient(const Patient* patient)` - Veri doğrulama

**Bellek Yönetimi:**
- `create_patient()` malloc ile bellek ayırır
- `free_patient()` free ile bellek serbest bırakır
- Caller sorumluluğu: Oluşturan free etmeli

### 4.2. Doctor (Doktor)

**Yapı Tanımı:**
```c
typedef struct {
    int id;              // Unique doctor identifier (1-999999)
    char name[100];      // Doctor's full name
    char department[50]; // Department name (e.g., "Kardiyoloji")
} Doctor;
```

**Fonksiyonlar:**
- `Doctor* create_doctor(int id, const char* name, const char* department)` - Yeni doktor oluşturur
- `void free_doctor(Doctor* doctor)` - Belleği serbest bırakır
- `void display_doctor(const Doctor* doctor)` - Doktor bilgilerini yazdırır
- `int validate_doctor(const Doctor* doctor)` - Veri doğrulama

### 4.3. Appointment (Randevu)

**Yapı Tanımı:**
```c
typedef struct {
    int appointment_id;  // Unique appointment identifier (1-999999)
    int patient_id;      // Reference to patient ID
    int doctor_id;       // Reference to doctor ID
    int start_time;      // Start time in minutes from midnight (0-1439)
    int end_time;        // End time in minutes from midnight (0-1439)
} Appointment;
```

**Zaman Gösterimi:**
- Zamanlar dakika cinsinden saklanır (gece yarısından itibaren)
- Örnek: 09:30 = 570 dakika, 14:45 = 885 dakika
- Geçerli aralık: 0-1439 (00:00 - 23:59)

**Fonksiyonlar:**
- `Appointment* create_appointment(int appointment_id, int patient_id, int doctor_id, int start_time, int end_time)` - Yeni randevu oluşturur
- `void free_appointment(Appointment* appointment)` - Belleği serbest bırakır
- `void display_appointment(const Appointment* appointment)` - Randevu bilgilerini yazdırır
- `int validate_appointment(const Appointment* appointment)` - Veri doğrulama
- `int appointments_overlap(const Appointment* a1, const Appointment* a2)` - Çakışma kontrolü
- `void minutes_to_time_string(int minutes, char* buffer)` - Dakikayı "HH:MM" formatına çevirir

**Çakışma Mantığı:**
İki randevu çakışır eğer:
```
start_time_1 < end_time_2 AND end_time_1 > start_time_2
```

---

## 5. VERİ YAPILARI DETAYLARI

### 5.1. AVL Tree

**Yapı Tanımı:**
```c
typedef struct AVLNode {
    Appointment* appointment;      // Randevu verisi
    struct AVLNode* left;          // Sol çocuk
    struct AVLNode* right;         // Sağ çocuk
    int height;                    // Düğüm yüksekliği
} AVLNode;

typedef struct {
    AVLNode* root;  // Kök düğüm
    int size;       // Ağaçtaki eleman sayısı
} AVLTree;
```

**Key (Anahtar):** `appointment->start_time` (zaman bazlı sıralama)

**Temel İşlemler:**
- `void avl_init(AVLTree* tree)` - O(1) - Boş ağaç başlatır
- `int avl_insert(AVLTree* tree, Appointment* appointment, int doctor_id)` - O(log n) - Randevu ekler, çakışma kontrolü yapar
- `Appointment* avl_delete(AVLTree* tree, int appointment_id)` - O(log n) - Randevu siler, dengeleme yapar
- `int avl_has_conflict(AVLTree* tree, int doctor_id, int start_time, int end_time)` - O(log n) - Çakışma kontrolü
- `Appointment* avl_search_by_id(AVLTree* tree, int appointment_id)` - O(log n) - ID ile arama
- `int avl_size(const AVLTree* tree)` - O(1) - Eleman sayısı
- `void avl_destroy(AVLTree* tree)` - O(n) - Tüm ağacı temizler

**Rotasyon İşlemleri:**
- `AVLNode* avl_rotate_right(AVLNode* y)` - O(1) - Sağa rotasyon (sol ağırlıklı için)
- `AVLNode* avl_rotate_left(AVLNode* x)` - O(1) - Sola rotasyon (sağ ağırlıklı için)

**Balance Factor:**
```
balance = height(left) - height(right)
Geçerli değerler: -1, 0, 1
Dengesizlik: |balance| > 1
```

**Rotasyon Durumları:**
1. **Left-Left:** balance > 1 ve sol çocuk sol ağırlıklı → Sağa rotasyon
2. **Right-Right:** balance < -1 ve sağ çocuk sağ ağırlıklı → Sola rotasyon
3. **Left-Right:** balance > 1 ama sol çocuk sağ ağırlıklı → Önce sola, sonra sağa rotasyon
4. **Right-Left:** balance < -1 ama sağ çocuk sol ağırlıklı → Önce sağa, sonra sola rotasyon

**Çakışma Kontrolü Algoritması:**
```c
int avl_has_conflict(AVLTree* tree, int doctor_id, int start_time, int end_time) {
    // Ağaçta aynı doktor için zaman aralığı kontrolü
    // Rekürsif olarak tüm düğümleri kontrol eder
    // İki zaman aralığı çakışır eğer:
    //   start_time < existing_end_time AND end_time > existing_start_time
}
```

**Önemli Notlar:**
- AVL Tree randevuları `start_time`'a göre saklar
- Çakışma kontrolü sadece aynı `doctor_id` için yapılır
- `avl_delete()` fonksiyonu `appointment_id` ile siler ama ağaç `start_time`'a göre sıralıdır
- Bu yüzden önce ID ile arama yapılır (O(n) worst case), sonra `start_time` ile silinir

### 5.2. Min-Heap

**Yapı Tanımı:**
```c
typedef struct {
    Appointment** appointments;  // Dizi (tam ikili ağaç gösterimi)
    int capacity;                // Maksimum kapasite
    int size;                    // Mevcut eleman sayısı
} MinHeap;
```

**Dizi Gösterimi:**
- Parent index: `(i-1)/2`
- Left child: `2*i + 1`
- Right child: `2*i + 2`
- Root: index 0

**Key (Anahtar):** `appointment->start_time` (minimum değer root'ta)

**Temel İşlemler:**
- `int heap_init(MinHeap* heap, int capacity)` - O(1) - Boş heap başlatır
- `int heap_insert(MinHeap* heap, Appointment* appointment)` - O(log n) - Randevu ekler, heapify_up yapar
- `Appointment* heap_extract_min(MinHeap* heap)` - O(log n) - En erken randevuyu çıkarır, heapify_down yapar
- `Appointment* heap_peek_min(const MinHeap* heap)` - O(1) - En erken randevuyu döndürür (silmeyen)
- `int heap_is_empty(const MinHeap* heap)` - O(1) - Boş mu kontrolü
- `int heap_size(const MinHeap* heap)` - O(1) - Eleman sayısı
- `void heap_destroy(MinHeap* heap)` - O(1) - Belleği serbest bırakır

**Heapify İşlemleri:**
- `void heapify_up(MinHeap* heap, int index)` - O(log n) - Insert sonrası yukarı doğru düzeltme
- `void heapify_down(MinHeap* heap, int index)` - O(log n) - Extract sonrası aşağı doğru düzeltme

**Min-Heap Özelliği:**
```
parent->start_time <= left_child->start_time
parent->start_time <= right_child->start_time
```

### 5.3. Queue (FIFO)

**Yapı Tanımı:**
```c
typedef struct QueueNode {
    Appointment* appointment;
    struct QueueNode* next;
} QueueNode;

typedef struct {
    QueueNode* front;  // İlk eleman (çıkış)
    QueueNode* rear;   // Son eleman (giriş)
    int size;
} Queue;
```

**Temel İşlemler:**
- `void queue_init(Queue* queue)` - O(1) - Boş kuyruk başlatır
- `int queue_enqueue(Queue* queue, Appointment* appointment)` - O(1) - Sona ekler
- `Appointment* queue_dequeue(Queue* queue)` - O(1) - Baştan çıkarır
- `int queue_is_empty(const Queue* queue)` - O(1) - Boş mu kontrolü
- `int queue_size(const Queue* queue)` - O(1) - Eleman sayısı
- `void queue_destroy(Queue* queue)` - O(n) - Tüm kuyruğu temizler

**Kullanım Senaryosu:**
Çakışan randevular bekleme listesine eklenir. İlk gelen randevu ilk işlenir (FIFO).

### 5.4. Stack (LIFO)

**Yapı Tanımı:**
```c
typedef struct StackNode {
    Appointment* appointment;
    struct StackNode* next;
} StackNode;

typedef struct {
    StackNode* top;  // En üst eleman
    int size;
} Stack;
```

**Temel İşlemler:**
- `void stack_init(Stack* stack)` - O(1) - Boş yığın başlatır
- `int stack_push(Stack* stack, Appointment* appointment)` - O(1) - Üste ekler
- `Appointment* stack_pop(Stack* stack)` - O(1) - Üstten çıkarır
- `int stack_is_empty(const Stack* stack)` - O(1) - Boş mu kontrolü
- `int stack_size(const Stack* stack)` - O(1) - Eleman sayısı
- `void stack_destroy(Stack* stack)` - O(n) - Tüm yığını temizler

**Kullanım Senaryosu:**
İptal edilen randevular stack'e eklenir. Son iptal edilen randevu ilk geri alınır (LIFO - undo).

---

## 6. ALGORİTMALAR DETAYLARI

### 6.1. MergeSort

**Fonksiyon:**
```c
void merge_sort_appointments(Appointment** appointments, int left, int right);
void merge_sort_patients(Patient** patients, int left, int right);
void merge_sort_doctors(Doctor** doctors, int left, int right);
```

**Algoritma:**
1. **Divide:** Diziyi ikiye böl
2. **Conquer:** Her yarıyı rekürsif olarak sırala
3. **Combine:** İki sıralı yarıyı birleştir (merge)

**Zaman Karmaşıklığı:**
- Best: O(n log n)
- Average: O(n log n)
- Worst: O(n log n)

**Uzay Karmaşıklığı:** O(n) - geçici dizi gerektirir

**Kararlılık:** Evet (stabil)

**Merge İşlemi:**
```c
void merge_appointments(Appointment** appointments, int left, int mid, int right);
```
- İki sıralı alt diziyi birleştirir
- O(n) zaman karmaşıklığı
- Geçici diziler kullanır

### 6.2. HeapSort

**Fonksiyon:**
```c
void heap_sort_appointments(Appointment** appointments, int n);
```

**Algoritma:**
1. Diziyi max-heap'e dönüştür (veya min-heap kullanarak ters sırala)
2. Root'u (maksimum/minimum) son elemanla değiştir
3. Heap boyutunu azalt
4. Heap özelliğini koru (heapify_down)
5. Adım 2-4'ü tekrarla

**Zaman Karmaşıklığı:**
- Best: O(n log n)
- Average: O(n log n)
- Worst: O(n log n)

**Uzay Karmaşıklığı:** O(1) - yerinde sıralama

**Kararlılık:** Hayır (unstable)

### 6.3. Binary Search

**Fonksiyonlar:**
```c
int binary_search_appointment_by_id(Appointment** appointments, int n, int appointment_id);
int binary_search_patient_by_id(Patient** patients, int n, int patient_id);
int binary_search_doctor_by_id(Doctor** doctors, int n, int doctor_id);
```

**Algoritma:**
1. Dizinin ortasındaki elemana bak
2. Aranan değer ortadaki değerden küçükse sol yarıda ara
3. Aranan değer ortadaki değerden büyükse sağ yarıda ara
4. Eşitse bulundu
5. Dizi boşalana kadar tekrarla

**Önkoşul:** Dizi sıralı olmalı

**Zaman Karmaşıklığı:**
- Best: O(1) - ortadaki eleman
- Average: O(log n)
- Worst: O(log n)

**Uzay Karmaşıklığı:** O(1) - iteratif versiyon

---

## 7. CSV DOSYA FORMATLARI

### 7.1. Patients CSV

**Dosya Yolu:** `data/patients.csv`

**Format:**
```csv
id,name,age
1,Ahmet Yilmaz,35
2,Ayse Demir,28
3,Mehmet Kaya,42
```

**Alanlar:**
- `id`: Hasta ID (1-999999)
- `name`: Hasta adı (maksimum 100 karakter)
- `age`: Yaş (0-150)

**Fonksiyonlar:**
- `int load_patients_from_csv(const char* filename, Patient** patients, int max_count)` - CSV'den yükler
- `int save_patients_to_csv(const char* filename, Patient** patients, int count)` - CSV'ye kaydeder

**Özellikler:**
- Header satırı otomatik algılanır ve atlanır
- Boş dosya güvenli şekilde işlenir (0 kayıt döner)
- Geçersiz satırlar atlanır
- Veri doğrulama yapılır

### 7.2. Doctors CSV

**Dosya Yolu:** `data/doctors.csv`

**Format:**
```csv
id,name,department
1,Dr. Zeynep Aydin,Kardiyoloji
2,Dr. Can Yildiz,Cocuk Hastaliklari
3,Dr. Ali Veli,Noroloji
```

**Alanlar:**
- `id`: Doktor ID (1-999999)
- `name`: Doktor adı (maksimum 100 karakter)
- `department`: Bölüm adı (maksimum 50 karakter)

**Fonksiyonlar:**
- `int load_doctors_from_csv(const char* filename, Doctor** doctors, int max_count)` - CSV'den yükler
- `int save_doctors_to_csv(const char* filename, Doctor** doctors, int count)` - CSV'ye kaydeder

### 7.3. Appointments CSV

**Dosya Yolu:** `data/appointments.csv`

**Format:**
```csv
appointment_id,patient_id,doctor_id,start_time,end_time
1,1,1,09:00,09:30
2,2,2,10:00,10:30
3,1,2,14:00,14:45
```

**Alanlar:**
- `appointment_id`: Randevu ID (1-999999)
- `patient_id`: Hasta ID referansı
- `doctor_id`: Doktor ID referansı
- `start_time`: Başlangıç zamanı (HH:MM formatı, örn: "09:30")
- `end_time`: Bitiş zamanı (HH:MM formatı, örn: "10:00")

**Zaman Formatı:**
- Giriş: "HH:MM" string formatı (örn: "09:30", "14:45")
- İç gösterim: Dakika cinsinden (0-1439)
- Dönüşüm: `parse_time_string()` ve `format_time_string()` fonksiyonları

**Fonksiyonlar:**
- `int load_appointments_from_csv(const char* filename, Appointment** appointments, int max_count)` - CSV'den yükler
- `int save_appointments_to_csv(const char* filename, Appointment** appointments, int count)` - CSV'ye kaydeder

**Özellikler:**
- Zaman formatı doğrulaması
- Zaman aralığı doğrulaması (start_time < end_time)
- Geçerli zaman aralığı kontrolü (0-1439 dakika)

**Yardımcı Fonksiyonlar:**
- `int parse_time_string(const char* time_str)` - "HH:MM" → dakika
- `void format_time_string(int minutes, char* buffer)` - dakika → "HH:MM"

---

## 8. ANA PROGRAM AKIŞI (main.c)

### 8.1. Program Başlatma

```c
int main(int argc, char *argv[]) {
    // 1. Veri yapılarını başlat
    AVLTree avl_tree;
    avl_init(&avl_tree);
    
    MinHeap heap;
    heap_init(&heap, MAX_RECORDS);
    
    Queue queue;
    queue_init(&queue);
    
    Stack stack;
    stack_init(&stack);
    
    // 2. Veri dizilerini başlat
    Patient* patients[MAX_RECORDS] = {0};
    Doctor* doctors[MAX_RECORDS] = {0};
    Appointment* appointments[MAX_RECORDS] = {0};
    
    int patient_count = 0;
    int doctor_count = 0;
    int appointment_count = 0;
    
    // 3. CSV dosyalarından veri yükle
    patient_count = load_patients_from_csv("data/patients.csv", patients, MAX_RECORDS);
    doctor_count = load_doctors_from_csv("data/doctors.csv", doctors, MAX_RECORDS);
    appointment_count = load_appointments_from_csv("data/appointments.csv", appointments, MAX_RECORDS);
    
    // 4. Yüklenen randevuları AVL ve heap'e ekle
    for (int i = 0; i < appointment_count; i++) {
        if (appointments[i] != NULL) {
            avl_insert(&avl_tree, appointments[i], appointments[i]->doctor_id);
            heap_insert(&heap, appointments[i]);
        }
    }
    
    // 5. Çalışma modunu belirle (CLI veya GUI)
    // 6. Menü döngüsü veya GUI event loop
    // 7. Çıkışta verileri kaydet ve bellek temizle
}
```

### 8.2. Çalışma Modları

**CLI Modu (varsayılan):**
```c
while (continue_program) {
    display_menu();
    choice = get_int_input("", 0, 8);
    continue_program = process_menu_choice(choice, ...);
}
```

**GUI Modu (`--gui` argümanı ile):**
```c
AppData app_data;
// ... app_data'yı doldur ...
start_gui(argc, argv, &app_data);
```

### 8.3. Program Sonlandırma

```c
// 1. Verileri CSV'ye kaydet
save_patients_to_csv("data/patients.csv", patients, patient_count);
save_doctors_to_csv("data/doctors.csv", doctors, doctor_count);
save_appointments_to_csv("data/appointments.csv", appointments, appointment_count);

// 2. Belleği temizle
// - Tüm dizilerdeki elemanları free et
// - Veri yapılarını destroy et
```

---

## 9. CLI MENÜ SİSTEMİ (menu.c/h)

### 9.1. Menü Seçenekleri

```
1. Hasta Ekle
2. Doktor Ekle
3. Randevu Olustur
4. Randevu Iptal Et
5. Iptali Geri Al
6. Bekleme Listesini Goster
7. En Erken Randevuyu Goster
8. Randevulari Listele
0. Cikis
```

### 9.2. Menü Fonksiyonları

**Ana Fonksiyonlar:**
- `void display_menu(void)` - Menüyü ekrana yazdırır
- `int process_menu_choice(int choice, ...)` - Seçimi işler, devam edilecekse 1 döner

**İşlev Fonksiyonları:**
- `void menu_add_patient(Patient** patients, int* patient_count)` - Hasta ekler
- `void menu_add_doctor(Doctor** doctors, int* doctor_count)` - Doktor ekler
- `void menu_create_appointment(...)` - Randevu oluşturur, çakışma kontrolü yapar
- `void menu_cancel_appointment(...)` - Randevu iptal eder, stack'e ekler
- `void menu_undo_cancel(...)` - Son iptali geri alır
- `void menu_show_waiting_queue(Queue* queue)` - Bekleme listesini gösterir
- `void menu_show_earliest_appointment(MinHeap* heap)` - En erken randevuyu gösterir
- `void menu_list_appointments(...)` - Randevuları sıralar ve listeler

**Yardımcı Fonksiyonlar:**
- `int get_int_input(const char* prompt, int min, int max)` - Kullanıcıdan integer girişi alır
- `void get_string_input(const char* prompt, char* buffer, int size)` - Kullanıcıdan string girişi alır
- `void clear_input_buffer(void)` - Giriş buffer'ını temizler

---

## 10. GUI SİSTEMİ (gui.c/h)

### 10.1. GUI Mimarisi

**Katmanlar:**
```
GTK3 Widgets (GtkWindow, GtkButton, GtkEntry, etc.)
    ↓
GUI Callbacks (on_add_patient_clicked, etc.)
    ↓
Core Functions (create_patient, avl_insert, etc.)
    ↓
Data Structures (AVL Tree, Heap, Queue, Stack)
```

### 10.2. AppData Yapısı

```c
typedef struct {
    Patient** patients;
    int* patient_count;
    Doctor** doctors;
    int* doctor_count;
    Appointment** appointments;
    int* appointment_count;
    AVLTree* avl_tree;
    MinHeap* heap;
    Queue* queue;
    Stack* stack;
} AppData;
```

**Amaç:** GUI callback fonksiyonlarının core veri yapılarına erişmesi için

### 10.3. Ana Pencere

**Başlık:** "Hastane Randevu Yonetim Sistemi"
**Boyut:** 800x600
**Butonlar:**
1. Hasta Ekle
2. Doktor Ekle
3. Randevu Olustur
4. Randevu Iptal Et
5. Iptali Geri Al
6. Randevulari Listele
7. Cikis

### 10.4. Dialog Pencereleri

**Add Patient Dialog:**
- Alanlar: Hasta ID, Ad, Yaş
- Butonlar: Kaydet, Iptal
- Validasyon: Boş alan, ID aralığı, yaş aralığı, duplicate ID

**Add Doctor Dialog:**
- Alanlar: Doktor ID, Ad, Bölüm
- Butonlar: Kaydet, Iptal
- Validasyon: Boş alan, ID aralığı, duplicate ID

**Create Appointment Dialog:**
- Alanlar: Randevu ID, Hasta ID, Doktor ID, Başlangıç Zamanı (HH:MM), Bitiş Zamanı (HH:MM)
- Butonlar: Kaydet, Iptal
- Validasyon: Boş alan, ID aralığı, hasta/doktor varlığı, zaman formatı, zaman aralığı, çakışma kontrolü

**Cancel Appointment Dialog:**
- Alan: Randevu ID
- İşlem: AVL'den sil, array'den çıkar, stack'e ekle

**List Appointments Window:**
- GtkTreeView ile tablo gösterimi
- Kolonlar: Randevu ID, Hasta ID, Doktor ID, Başlangıç Zamanı, Bitiş Zamanı
- Sıralı gösterim (MergeSort kullanılır)

### 10.5. GUI Callback Fonksiyonları

**Ana Pencere:**
- `static void on_window_close(...)` - Pencere kapatıldığında gtk_main_quit()
- `static void on_exit_clicked(...)` - Çıkış butonu

**Add Patient:**
- `static void on_add_patient_clicked(...)` - Dialog aç
- `static void on_add_patient_save(...)` - Hasta kaydet
- `static void on_add_patient_cancel(...)` - Dialog iptal

**Add Doctor:**
- `static void on_add_doctor_clicked(...)` - Dialog aç
- `static void on_add_doctor_save(...)` - Doktor kaydet
- `static void on_add_doctor_cancel(...)` - Dialog iptal

**Create Appointment:**
- `static void on_create_appointment_clicked(...)` - Dialog aç
- `static void on_create_appointment_save(...)` - Randevu oluştur, çakışma kontrolü yap
- `static void on_create_appointment_cancel(...)` - Dialog iptal

**Cancel Appointment:**
- `static void on_cancel_appointment_clicked(...)` - Randevu ID sor, iptal et

**Undo Cancel:**
- `static void on_undo_cancel_clicked(...)` - Stack'ten pop, çakışma kontrolü yap, geri ekle

**List Appointments:**
- `static void on_list_appointments_clicked(...)` - Randevuları listele

### 10.6. GUI Başlatma

```c
void start_gui(int argc, char *argv[], AppData* app_data_param) {
    app_data = app_data_param;  // Global static pointer
    gtk_init(&argc, &argv);
    create_main_window();
    gtk_main();  // Event loop
}
```

**Önemli Not:** `app_data` global static pointer olarak saklanır, böylece tüm callback fonksiyonları erişebilir.

---

## 11. BELLEK YÖNETİMİ

### 11.1. Bellek Ayırma

**Dinamik Bellek:**
- `Patient*`, `Doctor*`, `Appointment*` - malloc ile ayrılır
- AVL Tree node'ları - malloc ile ayrılır
- Queue/Stack node'ları - malloc ile ayrılır
- Heap array - malloc ile ayrılır

**Statik Bellek:**
- Ana diziler: `Patient* patients[MAX_RECORDS]` - stack'te
- MAX_RECORDS = 1000 (file_io.h'da tanımlı)

### 11.2. Bellek Serbest Bırakma

**Sıralama:**
1. Dizilerdeki elemanları free et (patients, doctors, appointments)
2. Veri yapılarını destroy et (avl_destroy, heap_destroy, queue_destroy, stack_destroy)

**Önemli Notlar:**
- Queue ve Stack node'ları free edilir ama Appointment pointer'ları free edilmez (caller sorumluluğu)
- AVL Tree destroy edilirken node'lar free edilir ama Appointment pointer'ları free edilmez
- Heap destroy edilirken sadece array free edilir, Appointment pointer'ları free edilmez

### 11.3. Bellek Sızıntısı Önleme

**Kurallar:**
- Her malloc için bir free olmalı
- Dizi elemanları free edilmeli
- Veri yapıları destroy edilmeli
- GUI dialog'larındaki entry array'leri free edilmeli

---

## 12. HATA YÖNETİMİ

### 12.1. Hata Türleri

**Bellek Hataları:**
- malloc NULL dönerse → hata mesajı, işlem iptal

**Dosya Hataları:**
- CSV dosyası açılamazsa → 0 kayıt döner (hata değil, boş dosya)
- Geçersiz satırlar → atlanır, hata mesajı yok

**Validasyon Hataları:**
- Geçersiz ID → hata mesajı
- Geçersiz zaman → hata mesajı
- Duplicate ID → hata mesajı
- Çakışma → uyarı mesajı, queue'ya ekleme

**Veri Yapısı Hataları:**
- Heap dolu → hata mesajı
- Stack boş → bilgi mesajı
- Queue boş → bilgi mesajı

### 12.2. Hata Mesajları

**CLI:**
- printf ile konsola yazdırılır
- Türkçe karakterler kullanılmaz (ASCII only)

**GUI:**
- GtkMessageDialog ile gösterilir
- Türkçe metinler kullanılır
- Tip: ERROR, WARNING, INFO

---

## 13. ÖRNEK KULLANIM SENARYOLARI

### Senaryo 1: Randevu Oluşturma (Başarılı)

```
1. Kullanıcı "Randevu Olustur" seçer
2. Bilgiler girilir:
   - Appointment ID: 1
   - Patient ID: 1
   - Doctor ID: 1
   - Start Time: 09:00
   - End Time: 09:30
3. Validasyonlar geçer
4. avl_has_conflict() çağrılır → çakışma yok
5. create_appointment() ile randevu oluşturulur
6. avl_insert() ile AVL'ye eklenir
7. heap_insert() ile heap'e eklenir
8. appointments[] dizisine eklenir
9. appointment_count artırılır
10. Başarı mesajı gösterilir
```

### Senaryo 2: Randevu Oluşturma (Çakışma)

```
1. Kullanıcı "Randevu Olustur" seçer
2. Bilgiler girilir:
   - Appointment ID: 2
   - Patient ID: 2
   - Doctor ID: 1 (aynı doktor)
   - Start Time: 09:15 (çakışıyor)
   - End Time: 09:45
3. Validasyonlar geçer
4. avl_has_conflict() çağrılır → çakışma var!
5. create_appointment() ile randevu oluşturulur
6. queue_enqueue() ile queue'ya eklenir (bekleme listesi)
7. appointments[] dizisine eklenir
8. appointment_count artırılır
9. Uyarı mesajı gösterilir: "Zaman cakismasi tespit edildi. Randevu bekleme listesine eklendi."
```

### Senaryo 3: Randevu İptal ve Undo

```
1. Kullanıcı "Randevu Iptal Et" seçer
2. Randevu ID: 1 girilir
3. avl_delete() ile AVL'den silinir
4. appointments[] dizisinden çıkarılır (son elemanla yer değiştir)
5. stack_push() ile stack'e eklenir
6. Başarı mesajı: "Randevu iptal edildi ve geri alma listesine eklendi."

7. Kullanıcı "Iptali Geri Al" seçer
8. stack_pop() ile stack'ten çıkarılır
9. avl_has_conflict() ile çakışma kontrolü yapılır
10. Çakışma yoksa:
    - avl_insert() ile AVL'ye eklenir
    - heap_insert() ile heap'e eklenir
    - appointments[] dizisine eklenir
11. Başarı mesajı: "Randevu geri alma basarili."
```

### Senaryo 4: En Erken Randevu Bulma

```
1. Kullanıcı "En Erken Randevuyu Goster" seçer
2. heap_peek_min() çağrılır (O(1))
3. En erken randevu döndürülür
4. Randevu bilgileri gösterilir
```

---

## 14. ÖNEMLİ NOTLAR VE SINIRLAMALAR

### 14.1. Sınırlamalar

**Maksimum Kayıt Sayısı:**
- MAX_RECORDS = 1000 (file_io.h'da tanımlı)
- Diziler statik boyutlu: `Patient* patients[MAX_RECORDS]`

**ID Aralıkları:**
- Patient ID: 1-999999
- Doctor ID: 1-999999
- Appointment ID: 1-999999

**Zaman Aralığı:**
- 0-1439 dakika (00:00 - 23:59)
- start_time < end_time olmalı

**Yaş Aralığı:**
- 0-150

### 14.2. Bilinen Sınırlamalar

**AVL Delete:**
- `avl_delete()` fonksiyonu `appointment_id` ile siler
- Ama ağaç `start_time`'a göre sıralıdır
- Bu yüzden önce ID ile arama yapılır (O(n) worst case)
- Sonra `start_time` ile silinir

**Heap Delete:**
- Heap'ten ID ile silme implement edilmemiş
- Sadece extract_min mevcut
- İptal işleminde heap'ten silme yapılmıyor (sadece AVL'den)

**CSV Çakışma Kontrolü:**
- CSV'den yüklenen randevular için çakışma kontrolü yapılmıyor
- Sadece yeni eklenen randevular için kontrol yapılıyor

### 14.3. Türkçe Karakter Desteği

**CLI:**
- Türkçe karakterler kullanılmaz (ş→s, ğ→g, ü→u, etc.)
- ASCII only

**GUI:**
- Türkçe karakterler kullanılır
- GTK3 UTF-8 desteği sayesinde çalışır

---

## 15. DERLEME VE ÇALIŞTIRMA

### 15.1. CLI Derleme

```bash
gcc -Wall -Wextra -std=c99 -g \
  main.c patient.c doctor.c appointment.c \
  avl.c heap.c queue.c stack.c \
  file_io.c sort_search.c menu.c \
  -o randevu_sistemi.exe
```

### 15.2. GUI Derleme (MSYS2 MinGW64)

```bash
gcc -Wall -Wextra -std=c99 -g \
  main.c gui.c \
  patient.c doctor.c appointment.c \
  avl.c heap.c queue.c stack.c \
  file_io.c sort_search.c menu.c \
  -o randevu_sistemi.exe \
  `pkg-config --cflags --libs gtk+-3.0`
```

### 15.3. Çalıştırma

**CLI:**
```bash
./randevu_sistemi.exe
```

**GUI:**
```bash
./randevu_sistemi.exe --gui
```

**PowerShell (Windows):**
```powershell
.\randevu_sistemi.exe --gui
.\run_gui.ps1  # GTK DLL PATH scripti ile
```

---

## 16. TEST SENARYOLARI

### 16.1. Temel İşlevler

1. **Hasta Ekleme:** Geçerli hasta ekleme, duplicate ID kontrolü
2. **Doktor Ekleme:** Geçerli doktor ekleme, duplicate ID kontrolü
3. **Randevu Oluşturma:** Başarılı ekleme, çakışma kontrolü
4. **Randevu İptal:** AVL'den silme, stack'e ekleme
5. **Undo:** Stack'ten geri alma, çakışma kontrolü
6. **Bekleme Listesi:** Queue'ya ekleme, queue'dan çıkarma
7. **En Erken Randevu:** Heap'ten minimum bulma

### 16.2. Edge Cases

1. **Boş Veri Yapıları:** Boş AVL, boş heap, boş queue, boş stack
2. **Maksimum Kapasite:** MAX_RECORDS'a ulaşma
3. **Geçersiz Girişler:** Negatif ID, geçersiz zaman, çakışan zamanlar
4. **Dosya Hataları:** CSV dosyası yok, geçersiz format

---

## 17. GELECEK İYİLEŞTİRMELER

### 17.1. Önerilen İyileştirmeler

1. **Heap Delete:** ID ile heap'ten silme implementasyonu
2. **AVL Delete Optimizasyonu:** O(log n) delete by ID
3. **CSV Çakışma Kontrolü:** Yüklenen randevular için çakışma kontrolü
4. **Veritabanı:** SQLite entegrasyonu
5. **Çoklu Doktor:** Aynı anda birden fazla doktor için çakışma kontrolü optimizasyonu
6. **Unit Testler:** Otomatik test suite
7. **Logging:** Hata ve işlem logları

---

## 18. SORU-CEVAP İÇİN HAZIR BİLGİLER

### 18.1. Neden AVL Tree Kullanıldı?

- Normal BST sıralı veri girişinde O(n) yüksekliğe sahip olabilir
- AVL Tree her zaman dengeli kalır, yükseklik O(log n) garantilidir
- Insert, search, delete işlemleri her zaman O(log n) karmaşıklığında kalır
- Randevu sistemi gibi dinamik veri ekleme/çıkarma olan sistemlerde performans kritiktir

### 18.2. Queue ve Stack Neden İkisi de Kullanıldı?

- **Queue (FIFO):** Çakışan randevular için adil sıralama (ilk gelen ilk işlenir)
- **Stack (LIFO):** Undo işlemi için (son iptal edilen ilk geri alınır)
- Farklı amaçlar için farklı veri yapıları kullanıldı

### 18.3. Min-Heap Neden Kullanıldı?

- En erken randevuyu O(1) zamanda bulmak için
- O(log n) insert/extract ile verimli
- HeapSort algoritması için de kullanılabilir

### 18.4. MergeSort vs HeapSort Farkları?

- **MergeSort:** Kararlı (stabil), O(n) ekstra bellek
- **HeapSort:** Kararsız (unstable), O(1) ekstra bellek (yerinde)
- Projede her ikisi de implement edilmiştir

---

## 19. KOD ÖRNEKLERİ

### 19.1. Randevu Oluşturma Örneği

```c
// Randevu oluştur
Appointment* appt = create_appointment(1, 1, 1, 540, 570); // 09:00-09:30

// Çakışma kontrolü
if (avl_has_conflict(&avl_tree, 1, 540, 570)) {
    // Çakışma var, queue'ya ekle
    queue_enqueue(&queue, appt);
} else {
    // Çakışma yok, AVL ve heap'e ekle
    avl_insert(&avl_tree, appt, 1);
    heap_insert(&heap, appt);
    appointments[appointment_count++] = appt;
}
```

### 19.2. Undo İşlemi Örneği

```c
// Stack'ten çıkar
Appointment* appt = stack_pop(&stack);

// Çakışma kontrolü
if (avl_has_conflict(&avl_tree, appt->doctor_id, 
                     appt->start_time, appt->end_time)) {
    // Çakışma var, geri ekle
    stack_push(&stack, appt);
} else {
    // Çakışma yok, geri ekle
    avl_insert(&avl_tree, appt, appt->doctor_id);
    heap_insert(&heap, appt);
    appointments[appointment_count++] = appt;
}
```

---

## 20. SONUÇ

Bu proje, veri yapıları ve algoritmalar dersinde öğrenilen temel kavramların pratik bir uygulamasıdır. AVL Tree, Queue, Stack, Min-Heap gibi veri yapıları ile MergeSort, HeapSort ve Binary Search algoritmaları kullanılarak verimli bir randevu yönetim sistemi geliştirilmiştir.

**Proje Özellikleri:**
- Modüler ve bakımı kolay kod yapısı
- Verimli algoritma seçimleri
- Kapsamlı hata yönetimi
- Hem CLI hem GUI desteği
- Veri kalıcılığı (CSV)

**Teknik Detaylar:**
- C99 standardı
- Manuel veri yapıları implementasyonu
- GTK3 GUI entegrasyonu
- CSV dosya işlemleri
- Kapsamlı bellek yönetimi

Bu rapor, ChatGPT'nin projeyi tamamen anlayabilmesi ve proje hakkında sorulara cevap verebilmesi için gerekli tüm bilgileri içermektedir.

