#ifndef STRUCT_H
#define STRUCT_H

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

    // Mostrar datos del registro
    void print() const {
        cout << "ID: " << id << ", Name: " << name << endl;
    }

    // Comparador por id
    bool operator==(const Record1& other) const {
        return strcmp(id, other.id) == 0;
    }

    Record1(const char* id_, const char* name_, const char* popularity_, const char* duration_ms_,
            const char* explicit__, const char* artists_, const char* id_artists_, const char* release_date_,
            const char* danceability_, const char* energy_, const char* key_str_, const char* loudness_,
            const char* mode_, const char* speechiness_, const char* acousticness_, const char* instrumentalness_,
            const char* liveness_, const char* valence_, const char* tempo_, const char* time_signature_) {

        strncpy(id, id_, sizeof(id) - 1);
        strncpy(name, name_, sizeof(name) - 1);
        strncpy(popularity, popularity_, sizeof(popularity) - 1);
        strncpy(duration_ms, duration_ms_, sizeof(duration_ms) - 1);
        strncpy(explicit_, explicit__, sizeof(explicit_) - 1);
        strncpy(artists, artists_, sizeof(artists) - 1);
        strncpy(id_artists, id_artists_, sizeof(id_artists) - 1);
        strncpy(release_date, release_date_, sizeof(release_date) - 1);
        strncpy(danceability, danceability_, sizeof(danceability) - 1);
        strncpy(energy, energy_, sizeof(energy) - 1);
        strncpy(key_str, key_str_, sizeof(key_str) - 1);
        strncpy(loudness, loudness_, sizeof(loudness) - 1);
        strncpy(mode, mode_, sizeof(mode) - 1);
        strncpy(speechiness, speechiness_, sizeof(speechiness) - 1);
        strncpy(acousticness, acousticness_, sizeof(acousticness) - 1);
        strncpy(instrumentalness, instrumentalness_, sizeof(instrumentalness) - 1);
        strncpy(liveness, liveness_, sizeof(liveness) - 1);
        strncpy(valence, valence_, sizeof(valence) - 1);
        strncpy(tempo, tempo_, sizeof(tempo) - 1);
        strncpy(time_signature, time_signature_, sizeof(time_signature) - 1);

    }

    // Record1() {};
    Record1() : left(-1), right(-1), height(1) {};

};



// template<typename R>

// vector<R> leerCSV(const string& filename, bool tipo) {
//     ifstream file(filename);
//     vector<R> records;
//     string line;

//     if (!file.is_open()) {
//         cerr << "No se pudo abrir el archivo.\n";
//         return records;
//     }
//     if(tipo==true){//RECORDTIPO1

//     while (getline(file, line)) {
//         stringstream ss(line);
//         string token;
//         R record;

//         // Leer cada campo del CSV y asignarlo al record (todas son cadenas de texto)

//         getline(ss, token, ',');
//         strncpy(record.id, token.c_str(), sizeof(record.id));
//         record.id[sizeof(record.id) - 1] = '\0';

//         getline(ss, token, ',');
//         strncpy(record.name, token.c_str(), sizeof(record.name));
//         record.name[sizeof(record.name) - 1] = '\0';

//         getline(ss, token, ',');
//         strncpy(record.popularity, token.c_str(), sizeof(record.popularity));
//         record.popularity[sizeof(record.popularity) - 1] = '\0';

//         getline(ss, token, ',');
//         strncpy(record.duration_ms, token.c_str(), sizeof(record.duration_ms));
//         record.duration_ms[sizeof(record.duration_ms) - 1] = '\0';

//         getline(ss, token, ',');
//         strncpy(record.explicit_, token.c_str(), sizeof(record.explicit_));
//         record.explicit_[sizeof(record.explicit_) - 1] = '\0';

//         getline(ss, token, ',');
//         strncpy(record.artists, token.c_str(), sizeof(record.artists));
//         record.artists[sizeof(record.artists) - 1] = '\0';

//         getline(ss, token, ',');
//         strncpy(record.id_artists, token.c_str(), sizeof(record.id_artists));
//         record.id_artists[sizeof(record.id_artists) - 1] = '\0';

//         getline(ss, token, ',');
//         strncpy(record.release_date, token.c_str(), sizeof(record.release_date));
//         record.release_date[sizeof(record.release_date) - 1] = '\0';

//         getline(ss, token, ',');
//         strncpy(record.danceability, token.c_str(), sizeof(record.danceability));
//         record.danceability[sizeof(record.danceability) - 1] = '\0';

//         getline(ss, token, ',');
//         strncpy(record.energy, token.c_str(), sizeof(record.energy));
//         record.energy[sizeof(record.energy) - 1] = '\0';

//         getline(ss, token, ',');
//         strncpy(record.key_str, token.c_str(), sizeof(record.key_str));
//         record.key_str[sizeof(record.key_str) - 1] = '\0';

//         getline(ss, token, ',');
//         strncpy(record.loudness, token.c_str(), sizeof(record.loudness));
//         record.loudness[sizeof(record.loudness) - 1] = '\0';

//         getline(ss, token, ',');
//         strncpy(record.mode, token.c_str(), sizeof(record.mode));
//         record.mode[sizeof(record.mode) - 1] = '\0';

//         getline(ss, token, ',');
//         strncpy(record.speechiness, token.c_str(), sizeof(record.speechiness));
//         record.speechiness[sizeof(record.speechiness) - 1] = '\0';

//         getline(ss, token, ',');
//         strncpy(record.acousticness, token.c_str(), sizeof(record.acousticness));
//         record.acousticness[sizeof(record.acousticness) - 1] = '\0';

//         getline(ss, token, ',');
//         strncpy(record.instrumentalness, token.c_str(), sizeof(record.instrumentalness));
//         record.instrumentalness[sizeof(record.instrumentalness) - 1] = '\0';

//         getline(ss, token, ',');
//         strncpy(record.liveness, token.c_str(), sizeof(record.liveness));
//         record.liveness[sizeof(record.liveness) - 1] = '\0';

//         getline(ss, token, ',');
//         strncpy(record.valence, token.c_str(), sizeof(record.valence));
//         record.valence[sizeof(record.valence) - 1] = '\0';

//         getline(ss, token, ',');
//         strncpy(record.tempo, token.c_str(), sizeof(record.tempo));
//         record.tempo[sizeof(record.tempo) - 1] = '\0';

//         getline(ss, token, ',');
//         strncpy(record.time_signature, token.c_str(), sizeof(record.time_signature));
//         record.time_signature[sizeof(record.time_signature) - 1] = '\0';
//         // Agregar el record al vector
//         records.push_back(record);
//     }
//     file.close();
//     return records;
//     }

//     else{//RECORD TIPO2
//             while (getline(file, line)) {
//         stringstream ss(line);
//         string token;
//         R record;

//         // Leer cada campo del CSV y asignarlo al record (todas son cadenas de texto)

//         getline(ss, token, ',');
//         strncpy(record.id, token.c_str(), sizeof(record.id));
//         record.id[sizeof(record.id) - 1] = '\0';

//         getline(ss, token, ',');
//         strncpy(record.date, token.c_str(), sizeof(record.date));
//         record.date[sizeof(record.date) - 1] = '\0';

//         getline(ss, token, ',');
//         strncpy(record.customer, token.c_str(), sizeof(record.customer));
//         record.customer[sizeof(record.customer) - 1] = '\0';

//         getline(ss, token, ',');
//         strncpy(record.model, token.c_str(), sizeof(record.model));
//         record.model[sizeof(record.model) - 1] = '\0';

//         getline(ss, token, ',');
//         strncpy(record.price, token.c_str(), sizeof(record.price));
//         record.price[sizeof(record.price) - 1] = '\0';

//         getline(ss, token, ',');
//         strncpy(record.quantity, token.c_str(), sizeof(record.quantity));
//         record.quantity[sizeof(record.quantity) - 1] = '\0';

//         getline(ss, token, ',');
//         strncpy(record.storelocation, token.c_str(), sizeof(record.storelocation));
//         record.storelocation[sizeof(record.storelocation) - 1] = '\0';

//         getline(ss, token, ',');
//         strncpy(record.salespersonid, token.c_str(), sizeof(record.salespersonid));
//         record.salespersonid[sizeof(record.salespersonid) - 1] = '\0';

//         getline(ss, token, ',');
//         strncpy(record.paymentmethod, token.c_str(), sizeof(record.paymentmethod));
//         record.paymentmethod[sizeof(record.paymentmethod) - 1] = '\0';

//         getline(ss, token, ',');
//         strncpy(record.customerage, token.c_str(), sizeof(record.customerage));
//         record.customerage[sizeof(record.customerage) - 1] = '\0';

//         getline(ss, token, ',');
//         strncpy(record.customergender, token.c_str(), sizeof(record.customergender));
//         record.customergender[sizeof(record.customergender) - 1] = '\0';
//         // Agregar el record al vector
//         records.push_back(record);
//     }
//     file.close();
//     return records;
//     }
// }

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


    long left = -1;
    long right = -1;
    int height = 1;

    // Mostrar datos del registro
    void print() const {
        cout << "ID: " << id << ", Name: " << customer << endl;
    }

    // Comparador por id
    bool operator==(const Record2& other) const {
        return strcmp(id, other.id) == 0;
    }

    Record2() : left(-1), right(-1), height(1) {};

    Record2(const char* id, const char* date, const char* customer, const char* model,
            const char* price, const char* quantity, const char* storelocation,
            const char* salespersonid, const char* paymentmethod,
            const char* customerage, const char* customergender) {
        strcpy(this->id, id);
        strcpy(this->date, date);
        strcpy(this->customer, customer);
        strcpy(this->model, model);
        strcpy(this->price, price);
        strcpy(this->quantity, quantity);
        strcpy(this->storelocation, storelocation);
        strcpy(this->salespersonid, salespersonid);
        strcpy(this->paymentmethod, paymentmethod);
        strcpy(this->customerage, customerage);
        strcpy(this->customergender, customergender);
    }

    ~Record2(){};
};



#endif // STRUCT_H
