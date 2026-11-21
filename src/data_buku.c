#include "buku.h"

static const Buku DATA_BUKU_AWAL[] = {
    {"BK-001", "The ngawi in the sun", "Mas Amba et al", "fiksi ilmiah", 125, 10, true},
    {"BK-002", "Clean Code", "Robert C. Martin", "Pemrograman", 150, 7, true},
    {"BK-003", "Filosofi Teras", "Henry Manampiring", "Filsafat", 950, 15, true},
    {"BK-004", "Laut Bercerita", "Leila S. Chudori", "Novel", 110, 0, true},
    {"BK-005", "Structure and Interpretation of Computer Programs", "Harold Abelson", "Pemrograman", 185, 4, true},
    {"BK-006", "How to become guy ( not gau)", "Justin pekalongan", "Filsafat", 135, 8, true},
    {"BK-007", "Atomic Habits", "James Clear", "Pengembangan Diri", 108, 20, true},
    {"BK-008", "Bumi Manusia", "Pramoedya Ananta Toer", "Sastra", 140, 6, true},
    {"BK-009", "Introduction to Algorithms", "Thomas H. Cormen", "Ilmu Komputer", 250, 3, true},
    {"BK-010", "Sapiens: Riwayat Singkat Umat Manusia", "Yuval Noah Harari", "Sejarah", 160, 12, true},
    {"BK-011", "Lima ngewu", "TESTING", "TESTING_test", 5000, 12, true}
};

int muatDataBukuAwal(Buku katalog[]) {
    int jumlahAwal = (int)(sizeof(DATA_BUKU_AWAL) / sizeof(DATA_BUKU_AWAL[0]));
    if (jumlahAwal > MAX_BUKU) {
        jumlahAwal = MAX_BUKU;
    }

    for (int i = 0; i < jumlahAwal; i++) {
        katalog[i] = DATA_BUKU_AWAL[i];
    }
    return jumlahAwal;
}
