/**
 * Hastane Randevu Yönetim Sistemi
 * 
 * Ana Giriş Noktası
 * 
 * Bu program çeşitli veri yapıları kullanarak hastane randevularını yönetir:
 * - AVL Ağacı: Verimli randevu saklama ve çakışma tespiti
 * - Kuyruk: Dolu doktorlar için bekleme listesi
 * - Yığın: İptal edilen randevular için geri alma sistemi
 * - Min-Heap: En erken randevuları verimli bulma
 * - Sıralama: Randevu listeleme için MergeSort ve HeapSort
 * - Binary Search: Hasta, doktor ve randevu araması
 * 
 * Yazar: Veri Yapıları Final Projesi
 * Ders: Veri Yapıları ve Algoritmalar
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "patient.h"
#include "doctor.h"
#include "appointment.h"
#include "avl.h"
#include "heap.h"
#include "queue.h"
#include "stack.h"
#include "file_io.h"
#include "sort_search.h"
#include "menu.h"
#include "gui.h"

/**
 * Ana fonksiyon - Programın giriş noktası
 * Veri yapılarını başlatır ve CLI veya GUI sistemini başlatır
 */
int main(int argc, char *argv[]) {
    printf("========================================\n");
    printf("  HASTANE RANDEVU YONETIM SISTEMI\n");
    printf("========================================\n");
    printf("Sistem baslatiliyor...\n\n");

    // Veri yapilarini baslat
    AVLTree avl_tree;
    avl_init(&avl_tree);

    MinHeap heap;
    if (!heap_init(&heap, MAX_RECORDS)) {
        printf("Hata: Heap baslatilamadi.\n");
        return 1;
    }

    Queue queue;
    queue_init(&queue);

    Stack stack;
    stack_init(&stack);

    // Veri dizilerini baslat
    Patient* patients[MAX_RECORDS] = {0};
    Doctor* doctors[MAX_RECORDS] = {0};
    Appointment* appointments[MAX_RECORDS] = {0};

    int patient_count = 0;
    int doctor_count = 0;
    int appointment_count = 0;

    // CSV dosyalarindan veri yukle
    printf("Veriler yukleniyor...\n");
    
    patient_count = load_patients_from_csv("data/patients.csv", patients, MAX_RECORDS);
    printf("  %d hasta yuklendi.\n", patient_count);

    doctor_count = load_doctors_from_csv("data/doctors.csv", doctors, MAX_RECORDS);
    printf("  %d doktor yuklendi.\n", doctor_count);

    appointment_count = load_appointments_from_csv("data/appointments.csv", appointments, MAX_RECORDS);
    printf("  %d randevu yuklendi.\n", appointment_count);

    // ID sayaçlarını güncelle (CSV'den yüklenen maksimum ID'lere göre)
    update_patient_id_counter(patients, patient_count);
    update_doctor_id_counter(doctors, doctor_count);
    update_appointment_id_counter(appointments, appointment_count);

    // Yuklenen randevulari AVL agacina ve heap'e ekle
    for (int i = 0; i < appointment_count; i++) {
        if (appointments[i] != NULL) {
            // Çakışma kontrolü yapmadan ekle (zaten dosyadan yüklendi)
            // Gerçek uygulamada çakışma kontrolü yapılmalı
            avl_insert(&avl_tree, appointments[i], appointments[i]->doctor_id);
            heap_insert(&heap, appointments[i]);
        }
    }

    printf("\nSistem hazir!\n");

    // Argumanlara gore calisma modu sec (CLI veya GUI)
    int use_gui = 0;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--gui") == 0) {
            use_gui = 1;
            break;
        }
    }

    if (use_gui) {
        // GUI modu: AppData yapisini hazirla ve GTK arayuzunu baslat
        AppData app_data;
        app_data.patients = patients;
        app_data.patient_count = &patient_count;
        app_data.doctors = doctors;
        app_data.doctor_count = &doctor_count;
        app_data.appointments = appointments;
        app_data.appointment_count = &appointment_count;
        app_data.avl_tree = &avl_tree;
        app_data.heap = &heap;
        app_data.queue = &queue;
        app_data.stack = &stack;

        printf("GUI modu baslatiliyor (--gui).\n");
        start_gui(argc, argv, &app_data);
    } else {
        // CLI modu: mevcut metin tabanli menuyu calistir
        printf("CLI modu baslatiliyor.\n");

        // Menü döngüsü
        int choice;
        int continue_program = 1;

        while (continue_program) {
            display_menu();
            choice = get_int_input("", 0, 8);
            continue_program = process_menu_choice(choice, &avl_tree, &heap, &queue, &stack,
                                                  patients, &patient_count,
                                                  doctors, &doctor_count,
                                                  appointments, &appointment_count);
        }
    }

    // Cikista verileri kaydet
    printf("\nVeriler kaydediliyor...\n");
    
    int saved_patients = save_patients_to_csv("data/patients.csv", patients, patient_count);
    printf("  %d hasta kaydedildi.\n", saved_patients);

    int saved_doctors = save_doctors_to_csv("data/doctors.csv", doctors, doctor_count);
    printf("  %d doktor kaydedildi.\n", saved_doctors);

    int saved_appointments = save_appointments_to_csv("data/appointments.csv", appointments, appointment_count);
    printf("  %d randevu kaydedildi.\n", saved_appointments);

    // Bellegi temizle
    printf("\nBellek temizleniyor...\n");

    // Hastalari serbest birak
    for (int i = 0; i < patient_count; i++) {
        if (patients[i] != NULL) {
            free_patient(patients[i]);
        }
    }

    // Doktorlari serbest birak
    for (int i = 0; i < doctor_count; i++) {
        if (doctors[i] != NULL) {
            free_doctor(doctors[i]);
        }
    }

    // Randevulari serbest birak
    for (int i = 0; i < appointment_count; i++) {
        if (appointments[i] != NULL) {
            free_appointment(appointments[i]);
        }
    }

    // Veri yapilarini temizle
    avl_destroy(&avl_tree);
    heap_destroy(&heap);
    queue_destroy(&queue);
    stack_destroy(&stack);

    printf("\nProgram sonlandirildi. Iyi gunler!\n");
    return 0;
}
