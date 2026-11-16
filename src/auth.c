#include <stdio.h>
#include <string.h>
#include "auth.h"
#include "tuih.h"

static const uint32_t HASH_ADMIN_VALID = 2417333444U;

uint32_t hitungHash(const char *str) {
    uint32_t hash = 5381;
    int karakter;

    while ((karakter = (unsigned char)*str++) != '\0') {
        hash = ((hash << 5) + hash) + karakter;
    }
    return hash;
}
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
    if (hitungHash(inputPassword) == HASH_ADMIN_VALID) {
        tunggu();
        return 1;
    } else {
        printf("Login gagal! Username atau password salah.\n");
        tunggu();
        return 0;
    }
}
