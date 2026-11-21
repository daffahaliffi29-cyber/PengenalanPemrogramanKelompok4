#ifndef AUTH_H
#define AUTH_H

#include <stdbool.h>
#include <stdint.h>

#define MAX_PANJANG_PW 32
#define MAX_PERCOBAAN_LOGIN 3

// Menghasilkan nilai hash 32-bit menggunakan algoritma DJB2
uint32_t hitungHash(const char *str);

// Memverifikasi login admin dengan batas percobaan (input terminal standar)
bool loginAdmin(void);

#endif
