#include "TUBES.h"
#include <iomanip>
#include <algorithm>
#include <cctype>

// ==================== GLOBAL VARIABLES DEFINITION ====================
addressNodeLagu libraryHead = nullptr;
addressNodeLagu libraryTail = nullptr;
int nextId = 1;

addressLagu currentPlaying = nullptr;
int isPlaying = 0;
addressPlaylist currentPlaylist = nullptr;
addressNodePlaylist currentPlaylistNode = nullptr;

Stack history = {nullptr};
Queue playQueue = {nullptr, nullptr};

// ==================== UTILITY FUNCTIONS ====================

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pause() {
    cout << "\nTekan Enter untuk melanjutkan...";
    cin.ignore();
    cin.get();
}

void toUpperCase(string &str) {
    transform(str.begin(), str.end(), str.begin(), ::toupper);
}

string formatDurasi(int detik) {
    int menit = detik / 60;
    int sisa = detik % 60;
    char buffer[10];
    sprintf(buffer, "%02d:%02d", menit, sisa);
    return string(buffer);
}

// ==================== STACK OPERATIONS ====================

void initStack(Stack *s) {
    s->top = nullptr;
}

void push(Stack *s, addressLagu lagu) {
    addressNodeStack newNode = new NodeStack;
    newNode->lagu = lagu;
    newNode->next = s->top;
    s->top = newNode;
}

addressLagu pop(Stack *s) {
    if(s->top == nullptr) return nullptr;
    addressNodeStack temp = s->top;
    addressLagu lagu = temp->lagu;
    s->top = s->top->next;
    delete temp;
    return lagu;
}

bool isStackEmpty(Stack *s) {
    return s->top == nullptr;
}

// ==================== QUEUE OPERATIONS ====================

void initQueue(Queue *q) {
    q->front = nullptr;
    q->rear = nullptr;
}

void enqueue(Queue *q, addressLagu lagu) {
    addressNodeQueue newNode = new NodeQueue;
    newNode->lagu = lagu;
    newNode->next = nullptr;

    if(q->rear == nullptr) {
        q->front = q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
}

addressLagu dequeue(Queue *q) {
    if(q->front == nullptr) return nullptr;
    addressNodeQueue temp = q->front;
    addressLagu lagu = temp->lagu;
    q->front = q->front->next;
    if(q->front == nullptr) q->rear = nullptr;
    delete temp;
    return lagu;
}

bool isQueueEmpty(Queue *q) {
    return q->front == nullptr;
}

// ==================== LIBRARY OPERATIONS ====================

void tambahLagu(string judul, string artis, string album, string genre, int tahun, int durasi) {
    addressNodeLagu newNode = new NodeLagu;
    newNode->data.id = nextId++;
    newNode->data.judul = judul;
    newNode->data.artis = artis;
    newNode->data.album = album;
    newNode->data.genre = genre;
    newNode->data.tahun = tahun;
    newNode->data.durasi = durasi;
    newNode->next = nullptr;
    newNode->prev = libraryTail;

    if(libraryHead == nullptr) {
        libraryHead = libraryTail = newNode;
    } else {
        libraryTail->next = newNode;
        libraryTail = newNode;
    }

    cout << "\n  Lagu '" << judul << "' berhasil ditambahkan dengan ID: " << newNode->data.id << endl;
}

void lihatSemuaLagu() {
    if(libraryHead == nullptr) {
        cout << "\nLibrary kosong!" << endl;
        return;
    }

    cout << "\n|============================================================================|" << endl;
    cout << "|                           LIBRARY LAGU                                     |" << endl;
    cout << "|============================================================================|" << endl;
    cout << "| ID | Judul                 | Artis             | Genre      | Tahun| Durasi|" << endl;
    cout << "|====|=======================|===================|============|======|=======|" << endl;

    addressNodeLagu current = libraryHead;
    while(current != nullptr) {
        cout << "|" << setw(4) << left << current->data.id
             << "| " << setw(21) << left << current->data.judul.substr(0, 21)
             << "| " << setw(17) << left << current->data.artis.substr(0, 17)
             << "| " << setw(10) << left << current->data.genre.substr(0, 10)
             << " | " << setw(4) << left << current->data.tahun
             << " | " << setw(5) << left << formatDurasi(current->data.durasi)
             << " |" << endl;
        current = current->next;
    }
    cout << "|===========================================================================|" << endl;
}

addressNodeLagu cariLaguById(int id) {
    addressNodeLagu current = libraryHead;
    while(current != nullptr) {
        if(current->data.id == id) return current;
        current = current->next;
    }
    return nullptr;
}

addressNodeLagu cariLaguByJudul(string judul) {
    addressNodeLagu current = libraryHead;
    string upperJudul = judul;
    toUpperCase(upperJudul);

    while(current != nullptr) {
        string currentJudul = current->data.judul;
        toUpperCase(currentJudul);

        if(currentJudul.find(upperJudul) != string::npos) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

void updateLagu(int id) {
    addressNodeLagu node = cariLaguById(id);
    if(node == nullptr) {
        cout << "\nLagu dengan ID " << id << " tidak ditemukan!" << endl;
        return;
    }

    cout << "\n=== Update Lagu ID: " << id << " ===" << endl;
    cout << "Data saat ini:" << endl;
    cout << "Judul: " << node->data.judul << endl;
    cout << "Artis: " << node->data.artis << endl;
    cout << "Album: " << node->data.album << endl;
    cout << "Genre: " << node->data.genre << endl;
    cout << "Tahun: " << node->data.tahun << endl;

    cout << "\nMasukkan data baru (tekan Enter untuk skip):" << endl;

    string buffer;
    cin.ignore();

    cout << "Judul baru: ";
    getline(cin, buffer);
    if(!buffer.empty()) node->data.judul = buffer;

    cout << "Artis baru: ";
    getline(cin, buffer);
    if(!buffer.empty()) node->data.artis = buffer;

    cout << "Album baru: ";
    getline(cin, buffer);
    if(!buffer.empty()) node->data.album = buffer;

    cout << "Genre baru: ";
    getline(cin, buffer);
    if(!buffer.empty()) node->data.genre = buffer;

    cout << "Tahun baru (0 untuk skip): ";
    int tahun;
    cin >> tahun;
    if(tahun != 0) node->data.tahun = tahun;

    cout << "\n  Lagu berhasil diupdate!" << endl;
}

void hapusLagu(int id) {
    addressNodeLagu node = cariLaguById(id);
    if(node == nullptr) {
        cout << "\nLagu dengan ID " << id << " tidak ditemukan!" << endl;
        return;
    }

    string judulLagu = node->data.judul;

    // Update linked list
    if(node->prev != nullptr) {
        node->prev->next = node->next;
    } else {
        libraryHead = node->next;
    }

    if(node->next != nullptr) {
        node->next->prev = node->prev;
    } else {
        libraryTail = node->prev;
    }

    cout << "\n  Lagu '" << judulLagu << "' berhasil dihapus!" << endl;
    delete node;
}

int hitungJumlahLagu() {
    int count = 0;
    addressNodeLagu current = libraryHead;
    while(current != nullptr) {
        count++;
        current = current->next;
    }
    return count;
}

// ==================== PLAYLIST OPERATIONS ====================

addressPlaylist buatPlaylist(string nama) {
    addressPlaylist pl = new Playlist;
    pl->nama = nama;
    pl->head = nullptr;
    pl->jumlahLagu = 0;
    return pl;
}

void tambahKePlaylist(addressPlaylist pl, int idLagu) {
    addressNodeLagu lagu = cariLaguById(idLagu);
    if(lagu == nullptr) {
        cout << "\nLagu dengan ID " << idLagu << " tidak ditemukan!" << endl;
        return;
    }

    addressNodePlaylist newNode = new NodePlaylist;
    newNode->lagu = &(lagu->data);
    newNode->next = nullptr;

    if(pl->head == nullptr) {
        pl->head = newNode;
    } else {
        addressNodePlaylist current = pl->head;
        while(current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
    }

    pl->jumlahLagu++;
    cout << "\n  Lagu '" << lagu->data.judul << "' ditambahkan ke playlist '" << pl->nama << "'" << endl;
}

void hapusDariPlaylist(addressPlaylist pl, int posisi) {
    if(pl == nullptr || pl->head == nullptr) {
        cout << "\nPlaylist kosong!" << endl;
        return;
    }

    if(posisi < 1 || posisi > pl->jumlahLagu) {
        cout << "\nPosisi tidak valid!" << endl;
        return;
    }

    addressNodePlaylist current = pl->head;
    addressNodePlaylist prev = nullptr;

    if(posisi == 1) {
        pl->head = current->next;
        delete current;
    } else {
        for(int i = 1; i < posisi; i++) {
            prev = current;
            current = current->next;
        }
        prev->next = current->next;
        delete current;
    }

    pl->jumlahLagu--;
    cout << "\n  Lagu berhasil dihapus dari playlist!" << endl;
}

void lihatPlaylist(addressPlaylist pl) {
    if(pl == nullptr || pl->head == nullptr) {
        cout << "\nPlaylist kosong!" << endl;
        return;
    }

    cout << "\n|============================================================|" << endl;
    cout << "|              PLAYLIST: " << setw(35) << left << pl->nama << " |" << endl;
    cout << "|===========================================================|" << endl;
    cout << "| No | Judul                 | Artis             | Durasi   |" << endl;
    cout << "|===========================================================|" << endl;

    addressNodePlaylist current = pl->head;
    int no = 1;
    while(current != nullptr) {
        cout << "|" << setw(4) << left << no++
             << "| " << setw(21) << left << current->lagu->judul.substr(0, 21)
             << "| " << setw(17) << left << current->lagu->artis.substr(0, 17)
             << "| " << setw(8) << left << formatDurasi(current->lagu->durasi)
             << " |" << endl;
        current = current->next;
    }
    cout << "|===========================================================|" << endl;
    cout << "Total: " << pl->jumlahLagu << " lagu" << endl;
}

void hapusPlaylist(addressPlaylist pl) {
    if(pl == nullptr) return;

    addressNodePlaylist current = pl->head;
    while(current != nullptr) {
        addressNodePlaylist temp = current;
        current = current->next;
        delete temp;
    }
    delete pl;
}

// ==================== PLAYER OPERATIONS ====================

void playLagu(addressLagu lagu) {
    if(currentPlaying != nullptr && isPlaying) {
        push(&history, currentPlaying);
    }

    currentPlaying = lagu;
    isPlaying = 1;

    cout << "\n NOW PLAYING " << endl;
    cout << "=======================================" << endl;
    cout << lagu->judul << endl;
    cout << lagu->artis << endl;
    cout << lagu->album << endl;
    cout << lagu->genre << " | " << lagu->tahun << endl;
    cout << formatDurasi(lagu->durasi) << endl;
    cout << "=======================================" << endl;
}

void stopLagu() {
    if(currentPlaying == nullptr) {
        cout << "\nTidak ada lagu yang sedang diputar!" << endl;
        return;
    }

    cout << "\n Lagu '" << currentPlaying->judul << "' dihentikan." << endl;
    isPlaying = 0;
}

addressLagu cariLaguMirip(addressLagu referensi) {
    if(referensi == nullptr) return nullptr;

    // Prioritas: artis sama > genre sama > tahun dekade sama
    addressNodeLagu current = libraryHead;

    // Cari artis sama dulu
    while(current != nullptr) {
        if(current->data.id != referensi->id &&
           current->data.artis == referensi->artis) {
            return &(current->data);
        }
        current = current->next;
    }

    // Cari genre sama
    current = libraryHead;
    while(current != nullptr) {
        if(current->data.id != referensi->id &&
           current->data.genre == referensi->genre) {
            return &(current->data);
        }
        current = current->next;
    }

    // Cari tahun dekade sama
    current = libraryHead;
    int dekadeRef = (referensi->tahun / 10) * 10;
    while(current != nullptr) {
        int dekadeCurrent = (current->data.tahun / 10) * 10;
        if(current->data.id != referensi->id && dekadeCurrent == dekadeRef) {
            return &(current->data);
        }
        current = current->next;
    }

    return nullptr;
}

void nextLagu() {
    if(currentPlaylist != nullptr && currentPlaylistNode != nullptr) {
        // Mode playlist
        if(currentPlaylistNode->next != nullptr) {
            currentPlaylistNode = currentPlaylistNode->next;
            playLagu(currentPlaylistNode->lagu);
        } else {
            cout << "\n Sudah di akhir playlist!" << endl;
        }
    } else {
        // Mode library - cari lagu mirip
        if(currentPlaying == nullptr) {
            cout << "\nTidak ada lagu yang sedang diputar!" << endl;
            return;
        }

        addressLagu mirip = cariLaguMirip(currentPlaying);
        if(mirip != nullptr) {
            playLagu(mirip);
        } else {
            cout << "\n Tidak ada lagu mirip yang ditemukan!" << endl;
        }
    }
}

void prevLagu() {
    if(currentPlaylist != nullptr) {
        // Mode playlist - cari node sebelumnya
        if(currentPlaylistNode != nullptr) {
            addressNodePlaylist temp = currentPlaylist->head;
            addressNodePlaylist prev = nullptr;

            while(temp != nullptr && temp != currentPlaylistNode) {
                prev = temp;
                temp = temp->next;
            }

            if(prev != nullptr) {
                currentPlaylistNode = prev;
                playLagu(currentPlaylistNode->lagu);
            } else {
                cout << "\n Sudah di awal playlist!" << endl;
            }
        }
    } else {
        // Mode library - ambil dari history
        addressLagu prev = pop(&history);
        if(prev != nullptr) {
            currentPlaying = prev;
            isPlaying = 1;
            playLagu(prev);
        } else {
            cout << "\n Tidak ada lagu sebelumnya!" << endl;
        }
    }
}

void tampilkanNowPlaying() {
    if(isPlaying && currentPlaying != nullptr) {
        cout << "\n Sedang memutar: " << currentPlaying->judul
             << " - " << currentPlaying->artis << endl;
    }
}

// ==================== MENU FUNCTIONS ====================

void menuAdmin() {
    int pilihan;
    do {
        clearScreen();
        cout << "\n|========================================|" << endl;
        cout << "|          MENU ADMIN                    |" << endl;
        cout << "|========================================|" << endl;
        cout << "|  1. Tambah Lagu                        |" << endl;
        cout << "|  2. Lihat Semua Lagu                   |" << endl;
        cout << "|  3. Update Lagu                        |" << endl;
        cout << "|  4. Hapus Lagu                         |" << endl;
        cout << "|  5. Statistik Library                  |" << endl;
        cout << "|  0. Kembali                            |" << endl;
        cout << "|========================================|" << endl;
        cout << "Pilihan: ";
        cin >> pilihan;

        switch(pilihan) {
            case 1: {
                string judul, artis, album, genre;
                int tahun, durasi;

                cout << "\n=== Tambah Lagu Baru ===" << endl;
                cin.ignore();
                cout << "Judul: ";
                getline(cin, judul);

                cout << "Artis: ";
                getline(cin, artis);

                cout << "Album: ";
                getline(cin, album);

                cout << "Genre: ";
                getline(cin, genre);

                cout << "Tahun: ";
                cin >> tahun;

                cout << "Durasi (detik): ";
                cin >> durasi;

                tambahLagu(judul, artis, album, genre, tahun, durasi);
                pause();
                break;
            }
            case 2:
                lihatSemuaLagu();
                pause();
                break;
            case 3: {
                int id;
                lihatSemuaLagu();
                cout << "\nMasukkan ID lagu yang akan diupdate: ";
                cin >> id;
                updateLagu(id);
                pause();
                break;
            }
            case 4: {
                int id;
                lihatSemuaLagu();
                cout << "\nMasukkan ID lagu yang akan dihapus: ";
                cin >> id;
                hapusLagu(id);
                pause();
                break;
            }
            case 5: {
                cout << "\n=== STATISTIK LIBRARY ===" << endl;
                cout << "Total Lagu: " << hitungJumlahLagu() << endl;
                pause();
                break;
            }
        }
    } while(pilihan != 0);
}

void menuUser() {
    int pilihan;
    static addressPlaylist myPlaylist = nullptr;

    do {
        clearScreen();
        cout << "\n|========================================|" << endl;
        cout << "|          MENU USER                     |" << endl;
        cout << "|========================================|" << endl;
        cout << "|  1. Lihat Library                      |" << endl;
        cout << "|  2. Cari Lagu                          |" << endl;
        cout << "|  3. Play Lagu                          |" << endl;
        cout << "|  4. Stop Lagu                          |" << endl;
        cout << "|  5. Next Lagu                          |" << endl;
        cout << "|  6. Previous Lagu                      |" << endl;
        cout << "|  7. Buat/Kelola Playlist               |" << endl;
        cout << "|  0. Kembali                            |" << endl;
        cout << "|========================================|" << endl;

        tampilkanNowPlaying();

        cout << "\nPilihan: ";
        cin >> pilihan;

        switch(pilihan) {
            case 1:
                lihatSemuaLagu();
                pause();
                break;
            case 2: {
                string judul;
                cout << "\nMasukkan judul lagu: ";
                cin.ignore();
                getline(cin, judul);

                addressNodeLagu hasil = cariLaguByJudul(judul);
                if(hasil != nullptr) {
                    cout << "\n Lagu ditemukan!" << endl;
                    cout << "ID: " << hasil->data.id << " | "
                         << hasil->data.judul << " - " << hasil->data.artis << endl;
                } else {
                    cout << "\n Lagu tidak ditemukan!" << endl;
                }
                pause();
                break;
            }
            case 3: {
                int id;
                lihatSemuaLagu();
                cout << "\nMasukkan ID lagu: ";
                cin >> id;
                addressNodeLagu lagu = cariLaguById(id);
                if(lagu != nullptr) {
                    playLagu(&(lagu->data));
                    currentPlaylist = nullptr;
                    currentPlaylistNode = nullptr;
                } else {
                    cout << "\nLagu tidak ditemukan!" << endl;
                }
                pause();
                break;
            }
            case 4:
                stopLagu();
                pause();
                break;
            case 5:
                nextLagu();
                pause();
                break;
            case 6:
                prevLagu();
                pause();
                break;
            case 7: {
                int subPilihan;
                do {
                    clearScreen();
                    cout << "\n=== KELOLA PLAYLIST ===" << endl;
                    cout << "1. Buat Playlist" << endl;
                    cout << "2. Tambah Lagu ke Playlist" << endl;
                    cout << "3. Hapus Lagu dari Playlist" << endl;
                    cout << "4. Lihat Playlist" << endl;
                    cout << "5. Play dari Playlist" << endl;
                    cout << "0. Kembali" << endl;
                    cout << "Pilihan: ";
                    cin >> subPilihan;

                    switch(subPilihan) {
                        case 1: {
                            string nama;
                            cout << "Nama playlist: ";
                            cin.ignore();
                            getline(cin, nama);
                            myPlaylist = buatPlaylist(nama);
                            cout << "\n  Playlist '" << nama << "' berhasil dibuat!" << endl;
                            pause();
                            break;
                        }
                        case 2: {
                            if(myPlaylist == nullptr) {
                                cout << "\nBuat playlist dulu!" << endl;
                            } else {
                                int id;
                                lihatSemuaLagu();
                                cout << "\nMasukkan ID lagu: ";
                                cin >> id;
                                tambahKePlaylist(myPlaylist, id);
                            }
                            pause();
                            break;
                        }
                        case 3: {
                            if(myPlaylist != nullptr) {
                                lihatPlaylist(myPlaylist);
                                int posisi;
                                cout << "\nMasukkan nomor lagu yang akan dihapus: ";
                                cin >> posisi;
                                hapusDariPlaylist(myPlaylist, posisi);
                            } else {
                                cout << "\nBelum ada playlist!" << endl;
                            }
                            pause();
                            break;
                        }
                        case 4:
                            if(myPlaylist != nullptr) {
                                lihatPlaylist(myPlaylist);
                            } else {
                                cout << "\nBelum ada playlist!" << endl;
                            }
                            pause();
                            break;
                        case 5: {
                            if(myPlaylist != nullptr && myPlaylist->head != nullptr) {
                                currentPlaylist = myPlaylist;
                                currentPlaylistNode = myPlaylist->head;
                                playLagu(currentPlaylistNode->lagu);
                            } else {
                                cout << "\nPlaylist kosong!" << endl;
                            }
                            pause();
                            break;
                        }
                    }
                } while(subPilihan != 0);
                break;
            }
        }
    } while(pilihan != 0);
}

void inisialisasiDataAwal() {
    cout << "Menginisialisasi data awal..." << endl;

    // Pop Indonesia
    tambahLagu("Komang", "Raim Laode", "Single", "Pop", 2023, 268);
    tambahLagu("Lagu Untuk Kamu", "Tiara Andini", "Single", "Pop", 2023, 234);
    tambahLagu("Sisa Rasa", "Mahalini", "Single", "Pop", 2022, 267);

    // Pop International
    tambahLagu("Shape of You", "Ed Sheeran", "Divide", "Pop", 2017, 233);
    tambahLagu("Blinding Lights", "The Weeknd", "After Hours", "Pop", 2020, 200);
    tambahLagu("Perfect", "Ed Sheeran", "Divide", "Pop", 2017, 263);
    tambahLagu("Someone Like You", "Adele", "21", "Pop", 2011, 285);

    // Rock
    tambahLagu("Bohemian Rhapsody", "Queen", "A Night at the Opera", "Rock", 1975, 354);
    tambahLagu("Stairway to Heaven", "Led Zeppelin", "Led Zeppelin IV", "Rock", 1971, 482);
    tambahLagu("Hotel California", "Eagles", "Hotel California", "Rock", 1976, 391);

    // Jazz
    tambahLagu("Fly Me to the Moon", "Frank Sinatra", "It Might as Well Be Swing", "Jazz", 1964, 148);
    tambahLagu("What a Wonderful World", "Louis Armstrong", "Single", "Jazz", 1967, 139);

    cout << "Data awal berhasil dimuat!" << endl;
}
