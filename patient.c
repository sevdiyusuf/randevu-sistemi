/**
 * Hasta Yönetimi Uygulaması
 * 
 * Hasta verilerinin oluşturulması, doğrulanması ve gösterilmesi
 */

#include "patient.h"

/**
 * Yeni bir hasta oluşturur
 * 
 * @param id: Benzersiz hasta ID'si
 * @param name: Hasta adı
 * @param age: Hasta yaşı
 * @return: Oluşturulan hasta işaretçisi, hata durumunda NULL
 */
Patient* create_patient(int id, const char* name, int age) {
    if (name == NULL || id <= 0 || age < 0 || age > 150) {
        return NULL;
    }

    Patient* patient = (Patient*)malloc(sizeof(Patient));
    if (patient == NULL) {
        return NULL; // Bellek hatası
    }

    patient->id = id;
    strncpy(patient->name, name, sizeof(patient->name) - 1);
    patient->name[sizeof(patient->name) - 1] = '\0';
    patient->age = age;

    return patient;
}

/**
 * Hasta için ayrılan belleği serbest bırakır
 * 
 * @param patient: Serbest bırakılacak hasta işaretçisi
 */
void free_patient(Patient* patient) {
    if (patient != NULL) {
        free(patient);
    }
}

/**
 * Hasta bilgilerini ekrana yazdırır
 * 
 * @param patient: Yazdırılacak hasta işaretçisi
 */
void display_patient(const Patient* patient) {
    if (patient == NULL) {
        printf("Hasta bilgisi bulunamadı.\n");
        return;
    }

    printf("Hasta ID: %d\n", patient->id);
    printf("Ad: %s\n", patient->name);
    printf("Yaş: %d\n", patient->age);
    printf("---\n");
}

/**
 * Hasta verilerini doğrular
 * 
 * @param patient: Doğrulanacak hasta işaretçisi
 * @return: Geçerliyse 1, değilse 0
 */
int validate_patient(const Patient* patient) {
    if (patient == NULL) {
        return 0;
    }

    if (patient->id <= 0) {
        return 0;
    }

    if (patient->age < 0 || patient->age > 150) {
        return 0;
    }

    if (strlen(patient->name) == 0) {
        return 0;
    }

    return 1;
}

