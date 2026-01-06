/**
 * GUI Module Implementation
 * 
 * GTK3-based graphical user interface for Hospital Appointment Management System
 * 
 * This module provides a GUI layer that calls existing core functions.
 * It does NOT contain data structure logic - that remains in core modules.
 */

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gui.h"
#include "patient.h"
#include "doctor.h"
#include "appointment.h"
#include "file_io.h"

/**
 * Static application data
 * This allows callback functions to access core data structures
 */
static AppData* app_data = NULL;

/**
 * Callback function for window close event
 * 
 * @param widget: The widget that triggered the event
 * @param data: User data (unused)
 * @return: FALSE to allow default behavior
 */
static gboolean on_window_close(GtkWidget *widget, gpointer data) {
    (void)widget;
    (void)data;
    
    // Exit GTK main loop
    gtk_main_quit();
    return FALSE;
}

/**
 * Button callback functions
 * These functions call existing core logic functions.
 */

/**
 * Callback for "Cancel" button in Add Patient dialog
 */
static void on_add_patient_cancel(GtkWidget *widget, gpointer data) {
    (void)widget;
    GtkWidget *dialog = GTK_WIDGET(data);
    
    // Get entries array from dialog data and free it
    GtkWidget **entries = (GtkWidget**)g_object_get_data(G_OBJECT(dialog), "entries_array");
    if (entries != NULL) {
        free(entries);
    }
    
    // Destroy dialog
    gtk_widget_destroy(dialog);
}

/**
 * Callback for "Save" button in Add Patient dialog
 */
static void on_add_patient_save(GtkWidget *widget, gpointer data) {
    (void)widget;
    
    if (app_data == NULL || app_data->patients == NULL || app_data->patient_count == NULL) {
        GtkWidget *error_dialog = gtk_message_dialog_new(
            NULL,
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Hata: Uygulama verileri baslatilmadi."
        );
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
        return;
    }
    
    // Get entry widgets from user data
    GtkWidget **entries = (GtkWidget**)data;
    GtkWidget *id_entry = entries[0];
    GtkWidget *name_entry = entries[1];
    GtkWidget *age_entry = entries[2];
    GtkWidget *dialog = entries[3];
    
    // Get values from entries
    const char *id_str = gtk_entry_get_text(GTK_ENTRY(id_entry));
    const char *name_str = gtk_entry_get_text(GTK_ENTRY(name_entry));
    const char *age_str = gtk_entry_get_text(GTK_ENTRY(age_entry));
    
    // Validate input (ID is auto-generated, so we only check name and age)
    if (name_str == NULL || strlen(name_str) == 0 ||
        age_str == NULL || strlen(age_str) == 0) {
        GtkWidget *error_dialog = gtk_message_dialog_new(
            GTK_WINDOW(dialog),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Hata: Tum alanlar doldurulmalidir."
        );
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
        return;
    }
    
    // Convert strings to integers (ID is already generated, just read it)
    int id = atoi(id_str);
    int age = atoi(age_str);
    
    // Validate age (ID is auto-generated, so no need to validate it)
    
    if (age < 0 || age > 150) {
        GtkWidget *error_dialog = gtk_message_dialog_new(
            GTK_WINDOW(dialog),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Hata: Gecersiz yas. 0 ile 150 arasinda olmalidir."
        );
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
        return;
    }
    
    // ID is auto-generated, so no need to check for duplicates
    
    // Check maximum capacity
    if (*(app_data->patient_count) >= MAX_RECORDS) {
        GtkWidget *error_dialog = gtk_message_dialog_new(
            GTK_WINDOW(dialog),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Hata: Maksimum hasta kapasitesine ulasildi."
        );
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
        return;
    }
    
    // Create patient using existing core function
    Patient* patient = create_patient(id, name_str, age);
    if (patient == NULL) {
        GtkWidget *error_dialog = gtk_message_dialog_new(
            GTK_WINDOW(dialog),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Hata: Hasta olusturulamadi."
        );
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
        return;
    }
    
    // Add patient to array
    app_data->patients[*(app_data->patient_count)] = patient;
    (*(app_data->patient_count))++;
    
    // Show success message
    GtkWidget *success_dialog = gtk_message_dialog_new(
        GTK_WINDOW(dialog),
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_INFO,
        GTK_BUTTONS_OK,
        "Hasta basariyla eklendi!"
    );
    gtk_dialog_run(GTK_DIALOG(success_dialog));
    gtk_widget_destroy(success_dialog);
    
    // Free entries array and remove from dialog data
    free(entries);
    g_object_set_data(G_OBJECT(dialog), "entries_array", NULL);
    
    // Close dialog
    gtk_widget_destroy(dialog);
}

/**
 * Creates and shows the Add Patient dialog window
 */
static void show_add_patient_dialog(GtkWidget *parent) {
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWidget *grid;
    GtkWidget *label;
    GtkWidget *id_entry;
    GtkWidget *name_entry;
    GtkWidget *age_entry;
    GtkWidget *save_button;
    GtkWidget *cancel_button;
    GtkWidget *button_box;
    
    // Create dialog
    dialog = gtk_dialog_new_with_buttons(
        "Hasta Ekle",
        GTK_WINDOW(parent),
        GTK_DIALOG_MODAL,
        NULL,
        NULL
    );
    
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    
    // Create grid for form fields
    grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 20);
    gtk_container_add(GTK_CONTAINER(content_area), grid);
    
    // Patient ID field (auto-generated, read-only)
    label = gtk_label_new("Hasta ID:");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);
    id_entry = gtk_entry_new();
    // Generate and display next ID
    int next_id = get_next_patient_id();
    char id_str[20];
    snprintf(id_str, sizeof(id_str), "%d", next_id);
    gtk_entry_set_text(GTK_ENTRY(id_entry), id_str);
    // Make read-only
    gtk_editable_set_editable(GTK_EDITABLE(id_entry), FALSE);
    gtk_widget_set_sensitive(id_entry, FALSE);
    gtk_entry_set_placeholder_text(GTK_ENTRY(id_entry), "Otomatik olusturuldu");
    gtk_grid_attach(GTK_GRID(grid), id_entry, 1, 0, 1, 1);
    
    // Name field
    label = gtk_label_new("Ad:");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 1, 1, 1);
    name_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(name_entry), "Hasta adini giriniz");
    gtk_grid_attach(GTK_GRID(grid), name_entry, 1, 1, 1, 1);
    
    // Age field
    label = gtk_label_new("Yas:");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);
    age_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(age_entry), "Yas giriniz (0-150)");
    gtk_grid_attach(GTK_GRID(grid), age_entry, 1, 2, 1, 1);
    
    // Create button box
    button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(button_box), GTK_BUTTONBOX_END);
    gtk_box_set_spacing(GTK_BOX(button_box), 10);
    gtk_container_set_border_width(GTK_CONTAINER(button_box), 10);
    gtk_grid_attach(GTK_GRID(grid), button_box, 0, 3, 2, 1);
    
    // Store entry widgets in array for callback access (must be done before connecting signals)
    GtkWidget **entries = (GtkWidget**)malloc(4 * sizeof(GtkWidget*));
    entries[0] = id_entry;
    entries[1] = name_entry;
    entries[2] = age_entry;
    entries[3] = dialog;
    
    // Store entries array in dialog object data for cleanup
    g_object_set_data(G_OBJECT(dialog), "entries_array", entries);
    
    // Save button
    save_button = gtk_button_new_with_label("Kaydet");
    g_signal_connect(save_button, "clicked", G_CALLBACK(on_add_patient_save), entries);
    gtk_container_add(GTK_CONTAINER(button_box), save_button);
    
    // Cancel button
    cancel_button = gtk_button_new_with_label("Iptal");
    g_signal_connect(cancel_button, "clicked", G_CALLBACK(on_add_patient_cancel), dialog);
    gtk_container_add(GTK_CONTAINER(button_box), cancel_button);
    
    // Show dialog
    gtk_widget_show_all(dialog);
}

/**
 * Callback for "Add Patient" button
 */
static void on_add_patient_clicked(GtkWidget *widget, gpointer data) {
    (void)widget;
    GtkWidget *parent = GTK_WIDGET(data);
    show_add_patient_dialog(parent);
}

/**
 * Callback for "Cancel" button in Add Doctor dialog
 */
static void on_add_doctor_cancel(GtkWidget *widget, gpointer data) {
    (void)widget;
    GtkWidget *dialog = GTK_WIDGET(data);
    
    // Get entries array from dialog data and free it
    GtkWidget **entries = (GtkWidget**)g_object_get_data(G_OBJECT(dialog), "entries_array");
    if (entries != NULL) {
        free(entries);
    }
    
    // Destroy dialog
    gtk_widget_destroy(dialog);
}

/**
 * Callback for "Save" button in Add Doctor dialog
 */
static void on_add_doctor_save(GtkWidget *widget, gpointer data) {
    (void)widget;
    
    if (app_data == NULL || app_data->doctors == NULL || app_data->doctor_count == NULL) {
        GtkWidget *error_dialog = gtk_message_dialog_new(
            NULL,
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Hata: Uygulama verileri baslatilmadi."
        );
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
        return;
    }
    
    // Get entry widgets from user data
    GtkWidget **entries = (GtkWidget**)data;
    GtkWidget *id_entry = entries[0];
    GtkWidget *name_entry = entries[1];
    GtkWidget *department_entry = entries[2];
    GtkWidget *dialog = entries[3];
    
    // Get values from entries
    const char *id_str = gtk_entry_get_text(GTK_ENTRY(id_entry));
    const char *name_str = gtk_entry_get_text(GTK_ENTRY(name_entry));
    const char *department_str = gtk_entry_get_text(GTK_ENTRY(department_entry));
    
    // Validate input (ID is auto-generated, so we only check name and department)
    if (name_str == NULL || strlen(name_str) == 0 ||
        department_str == NULL || strlen(department_str) == 0) {
        GtkWidget *error_dialog = gtk_message_dialog_new(
            GTK_WINDOW(dialog),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Hata: Tum alanlar doldurulmalidir."
        );
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
        return;
    }
    
    // Convert string to integer (ID is already generated, just read it)
    int id = atoi(id_str);
    
    // ID is auto-generated, so no need to validate or check for duplicates
    
    // Check maximum capacity
    if (*(app_data->doctor_count) >= MAX_RECORDS) {
        GtkWidget *error_dialog = gtk_message_dialog_new(
            GTK_WINDOW(dialog),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Hata: Maksimum doktor kapasitesine ulasildi."
        );
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
        return;
    }
    
    // Create doctor using existing core function
    Doctor* doctor = create_doctor(id, name_str, department_str);
    if (doctor == NULL) {
        GtkWidget *error_dialog = gtk_message_dialog_new(
            GTK_WINDOW(dialog),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Hata: Doktor olusturulamadi."
        );
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
        return;
    }
    
    // Add doctor to array
    app_data->doctors[*(app_data->doctor_count)] = doctor;
    (*(app_data->doctor_count))++;
    
    // Show success message
    GtkWidget *success_dialog = gtk_message_dialog_new(
        GTK_WINDOW(dialog),
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_INFO,
        GTK_BUTTONS_OK,
        "Doktor basariyla eklendi!"
    );
    gtk_dialog_run(GTK_DIALOG(success_dialog));
    gtk_widget_destroy(success_dialog);
    
    // Free entries array and remove from dialog data
    free(entries);
    g_object_set_data(G_OBJECT(dialog), "entries_array", NULL);
    
    // Close dialog
    gtk_widget_destroy(dialog);
}

/**
 * Creates and shows the Add Doctor dialog window
 */
static void show_add_doctor_dialog(GtkWidget *parent) {
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWidget *grid;
    GtkWidget *label;
    GtkWidget *id_entry;
    GtkWidget *name_entry;
    GtkWidget *department_entry;
    GtkWidget *save_button;
    GtkWidget *cancel_button;
    GtkWidget *button_box;
    
    // Create dialog
    dialog = gtk_dialog_new_with_buttons(
        "Doktor Ekle",
        GTK_WINDOW(parent),
        GTK_DIALOG_MODAL,
        NULL,
        NULL
    );
    
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    
    // Create grid for form fields
    grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 20);
    gtk_container_add(GTK_CONTAINER(content_area), grid);
    
    // Doctor ID field (auto-generated, read-only)
    label = gtk_label_new("Doktor ID:");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);
    id_entry = gtk_entry_new();
    // Generate and display next ID
    int next_id = get_next_doctor_id();
    char id_str[20];
    snprintf(id_str, sizeof(id_str), "%d", next_id);
    gtk_entry_set_text(GTK_ENTRY(id_entry), id_str);
    // Make read-only
    gtk_editable_set_editable(GTK_EDITABLE(id_entry), FALSE);
    gtk_widget_set_sensitive(id_entry, FALSE);
    gtk_entry_set_placeholder_text(GTK_ENTRY(id_entry), "Otomatik olusturuldu");
    gtk_grid_attach(GTK_GRID(grid), id_entry, 1, 0, 1, 1);
    
    // Name field
    label = gtk_label_new("Ad:");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 1, 1, 1);
    name_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(name_entry), "Doktor adini giriniz");
    gtk_grid_attach(GTK_GRID(grid), name_entry, 1, 1, 1, 1);
    
    // Department field
    label = gtk_label_new("Bolum:");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);
    department_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(department_entry), "Bolum adini giriniz");
    gtk_grid_attach(GTK_GRID(grid), department_entry, 1, 2, 1, 1);
    
    // Create button box
    button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(button_box), GTK_BUTTONBOX_END);
    gtk_box_set_spacing(GTK_BOX(button_box), 10);
    gtk_container_set_border_width(GTK_CONTAINER(button_box), 10);
    gtk_grid_attach(GTK_GRID(grid), button_box, 0, 3, 2, 1);
    
    // Store entry widgets in array for callback access (must be done before connecting signals)
    GtkWidget **entries = (GtkWidget**)malloc(4 * sizeof(GtkWidget*));
    entries[0] = id_entry;
    entries[1] = name_entry;
    entries[2] = department_entry;
    entries[3] = dialog;
    
    // Store entries array in dialog object data for cleanup
    g_object_set_data(G_OBJECT(dialog), "entries_array", entries);
    
    // Save button
    save_button = gtk_button_new_with_label("Kaydet");
    g_signal_connect(save_button, "clicked", G_CALLBACK(on_add_doctor_save), entries);
    gtk_container_add(GTK_CONTAINER(button_box), save_button);
    
    // Cancel button
    cancel_button = gtk_button_new_with_label("Iptal");
    g_signal_connect(cancel_button, "clicked", G_CALLBACK(on_add_doctor_cancel), dialog);
    gtk_container_add(GTK_CONTAINER(button_box), cancel_button);
    
    // Show dialog
    gtk_widget_show_all(dialog);
}

/**
 * Callback for "Add Doctor" button
 */
static void on_add_doctor_clicked(GtkWidget *widget, gpointer data) {
    (void)widget;
    GtkWidget *parent = GTK_WIDGET(data);
    show_add_doctor_dialog(parent);
}

/**
 * Callback for "Cancel" button in Create Appointment dialog
 */
static void on_create_appointment_cancel(GtkWidget *widget, gpointer data) {
    (void)widget;
    GtkWidget *dialog = GTK_WIDGET(data);
    
    // Get entries array from dialog data and free it
    GtkWidget **entries = (GtkWidget**)g_object_get_data(G_OBJECT(dialog), "entries_array");
    if (entries != NULL) {
        free(entries);
    }
    
    // Destroy dialog
    gtk_widget_destroy(dialog);
}

/**
 * Callback for "Save" button in Create Appointment dialog
 */
static void on_create_appointment_save(GtkWidget *widget, gpointer data) {
    (void)widget;
    
    if (app_data == NULL || app_data->appointments == NULL || app_data->appointment_count == NULL ||
        app_data->avl_tree == NULL || app_data->heap == NULL || app_data->queue == NULL ||
        app_data->patients == NULL || app_data->doctors == NULL) {
        GtkWidget *error_dialog = gtk_message_dialog_new(
            NULL,
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Hata: Uygulama verileri baslatilmadi."
        );
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
        return;
    }
    
    // Get entry widgets from user data
    GtkWidget **entries = (GtkWidget**)data;
    GtkWidget *appointment_id_entry = entries[0];
    GtkWidget *patient_id_entry = entries[1];
    GtkWidget *doctor_id_entry = entries[2];
    GtkWidget *start_time_entry = entries[3];
    GtkWidget *end_time_entry = entries[4];
    GtkWidget *dialog = entries[5];
    
    // Get values from entries
    const char *appointment_id_str = gtk_entry_get_text(GTK_ENTRY(appointment_id_entry));
    const char *patient_id_str = gtk_entry_get_text(GTK_ENTRY(patient_id_entry));
    const char *doctor_id_str = gtk_entry_get_text(GTK_ENTRY(doctor_id_entry));
    const char *start_time_str = gtk_entry_get_text(GTK_ENTRY(start_time_entry));
    const char *end_time_str = gtk_entry_get_text(GTK_ENTRY(end_time_entry));
    
    // Validate input (appointment ID is auto-generated, so we only check other fields)
    if (patient_id_str == NULL || strlen(patient_id_str) == 0 ||
        doctor_id_str == NULL || strlen(doctor_id_str) == 0 ||
        start_time_str == NULL || strlen(start_time_str) == 0 ||
        end_time_str == NULL || strlen(end_time_str) == 0) {
        GtkWidget *error_dialog = gtk_message_dialog_new(
            GTK_WINDOW(dialog),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Hata: Tum alanlar doldurulmalidir."
        );
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
        return;
    }
    
    // Convert strings to integers (appointment ID is already generated, just read it)
    int appointment_id = atoi(appointment_id_str);
    int patient_id = atoi(patient_id_str);
    int doctor_id = atoi(doctor_id_str);
    
    // Validate patient and doctor IDs (appointment ID is auto-generated, no need to validate)
    if (patient_id <= 0 || doctor_id <= 0) {
        GtkWidget *error_dialog = gtk_message_dialog_new(
            GTK_WINDOW(dialog),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Hata: Gecersiz Hasta ID veya Doktor ID."
        );
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
        return;
    }
    
    // Appointment ID is auto-generated, so no need to check for duplicates
    
    // Check if patient exists
    int patient_found = 0;
    for (int i = 0; i < *(app_data->patient_count); i++) {
        if (app_data->patients[i] != NULL && app_data->patients[i]->id == patient_id) {
            patient_found = 1;
            break;
        }
    }
    
    if (!patient_found) {
        GtkWidget *error_dialog = gtk_message_dialog_new(
            GTK_WINDOW(dialog),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Hata: Hasta bulunamadi. Lutfen once hasta ekleyin."
        );
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
        return;
    }
    
    // Check if doctor exists
    int doctor_found = 0;
    for (int i = 0; i < *(app_data->doctor_count); i++) {
        if (app_data->doctors[i] != NULL && app_data->doctors[i]->id == doctor_id) {
            doctor_found = 1;
            break;
        }
    }
    
    if (!doctor_found) {
        GtkWidget *error_dialog = gtk_message_dialog_new(
            GTK_WINDOW(dialog),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Hata: Doktor bulunamadi. Lutfen once doktor ekleyin."
        );
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
        return;
    }
    
    // Parse time strings
    int start_time = parse_time_string(start_time_str);
    int end_time = parse_time_string(end_time_str);
    
    if (start_time < 0 || end_time < 0) {
        GtkWidget *error_dialog = gtk_message_dialog_new(
            GTK_WINDOW(dialog),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Hata: Gecersiz zaman formati. Lutfen SS:DD formatini kullanin (ornek: 09:30)."
        );
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
        return;
    }
    
    if (start_time >= end_time) {
        GtkWidget *error_dialog = gtk_message_dialog_new(
            GTK_WINDOW(dialog),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Hata: Baslangic zamani bitis zamanindan once olmalidir."
        );
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
        return;
    }
    
    // Check maximum capacity
    if (*(app_data->appointment_count) >= MAX_RECORDS) {
        GtkWidget *error_dialog = gtk_message_dialog_new(
            GTK_WINDOW(dialog),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Hata: Maksimum randevu kapasitesine ulasildi."
        );
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
        return;
    }
    
    // Create appointment using existing core function
    Appointment* appointment = create_appointment(appointment_id, patient_id, 
                                                  doctor_id, start_time, end_time);
    if (appointment == NULL) {
        GtkWidget *error_dialog = gtk_message_dialog_new(
            GTK_WINDOW(dialog),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Error: Failed to create appointment."
        );
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
        return;
    }
    
    // Check for time conflict using AVL tree
    if (avl_has_conflict(app_data->avl_tree, doctor_id, start_time, end_time)) {
        // Conflict detected - add to waiting queue
        if (queue_enqueue(app_data->queue, appointment)) {
            GtkWidget *warning_dialog = gtk_message_dialog_new(
                GTK_WINDOW(dialog),
                GTK_DIALOG_MODAL,
                GTK_MESSAGE_WARNING,
                GTK_BUTTONS_OK,
                "Uyari: Zaman cakismasi tespit edildi. Doktorun bu saatte baska bir randevusu var.\n\nRandevu bekleme listesine eklendi."
            );
            gtk_dialog_run(GTK_DIALOG(warning_dialog));
            gtk_widget_destroy(warning_dialog);
        } else {
            GtkWidget *error_dialog = gtk_message_dialog_new(
                GTK_WINDOW(dialog),
                GTK_DIALOG_MODAL,
                GTK_MESSAGE_ERROR,
                GTK_BUTTONS_OK,
                "Hata: Randevu bekleme listesine eklenemedi."
            );
            gtk_dialog_run(GTK_DIALOG(error_dialog));
            gtk_widget_destroy(error_dialog);
            free_appointment(appointment);
        }
        
        // Free entries array and remove from dialog data
        free(entries);
        g_object_set_data(G_OBJECT(dialog), "entries_array", NULL);
        
        // Close dialog
        gtk_widget_destroy(dialog);
        return;
    }
    
    // No conflict - add to AVL tree and heap
    if (avl_insert(app_data->avl_tree, appointment, doctor_id)) {
        if (heap_insert(app_data->heap, appointment)) {
            // Successfully added to both AVL and heap
            app_data->appointments[*(app_data->appointment_count)] = appointment;
            (*(app_data->appointment_count))++;
            
            // Show success message
            char message[200];
            snprintf(message, sizeof(message), 
                    "Randevu basariyla olusturuldu!\n\n"
                    "Randevu ID: %d\n"
                    "Hasta ID: %d\n"
                    "Doktor ID: %d\n"
                    "Time: %s - %s",
                    appointment_id, patient_id, doctor_id,
                    start_time_str, end_time_str);
            
            GtkWidget *success_dialog = gtk_message_dialog_new(
                GTK_WINDOW(dialog),
                GTK_DIALOG_MODAL,
                GTK_MESSAGE_INFO,
                GTK_BUTTONS_OK,
                message
            );
            gtk_dialog_run(GTK_DIALOG(success_dialog));
            gtk_widget_destroy(success_dialog);
        } else {
            // Failed to add to heap - remove from AVL
            avl_delete(app_data->avl_tree, appointment_id);
            free_appointment(appointment);
            
            GtkWidget *error_dialog = gtk_message_dialog_new(
                GTK_WINDOW(dialog),
                GTK_DIALOG_MODAL,
                GTK_MESSAGE_ERROR,
                GTK_BUTTONS_OK,
                "Hata: Randevu heap'e eklenemedi."
            );
            gtk_dialog_run(GTK_DIALOG(error_dialog));
            gtk_widget_destroy(error_dialog);
        }
    } else {
        // Failed to add to AVL
        free_appointment(appointment);
        
        GtkWidget *error_dialog = gtk_message_dialog_new(
            GTK_WINDOW(dialog),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Hata: Randevu AVL agacina eklenemedi."
        );
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
    }
    
    // Free entries array and remove from dialog data
    free(entries);
    g_object_set_data(G_OBJECT(dialog), "entries_array", NULL);
    
    // Close dialog
    gtk_widget_destroy(dialog);
}

/**
 * Creates and shows the Create Appointment dialog window
 */
static void show_create_appointment_dialog(GtkWidget *parent) {
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWidget *grid;
    GtkWidget *label;
    GtkWidget *appointment_id_entry;
    GtkWidget *patient_id_entry;
    GtkWidget *doctor_id_entry;
    GtkWidget *start_time_entry;
    GtkWidget *end_time_entry;
    GtkWidget *save_button;
    GtkWidget *cancel_button;
    GtkWidget *button_box;
    
    // Check if patients and doctors exist
    if (app_data == NULL || app_data->patient_count == NULL || app_data->doctor_count == NULL) {
        GtkWidget *error_dialog = gtk_message_dialog_new(
            GTK_WINDOW(parent),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Hata: Uygulama verileri baslatilmadi."
        );
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
        return;
    }
    
    if (*(app_data->patient_count) == 0) {
        GtkWidget *error_dialog = gtk_message_dialog_new(
            GTK_WINDOW(parent),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_WARNING,
            GTK_BUTTONS_OK,
            "Uyari: Hasta bulunamadi. Lutfen once hasta ekleyin."
        );
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
        return;
    }
    
    if (*(app_data->doctor_count) == 0) {
        GtkWidget *error_dialog = gtk_message_dialog_new(
            GTK_WINDOW(parent),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_WARNING,
            GTK_BUTTONS_OK,
            "Uyari: Doktor bulunamadi. Lutfen once doktor ekleyin."
        );
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
        return;
    }
    
    // Create dialog
    dialog = gtk_dialog_new_with_buttons(
        "Randevu Olustur",
        GTK_WINDOW(parent),
        GTK_DIALOG_MODAL,
        NULL,
        NULL
    );
    
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    
    // Create grid for form fields
    grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 20);
    gtk_container_add(GTK_CONTAINER(content_area), grid);
    
    // Appointment ID field (auto-generated, read-only)
    label = gtk_label_new("Randevu ID:");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);
    appointment_id_entry = gtk_entry_new();
    // Generate and display next ID
    int next_id = get_next_appointment_id();
    char id_str[20];
    snprintf(id_str, sizeof(id_str), "%d", next_id);
    gtk_entry_set_text(GTK_ENTRY(appointment_id_entry), id_str);
    // Make read-only
    gtk_editable_set_editable(GTK_EDITABLE(appointment_id_entry), FALSE);
    gtk_widget_set_sensitive(appointment_id_entry, FALSE);
    gtk_entry_set_placeholder_text(GTK_ENTRY(appointment_id_entry), "Otomatik olusturuldu");
    gtk_grid_attach(GTK_GRID(grid), appointment_id_entry, 1, 0, 1, 1);
    
    // Patient ID field
    label = gtk_label_new("Hasta ID:");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 1, 1, 1);
    patient_id_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(patient_id_entry), "Hasta ID giriniz");
    gtk_grid_attach(GTK_GRID(grid), patient_id_entry, 1, 1, 1, 1);
    
    // Doctor ID field
    label = gtk_label_new("Doktor ID:");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);
    doctor_id_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(doctor_id_entry), "Doktor ID giriniz");
    gtk_grid_attach(GTK_GRID(grid), doctor_id_entry, 1, 2, 1, 1);
    
    // Start Time field
    label = gtk_label_new("Baslangic Zamani (SS:DD):");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 3, 1, 1);
    start_time_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(start_time_entry), "ornek: 09:30");
    gtk_grid_attach(GTK_GRID(grid), start_time_entry, 1, 3, 1, 1);
    
    // End Time field
    label = gtk_label_new("Bitis Zamani (SS:DD):");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 4, 1, 1);
    end_time_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(end_time_entry), "ornek: 10:00");
    gtk_grid_attach(GTK_GRID(grid), end_time_entry, 1, 4, 1, 1);
    
    // Create button box
    button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(button_box), GTK_BUTTONBOX_END);
    gtk_box_set_spacing(GTK_BOX(button_box), 10);
    gtk_container_set_border_width(GTK_CONTAINER(button_box), 10);
    gtk_grid_attach(GTK_GRID(grid), button_box, 0, 5, 2, 1);
    
    // Store entry widgets in array for callback access (must be done before connecting signals)
    GtkWidget **entries = (GtkWidget**)malloc(6 * sizeof(GtkWidget*));
    entries[0] = appointment_id_entry;
    entries[1] = patient_id_entry;
    entries[2] = doctor_id_entry;
    entries[3] = start_time_entry;
    entries[4] = end_time_entry;
    entries[5] = dialog;
    
    // Store entries array in dialog object data for cleanup
    g_object_set_data(G_OBJECT(dialog), "entries_array", entries);
    
    // Save button
    save_button = gtk_button_new_with_label("Kaydet");
    g_signal_connect(save_button, "clicked", G_CALLBACK(on_create_appointment_save), entries);
    gtk_container_add(GTK_CONTAINER(button_box), save_button);
    
    // Cancel button
    cancel_button = gtk_button_new_with_label("Iptal");
    g_signal_connect(cancel_button, "clicked", G_CALLBACK(on_create_appointment_cancel), dialog);
    gtk_container_add(GTK_CONTAINER(button_box), cancel_button);
    
    // Show dialog
    gtk_widget_show_all(dialog);
}

/**
 * Callback for "Create Appointment" button
 */
static void on_create_appointment_clicked(GtkWidget *widget, gpointer data) {
    (void)widget;
    GtkWidget *parent = GTK_WIDGET(data);
    show_create_appointment_dialog(parent);
}

/**
 * Callback for "Cancel Appointment" button
 */
static void on_cancel_appointment_clicked(GtkWidget *widget, gpointer data) {
    (void)widget;
    GtkWidget *parent = GTK_WIDGET(data);

    if (app_data == NULL || app_data->avl_tree == NULL || app_data->heap == NULL ||
        app_data->stack == NULL || app_data->appointments == NULL || app_data->appointment_count == NULL) {
        GtkWidget *error_dialog = gtk_message_dialog_new(
            parent ? GTK_WINDOW(parent) : NULL,
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Hata: Uygulama verileri baslatilmadi."
        );
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
        return;
    }

    if (*(app_data->appointment_count) == 0) {
        GtkWidget *info_dialog = gtk_message_dialog_new(
            parent ? GTK_WINDOW(parent) : NULL,
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_INFO,
            GTK_BUTTONS_OK,
            "Iptal edilecek randevu bulunmuyor."
        );
        gtk_dialog_run(GTK_DIALOG(info_dialog));
        gtk_widget_destroy(info_dialog);
        return;
    }

    // Ask for appointment ID using a simple dialog
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Cancel Appointment",
        parent ? GTK_WINDOW(parent) : NULL,
        GTK_DIALOG_MODAL,
        "_Cancel", GTK_RESPONSE_CANCEL,
        "_OK", GTK_RESPONSE_OK,
        NULL
    );

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(box), 10);
    gtk_container_add(GTK_CONTAINER(content_area), box);

    GtkWidget *label = gtk_label_new("Iptal edilecek Randevu ID giriniz:");
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 5);

    GtkWidget *entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Randevu ID");
    gtk_box_pack_start(GTK_BOX(box), entry, FALSE, FALSE, 5);

    gtk_widget_show_all(dialog);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
        const char *id_str = gtk_entry_get_text(GTK_ENTRY(entry));
        int appointment_id = atoi(id_str);

        if (appointment_id <= 0) {
            GtkWidget *error_dialog = gtk_message_dialog_new(
                GTK_WINDOW(dialog),
                GTK_DIALOG_MODAL,
                GTK_MESSAGE_ERROR,
                GTK_BUTTONS_OK,
                "Hata: Gecersiz Randevu ID."
            );
            gtk_dialog_run(GTK_DIALOG(error_dialog));
            gtk_widget_destroy(error_dialog);
        } else {
            // Delete from AVL tree
            Appointment *appointment = avl_delete(app_data->avl_tree, appointment_id);
            if (appointment == NULL) {
                GtkWidget *error_dialog = gtk_message_dialog_new(
                    GTK_WINDOW(dialog),
                    GTK_DIALOG_MODAL,
                    GTK_MESSAGE_ERROR,
                    GTK_BUTTONS_OK,
                    "Hata: Randevu bulunamadi."
                );
                gtk_dialog_run(GTK_DIALOG(error_dialog));
                gtk_widget_destroy(error_dialog);
            } else {
                // Remove from array
                int count = *(app_data->appointment_count);
                for (int i = 0; i < count; i++) {
                    if (app_data->appointments[i] != NULL &&
                        app_data->appointments[i]->appointment_id == appointment_id) {
                        app_data->appointments[i] = app_data->appointments[count - 1];
                        app_data->appointments[count - 1] = NULL;
                        (*(app_data->appointment_count))--;
                        break;
                    }
                }

                // Push to stack for undo
                if (stack_push(app_data->stack, appointment)) {
                    GtkWidget *info_dialog = gtk_message_dialog_new(
                        GTK_WINDOW(dialog),
                        GTK_DIALOG_MODAL,
                        GTK_MESSAGE_INFO,
                        GTK_BUTTONS_OK,
                        "Randevu iptal edildi ve geri alma listesine eklendi."
                    );
                    gtk_dialog_run(GTK_DIALOG(info_dialog));
                    gtk_widget_destroy(info_dialog);
                } else {
                    GtkWidget *warn_dialog = gtk_message_dialog_new(
                        GTK_WINDOW(dialog),
                        GTK_DIALOG_MODAL,
                        GTK_MESSAGE_WARNING,
                        GTK_BUTTONS_OK,
                        "Randevu iptal edildi ancak geri alma listesine eklenemedi."
                    );
                    gtk_dialog_run(GTK_DIALOG(warn_dialog));
                    gtk_widget_destroy(warn_dialog);
                }
            }
        }
    }

    gtk_widget_destroy(dialog);
}

/**
 * Callback for "Undo Cancel" button
 */
static void on_undo_cancel_clicked(GtkWidget *widget, gpointer data) {
    (void)widget;
    GtkWidget *parent = GTK_WIDGET(data);

    if (app_data == NULL || app_data->avl_tree == NULL || app_data->heap == NULL ||
        app_data->stack == NULL || app_data->appointments == NULL || app_data->appointment_count == NULL) {
        GtkWidget *error_dialog = gtk_message_dialog_new(
            parent ? GTK_WINDOW(parent) : NULL,
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Hata: Uygulama verileri baslatilmadi."
        );
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
        return;
    }

    if (stack_is_empty(app_data->stack)) {
        GtkWidget *info_dialog = gtk_message_dialog_new(
            parent ? GTK_WINDOW(parent) : NULL,
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_INFO,
            GTK_BUTTONS_OK,
            "Geri alinacak iptal edilmis randevu bulunmuyor."
        );
        gtk_dialog_run(GTK_DIALOG(info_dialog));
        gtk_widget_destroy(info_dialog);
        return;
    }

    // Pop last cancelled appointment
    Appointment *appointment = stack_pop(app_data->stack);
    if (appointment == NULL) {
        GtkWidget *error_dialog = gtk_message_dialog_new(
            parent ? GTK_WINDOW(parent) : NULL,
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Hata: Geri alma listesinden randevu alinamadi."
        );
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
        return;
    }

    // Check for time conflict before re-inserting
    if (avl_has_conflict(app_data->avl_tree,
                         appointment->doctor_id,
                         appointment->start_time,
                         appointment->end_time)) {
        // Conflict: push back to stack
        stack_push(app_data->stack, appointment);

        GtkWidget *warn_dialog = gtk_message_dialog_new(
            parent ? GTK_WINDOW(parent) : NULL,
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_WARNING,
            GTK_BUTTONS_OK,
            "Uyari: Zaman dilimi artik dolu. Geri alma basarisiz oldu ve randevu geri alma listesine geri eklendi."
        );
        gtk_dialog_run(GTK_DIALOG(warn_dialog));
        gtk_widget_destroy(warn_dialog);
        return;
    }

    // No conflict - reinsert into AVL and heap
    if (avl_insert(app_data->avl_tree, appointment, appointment->doctor_id)) {
        if (heap_insert(app_data->heap, appointment)) {
            if (*(app_data->appointment_count) < MAX_RECORDS) {
                app_data->appointments[*(app_data->appointment_count)] = appointment;
                (*(app_data->appointment_count))++;

                GtkWidget *info_dialog = gtk_message_dialog_new(
                    parent ? GTK_WINDOW(parent) : NULL,
                    GTK_DIALOG_MODAL,
                    GTK_MESSAGE_INFO,
                    GTK_BUTTONS_OK,
                    "Randevu geri alma basarili."
                );
                gtk_dialog_run(GTK_DIALOG(info_dialog));
                gtk_widget_destroy(info_dialog);
            } else {
                // No space in array
                avl_delete(app_data->avl_tree, appointment->appointment_id);
                GtkWidget *error_dialog = gtk_message_dialog_new(
                    parent ? GTK_WINDOW(parent) : NULL,
                    GTK_DIALOG_MODAL,
                    GTK_MESSAGE_ERROR,
                    GTK_BUTTONS_OK,
                    "Hata: Maksimum randevu kapasitesine ulasildi. Geri alma basarisiz."
                );
                gtk_dialog_run(GTK_DIALOG(error_dialog));
                gtk_widget_destroy(error_dialog);
            }
        } else {
            // Heap insert failed
            avl_delete(app_data->avl_tree, appointment->appointment_id);
            GtkWidget *error_dialog = gtk_message_dialog_new(
                parent ? GTK_WINDOW(parent) : NULL,
                GTK_DIALOG_MODAL,
                GTK_MESSAGE_ERROR,
                GTK_BUTTONS_OK,
                "Hata: Randevu heap'e eklenemedi. Geri alma basarisiz."
            );
            gtk_dialog_run(GTK_DIALOG(error_dialog));
            gtk_widget_destroy(error_dialog);
        }
    } else {
        // AVL insert failed
        GtkWidget *error_dialog = gtk_message_dialog_new(
            parent ? GTK_WINDOW(parent) : NULL,
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Hata: Randevu AVL agacina eklenemedi. Geri alma basarisiz."
        );
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
    }
}

/**
 * Shows a read-only window listing all appointments using GtkTreeView
 */
static void show_appointments_window(GtkWidget *parent) {
    if (app_data == NULL || app_data->appointments == NULL || app_data->appointment_count == NULL) {
        GtkWidget *error_dialog = gtk_message_dialog_new(
            GTK_WINDOW(parent),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Hata: Uygulama verileri baslatilmadi."
        );
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
        return;
    }

    int count = *(app_data->appointment_count);

    if (count == 0) {
        GtkWidget *info_dialog = gtk_message_dialog_new(
            GTK_WINDOW(parent),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_INFO,
            GTK_BUTTONS_OK,
            "Randevu bulunamadi."
        );
        gtk_dialog_run(GTK_DIALOG(info_dialog));
        gtk_widget_destroy(info_dialog);
        return;
    }

    // Create top-level window for listing appointments
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Randevular");
    gtk_window_set_default_size(GTK_WINDOW(window), 700, 400);
    if (parent != NULL) {
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(parent));
        gtk_window_set_destroy_with_parent(GTK_WINDOW(window), TRUE);
    }

    // Create scrolled window
    GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled),
                                   GTK_POLICY_AUTOMATIC,
                                   GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(window), scrolled);

    // Create list store: Appointment ID, Patient ID, Doctor ID, Start, End
    GtkListStore *store = gtk_list_store_new(5,
                                             G_TYPE_INT,   // Appointment ID
                                             G_TYPE_INT,   // Patient ID
                                             G_TYPE_INT,   // Doctor ID
                                             G_TYPE_STRING, // Start Time
                                             G_TYPE_STRING  // End Time
    );

    // Populate list store from existing appointments
    GtkTreeIter iter;
    char start_buf[6];
    char end_buf[6];

    for (int i = 0; i < count; i++) {
        Appointment *appt = app_data->appointments[i];
        if (appt == NULL) {
            continue;
        }

        // Convert times to HH:MM format using existing helper
        minutes_to_time_string(appt->start_time, start_buf);
        minutes_to_time_string(appt->end_time, end_buf);

        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
                           0, appt->appointment_id,
                           1, appt->patient_id,
                           2, appt->doctor_id,
                           3, start_buf,
                           4, end_buf,
                           -1);
    }

    // Create tree view and set model
    GtkWidget *treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
    gtk_container_add(GTK_CONTAINER(scrolled), treeview);

    // We can unref store now; treeview holds a reference
    g_object_unref(store);

    // Create columns
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

    // Appointment ID column
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Randevu ID", renderer,
                                                      "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    // Patient ID column
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Hasta ID", renderer,
                                                      "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    // Doctor ID column
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Doktor ID", renderer,
                                                      "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    // Start Time column
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Baslangic Zamani", renderer,
                                                      "text", 3, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    // End Time column
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Bitis Zamani", renderer,
                                                      "text", 4, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    // Show window
    gtk_widget_show_all(window);
}

/**
 * Callback for "List Appointments" button
 */
static void on_list_appointments_clicked(GtkWidget *widget, gpointer data) {
    (void)widget;
    GtkWidget *parent = GTK_WIDGET(data);
    show_appointments_window(parent);
}

/**
 * Shows a read-only window listing all patients using GtkTreeView
 */
static void show_patients_window(GtkWidget *parent) {
    if (app_data == NULL || app_data->patients == NULL || app_data->patient_count == NULL) {
        GtkWidget *error_dialog = gtk_message_dialog_new(
            parent ? GTK_WINDOW(parent) : NULL,
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Hata: Uygulama verileri baslatilmadi."
        );
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
        return;
    }

    int count = *(app_data->patient_count);

    if (count == 0) {
        GtkWidget *info_dialog = gtk_message_dialog_new(
            GTK_WINDOW(parent),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_INFO,
            GTK_BUTTONS_OK,
            "Hasta bulunamadi."
        );
        gtk_dialog_run(GTK_DIALOG(info_dialog));
        gtk_widget_destroy(info_dialog);
        return;
    }

    // Create top-level window for listing patients
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Hastalar");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
    if (parent != NULL) {
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(parent));
        gtk_window_set_destroy_with_parent(GTK_WINDOW(window), TRUE);
    }

    // Create scrolled window
    GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled),
                                   GTK_POLICY_AUTOMATIC,
                                   GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(window), scrolled);

    // Create list store: ID, Name, Age
    GtkListStore *store = gtk_list_store_new(3,
                                             G_TYPE_INT,    // ID
                                             G_TYPE_STRING, // Name
                                             G_TYPE_INT     // Age
    );

    // Populate list store from existing patients
    GtkTreeIter iter;

    for (int i = 0; i < count; i++) {
        Patient *patient = app_data->patients[i];
        if (patient == NULL) {
            continue;
        }

        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
                           0, patient->id,
                           1, patient->name,
                           2, patient->age,
                           -1);
    }

    // Create tree view and set model
    GtkWidget *treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
    gtk_container_add(GTK_CONTAINER(scrolled), treeview);

    // We can unref store now; treeview holds a reference
    g_object_unref(store);

    // Create columns
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

    // ID column
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("ID", renderer,
                                                      "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    // Name column
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Ad", renderer,
                                                      "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    // Age column
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Yas", renderer,
                                                      "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    // Show window
    gtk_widget_show_all(window);
}

/**
 * Shows a read-only window listing all doctors using GtkTreeView
 */
static void show_doctors_window(GtkWidget *parent) {
    if (app_data == NULL || app_data->doctors == NULL || app_data->doctor_count == NULL) {
        GtkWidget *error_dialog = gtk_message_dialog_new(
            parent ? GTK_WINDOW(parent) : NULL,
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Hata: Uygulama verileri baslatilmadi."
        );
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
        return;
    }

    int count = *(app_data->doctor_count);

    if (count == 0) {
        GtkWidget *info_dialog = gtk_message_dialog_new(
            GTK_WINDOW(parent),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_INFO,
            GTK_BUTTONS_OK,
            "Doktor bulunamadi."
        );
        gtk_dialog_run(GTK_DIALOG(info_dialog));
        gtk_widget_destroy(info_dialog);
        return;
    }

    // Create top-level window for listing doctors
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Doktorlar");
    gtk_window_set_default_size(GTK_WINDOW(window), 700, 400);
    if (parent != NULL) {
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(parent));
        gtk_window_set_destroy_with_parent(GTK_WINDOW(window), TRUE);
    }

    // Create scrolled window
    GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled),
                                   GTK_POLICY_AUTOMATIC,
                                   GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(window), scrolled);

    // Create list store: ID, Name, Department
    GtkListStore *store = gtk_list_store_new(3,
                                             G_TYPE_INT,    // ID
                                             G_TYPE_STRING, // Name
                                             G_TYPE_STRING  // Department
    );

    // Populate list store from existing doctors
    GtkTreeIter iter;

    for (int i = 0; i < count; i++) {
        Doctor *doctor = app_data->doctors[i];
        if (doctor == NULL) {
            continue;
        }

        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
                           0, doctor->id,
                           1, doctor->name,
                           2, doctor->department,
                           -1);
    }

    // Create tree view and set model
    GtkWidget *treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
    gtk_container_add(GTK_CONTAINER(scrolled), treeview);

    // We can unref store now; treeview holds a reference
    g_object_unref(store);

    // Create columns
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

    // ID column
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("ID", renderer,
                                                      "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    // Name column
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Ad", renderer,
                                                      "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    // Department column
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Bolum", renderer,
                                                      "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    // Show window
    gtk_widget_show_all(window);
}

/**
 * Callback for "List Patients" button
 */
static void on_list_patients_clicked(GtkWidget *widget, gpointer data) {
    (void)widget;
    GtkWidget *parent = GTK_WIDGET(data);
    show_patients_window(parent);
}

/**
 * Callback for "List Doctors" button
 */
static void on_list_doctors_clicked(GtkWidget *widget, gpointer data) {
    (void)widget;
    GtkWidget *parent = GTK_WIDGET(data);
    show_doctors_window(parent);
}

/**
 * Callback for "Exit" button
 */
static void on_exit_clicked(GtkWidget *widget, gpointer data) {
    (void)widget;
    (void)data;
    printf("Exit button clicked\n");
    gtk_main_quit();
}

/**
 * Creates and shows the main application window
 * 
 * This function sets up the main window structure:
 * - Window title: "Hospital Appointment System"
 * - Window size: 800x600
 * - Vertical layout container (GtkBox)
 * - Title label at the top
 * - Menu buttons vertically aligned
 */
static GtkWidget* create_main_window(void) {
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *title_label;
    GtkWidget *button;
    
    // Create main window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Hastane Randevu Yonetim Sistemi");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    
    // Connect close event
    g_signal_connect(window, "destroy", G_CALLBACK(on_window_close), NULL);
    
    // Create vertical box container
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 20);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    
    // Create and add title label
    title_label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(title_label), 
                        "<span size='xx-large' weight='bold'>Hastane Randevu Yonetim Sistemi</span>");
    gtk_label_set_justify(GTK_LABEL(title_label), GTK_JUSTIFY_CENTER);
    gtk_box_pack_start(GTK_BOX(vbox), title_label, FALSE, FALSE, 20);
    
    // Add separator
    GtkWidget *separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(vbox), separator, FALSE, FALSE, 10);
    
    // Create and add menu buttons
    
    // 1. Add Patient button
    button = gtk_button_new_with_label("1. Hasta Ekle");
    gtk_widget_set_size_request(button, 300, 40);
    g_signal_connect(button, "clicked", G_CALLBACK(on_add_patient_clicked), window);
    gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 5);
    
    // 2. Add Doctor button
    button = gtk_button_new_with_label("2. Doktor Ekle");
    gtk_widget_set_size_request(button, 300, 40);
    g_signal_connect(button, "clicked", G_CALLBACK(on_add_doctor_clicked), window);
    gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 5);
    
    // 3. Create Appointment button
    button = gtk_button_new_with_label("3. Randevu Olustur");
    gtk_widget_set_size_request(button, 300, 40);
    g_signal_connect(button, "clicked", G_CALLBACK(on_create_appointment_clicked), window);
    gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 5);
    
    // 4. Cancel Appointment button
    button = gtk_button_new_with_label("4. Randevu Iptal Et");
    gtk_widget_set_size_request(button, 300, 40);
    g_signal_connect(button, "clicked", G_CALLBACK(on_cancel_appointment_clicked), window);
    gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 5);
    
    // 5. Undo Cancel button
    button = gtk_button_new_with_label("5. Iptali Geri Al");
    gtk_widget_set_size_request(button, 300, 40);
    g_signal_connect(button, "clicked", G_CALLBACK(on_undo_cancel_clicked), window);
    gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 5);
    
    // 6. List Appointments button
    button = gtk_button_new_with_label("6. Randevulari Listele");
    gtk_widget_set_size_request(button, 300, 40);
    g_signal_connect(button, "clicked", G_CALLBACK(on_list_appointments_clicked), window);
    gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 5);
    
    // 7. List Patients button
    button = gtk_button_new_with_label("7. Hastalari Listele");
    gtk_widget_set_size_request(button, 300, 40);
    g_signal_connect(button, "clicked", G_CALLBACK(on_list_patients_clicked), window);
    gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 5);
    
    // 8. List Doctors button
    button = gtk_button_new_with_label("8. Doktorlari Listele");
    gtk_widget_set_size_request(button, 300, 40);
    g_signal_connect(button, "clicked", G_CALLBACK(on_list_doctors_clicked), window);
    gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 5);
    
    // Add separator before exit button
    separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(vbox), separator, FALSE, FALSE, 10);
    
    // 9. Exit button
    button = gtk_button_new_with_label("9. Cikis");
    gtk_widget_set_size_request(button, 300, 40);
    g_signal_connect(button, "clicked", G_CALLBACK(on_exit_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 5);
    
    // Show all widgets
    gtk_widget_show_all(window);
    
    return window;
}

/**
 * Starts the GTK GUI application
 * 
 * Initializes GTK, creates the main window, and starts the main event loop.
 * 
 * @param argc: Command line argument count
 * @param argv: Command line arguments array
 * @param app_data: Pointer to application data structure
 */
void start_gui(int argc, char *argv[], AppData* app_data_param) {
    // Store application data
    app_data = app_data_param;
    
    // Initialize GTK
    gtk_init(&argc, &argv);
    
    // Create and show main window
    create_main_window();
    
    // Start GTK main event loop
    gtk_main();
}

