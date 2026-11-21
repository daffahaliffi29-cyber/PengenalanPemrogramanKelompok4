# Sistem Informasi & Kasir Toko Buku (CLI)

Aplikasi konsol kasir dan manajemen katalog toko buku berbasis bahasa C (C11) untuk proyek **Pengenalan Pemrograman — Kelompok 4**. Program ini dibangun dengan arsitektur modular multi-file tanpa library eksternal, dilengkapi validasi input terminal standar (`stdio`) serta pembersihan layar otomatis lintas platform (`cls` pada Windows dan `clear` pada Linux/macOS).

## Fitur Utama

### Mode Pembeli
* **Katalog & Pencarian Buku:** Menampilkan daftar buku aktif beserta harga dan stok, serta pencarian parsial (*case-insensitive*) berdasarkan judul maupun kode buku.
* **Detail & Keranjang Belanja:** Menampilkan informasi lengkap buku dan sisa stok tersedia setelah dikurangi isi keranjang. Mendukung penggabungan kuantitas item yang sama (*cart merging*), penghapusan item tertentu, dan pengosongan keranjang (maksimal 20 jenis buku per transaksi).
* **Checkout & Pembayaran Tunai:** Menghitung total tagihan secara otomatis, memvalidasi nominal uang bayar, menghitung kembalian, mengurangi stok katalog, serta mencetak struk transaksi (`TRX-001`, dst.).

### Mode Admin
* **Autentikasi Terproteksi:** Verifikasi kata sandi admin menggunakan algoritma *hashing* **DJB2** dengan batas maksimal 3 kali percobaan login.
* **Manajemen Katalog & Stok:** Menambah stok buku (*restock*), menambahkan judul buku baru dengan validasi kode unik (kapasitas hingga 150 judul), serta menghapus buku dari etalase menggunakan metode *soft delete*.
* **Laporan Penjualan:** Menampilkan jumlah transaksi berhasil, total omzet kotor, dan riwayat seluruh transaksi pada sesi berjalan (hingga 100 transaksi).

## Struktur Direktori

```text
PengenalanPemrogramanKelompok4/
├── include/
│   ├── auth.h          # Deklarasi fungsi hashing DJB2 dan login admin
│   ├── buku.h          # Struktur data Buku dan prototipe operasi katalog
│   ├── cli.h           # Deklarasi kontrol layar dan antarmuka terminal
│   └── transaksi.h     # Struktur data ItemKeranjang, Transaksi, dan checkout
├── src/
│   ├── auth.c          # Implementasi autentikasi admin
│   ├── buku.c          # Implementasi pencarian, tambah, restock, dan hapus buku
│   ├── cli.c           # Implementasi alur menu Mode Pembeli dan Mode Admin
│   ├── data_buku.c     # Data awal katalog buku (BK-001 s.d. BK-011)
│   ├── main.c          # Titik masuk program (menu utama)
│   └── transaksi.c     # Implementasi keranjang belanja, checkout, dan omzet
├── Makefile            # Otomatisasi kompilasi proyek
└── README.md           # Dokumentasi proyek
```

## Cara Kompilasi & Menjalankan

Pastikan kompiler **GCC** (atau MinGW pada Windows) sudah terpasang di sistem.

### Menggunakan Make
```bash
make
./toko_buku
```
Untuk menghapus hasil kompilasi:
```bash
make clean
```

### Kompilasi Manual (GCC)

**Windows (Command Prompt / PowerShell):**
```bash
gcc -Wall -Wextra -std=c11 -Iinclude src/*.c -o toko_buku.exe
toko_buku.exe
```

**Linux / macOS:**
```bash
gcc -Wall -Wextra -std=c11 -Iinclude src/*.c -o toko_buku
./toko_buku
```

## Kredensial & Data Uji Default

* **Password Mode Admin:** `Admin123`
* **Kode Buku Awal:** `BK-001` sampai `BK-011` (contoh: `BK-002` untuk *Clean Code*, `BK-007` untuk *Atomic Habits*).
