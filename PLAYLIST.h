#ifndef PLAYLIST_H_INCLUDED
#define PLAYLIST_H_INCLUDED
#include <iostream>
using namespace std;

// ==================== STRUKTUR DATA ====================
// Record untuk data lagu
struct Lagu {
    int id;
    char judul[100];
    char artis[100];
    char album[100];
    char genre[50];
    int tahun;
    int durasi; // dalam detik
    int isFavorit; // penanda lagu favorit (0 = tidak, 1 = ya)
};

// Pointer untuk Lagu
typedef Lagu *addressLagu;

// Pointer untuk NodeLagu (forward declaration)
typedef struct NodeLagu *addressNodeLagu;

// Node untuk Doubly Linked List (Library Lagu)
struct NodeLagu {
    Lagu data;
    addressNodeLagu next;
    addressNodeLagu prev;
};

// Pointer untuk NodePlaylist (forward declaration)
typedef struct NodePlaylist *addressNodePlaylist;

// Node untuk Playlist (Singly Linked List)
struct NodePlaylist {
    addressLagu lagu; // pointer ke lagu di library
    addressNodePlaylist next;
};

// Pointer untuk Playlist (forward declaration)
typedef struct Playlist *addressPlaylist;

// Struktur Playlist
struct Playlist {
    char nama[100];
    addressNodePlaylist head;
    int jumlahLagu;
};

// Pointer untuk NodeQueue (forward declaration)
typedef struct NodeQueue *addressNodeQueue;

// Node untuk Queue (antrian pemutaran)
struct NodeQueue {
    addressLagu lagu;
    addressNodeQueue next;
};

// Queue untuk pemutaran
struct Queue {
    addressNodeQueue front;
    addressNodeQueue rear;
};

// Pointer untuk NodeStack (forward declaration)
typedef struct NodeStack *addressNodeStack;

// Node untuk Stack (history)
struct NodeStack {
    addressLagu lagu;
    addressNodeStack next;
};

// Stack untuk history
struct Stack {
    addressNodeStack top;
};

// Record untuk data user
struct User {
    char username[50];
    char password[50];
};

// Pointer untuk NodeUser (forward declaration)
typedef struct NodeUser *addressNodeUser;

// Node untuk User (Singly Linked List)
struct NodeUser {
    User data;
    addressNodeUser next;
};

// ==================== GLOBAL VARIABLES ====================
extern addressNodeLagu libraryHead;
extern addressNodeLagu libraryTail;
extern int nextId;

extern addressNodeUser userHead;
extern char currentUsername[50];

extern addressLagu currentPlaying;
extern int isPlaying;
extern addressPlaylist currentPlaylist;
extern addressNodePlaylist currentPlaylistNode;

extern Stack history;
extern Queue playQueue;

// ==================== FUNCTION DECLARATIONS ====================
// Utility Functions
void clearScreen();
void pause();
void toUpperCase(char *str);
void toLowerCase(char *str);
int stringLength(const char *str);
void stringCopy(char *dest, const char *src);
int stringCompare(const char *str1, const char *str2);
bool stringContains(const char *str, const char *substr);
void formatDurasi(int detik, char *buffer);

// Stack Operations
void initStack(Stack *s);
void push(Stack *s, addressLagu lagu);
addressLagu pop(Stack *s);
bool isStackEmpty(Stack *s);

// Queue Operations
void initQueue(Queue *q);
void enqueue(Queue *q, addressLagu lagu);
addressLagu dequeue(Queue *q);
bool isQueueEmpty(Queue *q);

// Library Operations (Doubly Linked List)
void stringCopy(char *dest, const char *src);
void tambahLagu(const char *judul, const char *artis, const char *album, const char *genre, int tahun, int durasi);
int stringLength(const char *str);
void lihatSemuaLagu();
addressNodeLagu cariLaguById(int id);
addressNodeLagu cariLaguByJudul(const char *judul);
void updateLagu(int id);
void hapusLagu(int id);
int hitungJumlahLagu();

// Playlist Operations
addressPlaylist buatPlaylist(const char *nama);
void tambahKePlaylist(addressPlaylist pl, int idLagu);
void hapusDariPlaylist(addressPlaylist pl, int posisi);
void lihatPlaylist(addressPlaylist pl);
void hapusPlaylist(addressPlaylist pl);

// Player Operations
void playLagu(addressLagu lagu);
void stopLagu();
addressLagu cariLaguMirip(addressLagu referensi);
void nextLagu();
void prevLagu();
void tampilkanNowPlaying();

// Fitur Tambahan
int hitungDurasiTotalPlaylist(addressPlaylist pl);
void tampilkanDurasiPlaylist(addressPlaylist pl);
void tandaiLaguFavorit(int id);
void hapusTandaFavorit(int id);
void lihatLaguFavorit();
void shufflePlaylist(addressPlaylist pl);
int generateRandom(int max);

// User Management
void tambahUser(const char *username, const char *password);
void registrasiUser();
void inisialisasiDataUser();
bool loginUser(const char *username, const char *password);

// Menu Functions
void menuAdmin();
void menuUser();
void inisialisasiDataAwal();

#endif // TUBES_H_INCLUDED
