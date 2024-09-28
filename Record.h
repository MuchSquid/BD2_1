#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stack>
#include <queue>
#include <cstring>
using namespace std;

struct Record1 {
    char id[23];
    char name[529];
    char popularity[3];
    char duration_ms[7];
    char explicit_[1];
    char artists[934];
    char id_artists[1508];
    char release_date[10];
    char danceability[6];
    char energy[8];
    char key_str[2];
    char loudness[7];
    char mode[1];
    char speechiness[6];
    char acousticness[8];
    char instrumentalness[8];
    char liveness[7];
    char valence[8];
    char tempo[7];
    char time_signature[1];

    long left = -1;
    long right = -1;
    int height = 1;
    Record1(){};
    // Mostrar datos del registro
    void print() const {
        cout << "ID: " << id << ", Name: " << name << endl;
    }

    // Comparador por id
    bool operator==(const Record& other) const {
        return strcmp(id, other.id) == 0;
    }
};

struct Record2 {
    char id[7];
    char date[10];
    char customer[4];
    char model[13];
    char price[7];
    char quantity[1];
    char storelocation[12];
    char salespersonid[3];
    char paymentmethod[11];
    char customerage[2];
    char customergender[6];
    Record2(){};

    long left = -1;
    long right = -1;
    int height = 1;

    // Mostrar datos del registro
    void print() const {
        cout << "ID: " << id << ", Numero de cliente: " << customer << endl;
    }

    // Comparador por id
    bool operator==(const Record2& other) const {
        return strcmp(id, other.id) == 0;
    }
};

vector<Record1>leerCSV(const string& filename) {
    ifstream file(filename);
    vector<Record1> records;
    string line;

    if (!file.is_open()) {
        cerr << "No se pudo abrir el archivo.\n";
        return records;
    }
        
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        Record1 record;

        // Leer cada campo del CSV y asignarlo al record (todas son cadenas de texto)

        getline(ss, token, ',');
        strncpy(record.id, token.c_str(), sizeof(record.id));
        record.id[sizeof(record.id) - 1] = '\0'; 

        getline(ss, token, ',');
        strncpy(record.name, token.c_str(), sizeof(record.name));
        record.name[sizeof(record.name) - 1] = '\0';

        getline(ss, token, ',');
        strncpy(record.popularity, token.c_str(), sizeof(record.popularity));
        record.popularity[sizeof(record.popularity) - 1] = '\0';

        getline(ss, token, ',');
        strncpy(record.duration_ms, token.c_str(), sizeof(record.duration_ms));
        record.duration_ms[sizeof(record.duration_ms) - 1] = '\0';

        getline(ss, token, ',');
        strncpy(record.explicit_, token.c_str(), sizeof(record.explicit_));
        record.explicit_[sizeof(record.explicit_) - 1] = '\0';

        getline(ss, token, ',');
        strncpy(record.artists, token.c_str(), sizeof(record.artists));
        record.artists[sizeof(record.artists) - 1] = '\0';

        getline(ss, token, ',');
        strncpy(record.id_artists, token.c_str(), sizeof(record.id_artists));
        record.id_artists[sizeof(record.id_artists) - 1] = '\0';

        getline(ss, token, ',');
        strncpy(record.release_date, token.c_str(), sizeof(record.release_date));
        record.release_date[sizeof(record.release_date) - 1] = '\0';

        getline(ss, token, ',');
        strncpy(record.danceability, token.c_str(), sizeof(record.danceability));
        record.danceability[sizeof(record.danceability) - 1] = '\0';

        getline(ss, token, ',');
        strncpy(record.energy, token.c_str(), sizeof(record.energy));
        record.energy[sizeof(record.energy) - 1] = '\0';

        getline(ss, token, ',');
        strncpy(record.key_str, token.c_str(), sizeof(record.key_str));
        record.key_str[sizeof(record.key_str) - 1] = '\0';
        
        getline(ss, token, ',');
        strncpy(record.loudness, token.c_str(), sizeof(record.loudness));
        record.loudness[sizeof(record.loudness) - 1] = '\0';

        getline(ss, token, ',');
        strncpy(record.mode, token.c_str(), sizeof(record.mode));
        record.mode[sizeof(record.mode) - 1] = '\0';

        getline(ss, token, ',');
        strncpy(record.speechiness, token.c_str(), sizeof(record.speechiness));
        record.speechiness[sizeof(record.speechiness) - 1] = '\0';

        getline(ss, token, ',');
        strncpy(record.acousticness, token.c_str(), sizeof(record.acousticness));
        record.acousticness[sizeof(record.acousticness) - 1] = '\0';

        getline(ss, token, ',');
        strncpy(record.instrumentalness, token.c_str(), sizeof(record.instrumentalness));
        record.instrumentalness[sizeof(record.instrumentalness) - 1] = '\0';

        getline(ss, token, ',');
        strncpy(record.liveness, token.c_str(), sizeof(record.liveness));
        record.liveness[sizeof(record.liveness) - 1] = '\0';

        getline(ss, token, ',');
        strncpy(record.valence, token.c_str(), sizeof(record.valence));
        record.valence[sizeof(record.valence) - 1] = '\0';

        getline(ss, token, ',');
        strncpy(record.tempo, token.c_str(), sizeof(record.tempo));
        record.tempo[sizeof(record.tempo) - 1] = '\0';

        getline(ss, token, ',');
        strncpy(record.time_signature, token.c_str(), sizeof(record.time_signature));
        record.time_signature[sizeof(record.time_signature) - 1] = '\0';
        // Agregar el record al vector
        records.push_back(record);
    }
    file.close();
    return records;
}

vector<Record2>leerCSV2(const string& filename) {
    ifstream file(filename);
    vector<Record2> records;
    string line;

    if (!file.is_open()) {
        cerr << "No se pudo abrir el archivo.\n";
        return records;
    }
        
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        Record2 record;

        // Leer cada campo del CSV y asignarlo al record (todas son cadenas de texto)
getline(ss, token, ',');
        strncpy(record.id, token.c_str(), sizeof(record.id));
        record.id[sizeof(record.id) - 1] = '\0'; 

        getline(ss, token, ',');
        strncpy(record.date, token.c_str(), sizeof(record.date));
        record.date[sizeof(record.date) - 1] = '\0';

        getline(ss, token, ',');
        strncpy(record.customer, token.c_str(), sizeof(record.customer));
        record.customer[sizeof(record.customer) - 1] = '\0';

        getline(ss, token, ',');
        strncpy(record.model, token.c_str(), sizeof(record.model));
        record.model[sizeof(record.model) - 1] = '\0';

        getline(ss, token, ',');
        strncpy(record.price, token.c_str(), sizeof(record.price));
        record.price[sizeof(record.price) - 1] = '\0';

        getline(ss, token, ',');
        strncpy(record.quantity, token.c_str(), sizeof(record.quantity));
        record.quantity[sizeof(record.quantity) - 1] = '\0';

        getline(ss, token, ',');
        strncpy(record.storelocation, token.c_str(), sizeof(record.storelocation));
        record.storelocation[sizeof(record.storelocation) - 1] = '\0';

        getline(ss, token, ',');
        strncpy(record.salespersonid, token.c_str(), sizeof(record.salespersonid));
        record.salespersonid[sizeof(record.salespersonid) - 1] = '\0';

        getline(ss, token, ',');
        strncpy(record.paymentmethod, token.c_str(), sizeof(record.paymentmethod));
        record.paymentmethod[sizeof(record.paymentmethod) - 1] = '\0';

        getline(ss, token, ',');
        strncpy(record.customerage, token.c_str(), sizeof(record.customerage));
        record.customerage[sizeof(record.customerage) - 1] = '\0';

        getline(ss, token, ',');
        strncpy(record.customergender, token.c_str(), sizeof(record.customergender));
        record.customergender[sizeof(record.customergender) - 1] = '\0';
        // Agregar el record al vector
        records.push_back(record);
    }
    file.close();
    return records;
}
