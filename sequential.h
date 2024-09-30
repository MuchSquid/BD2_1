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
// #include "selector.h"

using namespace std;

// Eliminamos las definiciones de Record y Record2 ya que ahora están en Record.h

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
class SequentialFile: public SeleccionarMetodo<RecordType> {
private:
    string filename;
    bool tipo;
    string filename_aux = "datos_aux.dat";
    static const int RegistroFull;
    function<bool(const RecordType&, const RecordType&)> comparator;
    function<void(const RecordType&)> printer;

public:
    SequentialFile(string filename,
                   function<bool(const RecordType&, const RecordType&)> comparator,
                   function<void(const RecordType&)> printer, bool tipo)
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
    void insertAll(vector<RecordType> records) override{
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

bool add(RecordType record) override{

        bool yaesta=finding(record.id);

        if(yaesta){
            return false;
        }

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

        int newPosAux = cabezera_aux.size_aux + 1;

        int newFilePos = sizeof(Cabezera_aux) + (newPosAux - 1) * RegistroFull;
        file_aux.seekp(newFilePos, ios::beg);
        file_aux.write(reinterpret_cast<const char*>(&record), sizeof(RecordType));

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
            bool auxFlag = true;  // Ya que el nuevo registro estÃ¡ en el archivo auxiliar
            prevFile.write(reinterpret_cast<const char*>(&auxFlag), sizeof(bool));
        }

        if (cabezera_aux.size_aux > log2(cabezera.size / 2)) {
            rebuild();
        }

        file.close();
        file_aux.close();

        return true;
    }

bool finding(char* key) {
        
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
            if (tipo) {
                tempKey = string(temp.id);
            } else {
                tempKey = string(temp.id);
            }

            if (tempKey == key) {
                mainFile.close();
                auxFile.close();
                return true;
            }

            currentPos = siguientePos;
            currentIsAux = isAux;
        }

        mainFile.close();
        auxFile.close();
        return false;
    }

    vector<RecordType> rangeSearch(char* begin, char* end) override{
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
            if (tipo) {
                key = string(temp.id);
            } else {
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

    pair<RecordType, bool> search(char* key) override{
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
            if (tipo) {
                tempKey = string(temp.id);
            } else{
                tempKey = string(temp.id);
            }

            if (tempKey == key) {
                mainFile.close();
                auxFile.close();
                return make_pair(temp, true);
            }

            currentPos = siguientePos;
            currentIsAux = isAux;
        }

        mainFile.close();
        auxFile.close();
        return make_pair(RecordType(), false);
    }

    // void rebuild() {
    //     cout << "\nReconstruyendo..." << endl;

    //     string tempMain = "datos_new.dat";

    //     fstream mainFile(this->filename, ios::in | ios::binary);
    //     if (!mainFile.is_open()) throw("No se pudo abrir el archivo principal.");
    //     fstream auxFile(this->filename_aux, ios::in | ios::binary);
    //     if (!auxFile.is_open()) throw("No se pudo abrir el archivo auxiliar.");

    //     ofstream newMainFile(tempMain, ios::binary | ios::trunc);
    //     if (!newMainFile.is_open()) throw("No se pudo crear el nuevo archivo principal.");

    //     Cabezera cabezera;
    //     mainFile.read(reinterpret_cast<char*>(&cabezera), sizeof(Cabezera));
    //     Cabezera_aux cabezera_aux;
    //     auxFile.read(reinterpret_cast<char*>(&cabezera_aux), sizeof(Cabezera_aux));

    //     Cabezera newCabezera;
    //     newCabezera.size = 0;
    //     newCabezera.pos = -1;
    //     newCabezera.isAux = false;
    //     newMainFile.write(reinterpret_cast<const char*>(&newCabezera), sizeof(Cabezera));

    //     int currentPos = cabezera.pos;
    //     bool currentIsAux = cabezera.isAux;
    //     RecordType temp;
    //     int siguientePos;
    //     bool isAux;
    //     int newRecordCount = 0;

    //     vector<RecordType> recordsToWrite;
    //     while (currentPos != -1) {
    //         fstream& currentFile = currentIsAux ? auxFile : mainFile;

    //         int filePos = currentIsAux ? sizeof(Cabezera_aux) + (currentPos - 1) * RegistroFull
    //                                    : sizeof(Cabezera) + (currentPos - 1) * RegistroFull;

    //         currentFile.seekg(filePos, ios::beg);
    //         currentFile.read(reinterpret_cast<char*>(&temp), sizeof(RecordType));
    //         currentFile.read(reinterpret_cast<char*>(&siguientePos), sizeof(int));
    //         currentFile.read(reinterpret_cast<char*>(&isAux), sizeof(bool));

    //         recordsToWrite.push_back(temp);

    //         currentPos = siguientePos;
    //         currentIsAux = isAux;
    //     }

    //     newRecordCount = recordsToWrite.size();
    //     newCabezera.size = newRecordCount;
    //     newCabezera.pos = (newRecordCount > 0) ? 1 : -1;
    //     newCabezera.isAux = false;

    //     newMainFile.seekp(0, ios::beg);
    //     newMainFile.write(reinterpret_cast<const char*>(&newCabezera), sizeof(Cabezera));

    //     for (size_t i = 0; i < recordsToWrite.size(); ++i) {
    //         newMainFile.write(reinterpret_cast<const char*>(&recordsToWrite[i]), sizeof(RecordType));
    //         int siguientePos = (i < recordsToWrite.size() - 1) ? (i + 2) : -1; // Posición basada en 1
    //         bool isAuxFlag = false;
    //         newMainFile.write(reinterpret_cast<const char*>(&siguientePos), sizeof(int));
    //         newMainFile.write(reinterpret_cast<const char*>(&isAuxFlag), sizeof(bool));
    //     }

    //     newMainFile.close();
    //     mainFile.close();
    //     auxFile.close();

    //     if (std::remove(this->filename.c_str()) != 0) {
    //         throw("No se pudo eliminar el archivo principal original.");
    //     }
    //     if (rename(tempMain.c_str(), this->filename.c_str()) != 0) {
    //         throw("No se pudo renombrar el archivo temporal al nombre principal.");
    //     }

    //     if (std::remove(filename_aux.c_str()) != 0) {
    //         throw("No se pudo eliminar el archivo auxiliar.");
    //     }

    //     ofstream newAuxFile(this->filename_aux, ios::out | ios::binary | ios::trunc);
    //     if (!newAuxFile.is_open()) throw("No se pudo resetear el archivo auxiliar.");
    //     Cabezera_aux new_cabezera_aux(0);
    //     newAuxFile.write(reinterpret_cast<const char*>(&new_cabezera_aux), sizeof(Cabezera_aux));
    //     newAuxFile.close();

    //     cout << "Reconstrucción completada.\n" << endl;
    // }

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
        int newPos = 1; 

        int lastNewPos = -1;

        while (currentPos != -1) {
            fstream& currentFile = currentIsAux ? auxFile : mainFile;
            
            int filePos = currentIsAux ? sizeof(Cabezera_aux) + (currentPos - 1) * RegistroFull
                                       : sizeof(Cabezera) + (currentPos - 1) * RegistroFull;

            currentFile.seekg(filePos, ios::beg);
            currentFile.read(reinterpret_cast<char*>(&temp), sizeof(RecordType));
            currentFile.read(reinterpret_cast<char*>(&siguientePos), sizeof(int));
            currentFile.read(reinterpret_cast<char*>(&isAux), sizeof(bool));
            
            newMainFile.write(reinterpret_cast<const char*>(&temp), sizeof(RecordType));
            
            int nuevoSiguientePos = (siguientePos != -1) ? (newPos + 1) : -1;
            bool nuevoIsAux = false;
            
            newMainFile.write(reinterpret_cast<const char*>(&nuevoSiguientePos), sizeof(int));
            newMainFile.write(reinterpret_cast<const char*>(&nuevoIsAux), sizeof(bool));
            
            if (newRecordCount == 0) {
                newCabezera.pos = newPos;
                newCabezera.isAux = false;
                newMainFile.seekp(0, ios::beg);
                newMainFile.write(reinterpret_cast<const char*>(&newCabezera), sizeof(Cabezera));
            }
            
            if (lastNewPos != -1) {
                int prevFilePos = sizeof(Cabezera) + (lastNewPos - 1) * RegistroFull + sizeof(RecordType);
                newMainFile.seekp(prevFilePos, ios::beg);
                newMainFile.write(reinterpret_cast<const char*>(&newPos), sizeof(int));
                newMainFile.write(reinterpret_cast<const char*>(&nuevoIsAux), sizeof(bool));
            }
            
            lastNewPos = newPos;
            newPos++;
            newRecordCount++;
            
            currentPos = siguientePos;
            currentIsAux = isAux;
        }
        
        if (lastNewPos != -1) {
            int lastRecordPos = sizeof(Cabezera) + (lastNewPos - 1) * RegistroFull + sizeof(RecordType);
            newMainFile.seekp(lastRecordPos, ios::beg);
            int finalNextPos = -1;
            newMainFile.write(reinterpret_cast<const char*>(&finalNextPos), sizeof(int));
            bool finalIsAux = false;
            newMainFile.write(reinterpret_cast<const char*>(&finalIsAux), sizeof(bool));
        }
        
        newCabezera.size = newRecordCount;
        newMainFile.seekp(0, ios::beg);
        newMainFile.write(reinterpret_cast<const char*>(&newCabezera), sizeof(Cabezera));

        newMainFile.close();
        mainFile.close();
        auxFile.close();
        
        if (std::remove(this->filename.c_str()) != 0) {
            throw("No se pudo eliminar el archivo principal original.");
        }
        if (rename(tempMain.c_str(), this->filename.c_str()) != 0) {
            throw("No se pudo renombrar el archivo temporal al nombre principal.");
        }
        
        if (std::remove(this->filename_aux.c_str()) != 0) {
            throw("No se pudo eliminar el archivo auxiliar.");
        }

        ofstream newAuxFile(this->filename_aux, ios::binary | ios::trunc);
        if (!newAuxFile.is_open()) throw("No se pudo resetear el archivo auxiliar.");
        Cabezera_aux new_cabezera_aux(0);
        newAuxFile.write(reinterpret_cast<const char*>(&new_cabezera_aux), sizeof(Cabezera_aux));
        newAuxFile.close();

        cout << "Reconstrucción completada.\n" << endl;
    }

    bool remove(char* key) override{
        fstream mainFile(this->filename, ios::in | ios::out | ios::binary);
        if (!mainFile.is_open()) throw("No se pudo abrir el archivo principal.");

        fstream auxFile(this->filename_aux, ios::in | ios::out | ios::binary);
        if (!auxFile.is_open()) throw("No se pudo abrir el archivo auxiliar.");

        Cabezera cabezera;
        mainFile.seekg(0, ios::beg);
        mainFile.read(reinterpret_cast<char*>(&cabezera), sizeof(Cabezera));

        Cabezera_aux cabezera_aux;
        auxFile.seekg(0, ios::beg);
        auxFile.read(reinterpret_cast<char*>(&cabezera_aux), sizeof(Cabezera_aux));

        int prevPos = -1;
        bool prevIsAux = false;
        int currentPos = cabezera.pos;
        bool currentIsAux = cabezera.isAux;

        RecordType temp;
        int tempSiguientePos;
        bool tempIsAux;

        while (currentPos != -1) {
            fstream& currentFile = currentIsAux ? auxFile : mainFile;

            int filePos = currentIsAux ? sizeof(Cabezera_aux) + (currentPos - 1) * RegistroFull
                                       : sizeof(Cabezera) + (currentPos - 1) * RegistroFull;

            currentFile.seekg(filePos, ios::beg);
            currentFile.read(reinterpret_cast<char*>(&temp), sizeof(RecordType));
            currentFile.read(reinterpret_cast<char*>(&tempSiguientePos), sizeof(int));
            currentFile.read(reinterpret_cast<char*>(&tempIsAux), sizeof(bool));

            string tempKey;
            if (tipo) {
                tempKey = string(temp.id);
            } else {
                tempKey = string(temp.id);
            }

            if (tempKey == key) {
                // se elimina el reg
                if (prevPos == -1) {
                    
                    cabezera.pos = tempSiguientePos;
                    cabezera.isAux = tempIsAux;
                    mainFile.seekp(0, ios::beg);
                    mainFile.write(reinterpret_cast<const char*>(&cabezera), sizeof(Cabezera));
                } else {
                    
                    fstream& prevFile = prevIsAux ? auxFile : mainFile;
                    int prevFilePos = prevIsAux ? sizeof(Cabezera_aux) + (prevPos - 1) * RegistroFull + sizeof(RecordType)
                                                : sizeof(Cabezera) + (prevPos - 1) * RegistroFull + sizeof(RecordType);
                    prevFile.seekp(prevFilePos, ios::beg);
                    prevFile.write(reinterpret_cast<const char*>(&tempSiguientePos), sizeof(int));
                    prevFile.write(reinterpret_cast<const char*>(&tempIsAux), sizeof(bool));
                }

                //actu de cabecera
                if (tempIsAux) {
                    cabezera_aux.size_aux--;
                    auxFile.seekp(0, ios::beg);
                    auxFile.write(reinterpret_cast<const char*>(&cabezera_aux), sizeof(Cabezera_aux));
                }

                mainFile.close();
                auxFile.close();
                return true;
            }

            prevPos = currentPos;
            prevIsAux = currentIsAux;
            currentPos = tempSiguientePos;
            currentIsAux = tempIsAux;
        }

        mainFile.close();
        auxFile.close();
        return false;
    }

    ~SequentialFile() {}
};


template <typename RecordType>
const int SequentialFile<RecordType>::RegistroFull = sizeof(RecordType) + sizeof(int) + sizeof(bool);


// template <typename RecordType>
// vector<RecordType> readCSV(const string& filename);

// template <>
// vector<Record1> readCSV<Record1>(const string& filename) {
//     vector<Record1> records;
//     ifstream file(filename);
//     if (!file.is_open()) {
//         throw("No se pudo abrir el archivo CSV.");
//     }

//     string line;
//     getline(file, line); // Saltar el encabezado
//     while (getline(file, line)) {
//         if (line.empty()) continue;

//         stringstream ss(line);
//         string field;
//         vector<string> fields;

//         while (getline(ss, field, ',')) {
//             fields.push_back(field);
//         }

//         if (fields.size() < 20) {
//             cerr << "Línea con campos insuficientes: " << line << endl;
//             continue;
//         }

//         Record1 record;

//         int idx = 0;
//         strncpy(record.id, fields[idx++].c_str(), sizeof(record.id));
//         strncpy(record.name, fields[idx++].c_str(), sizeof(record.name));
//         strncpy(record.popularity, fields[idx++].c_str(), sizeof(record.popularity));
//         strncpy(record.duration_ms, fields[idx++].c_str(), sizeof(record.duration_ms));
//         strncpy(record.explicit_, fields[idx++].c_str(), sizeof(record.explicit_));
//         strncpy(record.artists, fields[idx++].c_str(), sizeof(record.artists));
//         strncpy(record.id_artists, fields[idx++].c_str(), sizeof(record.id_artists));
//         strncpy(record.release_date, fields[idx++].c_str(), sizeof(record.release_date));
//         strncpy(record.danceability, fields[idx++].c_str(), sizeof(record.danceability));
//         strncpy(record.energy, fields[idx++].c_str(), sizeof(record.energy));
//         strncpy(record.key_str, fields[idx++].c_str(), sizeof(record.key_str));
//         strncpy(record.loudness, fields[idx++].c_str(), sizeof(record.loudness));
//         strncpy(record.mode, fields[idx++].c_str(), sizeof(record.mode));
//         strncpy(record.speechiness, fields[idx++].c_str(), sizeof(record.speechiness));
//         strncpy(record.acousticness, fields[idx++].c_str(), sizeof(record.acousticness));
//         strncpy(record.instrumentalness, fields[idx++].c_str(), sizeof(record.instrumentalness));
//         strncpy(record.liveness, fields[idx++].c_str(), sizeof(record.liveness));
//         strncpy(record.valence, fields[idx++].c_str(), sizeof(record.valence));
//         strncpy(record.tempo, fields[idx++].c_str(), sizeof(record.tempo));
//         strncpy(record.time_signature, fields[idx++].c_str(), sizeof(record.time_signature));

//         // Asegurar la terminación nula
//         record.id[sizeof(record.id) - 1] = '\0';
//         record.name[sizeof(record.name) - 1] = '\0';
//         record.popularity[sizeof(record.popularity) - 1] = '\0';
//         record.duration_ms[sizeof(record.duration_ms) - 1] = '\0';
//         record.explicit_[sizeof(record.explicit_) - 1] = '\0';
//         record.artists[sizeof(record.artists) - 1] = '\0';
//         record.id_artists[sizeof(record.id_artists) - 1] = '\0';
//         record.release_date[sizeof(record.release_date) - 1] = '\0';
//         record.danceability[sizeof(record.danceability) - 1] = '\0';
//         record.energy[sizeof(record.energy) - 1] = '\0';
//         record.key_str[sizeof(record.key_str) - 1] = '\0';
//         record.loudness[sizeof(record.loudness) - 1] = '\0';
//         record.mode[sizeof(record.mode) - 1] = '\0';
//         record.speechiness[sizeof(record.speechiness) - 1] = '\0';
//         record.acousticness[sizeof(record.acousticness) - 1] = '\0';
//         record.instrumentalness[sizeof(record.instrumentalness) - 1] = '\0';
//         record.liveness[sizeof(record.liveness) - 1] = '\0';
//         record.valence[sizeof(record.valence) - 1] = '\0';
//         record.tempo[sizeof(record.tempo) - 1] = '\0';
//         record.time_signature[sizeof(record.time_signature) - 1] = '\0';

//         records.push_back(record);
//     }

//     file.close();
//     return records;
// }

// template <>
// vector<Record2> readCSV<Record2>(const string& filename) {
//     vector<Record2> records;
//     ifstream file(filename);
//     if (!file.is_open()) {
//         throw("No se pudo abrir el archivo CSV.");
//     }

//     string line;
//     getline(file, line); // Saltar el encabezado
//     while (getline(file, line)) {
//         if (line.empty()) continue;

//         stringstream ss(line);
//         string token;
//         Record2 record;

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

//         records.push_back(record);
//     }
//     file.close();
//     return records;
// }

