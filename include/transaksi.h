#ifndef TRANSAKSI_H
#define TRANSAKSI_H

#include <stdbool.h>
#include "buku.h"

#define MAX_KERANJANG 20
#define MAX_RIWAYAT 100

typedef struct {
    int indeksBuku;
    char kodeBuku[10];
    char judul[65];
    int hargaSatuan;
    int kuantitas;
    int subtotal;
} ItemKeranjang;

typedef struct {
    int idTransaksi;
    char metodeBayar[15]; // "TUNAI"
    ItemKeranjang items[MAX_KERANJANG];
    int jumlahJenisItem;
    int totalHarga;
    int uangBayar;
    int kembalian;
} Transaksi;

void resetKeranjang(ItemKeranjang keranjang[], int *jumlahItem);
int getKuantitasDiKeranjang(const ItemKeranjang keranjang[], int jumlahItem, const char kodeBuku[]);
bool tambahKeKeranjang(ItemKeranjang keranjang[], int *jumlahItem, const Buku katalog[], int indeksBuku, int qty);
bool hapusItemKeranjang(ItemKeranjang keranjang[], int *jumlahItem, int indeksKeranjang);
int hitungTotalKeranjang(const ItemKeranjang keranjang[], int jumlahItem);
bool prosesCheckout(Buku katalog[], ItemKeranjang keranjang[], int *jumlahItem, int uangBayar, const char *metodeBayar, Transaksi *hasilTransaksi, int idBaru);
int hitungTotalOmzet(const Transaksi riwayat[], int totalTransaksi);

#endif
