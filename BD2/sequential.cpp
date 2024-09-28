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
#include "selector.h"

using namespace std;

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

template <typename R>
class SequentialFile: public SeleccionarMetodo<R> {
private:
    string filename;
    string filename_aux = "datos_aux.dat";
    static const int RegistroFull;
    function<bool(const R&, const R&)> comparator;
    function<void(const R&)> printer;

public:
    SequentialFile(string filename,
                   function<bool(const R&, const R&)> comparator,
                   function<void(const R&)> printer)
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

                R record;
                if (!currentFile.read(reinterpret_cast<char*>(&record), sizeof(R))) {
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
                R record;
                if (!file_aux.read(reinterpret_cast<char*>(&record), sizeof(R))) {
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
    void insertAll(vector<R> records) {
        fstream file(this->filename, ios::out | ios::binary | ios::trunc);
        if (!file.is_open()) throw("No se pudo abrir el archivo principal.");

        sort(records.begin(), records.end(), comparator);
        Cabezera cabezera(records.size(), 1, false);
        file.write(reinterpret_cast<const char*>(&cabezera), sizeof(Cabezera));

        for (int i = 0; i < records.size(); ++i) {
            file.write(reinterpret_cast<const char*>(&records[i]), sizeof(R));
            int siguientePos = (i == records.size() - 1) ? -1 : (i + 2);
            bool isAux = false;
            file.write(reinterpret_cast<const char*>(&siguientePos), sizeof(int));
            file.write(reinterpret_cast<const char*>(&isAux), sizeof(bool));
        }

        file.close();
    }

    bool add(R record) {
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

        R temp;
        int tempSiguientePos;
        bool tempIsAux;

        while (currentPos != -1) {
            fstream& currentFile = currentIsAux ? file_aux : file;

            int filePos = currentIsAux ? sizeof(Cabezera_aux) + (currentPos - 1) * RegistroFull
                                       : sizeof(Cabezera) + (currentPos - 1) * RegistroFull;

            currentFile.seekg(filePos, ios::beg);
            currentFile.read(reinterpret_cast<char*>(&temp), sizeof(R));
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
        file_aux.write(reinterpret_cast<const char*>(&record), sizeof(R));

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
            int prevFilePos = prevIsAux ? sizeof(Cabezera_aux) + (prevPos - 1) * RegistroFull + sizeof(R)
                                        : sizeof(Cabezera) + (prevPos - 1) * RegistroFull + sizeof(R);
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

    vector<R> rangeSearch(const string& begin, const string& end) {
        ifstream mainFile(this->filename, ios::binary);
        if (!mainFile.is_open()) throw("No se pudo abrir el archivo principal.");

        ifstream auxFile(this->filename_aux, ios::binary);
        if (!auxFile.is_open()) throw("No se pudo abrir el archivo auxiliar.");

        vector<R> registrosEncontrados;

        Cabezera cabezera;
        mainFile.read(reinterpret_cast<char*>(&cabezera), sizeof(Cabezera));
        Cabezera_aux cabezera_aux;
        auxFile.read(reinterpret_cast<char*>(&cabezera_aux), sizeof(Cabezera_aux));

        int currentPos = cabezera.pos;
        bool currentIsAux = cabezera.isAux;

        R temp;
        int siguientePos;
        bool isAux;

        while (currentPos != -1) {
            if (currentIsAux) {
                auxFile.seekg(sizeof(Cabezera_aux) + (currentPos - 1) * RegistroFull, ios::beg);
            } else {
                mainFile.seekg(sizeof(Cabezera) + (currentPos - 1) * RegistroFull, ios::beg);
            }

            if (currentIsAux) {
                auxFile.read(reinterpret_cast<char*>(&temp), sizeof(R));
                auxFile.read(reinterpret_cast<char*>(&siguientePos), sizeof(int));
                auxFile.read(reinterpret_cast<char*>(&isAux), sizeof(bool));
            } else {
                mainFile.read(reinterpret_cast<char*>(&temp), sizeof(R));
                mainFile.read(reinterpret_cast<char*>(&siguientePos), sizeof(int));
                mainFile.read(reinterpret_cast<char*>(&isAux), sizeof(bool));
            }


            string key;
            if constexpr (is_same<R, Record1>::value) {
                key = string(temp.key);
            } else if constexpr (is_same<R, Record2>::value) {
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

    R search(const string& key) {
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
        R temp;
        int siguientePos;
        bool isAux;

        while (currentPos != -1) {
            if (currentIsAux) {
                auxFile.seekg(sizeof(Cabezera_aux) + (currentPos - 1) * RegistroFull, ios::beg);
            } else {
                mainFile.seekg(sizeof(Cabezera) + (currentPos - 1) * RegistroFull, ios::beg);
            }

            if (currentIsAux) {
                auxFile.read(reinterpret_cast<char*>(&temp), sizeof(R));
                auxFile.read(reinterpret_cast<char*>(&siguientePos), sizeof(int));
                auxFile.read(reinterpret_cast<char*>(&isAux), sizeof(bool));
            } else {
                mainFile.read(reinterpret_cast<char*>(&temp), sizeof(R));
                mainFile.read(reinterpret_cast<char*>(&siguientePos), sizeof(int));
                mainFile.read(reinterpret_cast<char*>(&isAux), sizeof(bool));
            }

            string tempKey;
            if constexpr (is_same<R, Record1>::value) {
                tempKey = string(temp.key);
            } else if constexpr (is_same<R, Record2>::value) {
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
        R temp;
        int siguientePos;
        bool isAux;
        int newRecordCount = 0;

        vector<R> recordsToWrite;
        while (currentPos != -1) {
            fstream& currentFile = currentIsAux ? auxFile : mainFile;

            int filePos = currentIsAux ? sizeof(Cabezera_aux) + (currentPos - 1) * RegistroFull
                                       : sizeof(Cabezera) + (currentPos - 1) * RegistroFull;

            currentFile.seekg(filePos, ios::beg);
            currentFile.read(reinterpret_cast<char*>(&temp), sizeof(R));
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
            newMainFile.write(reinterpret_cast<const char*>(&recordsToWrite[i]), sizeof(R));
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


template <typename R>
const int SequentialFile<R>::RegistroFull = sizeof(R) + sizeof(int) + sizeof(bool);


template <typename R>
vector<R> readCSV(const string& filename);
