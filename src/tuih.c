#include <stdio.h>
#include <stdlib.h>
#include "tui.h"

void bersihLayar() {
    system("cls");
}

void garis(int panjang) {
    for (int i = 0; i < panjang; i++) {
        printf("=");
    }
    printf("\n");
}

void judul(const char *teks) {
    garis(40);
    printf("  %s\n", teks);
    garis(40);
}

void tunggu() {
    printf("\nTekan Enter untuk lanjut...");
    while (getchar() != '\n');
    getchar();
}