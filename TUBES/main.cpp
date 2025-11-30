#include "TUBES.h"

int main() {
    // Inisialisasi struktur data
    initStack(&history);
    initQueue(&playQueue);

    // Muat data awal
    inisialisasiDataAwal();

    int pilihan;

    do {
        clearScreen();
        cout << "\n========================================" << endl;
        cout << "     APLIKASI PEMUTAR MUSIK        " << endl;
        cout << "==========================================" << endl;
        cout << "  1. Login sebagai Admin                " << endl;
        cout << "  2. Login sebagai User                 " << endl;
        cout << "  0. Keluar                             " << endl;
        cout << "==========================================" << endl;
        cout << "Pilihan: ";
        cin >> pilihan;

        switch(pilihan) {
            case 1:
                menuAdmin();
                break;
            case 2:
                menuUser();
                break;
            case 0:
                cout << "\nTerima kasih telah menggunakan aplikasi!" << endl;
                cout << " Sampai jumpa! " << endl;
                break;
            default:
                cout << "\n Pilihan tidak valid!" << endl;
                pause();
        }
    } while(pilihan != 0);

    // Cleanup memory (opsional - bisa ditambahkan fungsi pembersihan)
    // destroyLibrary();
    // destroyPlaylist();

    return 0;
}
