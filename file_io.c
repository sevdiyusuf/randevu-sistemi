/**
 * Dosya G/Ç İşlemleri Uygulaması
 * 
 * CSV dosyalarından veri yükleme ve kaydetme işlemleri
 * Sadece stdio.h fonksiyonları kullanılmıştır
 * 
 * CSV Formatı:
 * - patients.csv: id,name,age
 * - doctors.csv: id,name,department
 * - appointments.csv: appointment_id,patient_id,doctor_id,start_time,end_time
 * 
 * Zaman Formatı: HH:MM (örn: "09:30", "14:45")
 */

#include "file_io.h"

/**
 * Yardımcı Fonksiyon: CSV satırından alanları ayırır
 * Virgülle ayrılmış değerleri parse eder
 * 
 * @param line: Okunan satır
 * @param fields: Alanları saklamak için dizi (en fazla max_fields)
 * @param max_fields: Maksimum alan sayısı
 * @return: Parse edilen alan sayısı
 */
static int parse_csv_line(char* line, char fields[][200], int max_fields) {
    if (line == NULL || max_fields <= 0) {
        return 0;
    }

    int field_count = 0;
    int i = 0;
    int field_start = 0;
    int in_quotes = 0;

    // Satırın sonundaki newline karakterini kaldır
    int len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') {
        line[len - 1] = '\0';
        len--;
    }
    if (len > 0 && line[len - 1] == '\r') {
        line[len - 1] = '\0';
    }

    // Boş satır kontrolü
    if (len == 0) {
        return 0;
    }

    // CSV satırını parse et
    while (i <= len && field_count < max_fields) {
        if (line[i] == '"') {
            in_quotes = !in_quotes;
        } else if ((line[i] == ',' || line[i] == '\0') && !in_quotes) {
            // Alan sonu bulundu
            int field_len = i - field_start;
            if (field_len > 0 && field_len < 200) {
                strncpy(fields[field_count], line + field_start, field_len);
                fields[field_count][field_len] = '\0';
                
                // Başındaki ve sonundaki boşlukları temizle
                // Trim işlemi
                char* start = fields[field_count];
                char* end = start + strlen(start) - 1;
                while (*start == ' ' || *start == '\t') start++;
                while (end > start && (*end == ' ' || *end == '\t')) end--;
                *(end + 1) = '\0';
                
                if (start != fields[field_count]) {
                    memmove(fields[field_count], start, strlen(start) + 1);
                }
                
                field_count++;
            }
            field_start = i + 1;
        }
        i++;
    }

    return field_count;
}

/**
 * Zaman string'ini (HH:MM) gece yarısından itibaren dakikaya çevirir
 * 
 * @param time_str: "HH:MM" formatında zaman string'i
 * @return: Gece yarısından itibaren dakika sayısı, hata durumunda -1
 */
int parse_time_string(const char* time_str) {
    if (time_str == NULL) {
        return -1;
    }

    int hour, minute;
    if (sscanf(time_str, "%d:%d", &hour, &minute) != 2) {
        return -1; // Parse hatası
    }

    // Geçerlilik kontrolü
    if (hour < 0 || hour > 23 || minute < 0 || minute > 59) {
        return -1; // Geçersiz saat veya dakika
    }

    return hour * 60 + minute; // Gece yarısından itibaren dakika
}

/**
 * Gece yarısından itibaren dakikayı "HH:MM" formatına çevirir
 * 
 * @param minutes: Gece yarısından itibaren dakika (0-1439)
 * @param buffer: Formatlanmış string'i saklamak için buffer (en az 6 karakter)
 */
void format_time_string(int minutes, char* buffer) {
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

/**
 * CSV dosyasından hastaları yükler
 * 
 * CSV Formatı: id,name,age
 * İlk satır header olabilir (otomatik atlanır)
 * 
 * @param filename: Hastalar CSV dosyasının yolu
 * @param patients: Yüklenen hastaları saklamak için dizi (önceden ayrılmış olmalı)
 * @param max_count: Yüklenecek maksimum hasta sayısı
 * @return: Başarıyla yüklenen hasta sayısı
 */
int load_patients_from_csv(const char* filename, Patient** patients, int max_count) {
    if (filename == NULL || patients == NULL || max_count <= 0) {
        return 0;
    }

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        // Dosya yoksa veya açılamazsa, boş dizi döndür (hata değil)
        return 0;
    }

    char line[500];
    int count = 0;
    int is_first_line = 1;
    char fields[3][200];

    // Dosyayı satır satır oku
    while (fgets(line, sizeof(line), file) != NULL && count < max_count) {
        // İlk satırı atla (header olabilir)
        if (is_first_line) {
            // Header kontrolü: "id" veya sayısal değer içeriyor mu?
            if (strstr(line, "id") != NULL || strstr(line, "ID") != NULL) {
                is_first_line = 0;
                continue; // Header satırını atla
            }
            is_first_line = 0;
        }

        // CSV satırını parse et
        int field_count = parse_csv_line(line, fields, 3);
        if (field_count < 3) {
            continue; // Geçersiz satır, atla
        }

        // Alanları dönüştür ve hasta oluştur
        int id = atoi(fields[0]);
        int age = atoi(fields[2]);

        // Geçerlilik kontrolü
        if (id <= 0 || age < 0 || age > 150) {
            continue; // Geçersiz veri, atla
        }

        // Hasta oluştur
        Patient* patient = (Patient*)malloc(sizeof(Patient));
        if (patient == NULL) {
            continue; // Bellek hatası, atla
        }

        patient->id = id;
        strncpy(patient->name, fields[1], sizeof(patient->name) - 1);
        patient->name[sizeof(patient->name) - 1] = '\0';
        patient->age = age;

        patients[count] = patient;
        count++;
    }

    fclose(file);
    return count;
}

/**
 * Hastaları CSV dosyasına kaydeder
 * 
 * CSV Formatı: id,name,age
 * Header satırı yazılır
 * 
 * @param filename: Hastalar CSV dosyasının yolu
 * @param patients: Kaydedilecek hasta işaretçileri dizisi
 * @param count: Kaydedilecek hasta sayısı
 * @return: Başarıyla kaydedilen hasta sayısı
 */
int save_patients_to_csv(const char* filename, Patient** patients, int count) {
    if (filename == NULL || patients == NULL || count < 0) {
        return 0;
    }

    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        return 0; // Dosya açılamadı
    }

    // Header satırını yaz
    fprintf(file, "id,name,age\n");

    // Hastaları yaz
    int saved = 0;
    for (int i = 0; i < count; i++) {
        if (patients[i] != NULL) {
            fprintf(file, "%d,%s,%d\n", 
                   patients[i]->id, 
                   patients[i]->name, 
                   patients[i]->age);
            saved++;
        }
    }

    fclose(file);
    return saved;
}

/**
 * CSV dosyasından doktorları yükler
 * 
 * CSV Formatı: id,name,department
 * İlk satır header olabilir (otomatik atlanır)
 * 
 * @param filename: Doktorlar CSV dosyasının yolu
 * @param doctors: Yüklenen doktorları saklamak için dizi (önceden ayrılmış olmalı)
 * @param max_count: Yüklenecek maksimum doktor sayısı
 * @return: Başarıyla yüklenen doktor sayısı
 */
int load_doctors_from_csv(const char* filename, Doctor** doctors, int max_count) {
    if (filename == NULL || doctors == NULL || max_count <= 0) {
        return 0;
    }

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        // Dosya yoksa veya açılamazsa, boş dizi döndür (hata değil)
        return 0;
    }

    char line[500];
    int count = 0;
    int is_first_line = 1;
    char fields[3][200];

    // Dosyayı satır satır oku
    while (fgets(line, sizeof(line), file) != NULL && count < max_count) {
        // İlk satırı atla (header olabilir)
        if (is_first_line) {
            if (strstr(line, "id") != NULL || strstr(line, "ID") != NULL) {
                is_first_line = 0;
                continue; // Header satırını atla
            }
            is_first_line = 0;
        }

        // CSV satırını parse et
        int field_count = parse_csv_line(line, fields, 3);
        if (field_count < 3) {
            continue; // Geçersiz satır, atla
        }

        // Alanları dönüştür ve doktor oluştur
        int id = atoi(fields[0]);

        // Geçerlilik kontrolü
        if (id <= 0) {
            continue; // Geçersiz veri, atla
        }

        // Doktor oluştur
        Doctor* doctor = (Doctor*)malloc(sizeof(Doctor));
        if (doctor == NULL) {
            continue; // Bellek hatası, atla
        }

        doctor->id = id;
        strncpy(doctor->name, fields[1], sizeof(doctor->name) - 1);
        doctor->name[sizeof(doctor->name) - 1] = '\0';
        strncpy(doctor->department, fields[2], sizeof(doctor->department) - 1);
        doctor->department[sizeof(doctor->department) - 1] = '\0';

        doctors[count] = doctor;
        count++;
    }

    fclose(file);
    return count;
}

/**
 * Doktorları CSV dosyasına kaydeder
 * 
 * CSV Formatı: id,name,department
 * Header satırı yazılır
 * 
 * @param filename: Doktorlar CSV dosyasının yolu
 * @param doctors: Kaydedilecek doktor işaretçileri dizisi
 * @param count: Kaydedilecek doktor sayısı
 * @return: Başarıyla kaydedilen doktor sayısı
 */
int save_doctors_to_csv(const char* filename, Doctor** doctors, int count) {
    if (filename == NULL || doctors == NULL || count < 0) {
        return 0;
    }

    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        return 0; // Dosya açılamadı
    }

    // Header satırını yaz
    fprintf(file, "id,name,department\n");

    // Doktorları yaz
    int saved = 0;
    for (int i = 0; i < count; i++) {
        if (doctors[i] != NULL) {
            fprintf(file, "%d,%s,%s\n", 
                   doctors[i]->id, 
                   doctors[i]->name, 
                   doctors[i]->department);
            saved++;
        }
    }

    fclose(file);
    return saved;
}

/**
 * CSV dosyasından randevuları yükler
 * 
 * CSV Formatı: appointment_id,patient_id,doctor_id,start_time,end_time
 * Zaman formatı: HH:MM (örn: "09:30")
 * İlk satır header olabilir (otomatik atlanır)
 * 
 * @param filename: Randevular CSV dosyasının yolu
 * @param appointments: Yüklenen randevuları saklamak için dizi (önceden ayrılmış olmalı)
 * @param max_count: Yüklenecek maksimum randevu sayısı
 * @return: Başarıyla yüklenen randevu sayısı
 */
int load_appointments_from_csv(const char* filename, Appointment** appointments, int max_count) {
    if (filename == NULL || appointments == NULL || max_count <= 0) {
        return 0;
    }

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        // Dosya yoksa veya açılamazsa, boş dizi döndür (hata değil)
        return 0;
    }

    char line[500];
    int count = 0;
    int is_first_line = 1;
    char fields[5][200];

    // Dosyayı satır satır oku
    while (fgets(line, sizeof(line), file) != NULL && count < max_count) {
        // İlk satırı atla (header olabilir)
        if (is_first_line) {
            if (strstr(line, "appointment_id") != NULL || strstr(line, "id") != NULL) {
                is_first_line = 0;
                continue; // Header satırını atla
            }
            is_first_line = 0;
        }

        // CSV satırını parse et
        int field_count = parse_csv_line(line, fields, 5);
        if (field_count < 5) {
            continue; // Geçersiz satır, atla
        }

        // Alanları dönüştür
        int appointment_id = atoi(fields[0]);
        int patient_id = atoi(fields[1]);
        int doctor_id = atoi(fields[2]);
        int start_time = parse_time_string(fields[3]);
        int end_time = parse_time_string(fields[4]);

        // Geçerlilik kontrolü
        if (appointment_id <= 0 || patient_id <= 0 || doctor_id <= 0) {
            continue; // Geçersiz ID'ler
        }

        if (start_time < 0 || end_time < 0 || start_time >= end_time) {
            continue; // Geçersiz zaman aralığı
        }

        if (start_time < 0 || start_time > 1439 || end_time < 0 || end_time > 1439) {
            continue; // Zaman aralığı dışında
        }

        // Randevu oluştur
        Appointment* appointment = (Appointment*)malloc(sizeof(Appointment));
        if (appointment == NULL) {
            continue; // Bellek hatası, atla
        }

        appointment->appointment_id = appointment_id;
        appointment->patient_id = patient_id;
        appointment->doctor_id = doctor_id;
        appointment->start_time = start_time;
        appointment->end_time = end_time;

        appointments[count] = appointment;
        count++;
    }

    fclose(file);
    return count;
}

/**
 * Randevuları CSV dosyasına kaydeder
 * 
 * CSV Formatı: appointment_id,patient_id,doctor_id,start_time,end_time
 * Zaman formatı: HH:MM (örn: "09:30")
 * Header satırı yazılır
 * 
 * @param filename: Randevular CSV dosyasının yolu
 * @param appointments: Kaydedilecek randevu işaretçileri dizisi
 * @param count: Kaydedilecek randevu sayısı
 * @return: Başarıyla kaydedilen randevu sayısı
 */
int save_appointments_to_csv(const char* filename, Appointment** appointments, int count) {
    if (filename == NULL || appointments == NULL || count < 0) {
        return 0;
    }

    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        return 0; // Dosya açılamadı
    }

    // Header satırını yaz
    fprintf(file, "appointment_id,patient_id,doctor_id,start_time,end_time\n");

    // Randevuları yaz
    int saved = 0;
    char start_time_str[10];
    char end_time_str[10];

    for (int i = 0; i < count; i++) {
        if (appointments[i] != NULL) {
            // Zamanları string formatına çevir
            format_time_string(appointments[i]->start_time, start_time_str);
            format_time_string(appointments[i]->end_time, end_time_str);

            fprintf(file, "%d,%d,%d,%s,%s\n", 
                   appointments[i]->appointment_id,
                   appointments[i]->patient_id,
                   appointments[i]->doctor_id,
                   start_time_str,
                   end_time_str);
            saved++;
        }
    }

    fclose(file);
    return saved;
}

/**
 * Automatic ID Generation Implementation
 * Static counters for sequential ID generation
 */

// Static ID counters (initialized to 0, will be updated when loading CSV)
static int next_patient_id = 0;
static int next_doctor_id = 0;
static int next_appointment_id = 0;

/**
 * Gets the next available patient ID and increments the counter
 * @return: Next patient ID (starts from 1)
 */
int get_next_patient_id(void) {
    next_patient_id++;
    return next_patient_id;
}

/**
 * Gets the next available doctor ID and increments the counter
 * @return: Next doctor ID (starts from 1)
 */
int get_next_doctor_id(void) {
    next_doctor_id++;
    return next_doctor_id;
}

/**
 * Gets the next available appointment ID and increments the counter
 * @return: Next appointment ID (starts from 1)
 */
int get_next_appointment_id(void) {
    next_appointment_id++;
    return next_appointment_id;
}

/**
 * Updates patient ID counter based on existing patients
 * Finds the maximum ID and sets counter to max + 1
 * @param patients: Array of patient pointers
 * @param count: Number of patients
 */
void update_patient_id_counter(Patient** patients, int count) {
    int max_id = 0;
    for (int i = 0; i < count; i++) {
        if (patients[i] != NULL && patients[i]->id > max_id) {
            max_id = patients[i]->id;
        }
    }
    next_patient_id = max_id; // Next ID will be max_id + 1
}

/**
 * Updates doctor ID counter based on existing doctors
 * Finds the maximum ID and sets counter to max + 1
 * @param doctors: Array of doctor pointers
 * @param count: Number of doctors
 */
void update_doctor_id_counter(Doctor** doctors, int count) {
    int max_id = 0;
    for (int i = 0; i < count; i++) {
        if (doctors[i] != NULL && doctors[i]->id > max_id) {
            max_id = doctors[i]->id;
        }
    }
    next_doctor_id = max_id; // Next ID will be max_id + 1
}

/**
 * Updates appointment ID counter based on existing appointments
 * Finds the maximum ID and sets counter to max + 1
 * @param appointments: Array of appointment pointers
 * @param count: Number of appointments
 */
void update_appointment_id_counter(Appointment** appointments, int count) {
    int max_id = 0;
    for (int i = 0; i < count; i++) {
        if (appointments[i] != NULL && appointments[i]->appointment_id > max_id) {
            max_id = appointments[i]->appointment_id;
        }
    }
    next_appointment_id = max_id; // Next ID will be max_id + 1
}

