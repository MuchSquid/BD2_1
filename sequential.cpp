#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <cstdio>
#include <functional>
#include <type_traits>

using namespace std;


struct Record {
    char key[40];
    char id[200];
    char name[100];
    char popularity[10];
    char duration_ms[15];
    char explicit_[5];
    char artists[200];
    char id_artists[100];
    char release_date[15];
    char danceability[10];
    char energy[10];
    char loudness[10];
    char mode[5];
    char speechiness[10];
    char acousticness[10];
    char instrumentalness[10];
    char liveness[10];
    char valence[10];
    char tempo[10];
};

struct Record2 {
    char id[7];
    char date[10];
    char customer[4];
    char model[13];
    char price[7];
    char quantity[2];
    char storelocation[12];
    char salespersonid[3];
    char paymentmethod[11];
    char customerage[3];
    char customergender[6];
};


struct Cabezera {
    int size;
    int pos;
    bool isAux;
    Cabezera() : size(0), pos(-1), isAux(false) {}
    Cabezera(int s, int p, bool aux) : size(s), pos(p), isAux(aux) {}
};

struct Cabezera_aux {
    int size_aux;
    Cabezera_aux() : size_aux(0) {}
    Cabezera_aux(int s) : size_aux(s) {}
};

template <typename RecordType>
class SequentialFile {
private:
    string filename;
    string filename_aux = "datos_aux.dat";
    static const int RegistroFull;
    function<bool(const RecordType&, const RecordType&)> comparator;
    function<void(const RecordType&)> printer;

public:
    SequentialFile(string filename,
                   function<bool(const RecordType&, const RecordType&)> comparator,
                   function<void(const RecordType&)> printer)
            : filename(filename), comparator(comparator), printer(printer) {
        fstream file(this->filename_aux, ios::out | ios::binary | ios::trunc);
        Cabezera_aux cabezera_aux(0);
        file.write(reinterpret_cast<char*>(&cabezera_aux), sizeof(Cabezera_aux));
        file.close();
    }

    void printData() {
        fstream file(this->filename, ios::in | ios::binary);
        fstream file_aux(this->filename_aux, ios::in | ios::binary);

        if (!file.is_open()) {
            throw("No se pudo abrir el archivo principal.");
        }

        if (!file_aux.is_open()) {
            throw("No se pudo abrir el archivo auxiliar.");
        }

        // Leer y mostrar la cabecera del archivo principal
        Cabezera cabezera;
        file.read(reinterpret_cast<char*>(&cabezera), sizeof(Cabezera));

        cout << "----- Cabecera Principal -----\n";
        cout << "Size: " << cabezera.size;
        cout << ", Pos: " << cabezera.pos;
        cout << ", Aux: " << (cabezera.isAux ? "true" : "false") << "\n\n";

        Cabezera_aux cabezera_aux;
        file_aux.read(reinterpret_cast<char*>(&cabezera_aux), sizeof(Cabezera_aux));
        cout << "----- Cabecera Auxiliar -----\n";
        cout << "Size_aux: " << cabezera_aux.size_aux << "\n\n";

        auto traverseAndPrint = [&](fstream& currentFile, int currentPos, bool currentIsAux) {
            while (currentPos != -1) {
                int filePos = currentIsAux ? sizeof(Cabezera_aux) + (currentPos - 1) * RegistroFull
                                           : sizeof(Cabezera) + (currentPos - 1) * RegistroFull;

                currentFile.seekg(filePos, ios::beg);

                RecordType record;
                if (!currentFile.read(reinterpret_cast<char*>(&record), sizeof(RecordType))) {
                    cerr << "Error al leer el registro." << endl;
                    break;
                }

                int siguientePos;
                char isAuxChar;
                if (!currentFile.read(reinterpret_cast<char*>(&siguientePos), sizeof(int)) ||
                    !currentFile.read(reinterpret_cast<char*>(&isAuxChar), sizeof(char))) {
                    cerr << "Error al leer los punteros del registro." << endl;
                    break;
                }

                bool isAux = (isAuxChar != 0);

                printer(record);
                currentPos = siguientePos;
                currentIsAux = isAux;
            }
        };

        cout << "----- Registros en Archivo Principal -----\n\n";
        if (cabezera.pos != -1) { // Verificar si hay registros
            traverseAndPrint(file, cabezera.pos, cabezera.isAux);
        } else {
            cout << "No hay registros en el archivo principal.\n\n";
        }

        cout << "----- Registros en Archivo Auxiliar -----\n\n";
        if (cabezera_aux.size_aux > 0) {
            file_aux.seekg(sizeof(Cabezera_aux), ios::beg);
            for (int i = 0; i < cabezera_aux.size_aux; ++i) {
                RecordType record;
                if (!file_aux.read(reinterpret_cast<char*>(&record), sizeof(RecordType))) {
                    cerr << "Error al leer el registro auxiliar." << endl;
                    break;
                }

                int siguientePos;
                char isAuxChar;
                if (!file_aux.read(reinterpret_cast<char*>(&siguientePos), sizeof(int)) ||
                    !file_aux.read(reinterpret_cast<char*>(&isAuxChar), sizeof(char))) {
                    cerr << "Error al leer los punteros del registro auxiliar." << endl;
                    break;
                }
                printer(record);
            }
        } else {
            cout << "No hay registros en el archivo auxiliar.\n\n";
        }

        file_aux.close();
        file.close();
    }

    // Insertar todos los registros iniciales
    void insertAll(vector<RecordType> records) {
        fstream file(this->filename, ios::out | ios::binary | ios::trunc);
        if (!file.is_open()) throw("No se pudo abrir el archivo principal.");

        sort(records.begin(), records.end(), comparator);
        Cabezera cabezera(records.size(), 1, false);
        file.write(reinterpret_cast<const char*>(&cabezera), sizeof(Cabezera));

        for (int i = 0; i < records.size(); ++i) {
            file.write(reinterpret_cast<const char*>(&records[i]), sizeof(RecordType));
            int siguientePos = (i == records.size() - 1) ? -1 : (i + 2);
            bool isAux = false;
            file.write(reinterpret_cast<const char*>(&siguientePos), sizeof(int));
            file.write(reinterpret_cast<const char*>(&isAux), sizeof(bool));
        }

        file.close();
    }

    void add(RecordType record) {
        fstream file(this->filename, ios::in | ios::out | ios::binary);
        fstream file_aux(this->filename_aux, ios::in | ios::out | ios::binary);
        if (!file.is_open()) throw("No se pudo abrir el archivo principal.");
        if (!file_aux.is_open()) throw("No se pudo abrir el archivo auxiliar.");

        Cabezera cabezera;
        file.seekg(0, ios::beg);
        file.read(reinterpret_cast<char*>(&cabezera), sizeof(Cabezera));

        Cabezera_aux cabezera_aux;
        file_aux.seekg(0, ios::beg);
        file_aux.read(reinterpret_cast<char*>(&cabezera_aux), sizeof(Cabezera_aux));

        int prevPos = -1;
        bool prevIsAux = false;
        int currentPos = cabezera.pos;
        bool currentIsAux = cabezera.isAux;

        RecordType temp;
        int tempSiguientePos;
        bool tempIsAux;

        while (currentPos != -1) {
            fstream& currentFile = currentIsAux ? file_aux : file;

            int filePos = currentIsAux ? sizeof(Cabezera_aux) + (currentPos - 1) * RegistroFull
                                       : sizeof(Cabezera) + (currentPos - 1) * RegistroFull;

            currentFile.seekg(filePos, ios::beg);
            currentFile.read(reinterpret_cast<char*>(&temp), sizeof(RecordType));
            currentFile.read(reinterpret_cast<char*>(&tempSiguientePos), sizeof(int));
            currentFile.read(reinterpret_cast<char*>(&tempIsAux), sizeof(bool));

            if (comparator(record, temp)) {
                break;
            } else {
                prevPos = currentPos;
                prevIsAux = currentIsAux;
                currentPos = tempSiguientePos;
                currentIsAux = tempIsAux;
            }
        }

        // Insertar el nuevo registro en el archivo auxiliar
        int newPosAux = cabezera_aux.size_aux + 1;

        int newFilePos = sizeof(Cabezera_aux) + (newPosAux - 1) * RegistroFull;
        file_aux.seekp(newFilePos, ios::beg);
        file_aux.write(reinterpret_cast<const char*>(&record), sizeof(RecordType));

        // El nuevo registro apunta a currentPos
        int nuevoSiguientePos = currentPos;
        bool nuevoIsAux = currentIsAux;
        file_aux.write(reinterpret_cast<const char*>(&nuevoSiguientePos), sizeof(int));
        file_aux.write(reinterpret_cast<const char*>(&nuevoIsAux), sizeof(bool));

        // Actualizar la cabecera del archivo auxiliar
        cabezera_aux.size_aux++;
        file_aux.seekp(0, ios::beg);
        file_aux.write(reinterpret_cast<const char*>(&cabezera_aux), sizeof(Cabezera_aux));

        // Actualizar el puntero del registro anterior
        if (prevPos == -1) {
            cabezera.pos = newPosAux;
            cabezera.isAux = true;
            file.seekp(0, ios::beg);
            file.write(reinterpret_cast<const char*>(&cabezera), sizeof(Cabezera));
        } else {
            fstream& prevFile = prevIsAux ? file_aux : file;
            int prevFilePos = prevIsAux ? sizeof(Cabezera_aux) + (prevPos - 1) * RegistroFull + sizeof(RecordType)
                                        : sizeof(Cabezera) + (prevPos - 1) * RegistroFull + sizeof(RecordType);
            prevFile.seekp(prevFilePos, ios::beg);
            prevFile.write(reinterpret_cast<const char*>(&newPosAux), sizeof(int));
            bool auxFlag = true;  // Ya que el nuevo registro está en el archivo auxiliar
            prevFile.write(reinterpret_cast<const char*>(&auxFlag), sizeof(bool));
        }

        if (cabezera_aux.size_aux > log2(cabezera.size / 2)) {
            rebuild();
        }

        file.close();
        file_aux.close();
    }

    vector<RecordType> rangeSearch(const string& begin, const string& end) {
        ifstream mainFile(this->filename, ios::binary);
        if (!mainFile.is_open()) throw("No se pudo abrir el archivo principal.");

        ifstream auxFile(this->filename_aux, ios::binary);
        if (!auxFile.is_open()) throw("No se pudo abrir el archivo auxiliar.");

        vector<RecordType> registrosEncontrados;

        Cabezera cabezera;
        mainFile.read(reinterpret_cast<char*>(&cabezera), sizeof(Cabezera));
        Cabezera_aux cabezera_aux;
        auxFile.read(reinterpret_cast<char*>(&cabezera_aux), sizeof(Cabezera_aux));

        int currentPos = cabezera.pos;
        bool currentIsAux = cabezera.isAux;

        RecordType temp;
        int siguientePos;
        bool isAux;

        while (currentPos != -1) {
            if (currentIsAux) {
                auxFile.seekg(sizeof(Cabezera_aux) + (currentPos - 1) * RegistroFull, ios::beg);
            } else {
                mainFile.seekg(sizeof(Cabezera) + (currentPos - 1) * RegistroFull, ios::beg);
            }

            if (currentIsAux) {
                auxFile.read(reinterpret_cast<char*>(&temp), sizeof(RecordType));
                auxFile.read(reinterpret_cast<char*>(&siguientePos), sizeof(int));
                auxFile.read(reinterpret_cast<char*>(&isAux), sizeof(bool));
            } else {
                mainFile.read(reinterpret_cast<char*>(&temp), sizeof(RecordType));
                mainFile.read(reinterpret_cast<char*>(&siguientePos), sizeof(int));
                mainFile.read(reinterpret_cast<char*>(&isAux), sizeof(bool));
            }


            string key;
            if constexpr (is_same<RecordType, Record>::value) {
                key = string(temp.key);
            } else if constexpr (is_same<RecordType, Record2>::value) {
                key = string(temp.id);
            }

            if (key >= begin && key <= end) {
                registrosEncontrados.push_back(temp);
            }

            currentPos = siguientePos;
            currentIsAux = isAux;
        }

        mainFile.close();
        auxFile.close();

        return registrosEncontrados;
    }

    RecordType search(const string& key) {
        ifstream mainFile(this->filename, ios::binary);
        if (!mainFile.is_open()) throw("No se pudo abrir el archivo principal.");

        ifstream auxFile(this->filename_aux, ios::binary);
        if (!auxFile.is_open()) throw("No se pudo abrir el archivo auxiliar.");

        Cabezera cabezera;
        mainFile.read(reinterpret_cast<char*>(&cabezera), sizeof(Cabezera));

        Cabezera_aux cabezera_aux;
        auxFile.read(reinterpret_cast<char*>(&cabezera_aux), sizeof(Cabezera_aux));

        int currentPos = cabezera.pos;
        bool currentIsAux = cabezera.isAux;
        RecordType temp;
        int siguientePos;
        bool isAux;

        while (currentPos != -1) {
            if (currentIsAux) {
                auxFile.seekg(sizeof(Cabezera_aux) + (currentPos - 1) * RegistroFull, ios::beg);
            } else {
                mainFile.seekg(sizeof(Cabezera) + (currentPos - 1) * RegistroFull, ios::beg);
            }

            if (currentIsAux) {
                auxFile.read(reinterpret_cast<char*>(&temp), sizeof(RecordType));
                auxFile.read(reinterpret_cast<char*>(&siguientePos), sizeof(int));
                auxFile.read(reinterpret_cast<char*>(&isAux), sizeof(bool));
            } else {
                mainFile.read(reinterpret_cast<char*>(&temp), sizeof(RecordType));
                mainFile.read(reinterpret_cast<char*>(&siguientePos), sizeof(int));
                mainFile.read(reinterpret_cast<char*>(&isAux), sizeof(bool));
            }

            string tempKey;
            if constexpr (is_same<RecordType, Record>::value) {
                tempKey = string(temp.key);
            } else if constexpr (is_same<RecordType, Record2>::value) {
                tempKey = string(temp.id);
            }

            if (tempKey == key) {
                mainFile.close();
                auxFile.close();
                return temp;
            }

            currentPos = siguientePos;
            currentIsAux = isAux;
        }

        mainFile.close();
        auxFile.close();
        throw("Registro no encontrado.");
    }

    void rebuild() {
        cout << "\nReconstruyendo..." << endl;

        string tempMain = "datos_new.dat";

        fstream mainFile(this->filename, ios::in | ios::binary);
        if (!mainFile.is_open()) throw("No se pudo abrir el archivo principal.");
        fstream auxFile(this->filename_aux, ios::in | ios::binary);
        if (!auxFile.is_open()) throw("No se pudo abrir el archivo auxiliar.");

        ofstream newMainFile(tempMain, ios::binary | ios::trunc);
        if (!newMainFile.is_open()) throw("No se pudo crear el nuevo archivo principal.");

        Cabezera cabezera;
        mainFile.read(reinterpret_cast<char*>(&cabezera), sizeof(Cabezera));
        Cabezera_aux cabezera_aux;
        auxFile.read(reinterpret_cast<char*>(&cabezera_aux), sizeof(Cabezera_aux));

        Cabezera newCabezera;
        newCabezera.size = 0;
        newCabezera.pos = -1;
        newCabezera.isAux = false;
        newMainFile.write(reinterpret_cast<const char*>(&newCabezera), sizeof(Cabezera));

        int currentPos = cabezera.pos;
        bool currentIsAux = cabezera.isAux;
        RecordType temp;
        int siguientePos;
        bool isAux;
        int newRecordCount = 0;

        vector<RecordType> recordsToWrite;
        while (currentPos != -1) {
            fstream& currentFile = currentIsAux ? auxFile : mainFile;

            int filePos = currentIsAux ? sizeof(Cabezera_aux) + (currentPos - 1) * RegistroFull
                                       : sizeof(Cabezera) + (currentPos - 1) * RegistroFull;

            currentFile.seekg(filePos, ios::beg);
            currentFile.read(reinterpret_cast<char*>(&temp), sizeof(RecordType));
            currentFile.read(reinterpret_cast<char*>(&siguientePos), sizeof(int));
            currentFile.read(reinterpret_cast<char*>(&isAux), sizeof(bool));

            recordsToWrite.push_back(temp);

            // Avanzar al siguiente registro
            currentPos = siguientePos;
            currentIsAux = isAux;
        }

        newRecordCount = recordsToWrite.size();
        newCabezera.size = newRecordCount;
        newCabezera.pos = (newRecordCount > 0) ? 1 : -1;
        newCabezera.isAux = false;

        newMainFile.seekp(0, ios::beg);
        newMainFile.write(reinterpret_cast<const char*>(&newCabezera), sizeof(Cabezera));


        for (size_t i = 0; i < recordsToWrite.size(); ++i) {
            newMainFile.write(reinterpret_cast<const char*>(&recordsToWrite[i]), sizeof(RecordType));
            int siguientePos = (i < recordsToWrite.size() - 1) ? (i + 2) : -1; // Posición basada en 1
            bool isAuxFlag = false;
            newMainFile.write(reinterpret_cast<const char*>(&siguientePos), sizeof(int));
            newMainFile.write(reinterpret_cast<const char*>(&isAuxFlag), sizeof(bool));
        }

        // Cerrar los archivos
        newMainFile.close();
        mainFile.close();
        auxFile.close();

        if (remove(this->filename.c_str()) != 0) {
            throw("No se pudo eliminar el archivo principal original.");
        }
        if (rename(tempMain.c_str(), this->filename.c_str()) != 0) {
            throw("No se pudo renombrar el archivo temporal al nombre principal.");
        }

        remove(filename_aux.c_str());

        ofstream newAuxFile(this->filename_aux, ios::out | ios::binary | ios::trunc);
        if (!newAuxFile.is_open()) throw("No se pudo resetear el archivo auxiliar.");
        Cabezera_aux new_cabezera_aux(0);
        newAuxFile.write(reinterpret_cast<const char*>(&new_cabezera_aux), sizeof(Cabezera_aux));
        newAuxFile.close();

        cout << "Reconstrucción completada.\n" << endl;
    }

    ~SequentialFile() {}
};


template <typename RecordType>
const int SequentialFile<RecordType>::RegistroFull = sizeof(RecordType) + sizeof(int) + sizeof(bool);


template <typename RecordType>
vector<RecordType> readCSV(const string& filename);

template <>
vector<Record> readCSV<Record>(const string& filename) {
    vector<Record> records;
    ifstream file(filename);
    if (!file.is_open()) {
        throw("No se pudo abrir el archivo CSV.");
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string field;
        vector<string> fields;

        while (getline(ss, field, ',')) {
            fields.push_back(field);
        }

        if (fields.size() < 19) {
            cerr << "Línea con campos insuficientes: " << line << endl;
            continue;
        }

        Record record;

        strncpy(record.key, fields[0].c_str(), sizeof(record.key));
        record.key[sizeof(record.key) - 1] = '\0';

        strncpy(record.id, fields[1].c_str(), sizeof(record.id));
        record.id[sizeof(record.id) - 1] = '\0';

        strncpy(record.name, fields[2].c_str(), sizeof(record.name));
        record.name[sizeof(record.name) - 1] = '\0';

        records.push_back(record);
    }

    file.close();
    return records;
}

template <>
vector<Record2> readCSV<Record2>(const string& filename) {
    vector<Record2> records;
    ifstream file(filename);
    if (!file.is_open()) {
        throw("No se pudo abrir el archivo CSV.");
    }

    string line;
    getline(file, line);
    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string token;
        Record2 record;

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

        records.push_back(record);
    }
    file.close();
    return records;
}

int main() {
    string csvFile;
    cout << "Ingrese el nombre del archivo CSV('Clean data.csv' o 'Bike data.csv'): ";
    getline(cin, csvFile);

    if (csvFile == "Clean data.csv") {
        vector<Record> registros;
        try {
            registros = readCSV<Record>(csvFile);
            cout << "Registros leídos desde el CSV: " << registros.size() << "\n" << endl;
        } catch (const char* msg) {
            cerr << msg << endl;
            return 1;
        }

        auto comparator = [](const Record& a, const Record& b) {
            return string(a.id) < string(b.id);
        };

        auto printer = [](const Record& record) {
            cout << "----- Registro -----\n";
            cout << "Id: " << record.key << "\n";
            cout << "Name: " << record.id << "\n";
            cout << "Key: " << record.name << "\n";
            cout << "---------------------\n\n";
        };

        SequentialFile<Record> sf("datos.dat", comparator, printer);

        try {
            sf.insertAll(registros);
            cout << "Registros insertados exitosamente en el archivo secuencial.\n" << endl;
        } catch (const char* msg) {
            cerr << msg << endl;
            return 1;
        }

        sf.printData();

        vector<Record> nuevosRegistros;
        for(int i = 1; i <= 20; ++i){
            Record nuevo;

            string key = to_string(registros.size() + i);
            strncpy(nuevo.key, key.c_str(), sizeof(nuevo.key));
            nuevo.key[sizeof(nuevo.key)-1] = '\0';

            string id = "ID" + to_string(1000 + registros.size() + i);
            strncpy(nuevo.id, id.c_str(), sizeof(nuevo.id));
            nuevo.id[sizeof(nuevo.id)-1] = '\0';

            string name = "New_Song_" + to_string(1000 + registros.size() + i);
            strncpy(nuevo.name, name.c_str(), sizeof(nuevo.name));
            nuevo.name[sizeof(nuevo.name)-1] = '\0';

            string popularity = to_string(60 + (i % 40));
            strncpy(nuevo.popularity, popularity.c_str(), sizeof(nuevo.popularity));
            nuevo.popularity[sizeof(nuevo.popularity)-1] = '\0';

            string duration = to_string(180000 + i * 1000);
            strncpy(nuevo.duration_ms, duration.c_str(), sizeof(nuevo.duration_ms));
            nuevo.duration_ms[sizeof(nuevo.duration_ms)-1] = '\0';

            strncpy(nuevo.explicit_, (i % 2 == 0) ? "0" : "1", sizeof(nuevo.explicit_));
            nuevo.explicit_[sizeof(nuevo.explicit_)-1] = '\0';

            string artists = "[NewArtist" + to_string(1000 + registros.size() + i) + "]";
            strncpy(nuevo.artists, artists.c_str(), sizeof(nuevo.artists));
            nuevo.artists[sizeof(nuevo.artists)-1] = '\0';

            string id_artists = "[NA" + to_string(1000 + registros.size() + i) + "]";
            strncpy(nuevo.id_artists, id_artists.c_str(), sizeof(nuevo.id_artists));
            nuevo.id_artists[sizeof(nuevo.id_artists)-1] = '\0';

            string release_date = "2023-" + to_string((i % 12) + 1) + "-15";
            strncpy(nuevo.release_date, release_date.c_str(), sizeof(nuevo.release_date));
            nuevo.release_date[sizeof(nuevo.release_date)-1] = '\0';

            string danceability = to_string(0.50 + (i * 0.01)).substr(0,4);
            strncpy(nuevo.danceability, danceability.c_str(), sizeof(nuevo.danceability));
            nuevo.danceability[sizeof(nuevo.danceability)-1] = '\0';

            string energy = to_string(0.60 + (i * 0.01)).substr(0,4);
            strncpy(nuevo.energy, energy.c_str(), sizeof(nuevo.energy));
            nuevo.energy[sizeof(nuevo.energy)-1] = '\0';

            string loudness = to_string(-6.0 - i * 0.2).substr(0,4);
            strncpy(nuevo.loudness, loudness.c_str(), sizeof(nuevo.loudness));
            nuevo.loudness[sizeof(nuevo.loudness)-1] = '\0';

            string mode = (i % 2 == 0) ? "1" : "0";
            strncpy(nuevo.mode, mode.c_str(), sizeof(nuevo.mode));
            nuevo.mode[sizeof(nuevo.mode)-1] = '\0';

            string speechiness = to_string(0.03 + (i * 0.01)).substr(0,4);
            strncpy(nuevo.speechiness, speechiness.c_str(), sizeof(nuevo.speechiness));
            nuevo.speechiness[sizeof(nuevo.speechiness)-1] = '\0';

            string acousticness = to_string(0.15 + (i * 0.01)).substr(0,4);
            strncpy(nuevo.acousticness, acousticness.c_str(), sizeof(nuevo.acousticness));
            nuevo.acousticness[sizeof(nuevo.acousticness)-1] = '\0';

            strncpy(nuevo.instrumentalness, "0.00", sizeof(nuevo.instrumentalness));
            nuevo.instrumentalness[sizeof(nuevo.instrumentalness)-1] = '\0';

            string liveness = to_string(0.10 + (i * 0.01)).substr(0,4);
            strncpy(nuevo.liveness, liveness.c_str(), sizeof(nuevo.liveness));
            nuevo.liveness[sizeof(nuevo.liveness)-1] = '\0';

            string valence = to_string(0.55 + (i * 0.01)).substr(0,4);
            strncpy(nuevo.valence, valence.c_str(), sizeof(nuevo.valence));
            nuevo.valence[sizeof(nuevo.valence)-1] = '\0';

            string tempo = to_string(110.0 + i * 1.5).substr(0,5);
            strncpy(nuevo.tempo, tempo.c_str(), sizeof(nuevo.tempo));
            nuevo.tempo[sizeof(nuevo.tempo)-1] = '\0';

            nuevosRegistros.push_back(nuevo);
        }

        // prueba con 20 registros
        for(const auto& nuevo : nuevosRegistros){
            try {
                sf.add(nuevo);
                cout << "Registro '" << nuevo.id << "' agregado exitosamente.\n" << endl;
            } catch (const char* msg) {
                cerr << msg << endl;
            }
        }


        cout << "\n----- Búsqueda por Rango -----\n";
        string rangoInicio = "0a9GUZK98uk5pOZR7ENGoH";
        string rangoFin = "0vd8DuejHuZgFlbfwvRVve";
        try {
            vector<Record> resultados = sf.rangeSearch(rangoInicio, rangoFin);
            cout << "Registros encontrados en el rango \"" << rangoInicio << "\" y \"" << rangoFin << "\":\n";
            for (const auto& r : resultados) {
                cout << "Name: " << r.id << ", Id: " << r.name << ", Key: " << r.key << "\n";
            }
        } catch (const char* msg) {
            cerr << msg << endl;
        }

        cout << "\n----- Búsqueda Después del rebuild -----\n";
        string buscarIDReubicado = "31UeQEthrXrEU88DGYJWf3";
        try {
            Record encontrado = sf.search(buscarIDReubicado);
            cout << "Registro encontrado:\n";
            cout << "ID: " << encontrado.id << ", Name: " << encontrado.name << ", Key: " << encontrado.key << "\n";
        } catch (const char* msg) {
            cerr << msg << endl;
        }

        /*
        remove("datos.dat");
        remove("datos_aux.dat");
        */


    } else if (csvFile == "Bike data.csv") {
        vector<Record2> registros;
        try {
            registros = readCSV<Record2>(csvFile);
            cout << "Registros leídos desde el CSV: " << registros.size() << "\n" << endl;
        } catch (const char* msg) {
            cerr << msg << endl;
            return 1;
        }

        auto comparator = [](const Record2& a, const Record2& b) {
            return string(a.id) < string(b.id);
        };

        auto printer = [](const Record2& record) {
            cout << "----- Registro -----\n";
            cout << "Id: " << record.id << "\n";
            cout << "Date: " << record.date << "\n";
            cout << "Customer: " << record.customer << "\n";
            cout << "---------------------\n\n";
        };

        SequentialFile<Record2> sf("datos.dat", comparator, printer);

        try {
            sf.insertAll(registros);
            cout << "Registros insertados exitosamente en el archivo secuencial.\n" << endl;
        } catch (const char* msg) {
            cerr << msg << endl;
            return 1;
        }

        sf.printData();

        cout << "\n----- Búsqueda por Rango -----\n";
        string rangoInicio = "000364";
        string rangoFin = "000374";
        try {
            vector<Record2> resultados = sf.rangeSearch(rangoInicio, rangoFin);
            cout << "Registros encontrados en el rango \"" << rangoInicio << "\" y \"" << rangoFin << "\":\n";
            for (const auto& r : resultados) {
                cout << "Id: " << r.id << ", Customer: " << r.customer << ", Data: " << r.date << "\n";
            }
        } catch (const char* msg) {
            cerr << msg << endl;
        }



    } else {
        cerr << "Archivo CSV desconocido." << endl;
        return 1;
    }

    return 0;
}

