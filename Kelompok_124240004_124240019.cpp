#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>

using namespace std;

const int JUMLAH_KAMAR = 20;

struct identitas {
    string nama;
    string no_hp;
    int no_kamar;
    string jenis_kamar;
    string tanggalMasuk;

    identitas* next;
    identitas* prev;
};

void simpanKeFile(identitas* head, const string& file){
    ofstream berkas(file);
    if(!berkas.is_open()){
        cout << "Gagal membuka file untuk menyimpan data." << endl;
        return;
    }

    identitas* current = head;
    while(current != nullptr){
        berkas << current->nama << endl;
        berkas << current->no_hp << endl;
        berkas << current->no_kamar << endl;
        berkas << current->jenis_kamar << endl;
        berkas << current->tanggalMasuk << endl;
        current = current->next;
    }

    berkas.close();
    cout << "Data berhasil disimpan ke file." << endl;
}

void urutkanData(identitas*& head, bool berdasarkanNoKamar) {
    if (!head || !head->next) return;

    for (identitas* i = head; i != nullptr; i = i->next) {
        for (identitas* j = i->next; j != nullptr; j = j->next) {
            bool tukar = false;

            if (berdasarkanNoKamar) {
                if (i->no_kamar > j->no_kamar) tukar = true;
            } else {
                if (i->tanggalMasuk > j->tanggalMasuk) tukar = true;
            }

            if (tukar) {
                swap(i->nama, j->nama);
                swap(i->no_hp, j->no_hp);
                swap(i->no_kamar, j->no_kamar);
                swap(i->jenis_kamar, j->jenis_kamar);
                swap(i->tanggalMasuk, j->tanggalMasuk);
            }
        }
    }
}

void bacaDariFile(identitas*& head, identitas*& tail, const string& file) {
    ifstream berkas(file);
    if (!berkas.is_open()) {
        return;
    }
    string nama, no_hp, jenis_kamar, tanggalMasuk;
    int no_kamar;
    while (getline(berkas, nama)) {
        getline(berkas, no_hp);
        berkas >> no_kamar;
        berkas.ignore();
        getline(berkas, jenis_kamar);
        getline(berkas, tanggalMasuk);

        identitas* newNode = new identitas;
        newNode->nama = nama;
        newNode->no_hp = no_hp;
        newNode->no_kamar = no_kamar;
        newNode->jenis_kamar = jenis_kamar;
        newNode->tanggalMasuk = tanggalMasuk;
        newNode->next = nullptr;
        newNode->prev = tail;

        if (tail != nullptr) {
            tail->next = newNode;
        } else {
            head = newNode;
        }
        tail = newNode;
    }
    berkas.close();
}

bool isKamarTersedia(identitas* head, int no_kamar) {
    identitas* current = head;
    while (current != nullptr) {
        if (current->no_kamar == no_kamar) {
            return false; // Sudah ditempati
        }
        current = current->next;
    }
    return true;
}

void tampilkanKamarKosong(identitas* head, const string& jenis_kamar) {
    cout << "Nomor kamar " << jenis_kamar << " yang masih kosong: ";
    bool ada = false;
    if (jenis_kamar == "VIP") {
        for (int i = 1; i <= 10; i++) {
            if (isKamarTersedia(head, i)) {
                cout << i << " ";
                ada = true;
            }
        }
    } else if (jenis_kamar == "Reguler") {
        for (int i = 11; i <= 20; i++) {
            if (isKamarTersedia(head, i)) {
                cout << i << " ";
                ada = true;
            }
        }
    }
    if (!ada) cout << "Tidak ada kamar kosong.";
    cout << endl;
}


void tambahDataPenghuni(identitas*& head, identitas*& tail){
    identitas* newNode = new identitas;

    // Cek apakah kamar VIP tersedia
    bool vipTersedia = false;
    for (int i = 1; i <= 10; i++) {
        if (isKamarTersedia(head, i)) {
            vipTersedia = true;
            break;
        }
    }

    // Cek apakah kamar Reguler tersedia
    bool regulerTersedia = false;
    for (int i = 11; i <= 20; i++) {
        if (isKamarTersedia(head, i)) {
            regulerTersedia = true;
            break;
        }
    }

    if (!vipTersedia && !regulerTersedia) {
        cout << "Maaf, semua kamar sudah penuh.\n";
        delete newNode;
        return;
    }

    int pilihanJenis = 0;
    if (vipTersedia && regulerTersedia) {
        cout << "Pilih jenis kamar (1. VIP, 2. Reguler): ";
        cin >> pilihanJenis;
        cin.ignore();
    } else if (vipTersedia) {
        cout << "Jenis kamar VIP tersedia dan akan dipilih secara otomatis.\n";
        pilihanJenis = 1;
    } else if (regulerTersedia) {
        cout << "Jenis kamar Reguler tersedia dan akan dipilih secara otomatis.\n";
        pilihanJenis = 2;
    }

    if (pilihanJenis == 1) {
        newNode->jenis_kamar = "VIP";
    } else {
        newNode->jenis_kamar = "Reguler";
    }

    tampilkanKamarKosong(head, newNode->jenis_kamar);

    do {
        cout << "Masukkan nama: ";
        getline(cin, newNode->nama);
        if (newNode->nama.empty()) {
            cout << "Nama tidak boleh kosong.\n";
        }
    } while (newNode->nama.empty());

    cout << "Masukkan no hp : ";
    getline(cin, newNode->no_hp);

    do {
        cout << "Masukkan no kamar ("
             << (newNode->jenis_kamar == "VIP" ? "1-10" : "11-20") << "): ";
        cin >> newNode->no_kamar;
        cin.ignore();

        bool validRange = false;
        if (newNode->jenis_kamar == "VIP" && newNode->no_kamar >= 1 && newNode->no_kamar <= 10) {
            validRange = true;
        } else if (newNode->jenis_kamar == "Reguler" && newNode->no_kamar >= 11 && newNode->no_kamar <= 20) {
            validRange = true;
        }

        if (!validRange) {
            cout << "Nomor kamar tidak sesuai dengan jenis kamar yang dipilih.\n";
        } else if (!isKamarTersedia(head, newNode->no_kamar)) {
            cout << "Nomor kamar sudah ditempati. Pilih nomor kamar lain.\n";
        } else {
            break;
        }
    } while (true);

    // Isi tanggal masuk
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char buffer[11];
    strftime(buffer, sizeof(buffer), "%d-%m-%Y", ltm);
    newNode->tanggalMasuk = buffer;
    cout << "Tanggal masuk : " << newNode->tanggalMasuk << endl;

    newNode->next = nullptr;
    newNode->prev = tail;

    if(tail != nullptr){
        tail->next = newNode;
    } else {
        head = newNode;
    }
    tail = newNode;
}

void tampilkanDataPenghuni(identitas* head) {
    if (head == nullptr) {
        cout << "Belum ada data penghuni." << endl;
        return;
    }

    char opsiUrut;
    cout << "Urutkan berdasarkan:\n";
    cout << "1. Nomor Kamar\n";
    cout << "2. Tanggal Masuk\n";
    cout << "Pilihan: ";
    cin >> opsiUrut;

    bool urutNoKamar = (opsiUrut == '1');
    urutkanData(head, urutNoKamar);

    identitas* current = head;
    int no = 1;

    cout << "+-----+----------------------+---------------+------------+--------------+-----------------+" << endl;
    cout << "| No  | Nama                 | No HP         | No Kamar   | Jenis        | Tanggal Masuk   |" << endl;
    cout << "+-----+----------------------+---------------+------------+--------------+-----------------+" << endl;

    while (current != nullptr) {
        cout << "| "
             << setw(3) << left << no++ << " | "
             << setw(20) << left << current->nama.substr(0, 20) << " | "
             << setw(13) << left << current->no_hp.substr(0, 13) << " | "
             << setw(10) << left << current->no_kamar << " | "
             << setw(12) << left << current->jenis_kamar << " | "
             << setw(15) << left << current->tanggalMasuk << " |" << endl;
        current = current->next;
    }

    cout << "+-----+----------------------+---------------+------------+--------------+-----------------+" << endl;
}

void tampilkanDataPenghuniApaAdanya(identitas* head) {
    if (head == nullptr) {
        cout << "Belum ada data penghuni." << endl;
        return;
    }

    identitas* current = head;
    int no = 1;

    cout << "+-----+----------------------+---------------+------------+--------------+-----------------+" << endl;
    cout << "| No  | Nama                 | No HP         | No Kamar   | Jenis        | Tanggal Masuk   |" << endl;
    cout << "+-----+----------------------+---------------+------------+--------------+-----------------+" << endl;

    while (current != nullptr) {
        cout << "| "
             << setw(3) << left << no++ << " | "
             << setw(20) << left << current->nama.substr(0, 20) << " | "
             << setw(13) << left << current->no_hp.substr(0, 13) << " | "
             << setw(10) << left << current->no_kamar << " | "
             << setw(12) << left << current->jenis_kamar << " | "
             << setw(15) << left << current->tanggalMasuk << " |" << endl;
        current = current->next;
    }

    cout << "+-----+----------------------+---------------+------------+--------------+-----------------+" << endl;
}

int hitungKamarTersedia(identitas* head) {
    bool kamarTerpakai[JUMLAH_KAMAR + 1] = {false}; // index 0 tidak dipakai

    identitas* current = head;
    while (current != nullptr) {
        if (current->no_kamar >= 1 && current->no_kamar <= JUMLAH_KAMAR)
            kamarTerpakai[current->no_kamar] = true;
        current = current->next;
    }

    int jumlahKosong = 0;
    for (int i = 1; i <= JUMLAH_KAMAR; ++i) {
        if (!kamarTerpakai[i]) jumlahKosong++;
    }
    return jumlahKosong;
}

void hapusDataPenghuni(identitas*& head, identitas*& tail) {
    if (head == nullptr) {
        cout << "Tidak ada data untuk dihapus." << endl;
        return;
    }

    tampilkanDataPenghuniApaAdanya(head);

    int noKamar;
    cout << "Masukkan nomor kamar yang ingin dihapus: ";
    cin >> noKamar;

    identitas* current = head;
    while (current != nullptr) {
        if (current->no_kamar == noKamar) {
            if (current->prev != nullptr) {
                current->prev->next = current->next;
            } else {
                head = current->next;
            }
            if (current->next != nullptr) {
                current->next->prev = current->prev;
            } else {
                tail = current->prev;
            }
            delete current;
            cout << "Data penghuni dengan nomor kamar " << noKamar << " telah dihapus." << endl;
            return;
        }
        current = current->next;
    }
    cout << "Data penghuni dengan nomor kamar " << noKamar << " tidak ditemukan." << endl;
}

void editDataPenghuni(identitas* head) {
    if (head == nullptr) {
        cout << "Tidak ada data untuk diedit." << endl;
        return;
    }

    tampilkanDataPenghuniApaAdanya(head);  // Menampilkan semua data terlebih dahulu

    int noKamar;
    cout << "\nMasukkan nomor kamar penghuni yang ingin diedit: ";
    cin >> noKamar;
    cin.ignore();

    identitas* current = head;
    while (current != nullptr) {
        if (current->no_kamar == noKamar) {
            cout << "\nData lama:" << endl;
            cout << "Nama         : " << current->nama << endl;
            cout << "No HP        : " << current->no_hp << endl;
            cout << "Jenis Kamar  : " << current->jenis_kamar << endl;
            cout << "Tanggal Masuk: " << current->tanggalMasuk << endl;

            cout << "\nMasukkan data baru (kosongkan jika tidak ingin mengubah):\n";

            string input;

            cout << "Nama baru: ";
            getline(cin, input);
            if (!input.empty()) current->nama = input;

            cout << "No HP baru: ";
            getline(cin, input);
            if (!input.empty()) current->no_hp = input;

            int pilihanJenis;
            cout << "Pilih jenis kamar baru (1. VIP, 2. Reguler, 0. Tidak ubah): ";
            cin >> pilihanJenis;
            cin.ignore();
            if (pilihanJenis == 1) current->jenis_kamar = "VIP";
            else if (pilihanJenis == 2) current->jenis_kamar = "Reguler";

            cout << "Nomor kamar baru (" 
                 << (current->jenis_kamar == "VIP" ? "1-10" : "11-20") 
                 << ", 0 untuk tidak ubah): ";
            int kamarBaru;
            cin >> kamarBaru;
            cin.ignore();

            if (kamarBaru != 0) {
                if ((current->jenis_kamar == "VIP" && kamarBaru >= 1 && kamarBaru <= 10) ||
                    (current->jenis_kamar == "Reguler" && kamarBaru >= 11 && kamarBaru <= 20)) {

                    if (isKamarTersedia(head, kamarBaru) || kamarBaru == current->no_kamar) {
                        current->no_kamar = kamarBaru;
                    } else {
                        cout << "Nomor kamar sudah ditempati. Tidak diubah.\n";
                    }
                } else {
                    cout << "Nomor kamar tidak sesuai dengan jenis kamar. Tidak diubah.\n";
                }
            }

            cout << "Data berhasil diperbarui.\n";
            return;
        }
        current = current->next;
    }

    cout << "Data penghuni dengan nomor kamar " << noKamar << " tidak ditemukan." << endl;
}


void cariDataPenghuni(identitas* head) {
    if (head == nullptr) {
        cout << "Tidak ada data penghuni." << endl;
        return;
    }

    string nama;
    cout << "Masukkan nama penghuni yang ingin dicari: ";
    cin.ignore();
    getline(cin, nama);

    identitas* current = head;
    bool ditemukan = false;

    while (current != nullptr) {
        if (current->nama == nama) {
            cout << "\nData ditemukan:\n";
            cout << "Nama         : " << current->nama << endl;
            cout << "No HP        : " << current->no_hp << endl;
            cout << "No Kamar     : " << current->no_kamar << endl;
            cout << "Jenis Kamar  : " << current->jenis_kamar << endl;
            cout << "Tanggal Masuk: " << current->tanggalMasuk << endl;
            ditemukan = true;
        }
        current = current->next;
    }

    if (!ditemukan) {
        cout << "Penghuni dengan nama \"" << nama << "\" tidak ditemukan.\n";
    }
}


int main(){
    identitas* head = nullptr;
    identitas* tail = nullptr;

    string file = "data.txt";
    char pilihan;

    bacaDariFile(head, tail, file);

    do {
        system("cls");
        cout << "==========================================" << endl;
        cout << "=       SISTEM DATA PENGHUNI KOS         =" << endl;
        cout << "==========================================" << endl;
        cout << "Jumlah kamar tersedia : " << hitungKamarTersedia(head) << " dari " << JUMLAH_KAMAR << endl;
        cout << "1. Tambahkan data penghuni" << endl;
        cout << "2. Tampilkan data penghuni" << endl;
        cout << "3. Hapus data penghuni" << endl;
        cout << "4. Cari data penghuni" << endl;
        cout << "5. Edit data penghuni" << endl;
        cout << "6. Keluar" << endl;
        cout << "==========================================" << endl;
        cout << "Silahkan Pilih Menu Disini: "; cin >> pilihan;
        cout << endl;
        switch(pilihan){
            case '1':
                cout << "=== Tambahkan Data Penghuni ===" << endl;
                tambahDataPenghuni(head, tail);
                simpanKeFile(head, file);
                system("pause");
                break;
            case '2':
                cout << "=== Daftar Data Penghuni ===" << endl;
                tampilkanDataPenghuni(head);
                system("pause");
                break;
            case '3':
                cout << "=== Hapus Data Penghuni ===" << endl;
                hapusDataPenghuni(head, tail);
                simpanKeFile(head, file);
                system("pause");
                break;
            case '4':
                cout << "=== Cari Data Penghuni ===" << endl;
                cariDataPenghuni(head);
                system("pause");
                break;
            case '5':
                editDataPenghuni(head);
                simpanKeFile(head, file);
                system("pause");
                break;
            case '6':
                cout << "Keluar..." << endl;
                break;
                default:
                cout << "Pilihan tidak valid!" << endl;
                system("pause");
        }
    } while(pilihan != '6');
}