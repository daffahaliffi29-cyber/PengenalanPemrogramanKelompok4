#include <string.h>
#include <ctype.h>
#include "buku.h"

static void ubahKeHurufKecil(char tujuan[], const char asal[]) {
    int i = 0;
    while (asal[i] != '\0') {
        tujuan[i] = (char)tolower((unsigned char)asal[i]);
        i++;
    }
    tujuan[i] = '\0';
}

int cariIndeksByKode(const Buku katalog[], int totalBuku, const char kode[]) {
    char kodeCari[10];
    char kodeItem[10];
    ubahKeHurufKecil(kodeCari, kode);

    for (int i = 0; i < totalBuku; i++) {
        if (katalog[i].aktif) {
            ubahKeHurufKecil(kodeItem, katalog[i].kode);
            if (strcmp(kodeItem, kodeCari) == 0) {
                return i;
            }
        }
    }
    return -1;
}

bool tambahBukuBaru(Buku katalog[], int *totalBuku, Buku bukuBaru) {
    if (*totalBuku >= MAX_BUKU) {
        return false; // Kapasitas array penuh
    }
    if (cariIndeksByKode(katalog, *totalBuku, bukuBaru.kode) != -1) {
        return false; // Mencegah kode buku kembar
    }

    bukuBaru.aktif = true;
    katalog[*totalBuku] = bukuBaru;
    (*totalBuku)++;
    return true;
}

void ubahStokBuku(Buku *targetBuku, int selisihStok) {
    targetBuku->stok += selisihStok;
    if (targetBuku->stok < 0) {
        targetBuku->stok = 0;
    }
}

bool hapusBuku(Buku katalog[], int totalBuku, const char kode[]) {
    int indeks = cariIndeksByKode(katalog, totalBuku, kode);
    if (indeks == -1) {
        return false;
    }
    katalog[indeks].aktif = false;
    return true;
}
