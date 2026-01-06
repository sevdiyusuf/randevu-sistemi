/**
 * Menü Sistemi Uygulaması
 * 
 * Hastane randevu yönetim sistemi için CLI (Komut Satırı Arayüzü) menüsü
 * Tüm kullanıcı etkileşimlerini yönetir
 */

#include "menu.h"
#include "sort_search.h"
#include "file_io.h"

/**
 * Ana menüyü ekrana yazdırır
 */
void display_menu(void) {
    printf("\n========================================\n");
    printf("  HASTANE RANDEVU YONETIM SISTEMI\n");
    printf("========================================\n");
    printf("1. Hasta Ekle\n");
    printf("2. Doktor Ekle\n");
    printf("3. Randevu Olustur\n");
    printf("4. Randevu Iptal Et\n");
    printf("5. Iptali Geri Al (Undo)\n");
    printf("6. Bekleme Listesini Göster\n");
    printf("7. En Erken Randevuyu Göster\n");
    printf("8. Tum Randevulari Listele (Sirali)\n");
    printf("0. Cikis\n");
    printf("========================================\n");
    printf("Seciminiz: ");
}

/**
 * Girdi buffer'ını temizler
 * scanf sonrası kalan karakterleri temizlemek için kullanılır
 */
void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // Buffer'ı temizle
    }
}

/**
 * Kullanıcıdan integer girdi alır ve doğrular
 * 
 * @param prompt: Gösterilecek mesaj
 * @param min: Minimum değer
 * @param max: Maksimum değer
 * @return: Geçerli integer değer
 */
int get_int_input(const char* prompt, int min, int max) {
    int value;
    int result;

    while (1) {
        printf("%s", prompt);
        result = scanf("%d", &value);
        clear_input_buffer();

        if (result == 1 && value >= min && value <= max) {
            return value;
        }

        printf("Hata: Lutfen %d ile %d arasinda bir deger girin.\n", min, max);
    }
}

/**
 * Kullanıcıdan string girdi alır
 * 
 * @param prompt: Gösterilecek mesaj
 * @param buffer: Girdiyi saklamak için buffer
 * @param buffer_size: Buffer boyutu
 */
void get_string_input(const char* prompt, char* buffer, int buffer_size) {
    if (buffer == NULL || buffer_size <= 0) {
        return;
    }

    printf("%s", prompt);
    if (fgets(buffer, buffer_size, stdin) != NULL) {
        // Newline karakterini kaldır
        int len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
    } else {
        buffer[0] = '\0';
    }
}

/**
 * Yeni hasta ekleme menü işlevi
 * 
 * @param patients: Hasta işaretçileri dizisi
 * @param patient_count: Mevcut hasta sayısı (güncellenecek)
 */
void menu_add_patient(Patient** patients, int* patient_count) {
    if (patients == NULL || patient_count == NULL) {
        printf("Hata: Geçersiz parametreler.\n");
        return;
    }

    if (*patient_count >= MAX_RECORDS) {
        printf("Hata: Maksimum hasta sayısına ulaşıldı (%d).\n", MAX_RECORDS);
        return;
    }

    printf("\n--- Yeni Hasta Ekleme ---\n");

    int id = get_int_input("Hasta ID: ", 1, 999999);
    
    // ID kontrolu (benzersiz olmali)
    for (int i = 0; i < *patient_count; i++) {
        if (patients[i] != NULL && patients[i]->id == id) {
            printf("Hata: Bu ID zaten kullaniliyor.\n");
            return;
        }
    }

    char name[100];
    get_string_input("Hasta Adi: ", name, sizeof(name));

    int age = get_int_input("Yas: ", 0, 150);

    Patient* patient = create_patient(id, name, age);
    if (patient == NULL) {
        printf("Hata: Hasta olusturulamadi.\n");
        return;
    }

    patients[*patient_count] = patient;
    (*patient_count)++;

    printf("Hasta basariyla eklendi!\n");
    display_patient(patient);
}

/**
 * Yeni doktor ekleme menü işlevi
 * 
 * @param doctors: Doktor işaretçileri dizisi
 * @param doctor_count: Mevcut doktor sayısı (güncellenecek)
 */
void menu_add_doctor(Doctor** doctors, int* doctor_count) {
    if (doctors == NULL || doctor_count == NULL) {
        printf("Hata: Geçersiz parametreler.\n");
        return;
    }

    if (*doctor_count >= MAX_RECORDS) {
        printf("Hata: Maksimum doktor sayısına ulaşıldı (%d).\n", MAX_RECORDS);
        return;
    }

    printf("\n--- Yeni Doktor Ekleme ---\n");

    int id = get_int_input("Doktor ID: ", 1, 999999);
    
    // ID kontrolu (benzersiz olmali)
    for (int i = 0; i < *doctor_count; i++) {
        if (doctors[i] != NULL && doctors[i]->id == id) {
            printf("Hata: Bu ID zaten kullaniliyor.\n");
            return;
        }
    }

    char name[100];
    get_string_input("Doktor Adi: ", name, sizeof(name));

    char department[50];
    get_string_input("Bolum: ", department, sizeof(department));

    Doctor* doctor = create_doctor(id, name, department);
    if (doctor == NULL) {
        printf("Hata: Doktor olusturulamadi.\n");
        return;
    }

    doctors[*doctor_count] = doctor;
    (*doctor_count)++;

    printf("Doktor basariyla eklendi!\n");
    display_doctor(doctor);
}

/**
 * Randevu oluşturma menü işlevi
 * Çakışma kontrolü yapar ve gerekirse bekleme listesine ekler
 * 
 * @param avl_tree: AVL ağacı (çakışma kontrolü için)
 * @param heap: Min-heap (randevu ekleme için)
 * @param queue: Bekleme listesi kuyruğu
 * @param appointments: Randevu işaretçileri dizisi
 * @param appointment_count: Mevcut randevu sayısı (güncellenecek)
 * @param patients: Hasta işaretçileri dizisi
 * @param patient_count: Hasta sayısı
 * @param doctors: Doktor işaretçileri dizisi
 * @param doctor_count: Doktor sayısı
 */
void menu_create_appointment(AVLTree* avl_tree, MinHeap* heap, Queue* queue,
                            Appointment** appointments, int* appointment_count,
                            Patient** patients, int patient_count,
                            Doctor** doctors, int doctor_count) {
    if (avl_tree == NULL || heap == NULL || queue == NULL || 
        appointments == NULL || appointment_count == NULL) {
        printf("Hata: Geçersiz parametreler.\n");
        return;
    }

    if (*appointment_count >= MAX_RECORDS) {
        printf("Hata: Maksimum randevu sayısına ulaşıldı (%d).\n", MAX_RECORDS);
        return;
    }

    printf("\n--- Yeni Randevu Olusturma ---\n");

    // Hasta ve doktor kontrolu
    if (patient_count == 0) {
        printf("Hata: Sistemde hasta bulunmuyor. Once hasta ekleyin.\n");
        return;
    }

    if (doctor_count == 0) {
        printf("Hata: Sistemde doktor bulunmuyor. Once doktor ekleyin.\n");
        return;
    }

    int appointment_id = get_int_input("Randevu ID: ", 1, 999999);
    
    // ID kontrolu
    for (int i = 0; i < *appointment_count; i++) {
        if (appointments[i] != NULL && appointments[i]->appointment_id == appointment_id) {
            printf("Hata: Bu randevu ID'si zaten kullaniliyor.\n");
            return;
        }
    }

    int patient_id = get_int_input("Hasta ID: ", 1, 999999);
    int doctor_id = get_int_input("Doktor ID: ", 1, 999999);

    // Hasta ve doktor var mi kontrol et
    int patient_found = 0, doctor_found = 0;
    for (int i = 0; i < patient_count; i++) {
        if (patients[i] != NULL && patients[i]->id == patient_id) {
            patient_found = 1;
            break;
        }
    }
    for (int i = 0; i < doctor_count; i++) {
        if (doctors[i] != NULL && doctors[i]->id == doctor_id) {
            doctor_found = 1;
            break;
        }
    }

    if (!patient_found) {
        printf("Hata: Hasta bulunamadi (ID: %d).\n", patient_id);
        return;
    }
    if (!doctor_found) {
        printf("Hata: Doktor bulunamadi (ID: %d).\n", doctor_id);
        return;
    }

    // Zaman bilgisi al
    char start_time_str[10], end_time_str[10];
    get_string_input("Baslangic saati (HH:MM): ", start_time_str, sizeof(start_time_str));
    get_string_input("Bitis saati (HH:MM): ", end_time_str, sizeof(end_time_str));

    int start_time = parse_time_string(start_time_str);
    int end_time = parse_time_string(end_time_str);

    if (start_time < 0 || end_time < 0) {
        printf("Hata: Gecersiz zaman formati. HH:MM formatinda girin (orn: 09:30).\n");
        return;
    }

    if (start_time >= end_time) {
        printf("Hata: Baslangic saati bitis saatinden once olmalidir.\n");
        return;
    }

    // Randevu olustur
    Appointment* appointment = create_appointment(appointment_id, patient_id, 
                                                doctor_id, start_time, end_time);
    if (appointment == NULL) {
        printf("Hata: Randevu olusturulamadi.\n");
        return;
    }

    // Cakisma kontrolu (AVL agacinda)
    if (avl_has_conflict(avl_tree, doctor_id, start_time, end_time)) {
        printf("Uyari: Bu zaman dilimi icin doktorun baska bir randevusu var.\n");
        printf("Randevu bekleme listesine eklendi.\n");
        
        // Bekleme listesine ekle
        if (queue_enqueue(queue, appointment)) {
            printf("Randevu bekleme listesine basariyla eklendi.\n");
        } else {
            printf("Hata: Bekleme listesine eklenemedi.\n");
            free_appointment(appointment);
        }
        return;
    }

    // Cakisma yok, randevuyu ekle
    if (avl_insert(avl_tree, appointment, doctor_id)) {
        // AVL'ye eklendi, simdi heap'e de ekle
        if (heap_insert(heap, appointment)) {
            appointments[*appointment_count] = appointment;
            (*appointment_count)++;
            printf("Randevu basariyla olusturuldu!\n");
            display_appointment(appointment);
        } else {
            printf("Hata: Heap'e eklenemedi.\n");
            // AVL'den sil (basitlestirilmis - gercek uygulamada daha dikkatli olunmali)
            avl_delete(avl_tree, appointment_id);
            free_appointment(appointment);
        }
    } else {
        printf("Hata: Randevu eklenemedi.\n");
        free_appointment(appointment);
    }
}

/**
 * Randevu iptal etme menü işlevi
 * İptal edilen randevu geri almak için stack'e eklenir
 * 
 * @param avl_tree: AVL ağacı
 * @param heap: Min-heap
 * @param stack: Geri alma stack'i
 * @param appointments: Randevu işaretçileri dizisi
 * @param appointment_count: Mevcut randevu sayısı (güncellenecek)
 */
void menu_cancel_appointment(AVLTree* avl_tree, MinHeap* heap, Stack* stack,
                            Appointment** appointments, int* appointment_count) {
    if (avl_tree == NULL || heap == NULL || stack == NULL || 
        appointments == NULL || appointment_count == NULL) {
        printf("Hata: Geçersiz parametreler.\n");
        return;
    }

    if (*appointment_count == 0) {
        printf("Iptal edilecek randevu bulunmuyor.\n");
        return;
    }

    printf("\n--- Randevu Iptal Etme ---\n");
    int appointment_id = get_int_input("Iptal edilecek Randevu ID: ", 1, 999999);

    // Randevuyu bul ve sil
    Appointment* appointment = avl_delete(avl_tree, appointment_id);
    if (appointment == NULL) {
        printf("Hata: Randevu bulunamadi (ID: %d).\n", appointment_id);
        return;
    }

    // Heap'ten de sil (basitlestirilmis - gercek uygulamada daha karmasik)
    // Not: Heap'ten ID ile silme zor oldugu icin, tum heap'i yeniden olusturmak gerekebilir
    // Bu ornekte sadece AVL'den sildik

    // Diziden de sil
    for (int i = 0; i < *appointment_count; i++) {
        if (appointments[i] != NULL && appointments[i]->appointment_id == appointment_id) {
            // Son elemani buraya tasi
            appointments[i] = appointments[*appointment_count - 1];
            appointments[*appointment_count - 1] = NULL;
            (*appointment_count)--;
            break;
        }
    }

    // Stack'e ekle (geri alma icin)
    if (stack_push(stack, appointment)) {
        printf("Randevu basariyla iptal edildi ve geri alma listesine eklendi.\n");
        display_appointment(appointment);
    } else {
        printf("Randevu iptal edildi ancak geri alma listesine eklenemedi.\n");
    }
}

/**
 * Son iptali geri alma menü işlevi
 * Stack'ten son iptal edilen randevuyu alır ve geri ekler
 * 
 * @param avl_tree: AVL ağacı
 * @param heap: Min-heap
 * @param stack: Geri alma stack'i
 * @param appointments: Randevu işaretçileri dizisi
 * @param appointment_count: Mevcut randevu sayısı (güncellenecek)
 */
void menu_undo_cancel(AVLTree* avl_tree, MinHeap* heap, Stack* stack,
                     Appointment** appointments, int* appointment_count) {
    if (avl_tree == NULL || heap == NULL || stack == NULL || 
        appointments == NULL || appointment_count == NULL) {
        printf("Hata: Geçersiz parametreler.\n");
        return;
    }

    if (stack_is_empty(stack)) {
        printf("Geri alinacak iptal bulunmuyor.\n");
        return;
    }

    printf("\n--- Iptali Geri Alma ---\n");

    // Stack'ten randevuyu al
    Appointment* appointment = stack_pop(stack);
    if (appointment == NULL) {
        printf("Hata: Randevu alinamadi.\n");
        return;
    }

    // Cakisma kontrolu
    if (avl_has_conflict(avl_tree, appointment->doctor_id, 
                        appointment->start_time, appointment->end_time)) {
        printf("Uyari: Bu zaman dilimi artik dolu. Randevu geri alinamadi.\n");
        printf("Randevu tekrar stack'e eklendi.\n");
        
        // Stack'e geri ekle (daha sonra tekrar denemek icin)
        stack_push(stack, appointment);
        return;
    }

    // Randevuyu geri ekle
    if (avl_insert(avl_tree, appointment, appointment->doctor_id)) {
        if (heap_insert(heap, appointment)) {
            if (*appointment_count < MAX_RECORDS) {
                appointments[*appointment_count] = appointment;
                (*appointment_count)++;
                printf("Randevu basariyla geri alindi!\n");
                display_appointment(appointment);
            } else {
                printf("Hata: Maksimum randevu sayisina ulasildi.\n");
                avl_delete(avl_tree, appointment->appointment_id);
                free_appointment(appointment);
            }
        } else {
            printf("Hata: Heap'e eklenemedi.\n");
            avl_delete(avl_tree, appointment->appointment_id);
            free_appointment(appointment);
        }
    } else {
        printf("Hata: Randevu geri eklenemedi.\n");
        free_appointment(appointment);
    }
}

/**
 * Bekleme listesini gösterir
 * 
 * @param queue: Bekleme listesi kuyruğu
 */
void menu_show_waiting_queue(Queue* queue) {
    if (queue == NULL) {
        printf("Hata: Geçersiz parametreler.\n");
        return;
    }

    printf("\n--- Bekleme Listesi ---\n");

    if (queue_is_empty(queue)) {
        printf("Bekleme listesi bos.\n");
        return;
    }

    printf("Bekleyen randevu sayisi: %d\n\n", queue_size(queue));
    
    // Kuyrugu gecici olarak kopyala ve goster (orijinal kuyrugu bozmadan)
    // Not: Bu basitlestirilmis bir gosterim
    // Gercek uygulamada queue'yu traverse edebilmek icin iterator gerekir
    printf("Not: Bekleme listesi goruntuleme icin queue traversal gerekir.\n");
    printf("Su anda %d randevu bekliyor.\n", queue_size(queue));
}

/**
 * En erken randevuyu gösterir (min-heap kullanarak)
 * 
 * @param heap: Min-heap
 */
void menu_show_earliest_appointment(MinHeap* heap) {
    if (heap == NULL) {
        printf("Hata: Geçersiz parametreler.\n");
        return;
    }

    printf("\n--- En Erken Randevu ---\n");

    Appointment* earliest = heap_peek_min(heap);
    if (earliest == NULL) {
        printf("Randevu bulunmuyor.\n");
        return;
    }

    printf("En erken randevu:\n");
    display_appointment(earliest);
}

/**
 * Tüm randevuları sıralı şekilde listeler (MergeSort kullanarak)
 * 
 * @param appointments: Randevu işaretçileri dizisi
 * @param appointment_count: Randevu sayısı
 * @param patients: Hasta işaretçileri dizisi
 * @param patient_count: Hasta sayısı
 * @param doctors: Doktor işaretçileri dizisi
 * @param doctor_count: Doktor sayısı
 */
void menu_list_appointments(Appointment** appointments, int appointment_count,
                           Patient** patients, int patient_count,
                           Doctor** doctors, int doctor_count) {
    // Parametreler kullanılmıyor ama fonksiyon imzası için gerekli
    (void)patients;
    (void)patient_count;
    (void)doctors;
    (void)doctor_count;
    
    if (appointments == NULL) {
        printf("Hata: Geçersiz parametreler.\n");
        return;
    }

    printf("\n--- Tum Randevular (Sirali) ---\n");

    if (appointment_count == 0) {
        printf("Randevu bulunmuyor.\n");
        return;
    }

    // Randevulari sirala (MergeSort kullanarak)
    Appointment** sorted_appointments = (Appointment**)malloc(appointment_count * sizeof(Appointment*));
    if (sorted_appointments == NULL) {
        printf("Hata: Bellek ayrilamadi.\n");
        return;
    }

    // Diziyi kopyala
    for (int i = 0; i < appointment_count; i++) {
        sorted_appointments[i] = appointments[i];
    }

    // Sırala
    merge_sort_appointments(sorted_appointments, 0, appointment_count - 1);

    // Listele
    printf("Toplam %d randevu:\n\n", appointment_count);
    for (int i = 0; i < appointment_count; i++) {
        if (sorted_appointments[i] != NULL) {
            printf("%d. ", i + 1);
            display_appointment(sorted_appointments[i]);
        }
    }

    free(sorted_appointments);
}

/**
 * Kullanıcı menü seçimini işler
 * 
 * @param choice: Kullanıcının seçimi
 * @param avl_tree: AVL ağacı
 * @param heap: Min-heap
 * @param queue: Bekleme listesi kuyruğu
 * @param stack: Geri alma stack'i
 * @param patients: Hasta işaretçileri dizisi
 * @param patient_count: Hasta sayısı (güncellenebilir)
 * @param doctors: Doktor işaretçileri dizisi
 * @param doctor_count: Doktor sayısı (güncellenebilir)
 * @param appointments: Randevu işaretçileri dizisi
 * @param appointment_count: Randevu sayısı (güncellenebilir)
 * @return: Devam etmek için 1, çıkış için 0
 */
int process_menu_choice(int choice, AVLTree* avl_tree, MinHeap* heap, 
                        Queue* queue, Stack* stack,
                        Patient** patients, int* patient_count,
                        Doctor** doctors, int* doctor_count,
                        Appointment** appointments, int* appointment_count) {
    switch (choice) {
        case 1:
            menu_add_patient(patients, patient_count);
            break;
        case 2:
            menu_add_doctor(doctors, doctor_count);
            break;
        case 3:
            menu_create_appointment(avl_tree, heap, queue, appointments, appointment_count,
                                   patients, *patient_count, doctors, *doctor_count);
            break;
        case 4:
            menu_cancel_appointment(avl_tree, heap, stack, appointments, appointment_count);
            break;
        case 5:
            menu_undo_cancel(avl_tree, heap, stack, appointments, appointment_count);
            break;
        case 6:
            menu_show_waiting_queue(queue);
            break;
        case 7:
            menu_show_earliest_appointment(heap);
            break;
        case 8:
            menu_list_appointments(appointments, *appointment_count,
                                 patients, *patient_count,
                                 doctors, *doctor_count);
            break;
        case 0:
            printf("\nCikiliyor...\n");
            return 0; // Cikis
        default:
            printf("Hata: Gecersiz secim. Lutfen 0-8 arasi bir deger girin.\n");
            break;
    }

    return 1; // Devam et
}

