/**
 * Randevu Yönetimi Uygulaması
 * 
 * Randevu verilerinin oluşturulması, doğrulanması ve gösterilmesi
 */

#include "appointment.h"

/**
 * Yeni bir randevu oluşturur
 * 
 * @param appointment_id: Benzersiz randevu ID'si
 * @param patient_id: Hasta ID'si
 * @param doctor_id: Doktor ID'si
 * @param start_time: Başlangıç saati (gece yarısından itibaren dakika)
 * @param end_time: Bitiş saati (gece yarısından itibaren dakika)
 * @return: Oluşturulan randevu işaretçisi, hata durumunda NULL
 */
Appointment* create_appointment(int appointment_id, int patient_id, 
                                 int doctor_id, int start_time, int end_time) {
    if (appointment_id <= 0 || patient_id <= 0 || doctor_id <= 0) {
        return NULL;
    }

    if (start_time < 0 || start_time > 1439 || end_time < 0 || end_time > 1439) {
        return NULL;
    }

    if (start_time >= end_time) {
        return NULL; // Başlangıç saati bitiş saatinden önce olmalı
    }

    Appointment* appointment = (Appointment*)malloc(sizeof(Appointment));
    if (appointment == NULL) {
        return NULL; // Bellek hatası
    }

    appointment->appointment_id = appointment_id;
    appointment->patient_id = patient_id;
    appointment->doctor_id = doctor_id;
    appointment->start_time = start_time;
    appointment->end_time = end_time;

    return appointment;
}

/**
 * Randevu için ayrılan belleği serbest bırakır
 * 
 * @param appointment: Serbest bırakılacak randevu işaretçisi
 */
void free_appointment(Appointment* appointment) {
    if (appointment != NULL) {
        free(appointment);
    }
}

/**
 * Randevu bilgilerini ekrana yazdırır
 * 
 * @param appointment: Yazdırılacak randevu işaretçisi
 */
void display_appointment(const Appointment* appointment) {
    if (appointment == NULL) {
        printf("Randevu bilgisi bulunamadı.\n");
        return;
    }

    char start_str[10], end_str[10];
    minutes_to_time_string(appointment->start_time, start_str);
    minutes_to_time_string(appointment->end_time, end_str);

    printf("Randevu ID: %d\n", appointment->appointment_id);
    printf("Hasta ID: %d\n", appointment->patient_id);
    printf("Doktor ID: %d\n", appointment->doctor_id);
    printf("Başlangıç: %s\n", start_str);
    printf("Bitiş: %s\n", end_str);
    printf("---\n");
}

/**
 * Randevu verilerini doğrular
 * 
 * @param appointment: Doğrulanacak randevu işaretçisi
 * @return: Geçerliyse 1, değilse 0
 */
int validate_appointment(const Appointment* appointment) {
    if (appointment == NULL) {
        return 0;
    }

    if (appointment->appointment_id <= 0) {
        return 0;
    }

    if (appointment->patient_id <= 0 || appointment->doctor_id <= 0) {
        return 0;
    }

    if (appointment->start_time < 0 || appointment->start_time > 1439) {
        return 0;
    }

    if (appointment->end_time < 0 || appointment->end_time > 1439) {
        return 0;
    }

    if (appointment->start_time >= appointment->end_time) {
        return 0;
    }

    return 1;
}

/**
 * İki randevunun zaman aralığının çakışıp çakışmadığını kontrol eder
 * 
 * İki aralık [a1, b1] ve [a2, b2] çakışır eğer: a1 < b2 && a2 < b1
 * 
 * @param a1: İlk randevu
 * @param a2: İkinci randevu
 * @return: Çakışıyorsa 1, değilse 0
 */
int appointments_overlap(const Appointment* a1, const Appointment* a2) {
    if (a1 == NULL || a2 == NULL) {
        return 0;
    }

    // Aynı doktor için kontrol et
    if (a1->doctor_id != a2->doctor_id) {
        return 0; // Farklı doktorlar, çakışma yok
    }

    // Zaman aralığı çakışması kontrolü
    // [a1.start, a1.end] ve [a2.start, a2.end] çakışır mı?
    if (a1->start_time < a2->end_time && a2->start_time < a1->end_time) {
        return 1; // Çakışma var
    }

    return 0; // Çakışma yok
}

/**
 * Dakikayı "HH:MM" formatına çevirir
 * 
 * @param minutes: Gece yarısından itibaren dakika (0-1439)
 * @param buffer: Formatlanmış string'i saklamak için buffer (en az 6 karakter)
 */
void minutes_to_time_string(int minutes, char* buffer) {
    if (buffer == NULL) {
        return;
    }

    if (minutes < 0 || minutes > 1439) {
        strcpy(buffer, "00:00");
        return;
    }

    int hour = minutes / 60;
    int minute = minutes % 60;

    sprintf(buffer, "%02d:%02d", hour, minute);
}

