# Sistem Informasi & Kasir Toko Buku (Versi Terminal Standar / CLI)

Program kasir dan manajemen katalog toko buku berbasis bahasa C menggunakan antarmuka terminal standar (`stdio` biasa tanpa TUI dan tanpa fitur QRIS), dengan pembersihan layar otomatis (`cls` / `clear`) setelah setiap aksi.

## Struktur Folder
- `include/` : Header file (`auth.h`, `buku.h`, `cli.h`, `transaksi.h`)
- `src/`     : Implementasi kode sumber (`main.c`, `cli.c`, `buku.c`, `data_buku.c`, `transaksi.c`, `auth.c`)
- `Makefile` : Skrip kompilasi otomatis

## Cara Kompilasi & Menjalankan

### Windows (MinGW / GCC)
```bash
gcc -Wall -Wextra -std=c11 -Iinclude src/*.c -o toko_buku.exe
toko_buku.exe
```

### Linux / macOS
```bash
gcc -Wall -Wextra -std=c11 -Iinclude src/*.c -o toko_buku
./toko_buku
```

## Akun Default Admin
- **Password Admin**: `Admin123`
