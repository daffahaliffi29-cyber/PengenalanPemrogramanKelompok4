#include <stdio.h>
#include <string.h>
#include "auth.h"
#include "tuih.h"

int login() {
    char username[50];
    char password[50];

    bersihLayar();
    judul("LOGIN SISTEM TOKO BUKU");

    printf("Username : ");
    scanf("%s", username);
    printf("Password : ");
    scanf("%s", password);

    garis(40);

    // Cek kecocokan username & password
    if (strcmp(username, "admin") == 0 && strcmp(password, "12345") == 0) {
        printf("Login berhasil! Selamat datang, %s.\n", username);
        tunggu();
        return 1; // Berhasil
    } else {
        printf("Login gagal! Username atau password salah.\n");
        tunggu();
        return 0; // Gagal
    }
}