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

          // Record1 record(id, name, popularity, duration_ms, explicit_, artists, id_artists, release_date,
          //                 danceability, energy, key_str, loudness, mode, speechiness, acousticness, instrumentalness,
          //                 liveness, valence, tempo, time_signature);
  Record1(char* id, char* name, char* popularity, char* duration_ms, char* explicit_, char* artists, char* id_artists, char* release_date,
                  char* danceability, char* energy, char* key_str, char* loudness, char* mode, char* speechiness, char* acousticness, char* instrumentalness,
                  char* liveness, char* valence, char* tempo, char* time_signature){
    strncpy(this->id, id, sizeof(this->id));
    this->id[sizeof(this->id) - 1] = '\0'; 

    strncpy(this->name, name, sizeof(this->name));
    this->name[sizeof(this->name) - 1] = '\0';

    strncpy(this->popularity, popularity, sizeof(this->popularity));
    this->popularity[sizeof(this->popularity) - 1] = '\0';

    strncpy(this->duration_ms, duration_ms, sizeof(this->duration_ms));
    this->duration_ms[sizeof(this->duration_ms) - 1] = '\0';

    strncpy(this->explicit_, explicit_, sizeof(this->explicit_));
    this->explicit_[sizeof(this->explicit_) - 1] = '\0';

    strncpy(this->artists, artists, sizeof(this->artists));
    this->artists[sizeof(this->artists) - 1] = '\0';

    strncpy(this->id_artists, id_artists, sizeof(this->id_artists));
    this->id_artists[sizeof(this->id_artists) - 1] = '\0';

    strncpy(this->release_date, release_date, sizeof(this->release_date));
    this->release_date[sizeof(this->release_date) - 1] = '\0';

    strncpy(this->danceability, danceability, sizeof(this->danceability));
    this->danceability[sizeof(this->danceability) - 1] = '\0';

    strncpy(this->energy, energy, sizeof(this->energy));
    this->energy[sizeof(this->energy) - 1] = '\0';

    strncpy(this->key_str, key_str, sizeof(this->key_str));
    this->key_str[sizeof(this->key_str) - 1] = '\0';
    
    strncpy(this->loudness, loudness, sizeof(this->loudness));
    this->loudness[sizeof(this->loudness) - 1] = '\0';

    strncpy(this->mode, mode, sizeof(this->mode));
    this->mode[sizeof(this->mode) - 1] = '\0';
    
    strncpy(this->speechiness, speechiness, sizeof(this->speechiness));
    this->speechiness[sizeof(this->speechiness) - 1] = '\0';

    strncpy(this->acousticness, acousticness, sizeof(this->acousticness));
    this->acousticness[sizeof(this->acousticness) - 1] = '\0';

    strncpy(this->instrumentalness, instrumentalness, sizeof(this->instrumentalness));
    this->instrumentalness[sizeof(this->instrumentalness) - 1] = '\0';

    strncpy(this->liveness, liveness, sizeof(this->liveness));
    this->liveness[sizeof(this->liveness) - 1] = '\0';
    
    strncpy(this->valence, valence, sizeof(this->valence));
    this->valence[sizeof(this->valence) - 1] = '\0';
    
    strncpy(this->tempo, tempo, sizeof(this->tempo));
    this->tempo[sizeof(this->tempo) - 1] = '\0';

    strncpy(this->time_signature, time_signature, sizeof(this->time_signature));
    this->time_signature[sizeof(this->time_signature) - 1] = '\0';
  }


  // Mostrar datos del registro
  void print() const {
      cout << "ID: " << id << ", Name: " << name << endl;
  }

  // Comparador por id
  bool operator==(const Record1& other) const {
      return strcmp(id, other.id) == 0;
  }
};

struct Record2 {
  char id[7];
  char date[11];
  char customer[5];
  char model[14];
  char price[8];
  char quantity[2];
  char storelocation[13];
  char salespersonid[4];
  char paymentmethod[12];
  char customerage[3];
  char customergender[7];
  Record2(){};

  Record2(char* id, char* date, char* customer, char* model, char* price, char* quantity, char* storelocation, char* salespersonid, char* paymentmethod, char* customerage, char* customergender){
    strncpy(this->id, id, sizeof(this->id));
    this->id[sizeof(this->id) - 1] = '\0'; 

    strncpy(this->date, date, sizeof(this->date));
    this->date[sizeof(this->date) - 1] = '\0';

    strncpy(this->customer, customer, sizeof(this->customer));
    this->customer[sizeof(this->customer) - 1] = '\0';

    strncpy(this->model, model, sizeof(this->model));
    this->model[sizeof(this->model) - 1] = '\0';

    strncpy(this->price, price, sizeof(this->price));
    this->price[sizeof(this->price) - 1] = '\0';

    strncpy(this->quantity, quantity, sizeof(this->quantity));
    this->quantity[sizeof(this->quantity) - 1] = '\0';

    strncpy(this->storelocation, storelocation, sizeof(this->storelocation));
    this->storelocation[sizeof(this->storelocation) - 1] = '\0';

    strncpy(this->salespersonid, salespersonid, sizeof(this->salespersonid));
    this->salespersonid[sizeof(this->salespersonid) - 1] = '\0';

    strncpy(this->paymentmethod, paymentmethod, sizeof(this->paymentmethod));
    this->paymentmethod[sizeof(this->paymentmethod) - 1] = '\0';

    strncpy(this->customerage, customerage, sizeof(this->customerage));
    this->customerage[sizeof(this->customerage) - 1] = '\0';

    strncpy(this->customergender, customergender, sizeof(this->customergender));
    this->customergender[sizeof(this->customergender) - 1] = '\0';
  }

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

// vector<Record1>leerCSV(const string& filename) {
//     ifstream file(filename);
//     vector<Record1> records;
//     string line;
//
//     if (!file.is_open()) {
//         cerr << "No se pudo abrir el archivo.\n";
//         return records;
//     }
//         
//     while (getline(file, line)) {
//         stringstream ss(line);
//         string token;
//         Record1 record;
//
//         // Leer cada campo del CSV y asignarlo al record (todas son cadenas de texto)
//
//         getline(ss, token, ',');
//         strncpy(record.id, token.c_str(), sizeof(record.id));
//         record.id[sizeof(record.id) - 1] = '\0'; 
//
//         getline(ss, token, ',');
//         strncpy(record.name, token.c_str(), sizeof(record.name));
//         record.name[sizeof(record.name) - 1] = '\0';
//
//         getline(ss, token, ',');
//         strncpy(record.popularity, token.c_str(), sizeof(record.popularity));
//         record.popularity[sizeof(record.popularity) - 1] = '\0';
//
//         getline(ss, token, ',');
//         strncpy(record.duration_ms, token.c_str(), sizeof(record.duration_ms));
//         record.duration_ms[sizeof(record.duration_ms) - 1] = '\0';
//
//         getline(ss, token, ',');
//         strncpy(record.explicit_, token.c_str(), sizeof(record.explicit_));
//         record.explicit_[sizeof(record.explicit_) - 1] = '\0';
//
//         getline(ss, token, ',');
//         strncpy(record.artists, token.c_str(), sizeof(record.artists));
//         record.artists[sizeof(record.artists) - 1] = '\0';
//
//         getline(ss, token, ',');
//         strncpy(record.id_artists, token.c_str(), sizeof(record.id_artists));
//         record.id_artists[sizeof(record.id_artists) - 1] = '\0';
//
//         getline(ss, token, ',');
//         strncpy(record.release_date, token.c_str(), sizeof(record.release_date));
//         record.release_date[sizeof(record.release_date) - 1] = '\0';
//
//         getline(ss, token, ',');
//         strncpy(record.danceability, token.c_str(), sizeof(record.danceability));
//         record.danceability[sizeof(record.danceability) - 1] = '\0';
//
//         getline(ss, token, ',');
//         strncpy(record.energy, token.c_str(), sizeof(record.energy));
//         record.energy[sizeof(record.energy) - 1] = '\0';
//
//         getline(ss, token, ',');
//         strncpy(record.key_str, token.c_str(), sizeof(record.key_str));
//         record.key_str[sizeof(record.key_str) - 1] = '\0';
//         
//         getline(ss, token, ',');
//         strncpy(record.loudness, token.c_str(), sizeof(record.loudness));
//         record.loudness[sizeof(record.loudness) - 1] = '\0';
//
//         getline(ss, token, ',');
//         strncpy(record.mode, token.c_str(), sizeof(record.mode));
//         record.mode[sizeof(record.mode) - 1] = '\0';
//
//         getline(ss, token, ',');
//         strncpy(record.speechiness, token.c_str(), sizeof(record.speechiness));
//         record.speechiness[sizeof(record.speechiness) - 1] = '\0';
//
//         getline(ss, token, ',');
//         strncpy(record.acousticness, token.c_str(), sizeof(record.acousticness));
//         record.acousticness[sizeof(record.acousticness) - 1] = '\0';
//
//         getline(ss, token, ',');
//         strncpy(record.instrumentalness, token.c_str(), sizeof(record.instrumentalness));
//         record.instrumentalness[sizeof(record.instrumentalness) - 1] = '\0';
//
//         getline(ss, token, ',');
//         strncpy(record.liveness, token.c_str(), sizeof(record.liveness));
//         record.liveness[sizeof(record.liveness) - 1] = '\0';
//
//         getline(ss, token, ',');
//         strncpy(record.valence, token.c_str(), sizeof(record.valence));
//         record.valence[sizeof(record.valence) - 1] = '\0';
//
//         getline(ss, token, ',');
//         strncpy(record.tempo, token.c_str(), sizeof(record.tempo));
//         record.tempo[sizeof(record.tempo) - 1] = '\0';
//
//         getline(ss, token, ',');
//         strncpy(record.time_signature, token.c_str(), sizeof(record.time_signature));
//         record.time_signature[sizeof(record.time_signature) - 1] = '\0';
//         // Agregar el record al vector
//         records.push_back(record);
//     }
//     file.close();
//     return records;
// }
//
// vector<Record2>leerCSV2(const string& filename) {
//     ifstream file(filename);
//     vector<Record2> records;
//     string line;
//
//     if (!file.is_open()) {
//         cerr << "No se pudo abrir el archivo.\n";
//         return records;
//     }
//         
//     while (getline(file, line)) {
//         stringstream ss(line);
//         string token;
//         Record2 record;
//
//         // Leer cada campo del CSV y asignarlo al record (todas son cadenas de texto)
// getline(ss, token, ',');
//         strncpy(record.id, token.c_str(), sizeof(record.id));
//         record.id[sizeof(record.id) - 1] = '\0'; 
//
//         getline(ss, token, ',');
//         strncpy(record.date, token.c_str(), sizeof(record.date));
//         record.date[sizeof(record.date) - 1] = '\0';
//
//         getline(ss, token, ',');
//         strncpy(record.customer, token.c_str(), sizeof(record.customer));
//         record.customer[sizeof(record.customer) - 1] = '\0';
//
//         getline(ss, token, ',');
//         strncpy(record.model, token.c_str(), sizeof(record.model));
//         record.model[sizeof(record.model) - 1] = '\0';
//
//         getline(ss, token, ',');
//         strncpy(record.price, token.c_str(), sizeof(record.price));
//         record.price[sizeof(record.price) - 1] = '\0';
//
//         getline(ss, token, ',');
//         strncpy(record.quantity, token.c_str(), sizeof(record.quantity));
//         record.quantity[sizeof(record.quantity) - 1] = '\0';
//
//         getline(ss, token, ',');
//         strncpy(record.storelocation, token.c_str(), sizeof(record.storelocation));
//         record.storelocation[sizeof(record.storelocation) - 1] = '\0';
//
//         getline(ss, token, ',');
//         strncpy(record.salespersonid, token.c_str(), sizeof(record.salespersonid));
//         record.salespersonid[sizeof(record.salespersonid) - 1] = '\0';
//
//         getline(ss, token, ',');
//         strncpy(record.paymentmethod, token.c_str(), sizeof(record.paymentmethod));
//         record.paymentmethod[sizeof(record.paymentmethod) - 1] = '\0';
//
//         getline(ss, token, ',');
//         strncpy(record.customerage, token.c_str(), sizeof(record.customerage));
//         record.customerage[sizeof(record.customerage) - 1] = '\0';
//
//         getline(ss, token, ',');
//         strncpy(record.customergender, token.c_str(), sizeof(record.customergender));
//         record.customergender[sizeof(record.customergender) - 1] = '\0';
//         // Agregar el record al vector
//         records.push_back(record);
//     }
//     file.close();
//     return records;
// }
