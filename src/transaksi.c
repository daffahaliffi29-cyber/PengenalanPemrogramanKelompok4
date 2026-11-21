#include <string.h>
#include "transaksi.h"

void resetKeranjang(ItemKeranjang keranjang[], int *jumlahItem) {
    (void)keranjang;
    *jumlahItem = 0;
}

int getKuantitasDiKeranjang(const ItemKeranjang keranjang[], int jumlahItem, const char kodeBuku[]) {
    for (int i = 0; i < jumlahItem; i++) {
        if (strcmp(keranjang[i].kodeBuku, kodeBuku) == 0) {
            return keranjang[i].kuantitas;
        }
    }
    return 0;
}

bool tambahKeKeranjang(ItemKeranjang keranjang[], int *jumlahItem, const Buku katalog[], int indeksBuku, int qty) {
    if (indeksBuku < 0 || qty <= 0 || !katalog[indeksBuku].aktif) {
        return false;
    }

    // 1. Cek apakah buku sudah ada di dalam keranjang (Cart Merging)
    for (int i = 0; i < *jumlahItem; i++) {
        if (strcmp(keranjang[i].kodeBuku, katalog[indeksBuku].kode) == 0) {
            int kuantitasBaru = keranjang[i].kuantitas + qty;
            if (kuantitasBaru > katalog[indeksBuku].stok) {
                return false;
            }
            keranjang[i].kuantitas = kuantitasBaru;
            keranjang[i].subtotal = kuantitasBaru * keranjang[i].hargaSatuan;
            return true;
        }
    }

    // 2. Jika belum ada, masukkan sebagai baris baru di keranjang
    if (*jumlahItem >= MAX_KERANJANG || qty > katalog[indeksBuku].stok) {
        return false;
    }

    int pos = *jumlahItem;
    keranjang[pos].indeksBuku = indeksBuku;
    strcpy(keranjang[pos].kodeBuku, katalog[indeksBuku].kode);
    strcpy(keranjang[pos].judul, katalog[indeksBuku].judul);
    keranjang[pos].hargaSatuan = katalog[indeksBuku].harga;
    keranjang[pos].kuantitas = qty;
    keranjang[pos].subtotal = qty * katalog[indeksBuku].harga;

    (*jumlahItem)++;
    return true;
}

bool hapusItemKeranjang(ItemKeranjang keranjang[], int *jumlahItem, int indeksKeranjang) {
    if (indeksKeranjang < 0 || indeksKeranjang >= *jumlahItem) {
        return false;
    }

    for (int i = indeksKeranjang; i < *jumlahItem - 1; i++) {
        keranjang[i] = keranjang[i + 1];
    }
    (*jumlahItem)--;
    return true;
}

int hitungTotalKeranjang(const ItemKeranjang keranjang[], int jumlahItem) {
    int total = 0;
    for (int i = 0; i < jumlahItem; i++) {
        total += keranjang[i].subtotal;
    }
    return total;
}

bool prosesCheckout(Buku katalog[], ItemKeranjang keranjang[], int *jumlahItem, int uangBayar, const char *metodeBayar, Transaksi *hasilTransaksi, int idBaru) {
    if (*jumlahItem <= 0) {
        return false;
    }

    int totalTagihan = hitungTotalKeranjang(keranjang, *jumlahItem);
    if (uangBayar < totalTagihan) {
        return false;
    }

    hasilTransaksi->idTransaksi = idBaru;
    strcpy(hasilTransaksi->metodeBayar, metodeBayar);
    hasilTransaksi->jumlahJenisItem = *jumlahItem;
    hasilTransaksi->totalHarga = totalTagihan;
    hasilTransaksi->uangBayar = uangBayar;
    hasilTransaksi->kembalian = uangBayar - totalTagihan;

    for (int i = 0; i < *jumlahItem; i++) {
        hasilTransaksi->items[i] = keranjang[i];
        int idxKatalog = keranjang[i].indeksBuku;
        ubahStokBuku(&katalog[idxKatalog], -keranjang[i].kuantitas);
    }

    resetKeranjang(keranjang, jumlahItem);
    return true;
}

int hitungTotalOmzet(const Transaksi riwayat[], int totalTransaksi) {
    int omzet = 0;
    for (int i = 0; i < totalTransaksi; i++) {
        omzet += riwayat[i].totalHarga;
    }
    return omzet;
}
