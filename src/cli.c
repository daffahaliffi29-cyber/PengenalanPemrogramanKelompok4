#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "cli.h"

// --- KONTROL LAYAR TERMINAL ---

void bersihkanLayar(void) {
#ifdef _WIN32
    int status = system("cls");
#else
    int status = system("clear");
#endif
    (void)status;
}

void jedaDanBersihkan(void) {
    char dummy[8];
    printf("\nTekan Enter untuk melanjutkan...");
    if (fgets(dummy, sizeof(dummy), stdin) != NULL) {
        if (strchr(dummy, '\n') == NULL) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }
    }
    bersihkanLayar();
}

// --- FUNGSI PEMBANTU INPUT/OUTPUT TERMINAL STANDAR ---

static void bacaTeksAman(const char *prompt, char buffer[], int ukuran) {
    printf("%s", prompt);
    if (fgets(buffer, ukuran, stdin) != NULL) {
        buffer[strcspn(buffer, "\r\n")] = '\0';
    } else {
        buffer[0] = '\0';
    }
}

static int bacaAngkaRentang(const char *prompt, int minNilai, int maxNilai) {
    char baris[64];
    char *akhir;
    long nilai;

    while (1) {
        printf("%s", prompt);
        if (fgets(baris, sizeof(baris), stdin) == NULL) {
            return minNilai;
        }
        baris[strcspn(baris, "\r\n")] = '\0';

        char *mulai = baris;
        while (isspace((unsigned char)*mulai)) {
            mulai++;
        }
        if (*mulai == '\0') {
            printf("[!] Input tidak boleh kosong. Masukkan angka (%d - %d).\n", minNilai, maxNilai);
            continue;
        }

        nilai = strtol(mulai, &akhir, 10);
        while (isspace((unsigned char)*akhir)) {
            akhir++;
        }

        if (*akhir == '\0' && nilai >= minNilai && nilai <= maxNilai) {
            return (int)nilai;
        }

        printf("[!] Input tidak valid. Masukkan angka antara %d hingga %d.\n", minNilai, maxNilai);
    }
}

static bool bacaKonfirmasiYaTidak(const char *prompt) {
    char jawab[16];
    while (1) {
        bacaTeksAman(prompt, jawab, sizeof(jawab));
        if (jawab[0] == 'y' || jawab[0] == 'Y') return true;
        if (jawab[0] == 'n' || jawab[0] == 'N' || jawab[0] == 't' || jawab[0] == 'T') return false;
        printf("[!] Ketik 'y' untuk Ya atau 'n' untuk Tidak.\n");
    }
}

static void cetakGaris(void) {
    printf("================================================================================\n");
}

static void cetakStrukTransaksi(const Transaksi *trx) {
    bersihkanLayar();
    cetakGaris();
    printf("                         STRUK PEMBAYARAN TOKO BUKU\n");
    printf("                         No. Transaksi : TRX-%03d\n", trx->idTransaksi);
    printf("                         Metode Bayar  : %s\n", trx->metodeBayar);
    cetakGaris();
    for (int i = 0; i < trx->jumlahJenisItem; i++) {
        printf(" %d. %s (%s)\n", i + 1, trx->items[i].judul, trx->items[i].kodeBuku);
        printf("    %d x Rp %d = Rp %d\n",
               trx->items[i].kuantitas,
               trx->items[i].hargaSatuan,
               trx->items[i].subtotal);
    }
    printf("--------------------------------------------------------------------------------\n");
    printf(" Total Tagihan : Rp %d\n", trx->totalHarga);
    printf(" Dibayar       : Rp %d\n", trx->uangBayar);
    printf(" Kembalian     : Rp %d\n", trx->kembalian);
    cetakGaris();
    printf("                 Terima kasih telah berbelanja di toko kami!\n");
    cetakGaris();
    jedaDanBersihkan();
}

// --- FITUR MODE PEMBELI ---

static void tampilkanTabelKatalog(const Buku katalog[], int totalBuku) {
    bersihkanLayar();
    cetakGaris();
    printf(" DAFTAR KATALOG BUKU\n");
    cetakGaris();
    printf(" %-7s | %-42s | %-11s | %-4s\n", "KODE", "JUDUL BUKU", "HARGA", "STOK");
    printf("---------+--------------------------------------------+-------------+------\n");

    int jumlahAktif = 0;
    for (int i = 0; i < totalBuku; i++) {
        if (katalog[i].aktif) {
            printf(" %-7s | %-42.42s | Rp %8d | %4d\n",
                   katalog[i].kode,
                   katalog[i].judul,
                   katalog[i].harga,
                   katalog[i].stok);
            jumlahAktif++;
        }
    }

    if (jumlahAktif == 0) {
        printf(" (Katalog buku saat ini kosong)\n");
    }
    cetakGaris();
}

static void prosesDetailDanTambahKeranjang(
    Buku katalog[],
    int totalBuku,
    ItemKeranjang keranjang[],
    int *jumlahItem,
    const char *kodeAwal
) {
    bersihkanLayar();
    cetakGaris();
    printf(" DETAIL INFORMASI BUKU\n");
    cetakGaris();

    char kodeInput[16];
    if (kodeAwal != NULL && kodeAwal[0] != '\0') {
        strncpy(kodeInput, kodeAwal, sizeof(kodeInput) - 1);
        kodeInput[sizeof(kodeInput) - 1] = '\0';
    } else {
        bacaTeksAman("Masukkan Kode Buku (contoh: BK-001): ", kodeInput, sizeof(kodeInput));
    }

    int idx = cariIndeksByKode(katalog, totalBuku, kodeInput);
    if (idx == -1) {
        printf("[!] Buku dengan kode '%s' tidak ditemukan di katalog.\n", kodeInput);
        jedaDanBersihkan();
        return;
    }

    const Buku *b = &katalog[idx];
    int sudahDiKeranjang = getKuantitasDiKeranjang(keranjang, *jumlahItem, b->kode);
    int sisaTersedia = b->stok - sudahDiKeranjang;

    printf(" Kode Buku          : %s\n", b->kode);
    printf(" Judul              : %s\n", b->judul);
    printf(" Penulis            : %s\n", b->penulis);
    printf(" Kategori           : %s\n", b->kategori);
    printf(" Harga Satuan       : Rp %d\n", b->harga);
    printf(" Stok Gudang        : %d eksemplar\n", b->stok);
    printf(" Sudah di Keranjang : %d eksemplar\n", sudahDiKeranjang);
    if (sisaTersedia > 0) {
        printf(" Status             : Tersedia (%d eksemplar bisa dibeli)\n", sisaTersedia);
    } else {
        printf(" Status             : TIDAK TERSEDIA / STOK HABIS\n");
    }
    cetakGaris();

    if (sisaTersedia <= 0) {
        jedaDanBersihkan();
        return;
    }

    if (!bacaKonfirmasiYaTidak("Tambahkan buku ini ke keranjang? (y/n): ")) {
        bersihkanLayar();
        return;
    }

    char promptQty[64];
    snprintf(promptQty, sizeof(promptQty), "Masukkan jumlah beli (1 - %d): ", sisaTersedia);
    int qty = bacaAngkaRentang(promptQty, 1, sisaTersedia);

    if (tambahKeKeranjang(keranjang, jumlahItem, katalog, idx, qty)) {
        printf("[OK] %d eksemplar \"%s\" berhasil ditambahkan ke keranjang.\n", qty, b->judul);
    } else {
        printf("[!] Gagal menambahkan ke keranjang (keranjang penuh atau stok tidak cukup).\n");
    }
    jedaDanBersihkan();
}

static void tampilkanIsiKeranjang(const ItemKeranjang keranjang[], int jumlahItem) {
    bersihkanLayar();
    cetakGaris();
    printf(" KERANJANG BELANJA (%d Jenis Buku)\n", jumlahItem);
    cetakGaris();

    if (jumlahItem == 0) {
        printf(" Keranjang belanja masih kosong.\n");
        cetakGaris();
        return;
    }

    printf(" %-3s | %-7s | %-36s | %-4s | %-12s\n", "NO", "KODE", "JUDUL BUKU", "QTY", "SUBTOTAL");
    printf("-----+---------+--------------------------------------+------+--------------\n");
    for (int i = 0; i < jumlahItem; i++) {
        printf(" %-3d | %-7s | %-36.36s | %4d | Rp %9d\n",
               i + 1,
               keranjang[i].kodeBuku,
               keranjang[i].judul,
               keranjang[i].kuantitas,
               keranjang[i].subtotal);
    }
    printf("--------------------------------------------------------------------------------\n");
    printf(" TOTAL TAGIHAN : Rp %d\n", hitungTotalKeranjang(keranjang, jumlahItem));
    cetakGaris();
}

static void menuKeranjang(ItemKeranjang keranjang[], int *jumlahItem) {
    while (1) {
        tampilkanIsiKeranjang(keranjang, *jumlahItem);
        if (*jumlahItem == 0) {
            jedaDanBersihkan();
            return;
        }

        printf(" Opsi Keranjang:\n");
        printf("   1. Hapus salah satu item dari keranjang\n");
        printf("   2. Kosongkan seluruh keranjang\n");
        printf("   0. Kembali ke Menu Pembeli\n");
        int pilih = bacaAngkaRentang("Pilih opsi (0-2): ", 0, 2);

        if (pilih == 0) {
            bersihkanLayar();
            return;
        } else if (pilih == 1) {
            int nomor = bacaAngkaRentang("Masukkan nomor urut item yang akan dihapus: ", 1, *jumlahItem);
            if (hapusItemKeranjang(keranjang, jumlahItem, nomor - 1)) {
                printf("[OK] Item berhasil dihapus dari keranjang.\n");
            }
            jedaDanBersihkan();
        } else if (pilih == 2) {
            if (bacaKonfirmasiYaTidak("Yakin ingin mengosongkan seluruh keranjang? (y/n): ")) {
                resetKeranjang(keranjang, jumlahItem);
                printf("[OK] Keranjang telah dikosongkan.\n");
                jedaDanBersihkan();
                return;
            }
        }
    }
}

static void menuCheckout(Buku katalog[], ItemKeranjang keranjang[], int *jumlahItem, Transaksi riwayat[], int *totalTransaksi) {
    bersihkanLayar();
    if (*jumlahItem == 0) {
        printf("[!] Keranjang belanja masih kosong. Silakan pilih buku terlebih dahulu.\n");
        jedaDanBersihkan();
        return;
    }

    if (*totalTransaksi >= MAX_RIWAYAT) {
        printf("[!] Kapasitas riwayat transaksi sesi ini telah penuh.\n");
        jedaDanBersihkan();
        return;
    }

    tampilkanIsiKeranjang(keranjang, *jumlahItem);
    int totalTagihan = hitungTotalKeranjang(keranjang, *jumlahItem);

    if (!bacaKonfirmasiYaTidak("Lanjutkan proses pembayaran tunai? (y/n): ")) {
        printf("Checkout dibatalkan.\n");
        jedaDanBersihkan();
        return;
    }

    printf("\n--- PEMBAYARAN TUNAI ---\n");
    printf("Total Tagihan : Rp %d\n", totalTagihan);
    int uangBayar = bacaAngkaRentang("Masukkan nominal uang tunai (Rp): ", totalTagihan, 1000000000);
    printf("Kembalian     : Rp %d\n", uangBayar - totalTagihan);

    if (!bacaKonfirmasiYaTidak("Konfirmasi selesai pembayaran tunai? (y/n): ")) {
        printf("Pembayaran dibatalkan.\n");
        jedaDanBersihkan();
        return;
    }

    int idBaru = *totalTransaksi + 1;
    if (prosesCheckout(katalog, keranjang, jumlahItem, uangBayar, "TUNAI", &riwayat[*totalTransaksi], idBaru)) {
        (*totalTransaksi)++;
        cetakStrukTransaksi(&riwayat[*totalTransaksi - 1]);
    } else {
        printf("[!] Gagal memproses transaksi.\n");
        jedaDanBersihkan();
    }
}

void jalankanModePembeli(Buku katalog[], int totalBuku, ItemKeranjang keranjang[], int *jumlahItem, Transaksi riwayat[], int *totalTransaksi) {
    bersihkanLayar();
    while (1) {
        int totalKeranjang = hitungTotalKeranjang(keranjang, *jumlahItem);
        cetakGaris();
        printf(" MODE PEMBELI - TOKO BUKU\n");
        printf(" Keranjang Saat Ini: %d jenis buku (Total: Rp %d)\n", *jumlahItem, totalKeranjang);
        cetakGaris();
        printf("   1. Lihat Daftar Katalog Buku\n");
        printf("   2. Cari Buku (Judul / Kode)\n");
        printf("   3. Lihat Detail & Tambah Buku ke Keranjang\n");
        printf("   4. Lihat Keranjang Belanja & Hapus Item\n");
        printf("   5. Checkout & Pembayaran Tunai\n");
        printf("   6. Kembali ke Menu Utama\n");
        printf("--------------------------------------------------------------------------------\n");

        int pilihan = bacaAngkaRentang("Pilih menu pembeli (1-6): ", 1, 6);

        if (pilihan == 1) {
            tampilkanTabelKatalog(katalog, totalBuku);
            char kodePilih[16];
            bacaTeksAman("Masukkan Kode Buku untuk detail & beli (atau Enter untuk kembali): ", kodePilih, sizeof(kodePilih));
            if (kodePilih[0] != '\0') {
                prosesDetailDanTambahKeranjang(katalog, totalBuku, keranjang, jumlahItem, kodePilih);
            } else {
                bersihkanLayar();
            }
        } else if (pilihan == 2) {
            menuCariBuku(katalog, totalBuku, keranjang, jumlahItem);
        } else if (pilihan == 3) {
            prosesDetailDanTambahKeranjang(katalog, totalBuku, keranjang, jumlahItem, NULL);
        } else if (pilihan == 4) {
            menuKeranjang(keranjang, jumlahItem);
        } else if (pilihan == 5) {
            menuCheckout(katalog, keranjang, jumlahItem, riwayat, totalTransaksi);
        } else if (pilihan == 6) {
            bersihkanLayar();
            return;
        }
    }
}

// --- FITUR MODE ADMIN ---

void jalankanModeAdmin(Buku katalog[], int *totalBuku, const Transaksi riwayat[], int totalTransaksi) {
    bersihkanLayar();
    while (1) {
        cetakGaris();
        printf(" DASHBOARD ADMIN TOKO BUKU\n");
        cetakGaris();
        printf("   1. Lihat Semua Daftar Buku & Stok\n");
        printf("   2. Tambah / Update Stok Buku (Restock)\n");
        printf("   3. Tambah Judul Buku Baru ke Katalog\n");
        printf("   4. Hapus Buku dari Katalog\n");
        printf("   5. Lihat Laporan Omzet & Riwayat Transaksi\n");
        printf("   6. Logout ke Menu Utama\n");
        printf("--------------------------------------------------------------------------------\n");

        int pilihan = bacaAngkaRentang("Pilih menu admin (1-6): ", 1, 6);

        if (pilihan == 1) {
            tampilkanTabelKatalog(katalog, *totalBuku);
            jedaDanBersihkan();
        } else if (pilihan == 2) {
            bersihkanLayar();
            cetakGaris();
            printf(" TAMBAH STOK BUKU (RESTOCK)\n");
            cetakGaris();
            char kode[16];
            bacaTeksAman("Masukkan Kode Buku (misal BK-001): ", kode, sizeof(kode));
            int idx = cariIndeksByKode(katalog, *totalBuku, kode);
            if (idx == -1) {
                printf("[!] Buku dengan kode '%s' tidak ditemukan.\n", kode);
            } else {
                printf("Buku: %s | Stok saat ini: %d\n", katalog[idx].judul, katalog[idx].stok);
                int tambahan = bacaAngkaRentang("Masukkan jumlah penambahan stok (1 - 10000): ", 1, 10000);
                ubahStokBuku(&katalog[idx], tambahan);
                printf("[OK] Stok berhasil diperbarui menjadi %d eksemplar.\n", katalog[idx].stok);
            }
            jedaDanBersihkan();
        } else if (pilihan == 3) {
            bersihkanLayar();
            cetakGaris();
            printf(" TAMBAH JUDUL BUKU BARU\n");
            cetakGaris();
            Buku baru;
            bacaTeksAman("Kode Buku (unik, misal BK-012) : ", baru.kode, sizeof(baru.kode));
            if (baru.kode[0] == '\0') {
                printf("[!] Kode buku tidak boleh kosong.\n");
                jedaDanBersihkan();
                continue;
            }
            bacaTeksAman("Judul Buku                     : ", baru.judul, sizeof(baru.judul));
            bacaTeksAman("Penulis                        : ", baru.penulis, sizeof(baru.penulis));
            bacaTeksAman("Kategori                       : ", baru.kategori, sizeof(baru.kategori));
            baru.harga = bacaAngkaRentang("Harga Satuan (Rp, min 100)     : ", 100, 100000000);
            baru.stok = bacaAngkaRentang("Stok Awal (0 - 10000)          : ", 0, 10000);

            if (tambahBukuBaru(katalog, totalBuku, baru)) {
                printf("[OK] Buku '%s' (%s) berhasil ditambahkan ke katalog.\n", baru.judul, baru.kode);
            } else {
                printf("[!] Gagal menambahkan: kode buku sudah dipakai atau kapasitas penuh.\n");
            }
            jedaDanBersihkan();
        } else if (pilihan == 4) {
            bersihkanLayar();
            cetakGaris();
            printf(" HAPUS BUKU DARI KATALOG\n");
            cetakGaris();
            char kode[16];
            bacaTeksAman("Masukkan Kode Buku yang akan dihapus: ", kode, sizeof(kode));
            if (hapusBuku(katalog, *totalBuku, kode)) {
                printf("[OK] Buku '%s' berhasil dihapus dari etalase.\n", kode);
            } else {
                printf("[!] Buku dengan kode '%s' tidak ditemukan.\n", kode);
            }
            jedaDanBersihkan();
        } else if (pilihan == 5) {
            bersihkanLayar();
            cetakGaris();
            printf(" LAPORAN PENDAPATAN & RIWAYAT TRANSAKSI\n");
            cetakGaris();
            printf(" Total Transaksi Berhasil : %d transaksi\n", totalTransaksi);
            printf(" Total Omzet Kotor        : Rp %d\n", hitungTotalOmzet(riwayat, totalTransaksi));
            printf("--------------------------------------------------------------------------------\n");
            if (totalTransaksi == 0) {
                printf(" Belum ada transaksi tercatat pada sesi ini.\n");
            } else {
                for (int i = 0; i < totalTransaksi; i++) {
                    printf(" TRX-%03d | Metode: %-5s | %d jenis buku | Total: Rp %d\n",
                           riwayat[i].idTransaksi,
                           riwayat[i].metodeBayar,
                           riwayat[i].jumlahJenisItem,
                           riwayat[i].totalHarga);
                }
            }
            cetakGaris();
            jedaDanBersihkan();
        } else if (pilihan == 6) {
            printf("Logout dari mode admin.\n");
            jedaDanBersihkan();
            return;
        }
    }
}
