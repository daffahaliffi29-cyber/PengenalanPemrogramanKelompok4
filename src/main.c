#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "buku.h"
#include "auth.h"
#include "transaksi.h"
#include "cli.h"

static int bacaPilihanUtama(void) {
    char baris[32];
    char *akhir;
    long nilai;

    while (1) {
        printf("Pilih menu (1-3): ");
        if (fgets(baris, sizeof(baris), stdin) == NULL) {
            return 3;
        }
        baris[strcspn(baris, "\r\n")] = '\0';

        char *mulai = baris;
        while (isspace((unsigned char)*mulai)) {
            mulai++;
        }
        if (*mulai == '\0') {
            printf("[!] Input tidak boleh kosong. Masukkan angka 1, 2, atau 3.\n");
            continue;
        }

        nilai = strtol(mulai, &akhir, 10);
        while (isspace((unsigned char)*akhir)) {
            akhir++;
        }
        if (*akhir == '\0' && nilai >= 1 && nilai <= 3) {
            return (int)nilai;
        }
        printf("[!] Pilihan tidak valid. Masukkan angka 1, 2, atau 3.\n");
    }
}

int main(void) {
    Buku katalog[MAX_BUKU];
    int totalBuku = muatDataBukuAwal(katalog);

    ItemKeranjang keranjang[MAX_KERANJANG];
    int jumlahItemKeranjang = 0;

    Transaksi riwayatTransaksi[MAX_RIWAYAT];
    int totalTransaksi = 0;

    bersihkanLayar();

    while (1) {
        printf("================================================================================\n");
        printf(" SISTEM INFORMASI & KASIR TOKO BUKU\n");
        printf("================================================================================\n");
        printf("   1. Masuk sebagai Pembeli (Jelajah Katalog & Belanja)\n");
        printf("   2. Masuk sebagai Admin (Kelola Buku & Laporan Omzet)\n");
        printf("   3. Keluar dari Aplikasi\n");
        printf("--------------------------------------------------------------------------------\n");

        int peran = bacaPilihanUtama();

        if (peran == 1) {
            jalankanModePembeli(
                katalog,
                totalBuku,
                keranjang,
                &jumlahItemKeranjang,
                riwayatTransaksi,
                &totalTransaksi
            );
        } else if (peran == 2) {
            bersihkanLayar();
            printf("================================================================================\n");
            printf(" AUTENTIKASI MODE ADMIN\n");
            printf("================================================================================\n");
            if (loginAdmin()) {
                jedaDanBersihkan();
                jalankanModeAdmin(katalog, &totalBuku, riwayatTransaksi, totalTransaksi);
            } else {
                jedaDanBersihkan();
            }
        } else if (peran == 3) {
            bersihkanLayar();
            printf("Aplikasi ditutup. Terima kasih.\n");
            break;
        }
    }

    return 0;
}
