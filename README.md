# Pengenalan Pemrograman Kelompok 4

Repository ini berisi proyek pemrograman berbasis bahasa C yang dibuat untuk pembelajaran konsep dasar pemrograman dan pengelolaan program menggunakan struktur modular.

## Deskripsi

Proyek ini merupakan aplikasi console sederhana yang berfokus pada sistem toko buku. Saat ini repository mencakup modul autentikasi login dan utility antarmuka (TUI) untuk menampilkan form dan menu program.

## Fitur

- Login sistem dengan username dan password default
- Antarmuka konsol sederhana
- Struktur proyek terpisah berdasarkan fungsi:
  - `include/` untuk header file
  - `src/` untuk implementasi program
- Dapat dikembangkan menjadi sistem manajemen buku, transaksi, dan admin.

## Struktur Project

```text
PengenalanPemrogramanKelompok4/
├── include/
│   ├── auth.h
│   ├── buku.h
│   ├── main.h
│   ├── transaksi.h
│   ├── data_buku.h
│   └── tuih.h
├── src/
│   ├── auth.c
│   ├── buku.c
│   ├── main.c
│   ├── transaksi.c
│   ├── data_buku.c
│   └── tuih.c
└── README.md

