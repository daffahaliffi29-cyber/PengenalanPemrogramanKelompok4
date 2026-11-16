#ifndef TUIH_H
#define TUIH_H

void bersihLayar();
void garis(int panjang);
void judul(const char *teks);
void tampilMenu(const char *teks, const char *pilihan[], int jumlah);
int inputAngka(const char *prompt);
void inputTeks(const char *prompt, char *hasil, int maks);
void pesan(const char *teks);
void tunggu();

#endif
