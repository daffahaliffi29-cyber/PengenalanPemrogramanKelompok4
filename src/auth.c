#include <stdio.h>
#include <string.h>
#include "auth.h"

// Nilai hash DJB2 untuk password default: "Admin123"
static const uint32_t HASH_ADMIN_VALID = 2417333444U;

uint32_t hitungHash(const char *str) {
    uint32_t hash = 5381;
    int karakter;

    while ((karakter = (unsigned char)*str++) != '\0') {
        hash = ((hash << 5) + hash) + karakter; // hash * 33 + karakter
    }
    return hash;
}

bool loginAdmin(void) {
    char inputPassword[MAX_PANJANG_PW];

    for (int percobaan = 1; percobaan <= MAX_PERCOBAAN_LOGIN; percobaan++) {
        printf("Masukkan Password Admin (Percobaan %d/%d): ", percobaan, MAX_PERCOBAAN_LOGIN);
        if (fgets(inputPassword, sizeof(inputPassword), stdin) == NULL) {
            return false;
        }
        inputPassword[strcspn(inputPassword, "\r\n")] = '\0';

        if (hitungHash(inputPassword) == HASH_ADMIN_VALID) {
            printf("[OK] Login admin berhasil.\n");
            return true;
        }

        if (percobaan < MAX_PERCOBAAN_LOGIN) {
            printf("[!] Password salah. Silakan coba lagi.\n\n");
        }
    }

    printf("[!] Akses ditolak: batas percobaan login telah habis.\n");
    return false;
}
