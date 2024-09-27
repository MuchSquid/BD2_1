#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <filesystem>
#include <algorithm>
using namespace std;

struct Record {
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
    bool operator==(const Record& other) const {
        return strcmp(id, other.id) == 0;
    }
};