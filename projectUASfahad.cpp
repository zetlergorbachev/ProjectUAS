#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <limits>

using namespace std;

const string FILE_PENGGUNA = "users.txt";

struct Tugas {
    string judul;
    string deskripsi;
    int prioritas;
    bool selesai;
};

void tampilkanMenuAwal();
bool registrasi();
string login();

void tampilkanMenuTugas();
void tambahTugas(vector<Tugas>& daftarTugas);
void tampilkanSemuaTugas(const vector<Tugas>& daftarTugas);
void tandaiSelesai(vector<Tugas>& daftarTugas);
void hapusTugas(vector<Tugas>& daftarTugas);
void simpanKeFile(const vector<Tugas>& daftarTugas, const string& username);
void muatDariFile(vector<Tugas>& daftarTugas, const string& username);
void urutkanTugas(vector<Tugas>& daftarTugas);
void bubbleSortByPrioritas(vector<Tugas>& daftarTugas);
void bubbleSortByAbjad(vector<Tugas>& daftarTugas);
void bersihkanLayar();
void tungguEnter();

int main() {
    string penggunaAktif = "";

    while (true) {
        bersihkanLayar();
        tampilkanMenuAwal();
        int pilihanAwal;
        cout << "Masukkan pilihan Anda: ";
        cin >> pilihanAwal;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            pilihanAwal = 0;
        }

        switch(pilihanAwal) {
            case 1:
                penggunaAktif = login();
                if (!penggunaAktif.empty()) {
                    cout << "\nLogin berhasil! Selamat datang, " << penggunaAktif << "!" << endl;
                    tungguEnter();
                } else {
                    cout << "\nLogin gagal. Username atau password salah." << endl;
                    tungguEnter();
                }
                break;
            case 2:
                if (registrasi()) {
                     cout << "\nRegistrasi berhasil! Silakan login." << endl;
                }
                tungguEnter();
                break;
            case 3:
                cout << "Terima kasih!" << endl;
                return 0;
            default:
                cout << "Pilihan tidak valid." << endl;
                tungguEnter();
                break;
        }

        if(!penggunaAktif.empty()){
            break;
        }
    }

    vector<Tugas> daftarTugas;
    muatDariFile(daftarTugas, penggunaAktif);

    int pilihanTugas;
    do {
        bersihkanLayar();
        tampilkanMenuTugas();
        cout << "Pengguna: " << penggunaAktif << endl;
        cout << "======================================" << endl;
        cout << "Masukkan pilihan Anda: ";
        cin >> pilihanTugas;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            pilihanTugas = 0;
        }

        switch (pilihanTugas) {
            case 1: tambahTugas(daftarTugas); break;
            case 2: tampilkanSemuaTugas(daftarTugas); break;
            case 3: tandaiSelesai(daftarTugas); break;
            case 4: hapusTugas(daftarTugas); break;
            case 5: urutkanTugas(daftarTugas); break;
            case 6:
                simpanKeFile(daftarTugas, penggunaAktif);
                cout << "Tugas berhasil disimpan!" << endl;
                break;
            case 7:
                simpanKeFile(daftarTugas, penggunaAktif);
                cout << "Menyimpan dan keluar. Terima kasih!" << endl;
                break;
            default:
                cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
                break;
        }

        if (pilihanTugas != 7) {
            tungguEnter();
        }

    } while (pilihanTugas != 7);

    return 0;
}


void bersihkanLayar() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void tungguEnter() {
    cout << "\nTekan Enter untuk melanjutkan...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void tampilkanMenuAwal() {
    cout << "======================================" << endl;
    cout << "   Selamat Datang di Aplikasi To-Do   " << endl;
    cout << "======================================" << endl;
    cout << "1. Login" << endl;
    cout << "2. Registrasi" << endl;
    cout << "3. Keluar" << endl;
    cout << "======================================" << endl;
}

bool registrasi() {
    string username, password, passConfirm, storedUser;
    bersihkanLayar();
    cout << "--- Halaman Registrasi ---" << endl;
    
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    cout << "Masukkan username baru: ";
    getline(cin, username);

    if (username.empty()) {
        cout << "Username tidak boleh kosong." << endl;
        return false;
    }

    ifstream fileCek(FILE_PENGGUNA);
    if (fileCek.is_open()) {
        string line;
        int lineNum = 0;
        while (getline(fileCek, line)) {
            lineNum++;
            if (lineNum % 2 != 0) { // Hanya cek baris ganjil (username)
                if (line == username) {
                    cout << "Username '" << username << "' sudah digunakan." << endl;
                    fileCek.close();
                    return false;
                }
            }
        }
        fileCek.close();
    }
    
    cout << "Masukkan password baru (min. 6 karakter): ";
    getline(cin, password);

    if(password.length() < 6){
        cout << "Password terlalu pendek. Harap gunakan minimal 6 karakter." << endl;
        return false;
    }

    cout << "Konfirmasi password: ";
    getline(cin, passConfirm);

    if(password != passConfirm){
        cout << "Password tidak cocok. Registrasi dibatalkan." << endl;
        return false;
    }

    ofstream file(FILE_PENGGUNA, ios::app);
    if (!file.is_open()) {
        cout << "Error: Tidak bisa membuka file pengguna." << endl;
        return false;
    }
    file << username << '\n' << password << '\n';
    file.close();
    return true;
}

string login() {
    string username, password, storedUser, storedPass;
    bersihkanLayar();
    cout << "--- Halaman Login ---" << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Username: ";
    getline(cin, username);
    cout << "Password: ";
    getline(cin, password);

    ifstream file(FILE_PENGGUNA);
    if (!file.is_open()) {
        return "";
    }

    while (getline(file, storedUser) && getline(file, storedPass)) {
        if (storedUser == username && storedPass == password) {
            file.close();
            return username;
        }
    }

    file.close();
    return "";
}


void tampilkanMenuTugas() {
    cout << "======================================" << endl;
    cout << "      Aplikasi To-Do List C++" << endl;
    cout << "======================================" << endl;
    cout << "1. Tambah Tugas Baru" << endl;
    cout << "2. Tampilkan Semua Tugas" << endl;
    cout << "3. Tandai Tugas sebagai Selesai" << endl;
    cout << "4. Hapus Tugas" << endl;
    cout << "5. Urutkan Tugas" << endl;
    cout << "6. Simpan Tugas" << endl;
    cout << "7. Simpan dan Keluar" << endl;
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
    } else {
        string judulDihapus = daftarTugas[nomor - 1].judul;
        daftarTugas.erase(daftarTugas.begin() + (nomor - 1));
        cout << "Tugas '" << judulDihapus << "' telah dihapus." << endl;
    }
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

    if(cin.fail()){
        cin.clear();
        pilihan = 0;
    }

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
    size_t n = daftarTugas.size();
    for (size_t i = 0; i < n - 1; i++) {
        for (size_t j = 0; j < n - i - 1; j++) {
            if (daftarTugas[j].prioritas > daftarTugas[j + 1].prioritas) {
                swap(daftarTugas[j], daftarTugas[j + 1]);
            }
        }
    }
}

void bubbleSortByAbjad(vector<Tugas>& daftarTugas) {
    size_t n = daftarTugas.size();
    for (size_t i = 0; i < n - 1; i++) {
        for (size_t j = 0; j < n - i - 1; j++) {
            if (daftarTugas[j].judul > daftarTugas[j + 1].judul) {
                swap(daftarTugas[j], daftarTugas[j + 1]);
            }
        }
    }
}

void simpanKeFile(const vector<Tugas>& daftarTugas, const string& username) {
    string namaFile = username + "_todolist.txt";
    ofstream file(namaFile);
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

void muatDariFile(vector<Tugas>& daftarTugas, const string& username) {
    string namaFile = username + "_todolist.txt";
    ifstream file(namaFile);
    if (!file.is_open()) {
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