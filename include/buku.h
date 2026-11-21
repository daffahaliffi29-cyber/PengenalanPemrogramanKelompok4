#ifndef BUKU_H
#define BUKU_H

#include <stdbool.h>

#define MAX_BUKU 150

typedef struct {
    char kode[10];       // Contoh: "BK-001"
    char judul[65];
    char penulis[45];
    char kategori[25];
    int harga;
    int stok;
    bool aktif;          // true = tampil di katalog, false = dihapus (soft delete)
} Buku;

// Memuat data awal dari data_buku.c ke array aktif di RAM (mengembalikan jumlah buku)
int muatDataBukuAwal(Buku katalog[]);

// Mencari posisi indeks buku berdasarkan kode (-1 jika tidak ditemukan)
int cariIndeksByKode(const Buku katalog[], int totalBuku, const char kode[]);

// Mencari buku secara parsial (case-insensitive) berdasarkan judul
int cariBukuByJudul(const Buku katalog[], int totalBuku, const char kataKunci[], int hasilIndeks[]);

// Menambah buku baru ke katalog (mengembalikan false jika array penuh atau kode duplikat)
bool tambahBukuBaru(Buku katalog[], int *totalBuku, Buku bukuBaru);

// Menambah atau mengurangi stok buku
void ubahStokBuku(Buku *targetBuku, int selisihStok);

// Menonaktifkan buku dari katalog tanpa menggeser elemen array
bool hapusBuku(Buku katalog[], int totalBuku, const char kode[]);

#endif
