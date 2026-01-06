/**
 * Sıralama ve Arama Algoritmaları Uygulaması
 * 
 * Bu dosya MergeSort, HeapSort ve Binary Search algoritmalarını içerir.
 * Tüm algoritmalar manuel olarak uygulanmıştır (hazır kütüphane kullanılmamıştır).
 */

#include "sort_search.h"

/**
 * Yardımcı Fonksiyon: İki sıralı randevu alt dizisini birleştirir
 * MergeSort algoritmasının birleştirme (merge) adımı
 * 
 * Zaman Karmaşıklığı: O(n) - n = right - left + 1
 * Uzay Karmaşıklığı: O(n) - geçici dizi gerektirir
 * 
 * @param appointments: Randevu işaretçileri dizisi
 * @param left: Sol alt dizinin başlangıç indeksi
 * @param mid: Orta nokta (sol dizinin sonu, sağ dizinin başlangıcı)
 * @param right: Sağ alt dizinin bitiş indeksi
 */
void merge_appointments(Appointment** appointments, int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;  // Sol alt dizinin boyutu
    int n2 = right - mid;     // Sağ alt dizinin boyutu

    // Geçici diziler oluştur
    Appointment** left_arr = (Appointment**)malloc(n1 * sizeof(Appointment*));
    Appointment** right_arr = (Appointment**)malloc(n2 * sizeof(Appointment*));

    if (left_arr == NULL || right_arr == NULL) {
        // Bellek hatası - işlemi durdur
        if (left_arr != NULL) free(left_arr);
        if (right_arr != NULL) free(right_arr);
        return;
    }

    // Verileri geçici dizilere kopyala
    for (i = 0; i < n1; i++) {
        left_arr[i] = appointments[left + i];
    }
    for (j = 0; j < n2; j++) {
        right_arr[j] = appointments[mid + 1 + j];
    }

    // İki alt diziyi birleştir (sıralı şekilde)
    i = 0;  // Sol alt dizinin indeksi
    j = 0;  // Sağ alt dizinin indeksi
    k = left;  // Birleştirilmiş dizinin indeksi

    while (i < n1 && j < n2) {
        // start_time'a göre karşılaştır
        if (left_arr[i]->start_time <= right_arr[j]->start_time) {
            appointments[k] = left_arr[i];
            i++;
        } else {
            appointments[k] = right_arr[j];
            j++;
        }
        k++;
    }

    // Kalan elemanları kopyala (sol alt diziden)
    while (i < n1) {
        appointments[k] = left_arr[i];
        i++;
        k++;
    }

    // Kalan elemanları kopyala (sağ alt diziden)
    while (j < n2) {
        appointments[k] = right_arr[j];
        j++;
        k++;
    }

    // Geçici dizileri serbest bırak
    free(left_arr);
    free(right_arr);
}

/**
 * MergeSort algoritması - Randevular için
 * Böl ve fethet (divide and conquer) yaklaşımı kullanır
 * 
 * Zaman Karmaşıklığı: O(n log n) - her durumda garantili
 * Uzay Karmaşıklığı: O(n) - geçici dizi gerektirir
 * Kararlılık: Evet (stabil sıralama)
 * 
 * @param appointments: Randevu işaretçileri dizisi
 * @param left: Sol indeks
 * @param right: Sağ indeks
 */
void merge_sort_appointments(Appointment** appointments, int left, int right) {
    if (appointments == NULL || left < 0 || right < 0) {
        return;
    }

    if (left < right) {
        // Orta noktayı bul
        int mid = left + (right - left) / 2;

        // Sol yarıyı sırala
        merge_sort_appointments(appointments, left, mid);

        // Sağ yarıyı sırala
        merge_sort_appointments(appointments, mid + 1, right);

        // İki sıralı yarıyı birleştir
        merge_appointments(appointments, left, mid, right);
    }
}

/**
 * Yardımcı Fonksiyon: İki sıralı hasta alt dizisini birleştirir
 * Zaman Karmaşıklığı: O(n)
 * 
 * @param patients: Hasta işaretçileri dizisi
 * @param left: Sol alt dizinin başlangıç indeksi
 * @param mid: Orta nokta
 * @param right: Sağ alt dizinin bitiş indeksi
 */
void merge_patients(Patient** patients, int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Patient** left_arr = (Patient**)malloc(n1 * sizeof(Patient*));
    Patient** right_arr = (Patient**)malloc(n2 * sizeof(Patient*));

    if (left_arr == NULL || right_arr == NULL) {
        if (left_arr != NULL) free(left_arr);
        if (right_arr != NULL) free(right_arr);
        return;
    }

    for (i = 0; i < n1; i++) {
        left_arr[i] = patients[left + i];
    }
    for (j = 0; j < n2; j++) {
        right_arr[j] = patients[mid + 1 + j];
    }

    i = 0;
    j = 0;
    k = left;

    while (i < n1 && j < n2) {
        // ID'ye göre karşılaştır
        if (left_arr[i]->id <= right_arr[j]->id) {
            patients[k] = left_arr[i];
            i++;
        } else {
            patients[k] = right_arr[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        patients[k] = left_arr[i];
        i++;
        k++;
    }

    while (j < n2) {
        patients[k] = right_arr[j];
        j++;
        k++;
    }

    free(left_arr);
    free(right_arr);
}

/**
 * MergeSort algoritması - Hastalar için
 * ID'ye göre sıralar
 * 
 * Zaman Karmaşıklığı: O(n log n)
 * 
 * @param patients: Hasta işaretçileri dizisi
 * @param left: Sol indeks
 * @param right: Sağ indeks
 */
void merge_sort_patients(Patient** patients, int left, int right) {
    if (patients == NULL || left < 0 || right < 0) {
        return;
    }

    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort_patients(patients, left, mid);
        merge_sort_patients(patients, mid + 1, right);
        merge_patients(patients, left, mid, right);
    }
}

/**
 * Yardımcı Fonksiyon: İki sıralı doktor alt dizisini birleştirir
 * Zaman Karmaşıklığı: O(n)
 * 
 * @param doctors: Doktor işaretçileri dizisi
 * @param left: Sol alt dizinin başlangıç indeksi
 * @param mid: Orta nokta
 * @param right: Sağ alt dizinin bitiş indeksi
 */
void merge_doctors(Doctor** doctors, int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Doctor** left_arr = (Doctor**)malloc(n1 * sizeof(Doctor*));
    Doctor** right_arr = (Doctor**)malloc(n2 * sizeof(Doctor*));

    if (left_arr == NULL || right_arr == NULL) {
        if (left_arr != NULL) free(left_arr);
        if (right_arr != NULL) free(right_arr);
        return;
    }

    for (i = 0; i < n1; i++) {
        left_arr[i] = doctors[left + i];
    }
    for (j = 0; j < n2; j++) {
        right_arr[j] = doctors[mid + 1 + j];
    }

    i = 0;
    j = 0;
    k = left;

    while (i < n1 && j < n2) {
        // ID'ye göre karşılaştır
        if (left_arr[i]->id <= right_arr[j]->id) {
            doctors[k] = left_arr[i];
            i++;
        } else {
            doctors[k] = right_arr[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        doctors[k] = left_arr[i];
        i++;
        k++;
    }

    while (j < n2) {
        doctors[k] = right_arr[j];
        j++;
        k++;
    }

    free(left_arr);
    free(right_arr);
}

/**
 * MergeSort algoritması - Doktorlar için
 * ID'ye göre sıralar
 * 
 * Zaman Karmaşıklığı: O(n log n)
 * 
 * @param doctors: Doktor işaretçileri dizisi
 * @param left: Sol indeks
 * @param right: Sağ indeks
 */
void merge_sort_doctors(Doctor** doctors, int left, int right) {
    if (doctors == NULL || left < 0 || right < 0) {
        return;
    }

    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort_doctors(doctors, left, mid);
        merge_sort_doctors(doctors, mid + 1, right);
        merge_doctors(doctors, left, mid, right);
    }
}

/**
 * Yardımcı Fonksiyon: Min-heap özelliğini korur (aşağı doğru)
 * HeapSort için kullanılır
 * 
 * Zaman Karmaşıklığı: O(log n)
 * 
 * @param appointments: Randevu işaretçileri dizisi
 * @param n: Dizinin boyutu
 * @param i: Heapify yapılacak düğümün indeksi
 */
static void heapify_appointments(Appointment** appointments, int n, int i) {
    int largest = i;        // En büyük değerli düğüm (root)
    int left = 2 * i + 1;   // Sol çocuk indeksi
    int right = 2 * i + 2;  // Sağ çocuk indeksi

    // Sol çocuk root'tan büyükse
    if (left < n && appointments[left]->start_time > appointments[largest]->start_time) {
        largest = left;
    }

    // Sağ çocuk şu ana kadar en büyük olandan büyükse
    if (right < n && appointments[right]->start_time > appointments[largest]->start_time) {
        largest = right;
    }

    // En büyük root değilse, değiştir ve devam et
    if (largest != i) {
        // Swap işlemi
        Appointment* temp = appointments[i];
        appointments[i] = appointments[largest];
        appointments[largest] = temp;

        // Etkilenen alt ağacı heapify et
        heapify_appointments(appointments, n, largest);
    }
}

/**
 * HeapSort algoritması - Randevular için
 * Yerinde (in-place) sıralama algoritması
 * 
 * Zaman Karmaşıklığı: O(n log n) - her durumda
 * Uzay Karmaşıklığı: O(1) - ekstra bellek gerektirmez
 * Kararlılık: Hayır (kararsız sıralama)
 * 
 * Algoritma Adımları:
 * 1. Diziyi max-heap'e dönüştür
 * 2. Heap'in kökünü (en büyük) al ve dizinin sonuna taşı
 * 3. Heap boyutunu azalt
 * 4. Yeni kök için heapify yap
 * 5. Adım 2-4'ü tekrarla
 * 
 * @param appointments: Randevu işaretçileri dizisi
 * @param n: Randevu sayısı
 */
void heap_sort_appointments(Appointment** appointments, int n) {
    if (appointments == NULL || n <= 0) {
        return;
    }

    // Diziyi max-heap'e dönüştür (build heap)
    // Son yaprak düğümün ebeveyninden başla
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify_appointments(appointments, n, i);
    }

    // Heap'ten elemanları tek tek çıkar
    for (int i = n - 1; i > 0; i--) {
        // Kökü (en büyük) son pozisyona taşı
        Appointment* temp = appointments[0];
        appointments[0] = appointments[i];
        appointments[i] = temp;

        // Azaltılmış heap'i heapify et
        heapify_appointments(appointments, i, 0);
    }
}

/**
 * Binary Search algoritması - Randevu ID'sine göre arama
 * 
 * ÖNKOŞUL: Dizi appointment_id'ye göre sıralı olmalıdır!
 * 
 * Zaman Karmaşıklığı: O(log n) - her adımda arama alanı yarıya iner
 * Uzay Karmaşıklığı: O(1) - iteratif uygulama
 * 
 * Algoritma:
 * 1. Dizinin ortasındaki elemanı kontrol et
 * 2. Eğer aranan değer ortadaki değerden küçükse, sol yarıda ara
 * 3. Eğer aranan değer ortadaki değerden büyükse, sağ yarıda ara
 * 4. Bulunana kadar veya arama alanı tükenene kadar tekrarla
 * 
 * @param appointments: Sıralı randevu işaretçileri dizisi
 * @param n: Dizinin boyutu
 * @param appointment_id: Aranacak randevu ID'si
 * @return: Bulunan randevunun indeksi, bulunamazsa -1
 */
int binary_search_appointment_by_id(Appointment** appointments, int n, int appointment_id) {
    if (appointments == NULL || n <= 0) {
        return -1;
    }

    int left = 0;
    int right = n - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;  // Overflow'u önlemek için

        // Ortadaki elemanı kontrol et
        if (appointments[mid]->appointment_id == appointment_id) {
            return mid;  // Bulundu!
        }

        // Aranan değer ortadaki değerden küçükse, sol yarıda ara
        if (appointments[mid]->appointment_id > appointment_id) {
            right = mid - 1;
        } else {
            // Aranan değer ortadaki değerden büyükse, sağ yarıda ara
            left = mid + 1;
        }
    }

    return -1;  // Bulunamadı
}

/**
 * Binary Search algoritması - Hasta ID'sine göre arama
 * 
 * ÖNKOŞUL: Dizi patient_id'ye göre sıralı olmalıdır!
 * 
 * Zaman Karmaşıklığı: O(log n)
 * 
 * @param patients: Sıralı hasta işaretçileri dizisi
 * @param n: Dizinin boyutu
 * @param patient_id: Aranacak hasta ID'si
 * @return: Bulunan hastanın indeksi, bulunamazsa -1
 */
int binary_search_patient_by_id(Patient** patients, int n, int patient_id) {
    if (patients == NULL || n <= 0) {
        return -1;
    }

    int left = 0;
    int right = n - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (patients[mid]->id == patient_id) {
            return mid;  // Bulundu!
        }

        if (patients[mid]->id > patient_id) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }

    return -1;  // Bulunamadı
}

/**
 * Binary Search algoritması - Doktor ID'sine göre arama
 * 
 * ÖNKOŞUL: Dizi doctor_id'ye göre sıralı olmalıdır!
 * 
 * Zaman Karmaşıklığı: O(log n)
 * 
 * @param doctors: Sıralı doktor işaretçileri dizisi
 * @param n: Dizinin boyutu
 * @param doctor_id: Aranacak doktor ID'si
 * @return: Bulunan doktorun indeksi, bulunamazsa -1
 */
int binary_search_doctor_by_id(Doctor** doctors, int n, int doctor_id) {
    if (doctors == NULL || n <= 0) {
        return -1;
    }

    int left = 0;
    int right = n - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (doctors[mid]->id == doctor_id) {
            return mid;  // Bulundu!
        }

        if (doctors[mid]->id > doctor_id) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }

    return -1;  // Bulunamadı
}

