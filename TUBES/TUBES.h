#ifndef TUBES_H_INCLUDED
#define TUBES_H_INCLUDED
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>

using namespace std;

// ==================== STRUKTUR DATA ====================

// Record untuk data lagu
struct Lagu {
    int id;
    string judul;
    string artis;
    string album;
    string genre;
    int tahun;
    int durasi; // dalam detik
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
    string nama;
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

// ==================== GLOBAL VARIABLES ====================
extern addressNodeLagu libraryHead;
extern addressNodeLagu libraryTail;
extern int nextId;

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
void toUpperCase(string &str);
string formatDurasi(int detik);

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
void tambahLagu(string judul, string artis, string album, string genre, int tahun, int durasi);
void lihatSemuaLagu();
addressNodeLagu cariLaguById(int id);
addressNodeLagu cariLaguByJudul(string judul);
void updateLagu(int id);
void hapusLagu(int id);
int hitungJumlahLagu();

// Playlist Operations
addressPlaylist buatPlaylist(string nama);
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

// Menu Functions
void menuAdmin();
void menuUser();
void inisialisasiDataAwal();

#endif // TUBES_H_INCLUDED
