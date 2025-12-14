#include <iostream>
#include "PLAYLIST.h"

int main() {
    // Inisialisasi struktur data
    initStack(&history);
    initQueue(&playQueue);

    // Muat data awal
    inisialisasiDataAwal();
    inisialisasiDataUser();

    int pilihan;
    char username[50], password[50];  // ← DEKLARASI SEKALI DI SINI

    do {
        clearScreen();
        cout << "\n==============================================================" << endl;
        cout << "                           Celestia                           " << endl;
        cout << "                     APLIKASI PEMUTAR MUSIK                   " << endl;
        cout << "==============================================================" << endl;
        cout << "  1. Login sebagai Admin                                      " << endl;
        cout << "  2. Login sebagai User                                       " << endl;
        cout << "  3. Registrasi User Baru                                     " << endl;
        cout << "  0. Keluar                                                   " << endl;
        cout << "==============================================================" << endl;
        cout << "  Pilihan: ";
        cin >> pilihan;

        switch(pilihan) {
            case 1:
                cout << "\n========== LOGIN ADMIN ==========" << endl;
                cout << "Username: ";
                cin >> username;
                cout << "Password: ";
                cin >> password;

                if(stringCompare(username, "admin") == 0 &&
                   stringCompare(password, "adminCakep") == 0) {
                    cout << "\nLogin berhasil! Selamat datang Admin!" << endl;
                    pause();
                    menuAdmin();
                } else {
                    cout << "\nUsername atau password salah!" << endl;
                    pause();
                }
                break;
            case 2:
                cout << "\n========== LOGIN USER ==========" << endl;
                cout << "Username: ";
                cin >> username;
                cout << "Password: ";
                cin >> password;

                if(loginUser(username, password)) {
                    cout << "\nLogin berhasil! Halo " << username << ", selamat datang di Celestia!" << endl;
                    pause();
                    menuUser();
                } else {
                    cout << "\nUsername atau password salah!" << endl;
                    pause();
                }
                break;
            case 3:
                registrasiUser();
                break;
            case 0:
                cout << "\n========================================================================" << endl;
                cout << "            Terima kasih telah menggunakan aplikasi Celestia!           " << endl;
                cout << "                       Sampai jumpa lagi di Celestia!                   " << endl;
                cout << "========================================================================" << endl;
                break;

            default:
                cout << "\n Pilihan tidak valid!" << endl;
                pause();
        }
    }
    while(pilihan != 0);
    return 0;
}
