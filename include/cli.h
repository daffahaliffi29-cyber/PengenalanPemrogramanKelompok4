#ifndef CLI_H
#define CLI_H

#include "buku.h"
#include "transaksi.h"

// Membersihkan layar terminal (cls di Windows, clear di Linux/macOS)
void bersihkanLayar(void);

// Menunggu pengguna menekan Enter lalu membersihkan layar terminal
void jedaDanBersihkan(void);

// Menjalankan alur lengkap Mode Pembeli melalui terminal standar (CLI)
void jalankanModePembeli(Buku katalog[], int totalBuku, ItemKeranjang keranjang[], int *jumlahItem, Transaksi riwayat[], int *totalTransaksi);

// Menjalankan alur lengkap Mode Admin melalui terminal standar (CLI)
void jalankanModeAdmin(Buku katalog[], int *totalBuku, const Transaksi riwayat[], int totalTransaksi);

#endif
