#include "PLAYLIST.h"

// ==================== GLOBAL VARIABLES DEFINITION ====================
addressNodeLagu libraryHead = nullptr;
addressNodeLagu libraryTail = nullptr;
int nextId = 1;

addressNodeUser userHead = nullptr;
char currentUsername[50] = "";

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

void clearInputBuffer();

void pause() {
    cout << "\n[Tekan Enter untuk melanjutkan...]";
    clearInputBuffer();
    cin.get();
}

void clearInputBuffer() {
    cin.clear();
    cin.ignore(10000, '\n');
}

bool validateInput() {
    if(cin.fail()) {
        clearInputBuffer();
        return false;
    }
    return true;
}

void toUpperCase(char *str) {
    for(int i = 0; str[i] != '\0'; i++) {
        if(str[i] >= 'a' && str[i] <= 'z') {
            str[i] = str[i] - 32;
        }
    }
}

void formatDurasi(int detik, char *buffer) {
    int menit = detik / 60;
    int sisa = detik % 60;

    buffer[0] = (menit / 10) + '0';
    buffer[1] = (menit % 10) + '0';
    buffer[2] = ':';
    buffer[3] = (sisa / 10) + '0';
    buffer[4] = (sisa % 10) + '0';
    buffer[5] = '\0';
}

int stringCompare(const char *str1, const char *str2) {
    int i = 0;
    while(str1[i] != '\0' && str2[i] != '\0') {
        if(str1[i] != str2[i]) {
            return str1[i] - str2[i];
        }
        i++;
    }
    return str1[i] - str2[i];
}

bool stringContains(const char *str, const char *src) {
    int i = 0;
    int j = 0;
    while(str[i] != '\0') {
        j = 0;
        int temp = i;
        while(src[j] != '\0' && str[temp] == src[j]) {
            temp++;
            j++;
        }
        if(src[j] == '\0') {
            return true;
        }
        i++;
    }
    return false;
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

void clearStack(Stack *s) {
    while(!isStackEmpty(s)) {
        pop(s);
    }
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
    if(q->front == nullptr) {
        return 1;
    }
    return 0;
}

// ==================== LIBRARY OPERATIONS ====================

void stringCopy(char *dest, const char *src) {
    int i = 0;
    while(src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';  // Jangan lupa null terminator
}

void tambahLagu(const char *judul, const char *artis, const char *album, const char *genre, int tahun, int durasi) {
    addressNodeLagu newNode = new NodeLagu;
    newNode->data.id = nextId++;
    stringCopy(newNode->data.judul, judul);
    stringCopy(newNode->data.artis, artis);
    stringCopy(newNode->data.album, album);
    stringCopy(newNode->data.genre, genre);
    newNode->data.tahun = tahun;
    newNode->data.durasi = durasi;
    newNode->data.isFavorit = 0;
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

int stringLength(const char *str) {
    int len = 0;
    while(str[len] != '\0') {
        len++;
    }
    return len;
}

void printWithWidth(const char *str, int width) {
    int len = stringLength(str);
    if(len > width) {
        for(int i = 0; i < width; i++) {
            cout << str[i];
        }
    } else {
        cout << str;
        for(int i = len; i < width; i++) {
            cout << " ";
        }
    }
}

void lihatSemuaLagu() {
    if(libraryHead == nullptr) {
        cout << "\nLibrary kosong!" << endl;
        return;
    }
    cout << "\n|============================================================================|" << endl;
    cout << "|                                    LIBRARY LAGU                            |" << endl;
    cout << "|============================================================================|" << endl;
    cout << "| ID | Judul                 | Artis             | Genre      | Tahun| Durasi|" << endl;
    cout << "|====|=======================|===================|============|======|=======|" << endl;

    addressNodeLagu current = libraryHead;
    char durasi[10];

    while(current != nullptr) {
        formatDurasi(current->data.durasi, durasi);

        cout << "|";
        if(current->data.id < 10) cout << "   ";
        else if(current->data.id < 100) cout << "  ";
        else if(current->data.id < 1000) cout << " ";
        cout << current->data.id << "| ";

        printWithWidth(current->data.judul, 21);
        cout << " | ";
        printWithWidth(current->data.artis, 17);
        cout << " | ";
        printWithWidth(current->data.genre, 10);
        cout << " | " << current->data.tahun << " | ";
        printWithWidth(durasi, 5);
        cout << " |" << endl;

        current = current->next;
    }
    cout << "|============================================================================|" << endl;
}

addressNodeLagu cariLaguById(int id) {
    addressNodeLagu current = libraryHead;
    while(current != nullptr) {
        if(current->data.id == id) return current;
        current = current->next;
    }
    return nullptr;
}

addressNodeLagu cariLaguByJudul(const char *judul) {
    addressNodeLagu current = libraryHead;
    char upperJudul[100];
    stringCopy(upperJudul, judul);
    toUpperCase(upperJudul);
    while(current != nullptr) {
        char currentJudul[100];
        stringCopy(currentJudul, current->data.judul);
        toUpperCase(currentJudul);
        if(stringContains(currentJudul, upperJudul)) {
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

    char buffer[100];
    cin.ignore();

    cout << "Judul baru: ";
    cin.getline(buffer, 100);
    if(stringLength(buffer) > 0) stringCopy(node->data.judul, buffer);

    cout << "Artis baru: ";
    cin.getline(buffer, 100);
    if(stringLength(buffer) > 0) stringCopy(node->data.artis, buffer);

    cout << "Album baru: ";
    cin.getline(buffer, 100);
    if(stringLength(buffer) > 0) stringCopy(node->data.album, buffer);

    cout << "Genre baru: ";
    cin.getline(buffer, 100);
    if(stringLength(buffer) > 0) stringCopy(node->data.genre, buffer);

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
    char judulLagu[100];
    stringCopy(judulLagu, node->data.judul);

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

addressPlaylist buatPlaylist(const char *nama) {
    addressPlaylist pl = new Playlist;
    stringCopy(pl->nama, nama);
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
    cout << "|              PLAYLIST: ";
    printWithWidth(pl->nama, 35);
    cout << " |" << endl;
    cout << "|============================================================|" << endl;
    cout << "| No | Judul                 | Artis             | Durasi   |" << endl;
    cout << "|============================================================|" << endl;

    addressNodePlaylist current = pl->head;
    int no = 1;
    char durasi[10];

    while(current != nullptr) {
        formatDurasi(current->lagu->durasi, durasi);

        cout << "|";
        if(no < 10) cout << "   ";
        else if(no < 100) cout << "  ";
        else if(no < 1000) cout << " ";
        cout << no++ << "| ";

        printWithWidth(current->lagu->judul, 21);
        cout << "| ";
        printWithWidth(current->lagu->artis, 17);
        cout << "| ";
        printWithWidth(durasi, 8);
        cout << " |" << endl;

        current = current->next;
    }
    cout << "|============================================================|" << endl;
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

    char durasi[10];
    formatDurasi(lagu->durasi, durasi);

    clearScreen();
    cout << "\n===============================================" << endl;
    cout << "              NOW PLAYING                     " << endl;
    cout << "===============================================" << endl;
    cout << "  Judul  : " << lagu->judul << endl;
    cout << "  Artis  : " << lagu->artis << endl;
    cout << "  Album  : " << lagu->album << endl;
    cout << "  Genre  : " << lagu->genre << endl;
    cout << "  Tahun  : " << lagu->tahun << endl;
    cout << "  Durasi : " << durasi << endl;
    cout << "===============================================" << endl;
    cout << "\n          Status: SEDANG DIPUTAR          " << endl;
    cout << "\n-----------------------------------------------" << endl;
    cout << "Catatan: Ini adalah SIMULASI pemutaran musik." << endl;
    cout << "-----------------------------------------------" << endl;
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

    addressNodeLagu current = libraryHead;

    while(current != nullptr) {
        if(current->data.id != referensi->id &&
           stringCompare(current->data.artis, referensi->artis) == 0) {
            return &(current->data);
        }
        current = current->next;
    }
    current = libraryHead;
    while(current != nullptr) {
        if(current->data.id != referensi->id &&
           stringCompare(current->data.genre, referensi->genre) == 0) {
            return &(current->data);
        }
        current = current->next;
    }
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
        if(currentPlaylistNode->next != nullptr) {
            currentPlaylistNode = currentPlaylistNode->next;
            playLagu(currentPlaylistNode->lagu);
        } else {
            cout << "\n Sudah di akhir playlist!" << endl;
        }
    } else {
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
    cout << "\n=========================================================" << endl;
    if(isPlaying && currentPlaying != nullptr) {
        cout << "Sedang memutar: " << currentPlaying->judul
             << " - " << currentPlaying->artis << " " << endl;
    } else {
        cout << "Tidak ada lagu yang sedang diputar nih untuk sekarang" << endl;
    }
    cout << "=========================================================" << endl;
}

// ==================== USER MANAGEMENT ====================

void tambahUser(const char *username, const char *password) {
    addressNodeUser newNode = new NodeUser;
    stringCopy(newNode->data.username, username);
    stringCopy(newNode->data.password, password);
    newNode->next = userHead;
    userHead = newNode;
}

void inisialisasiDataUser() {
    cout << "Menginisialisasi data user..." << endl;

    // 10 user dummy
    tambahUser("user1", "pass123");
    tambahUser("user2", "pass456");
    tambahUser("user3", "pass789");
    tambahUser("habibie", "habibie123");
    tambahUser("anjaly", "anjaly456");
    tambahUser("shofi", "shofi321");
    tambahUser("8gacorr", "yakaliGaGacor");
    tambahUser("doni", "doni456");
    tambahUser("lina", "lina789");
    tambahUser("andi", "andi123");

    cout << "Data user berhasil dimuat!" << endl;
}

bool loginUser(const char *username, const char *password) {
    addressNodeUser current = userHead;

    while(current != nullptr) {
        if(stringCompare(current->data.username, username) == 0 &&
           stringCompare(current->data.password, password) == 0) {
            stringCopy(currentUsername, username);  // Simpan username yang login
            return true;
        }
        current = current->next;
    }

    return false;
}

bool cekUsernameExists(const char *username) {
    addressNodeUser current = userHead;

    while(current != nullptr) {
        if(stringCompare(current->data.username, username) == 0) {
            return true;  // Username sudah ada
        }
        current = current->next;
    }
    return false;  // Username belum ada
}

void registrasiUser() {
    char username[50], password[50], konfirmasiPassword[50];

    clearScreen();
    cout << "\n===============================================" << endl;
    cout << "            REGISTRASI USER BARU              " << endl;
    cout << "===============================================" << endl;

    // Input username
    cout << "\nMasukkan username (max 15 karakter): ";
    cin >> username;
    // Validasi panjang username
    if(stringLength(username) < 3) {
        cout << "\n Username minimal 3 karakter!" << endl;
        pause();
        return;
    }
    // Cek apakah username sudah ada
    if(cekUsernameExists(username)) {
        cout << "\n Username '" << username << "' sudah ada!" << endl;
        cout << "Silakan pilih username lain." << endl;
        pause();
        return;
    }
    // Input password
    cout << "Masukkan password (max 10 karakter): ";
    cin >> password;
    // Validasi panjang password
    if(stringLength(password) < 5) {
        cout << "\n Password minimal 5 karakter!" << endl;
        pause();
        return;
    }
    // Konfirmasi password
    cout << "Konfirmasi password: ";
    cin >> konfirmasiPassword;
    // Cek apakah password cocok
    if(stringCompare(password, konfirmasiPassword) != 0) {
        cout << "\n Password tidak cocok!" << endl;
        pause();
        return;
    }
    // Tambahkan user baru
    tambahUser(username, password);
    cout << "\n===============================================" << endl;
    cout << "              REGISTRASI BERHASIL!             " << endl;
    cout << "===============================================" << endl;
    cout << "Username: " << username << endl;
    cout << "Password: ******" << endl;
    cout << "\nAnda sekarang bisa login menggunakan akun ini." << endl;
    cout << "===============================================" << endl;
    pause();
}

// ==================== MENU FUNCTIONS ====================

void menuAdmin() {
    int pilihan;
    do {
        clearScreen();
        cout << "\n|========================================|" << endl;
        cout << "|               MENU ADMIN               |" << endl;
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
                char judul[100], artis[100], album[100], genre[50];
                int tahun, durasi;

                cout << "\n=== Tambah Lagu Baru ===" << endl;
                cin.ignore();
                cout << "Judul: ";
                cin.getline(judul, 100);

                cout << "Artis: ";
                cin.getline(artis, 100);

                cout << "Album: ";
                cin.getline(album, 100);

                cout << "Genre: ";
                cin.getline(genre, 50);

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
        cout << "|                MENU USER               |" << endl;
        cout << "|========================================|" << endl;
        cout << "|  1. Lihat Library                      |" << endl;
        cout << "|  2. Cari Lagu                          |" << endl;
        cout << "|  3. Play Lagu                          |" << endl;
        cout << "|  4. Stop Lagu                          |" << endl;
        cout << "|  5. Next Lagu                          |" << endl;
        cout << "|  6. Previous Lagu                      |" << endl;
        cout << "|  7. Buat/Kelola Playlist               |" << endl;
        cout << "|  8. Tandai/Lihat Lagu Favorit          |" << endl;
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
                char judul[100];
                cout << "\nMasukkan judul lagu: ";
                cin.ignore();
                cin.getline(judul, 100);

                addressNodeLagu hasil = cariLaguByJudul(judul);
                if(hasil != nullptr) {
                    cout << "\nLagu ditemukan!" << endl;
                    cout << "ID: " << hasil->data.id << " | "
                         << hasil->data.judul << " - " << hasil->data.artis << endl;
                } else {
                    cout << "\nLagu tidak ditemukan!" << endl;
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
                    cout << "6. Lihat Durasi Total Playlist" << endl;
                    cout << "7. Shuffle Playlist" << endl;
                    cout << "0. Kembali" << endl;
                    cout << "Pilihan: ";
                    cin >> subPilihan;

                    switch(subPilihan) {
                        case 1: {
                            char nama[100];
                            cout << "Nama playlist: ";
                            cin.ignore();
                            cin.getline(nama, 100);
                            myPlaylist = buatPlaylist(nama);
                            cout << "\nPlaylist '" << nama << "' berhasil dibuat!" << endl;
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
                        case 6: {
                            if(myPlaylist != nullptr) {
                                tampilkanDurasiPlaylist(myPlaylist);
                            } else {
                                cout << "\nBelum ada playlist!" << endl;
                            }
                            pause();
                            break;
                        }
                        case 7: {
                            if(myPlaylist != nullptr) {
                                shufflePlaylist(myPlaylist);
                            } else {
                                cout << "\nBelum ada playlist!" << endl;
                            }
                            pause();
                            break;
                        }
                    }
                } while(subPilihan != 0);
                break;
            }
            case 8: {
                int subPilihan;
                do {
                    clearScreen();
                    cout << "\n=== KELOLA LAGU FAVORIT ===" << endl;
                    cout << "1. Tandai Lagu sebagai Favorit" << endl;
                    cout << "2. Hapus Tanda Favorit" << endl;
                    cout << "3. Lihat Daftar Lagu Favorit" << endl;
                    cout << "0. Kembali" << endl;
                    cout << "Pilihan: ";
                    cin >> subPilihan;

                    switch(subPilihan) {
                        case 1: {
                            int id;
                            lihatSemuaLagu();
                            cout << "\nMasukkan ID lagu yang akan ditandai favorit: ";
                            cin >> id;
                            tandaiLaguFavorit(id);
                            pause();
                            break;
                        }
                        case 2: {
                            int id;
                            lihatLaguFavorit();
                            cout << "\nMasukkan ID lagu yang akan dihapus dari favorit: ";
                            cin >> id;
                            hapusTandaFavorit(id);
                            pause();
                            break;
                        }
                        case 3:
                            lihatLaguFavorit();
                            pause();
                            break;
                    }
                } while(subPilihan != 0);
                break;
            }
        }
    } while(pilihan != 0);
}

void inisialisasiDataAwal() {
    cout << "Menginisialisasi data awal..." << endl;

    tambahLagu("Komang", "Raim Laode", "Single", "Pop", 2023, 268);
    tambahLagu("Lagu Untuk Kamu", "Tiara Andini", "Single", "Pop", 2023, 234);
    tambahLagu("Sisa Rasa", "Mahalini", "Single", "Pop", 2022, 267);

    tambahLagu("Shape of You", "Ed Sheeran", "Divide", "Pop", 2017, 233);
    tambahLagu("Blinding Lights", "The Weeknd", "After Hours", "Pop", 2020, 200);
    tambahLagu("Perfect", "Ed Sheeran", "Divide", "Pop", 2017, 263);
    tambahLagu("Someone Like You", "Adele", "21", "Pop", 2011, 285);

    tambahLagu("Bohemian Rhapsody", "Queen", "A Night at the Opera", "Rock", 1975, 354);
    tambahLagu("Stairway to Heaven", "Led Zeppelin", "Led Zeppelin IV", "Rock", 1971, 482);
    tambahLagu("Hotel California", "Eagles", "Hotel California", "Rock", 1976, 391);

    tambahLagu("Fly Me to the Moon", "Frank Sinatra", "It Might as Well Be Swing", "Jazz", 1964, 148);
    tambahLagu("What a Wonderful World", "Louis Armstrong", "Single", "Jazz", 1967, 139);
    tambahLagu("33X", "Perunggu", "Memorandum", "Indie", 2022, 434);

    tambahLagu("All We Know", "The Chainsmokers", "Memories...Do Not Open", "EDM", 2016, 194);
    tambahLagu("Perfect Strangers", "Jonas Blue", "Single", "Dance-Pop", 2016, 196);
    tambahLagu("33X", "Perunggu", "Memorandum", "Indie", 2022, 434);

    cout << "Data awal berhasil dimuat!" << endl;
}

// ==================== FITUR TAMBAHAN ====================

// 1. DURASI TOTAL PLAYLIST
int hitungDurasiTotalPlaylist(addressPlaylist pl) {
    if(pl == nullptr || pl->head == nullptr) return 0;

    int totalDurasi = 0;
    addressNodePlaylist current = pl->head;

    while(current != nullptr) {
        totalDurasi += current->lagu->durasi;
        current = current->next;
    }

    return totalDurasi;
}

void tampilkanDurasiPlaylist(addressPlaylist pl) {
    if(pl == nullptr) {
        cout << "\nPlaylist tidak ada!" << endl;
        return;
    }

    int totalDetik = hitungDurasiTotalPlaylist(pl);
    int jam = totalDetik / 3600;
    int menit = (totalDetik % 3600) / 60;
    int detik = totalDetik % 60;

    cout << "\n=== DURASI PLAYLIST: " << pl->nama << " ===" << endl;
    cout << "Total Lagu: " << pl->jumlahLagu << endl;
    cout << "Total Durasi: ";

    if(jam > 0) {
        cout << jam << " jam ";
    }
    cout << menit << " menit " << detik << " detik" << endl;

    char buffer[10];
    formatDurasi(totalDetik, buffer);
    cout << "Format: " << buffer << endl;
}

// 2. FAVORITE / TANDAI LAGU
void tandaiLaguFavorit(int id) {
    addressNodeLagu node = cariLaguById(id);
    if(node == nullptr) {
        cout << "\nLagu dengan ID " << id << " tidak ditemukan!" << endl;
        return;
    }

    if(node->data.isFavorit) {
        cout << "\nLagu '" << node->data.judul << "' sudah ditandai sebagai favorit!" << endl;
    } else {
        node->data.isFavorit = 1;
        cout << "\n  Lagu '" << node->data.judul << "' berhasil ditandai sebagai FAVORIT!" << endl;
    }
}

void hapusTandaFavorit(int id) {
    addressNodeLagu node = cariLaguById(id);
    if(node == nullptr) {
        cout << "\nLagu dengan ID " << id << " tidak ditemukan!" << endl;
        return;
    }

    if(!node->data.isFavorit) {
        cout << "\nLagu '" << node->data.judul << "' bukan lagu favorit!" << endl;
    } else {
        node->data.isFavorit = 0;
        cout << "\n  Tanda favorit pada lagu '" << node->data.judul << "' berhasil dihapus!" << endl;
    }
}

void lihatLaguFavorit() {
    int adaFavorit = 0;
    addressNodeLagu current = libraryHead;

    // Cek apakah ada lagu favorit
    while(current != nullptr) {
        if(current->data.isFavorit) {
            adaFavorit = 1;
            break;
        }
        current = current->next;
    }

    if(!adaFavorit) {
        cout << "\nBelum ada lagu favorit!" << endl;
        return;
    }

    cout << "\n|============================================================================|" << endl;
    cout << "|                         DAFTAR LAGU FAVORIT                                |" << endl;
    cout << "|============================================================================|" << endl;
    cout << "| ID | Judul                 | Artis             | Genre      | Tahun| Durasi|" << endl;
    cout << "|====|=======================|===================|============|======|=======|" << endl;

    current = libraryHead;
    char durasi[10];

    while(current != nullptr) {
        if(current->data.isFavorit) {
            formatDurasi(current->data.durasi, durasi);

            cout << "|";
            if(current->data.id < 10) cout << "   ";
            else if(current->data.id < 100) cout << "  ";
            else if(current->data.id < 1000) cout << " ";
            cout << current->data.id << "| ";

            printWithWidth(current->data.judul, 21);
            cout << "| ";
            printWithWidth(current->data.artis, 17);
            cout << "| ";
            printWithWidth(current->data.genre, 10);
            cout << " | " << current->data.tahun << " | ";
            printWithWidth(durasi, 5);
            cout << " |" << endl;
        }
        current = current->next;
    }
    cout << "|============================================================================|" << endl;
}

// 3. SHUFFLE PLAYLIST (Menggunakan DLL)
int generateRandom(int max) {
    // Simple pseudo-random generator menggunakan operasi aritmatik
    static int seed = 12345;
    seed = (seed * 1103515245 + 12345) & 0x7fffffff;
    return seed % max;
}

// Fungsi untuk mendapatkan node ke-n dari playlist
addressNodePlaylist getNodeAt(addressPlaylist pl, int index) {
    addressNodePlaylist current = pl->head;
    int count = 0;

    while(current != nullptr && count < index) {
        current = current->next;
        count++;
    }

    return current;
}

void shufflePlaylist(addressPlaylist pl) {
    if(pl == nullptr || pl->head == nullptr || pl->jumlahLagu <= 1) {
        cout << "\nPlaylist kosong atau hanya memiliki 1 lagu!" << endl;
        return;
    }

    // Fisher-Yates shuffle menggunakan DLL
    // Kita akan menukar data lagu (pointer ke lagu) antar node
    for(int i = pl->jumlahLagu - 1; i > 0; i--) {
        // Generate indeks acak dari 0 sampai i
        int j = generateRandom(i + 1);

        // Dapatkan node ke-i dan node ke-j
        addressNodePlaylist nodeI = getNodeAt(pl, i);
        addressNodePlaylist nodeJ = getNodeAt(pl, j);

        if(nodeI != nullptr && nodeJ != nullptr) {
            // Swap pointer lagu di kedua node
            addressLagu temp = nodeI->lagu;
            nodeI->lagu = nodeJ->lagu;
            nodeJ->lagu = temp;
        }
    }

    cout << "\n  Playlist '" << pl->nama << "' berhasil di-shuffle!" << endl;
}
