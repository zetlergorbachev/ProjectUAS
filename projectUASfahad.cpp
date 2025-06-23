#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm> 
#include <limits>    
using namespace std;

const string NAMA_FILE = "todolistbyfahad.txt";

struct Tugas {
    string judul;
    string deskripsi;
    int prioritas; 
    bool selesai;
};


void tampilkanMenu();
void tambahTugas(vector<Tugas>& daftarTugas);
void tampilkanSemuaTugas(const vector<Tugas>& daftarTugas);
void tandaiSelesai(vector<Tugas>& daftarTugas);
void hapusTugas(vector<Tugas>& daftarTugas);
void simpanKeFile(const vector<Tugas>& daftarTugas);
void muatDariFile(vector<Tugas>& daftarTugas);
void urutkanTugas(vector<Tugas>& daftarTugas);
void bubbleSortByPrioritas(vector<Tugas>& daftarTugas);
void bubbleSortByAbjad(vector<Tugas>& daftarTugas);
void bersihkanLayar();


int main() {
    vector<Tugas> daftarTugas; 
    muatDariFile(daftarTugas); 

    int pilihan;
    do {
        bersihkanLayar();
        tampilkanMenu();
        cout << "Masukkan pilihan Anda: ";
        cin >> pilihan;

        
        if (cin.fail()) {
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            pilihan = 0; 
        }

        switch (pilihan) {
            case 1:
                tambahTugas(daftarTugas);
                break;
            case 2:
                tampilkanSemuaTugas(daftarTugas);
                break;
            case 3:
                tandaiSelesai(daftarTugas);
                break;
            case 4:
                hapusTugas(daftarTugas);
                break;
            case 5:
                urutkanTugas(daftarTugas);
                break;
            case 6:
                simpanKeFile(daftarTugas);
                cout << "Tugas berhasil disimpan ke file." << endl;
                break;
            case 7:
                cout << "Terima kasih telah menggunakan aplikasi To-Do List!" << endl;
                break;
            default:
                cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
                break;
        }

        if (pilihan != 7) {
            cout << "\nTekan Enter untuk kembali ke menu...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            cin.get(); 
        }

    } while (pilihan != 7);

    return 0;
}


void bersihkanLayar() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}


void tampilkanMenu() {
    cout << "======================================" << endl;
    cout << "    Aplikasi To-Do List C++" << endl;
    cout << "======================================" << endl;
    cout << "1. Tambah Tugas Baru" << endl;
    cout << "2. Tampilkan Semua Tugas" << endl;
    cout << "3. Tandai Tugas sebagai Selesai" << endl;
    cout << "4. Hapus Tugas" << endl;
    cout << "5. Urutkan Tugas" << endl;
    cout << "6. Simpan Tugas ke File" << endl;
    cout << "7. Keluar" << endl;
    cout << "======================================" << endl;
}


void tambahTugas(vector<Tugas>& daftarTugas) {
    bersihkanLayar();
    Tugas tugasBaru;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

    cout << "--- Tambah Tugas Baru ---" << endl;
    cout << "Masukkan judul tugas: ";
    getline(cin, tugasBaru.judul);
    cout << "Masukkan deskripsi tugas: ";
    getline(cin, tugasBaru.deskripsi);
    cout << "Masukkan prioritas (1:Tinggi, 2:Sedang, 3:Rendah): ";
    cin >> tugasBaru.prioritas;

    
    while (cin.fail() || (tugasBaru.prioritas < 1 || tugasBaru.prioritas > 3)) {
        cout << "Input tidak valid. Masukkan angka antara 1 dan 3: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> tugasBaru.prioritas;
    }

    tugasBaru.selesai = false; 
    daftarTugas.push_back(tugasBaru);

    cout << "\nTugas berhasil ditambahkan!" << endl;
}


void tampilkanSemuaTugas(const vector<Tugas>& daftarTugas) {
    bersihkanLayar();
    cout << "--- Daftar Semua Tugas ---" << endl;
    if (daftarTugas.empty()) {
        cout << "Tidak ada tugas dalam daftar." << endl;
    } else {
        for (size_t i = 0; i < daftarTugas.size(); ++i) {
            cout << "No. " << i + 1 << endl;
            cout << "Judul     : " << daftarTugas[i].judul << endl;
            cout << "Deskripsi : " << daftarTugas[i].deskripsi << endl;
            cout << "Prioritas : ";
            switch(daftarTugas[i].prioritas) {
                case 1: cout << "Tinggi"; break;
                case 2: cout << "Sedang"; break;
                case 3: cout << "Rendah"; break;
            }
            cout << endl;
            cout << "Status    : " << (daftarTugas[i].selesai ? "Selesai" : "Belum Selesai") << endl;
            cout << "--------------------------------------" << endl;
        }
    }
}


void tandaiSelesai(vector<Tugas>& daftarTugas) {
    if (daftarTugas.empty()) {
        cout << "Tidak ada tugas untuk ditandai." << endl;
        return;
    }
    tampilkanSemuaTugas(daftarTugas);
    cout << "Masukkan nomor tugas yang ingin ditandai selesai: ";
    int nomor;
    cin >> nomor;

    if (cin.fail() || nomor < 1 || nomor > daftarTugas.size()) {
        cout << "Nomor tugas tidak valid." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    } else {
        daftarTugas[nomor - 1].selesai = true;
        cout << "Tugas '" << daftarTugas[nomor - 1].judul << "' telah ditandai selesai." << endl;
    }
}


void hapusTugas(vector<Tugas>& daftarTugas) {
    if (daftarTugas.empty()) {
        cout << "Tidak ada tugas untuk dihapus." << endl;
        return;
    }
    tampilkanSemuaTugas(daftarTugas);
    cout << "Masukkan nomor tugas yang ingin dihapus: ";
    int nomor;
    cin >> nomor;

    if (cin.fail() || nomor < 1 || nomor > daftarTugas.size()) {
        cout << "Nomor tugas tidak valid." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    } else {
        string judulDihapus = daftarTugas[nomor - 1].judul;
        daftarTugas.erase(daftarTugas.begin() + (nomor - 1));
        cout << "Tugas '" << judulDihapus << "' telah dihapus." << endl;
    }
}


void simpanKeFile(const vector<Tugas>& daftarTugas) {
    ofstream file(NAMA_FILE); 
    if (!file.is_open()) {
        cout << "Error: Gagal membuka file untuk menyimpan." << endl;
        return;
    }

    for (const auto& tugas : daftarTugas) {
        file << tugas.judul << '\n';
        file << tugas.deskripsi << '\n';
        file << tugas.prioritas << '\n';
        file << tugas.selesai << '\n';
    }

    file.close();
}


void muatDariFile(vector<Tugas>& daftarTugas) {
    ifstream file(NAMA_FILE); 
    if (!file.is_open()) {
        cout << "File data tidak ditemukan. Memulai dengan daftar kosong." << endl;
        return;
    }

    daftarTugas.clear(); 
    Tugas tugasSementara;
    while (getline(file, tugasSementara.judul)) {
        getline(file, tugasSementara.deskripsi);
        file >> tugasSementara.prioritas;
        file >> tugasSementara.selesai;
        file.ignore(numeric_limits<streamsize>::max(), '\n'); 
        daftarTugas.push_back(tugasSementara);
    }

    file.close();
}


void urutkanTugas(vector<Tugas>& daftarTugas) {
    bersihkanLayar();
    if (daftarTugas.empty()) {
        cout << "Tidak ada tugas untuk diurutkan." << endl;
        return;
    }
    cout << "--- Urutkan Tugas ---" << endl;
    cout << "1. Berdasarkan Prioritas (Tinggi ke Rendah)" << endl;
    cout << "2. Berdasarkan Abjad Judul (A-Z)" << endl;
    cout << "Pilih tipe pengurutan: ";
    int pilihan;
    cin >> pilihan;

    switch (pilihan) {
        case 1:
            bubbleSortByPrioritas(daftarTugas);
            cout << "Tugas berhasil diurutkan berdasarkan prioritas." << endl;
            break;
        case 2:
            bubbleSortByAbjad(daftarTugas);
            cout << "Tugas berhasil diurutkan berdasarkan abjad." << endl;
            break;
        default:
            cout << "Pilihan tidak valid." << endl;
            break;
    }
}


void bubbleSortByPrioritas(vector<Tugas>& daftarTugas) {
    int n = daftarTugas.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (daftarTugas[j].prioritas > daftarTugas[j + 1].prioritas) {
                swap(daftarTugas[j], daftarTugas[j + 1]);
            }
        }
    }
}

void bubbleSortByAbjad(vector<Tugas>& daftarTugas) {
    int n = daftarTugas.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (daftarTugas[j].judul > daftarTugas[j + 1].judul) {
                swap(daftarTugas[j], daftarTugas[j + 1]);
            }
        }
    }
}