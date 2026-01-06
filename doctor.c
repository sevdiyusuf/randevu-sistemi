/**
 * Doktor Yönetimi Uygulaması
 * 
 * Doktor verilerinin oluşturulması, doğrulanması ve gösterilmesi
 */

#include "doctor.h"

/**
 * Yeni bir doktor oluşturur
 * 
 * @param id: Benzersiz doktor ID'si
 * @param name: Doktor adı
 * @param department: Bölüm adı
 * @return: Oluşturulan doktor işaretçisi, hata durumunda NULL
 */
Doctor* create_doctor(int id, const char* name, const char* department) {
    if (name == NULL || department == NULL || id <= 0) {
        return NULL;
    }

    Doctor* doctor = (Doctor*)malloc(sizeof(Doctor));
    if (doctor == NULL) {
        return NULL; // Bellek hatası
    }

    doctor->id = id;
    strncpy(doctor->name, name, sizeof(doctor->name) - 1);
    doctor->name[sizeof(doctor->name) - 1] = '\0';
    strncpy(doctor->department, department, sizeof(doctor->department) - 1);
    doctor->department[sizeof(doctor->department) - 1] = '\0';

    return doctor;
}

/**
 * Doktor için ayrılan belleği serbest bırakır
 * 
 * @param doctor: Serbest bırakılacak doktor işaretçisi
 */
void free_doctor(Doctor* doctor) {
    if (doctor != NULL) {
        free(doctor);
    }
}

/**
 * Doktor bilgilerini ekrana yazdırır
 * 
 * @param doctor: Yazdırılacak doktor işaretçisi
 */
void display_doctor(const Doctor* doctor) {
    if (doctor == NULL) {
        printf("Doktor bilgisi bulunamadı.\n");
        return;
    }

    printf("Doktor ID: %d\n", doctor->id);
    printf("Ad: %s\n", doctor->name);
    printf("Bölüm: %s\n", doctor->department);
    printf("---\n");
}

/**
 * Doktor verilerini doğrular
 * 
 * @param doctor: Doğrulanacak doktor işaretçisi
 * @return: Geçerliyse 1, değilse 0
 */
int validate_doctor(const Doctor* doctor) {
    if (doctor == NULL) {
        return 0;
    }

    if (doctor->id <= 0) {
        return 0;
    }

    if (strlen(doctor->name) == 0) {
        return 0;
    }

    if (strlen(doctor->department) == 0) {
        return 0;
    }

    return 1;
}

