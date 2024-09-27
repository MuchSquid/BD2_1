#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stack>
#include <queue>
#include <cstring>

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

template <typename TK>
class AVLFile {
private:
    string filename;
    long pos_root;

public:
    AVLFile(string filename) {
        this->filename = filename;
        pos_root = -1;
    }

    bool find(const char* key) {
        return find(pos_root, key);
    }

    void insert(Record record) {
        pos_root = insert(pos_root, record);
    }

    vector<Record> inorder() {
        return inorder(pos_root);
    }

    int countRecords(){
        return countRecords(pos_root);
    }

    bool remove(const char* key) {
        bool eliminated = false;
        pos_root = remove(pos_root, key, eliminated);
        return eliminated;
    }

    Record findRecord(const char* key) {
        return findRecord(pos_root, key);
    }

    Record if_not_found_succesor(const char* key) {
        return if_not_found_succesor(pos_root, key);
    }

    Record if_not_found_predecessor(const char* key) {
        return if_not_found_predecessor(pos_root, key);
    }

    vector<Record> rangeSearch(const char* begin_key, const char* end_key) {
        if (!find(begin_key))
            begin_key = if_not_found_succesor(begin_key).id;
        if (!find(end_key))
            end_key = if_not_found_predecessor(end_key).id;
        return rangeSearch(pos_root, begin_key, end_key);
    }

    Record if_not_found_succesor(long pos_long, const char* key) {
        Record n;
        stack<Record> mstack;
        while (pos_long != -1) {
            Record temp = getRecord(pos_long);
            if (strcmp(temp.id, key) > 0) {
                mstack.push(temp);
                pos_long = temp.left;
            } else {
                pos_long = temp.right;
            }
        }
        if (!mstack.empty()) {
            n = mstack.top();
        }
        return n;
    }

    Record if_not_found_predecessor(long pos_long, const char* key) {
        Record n;
        queue<Record> mqueue;
        while (pos_long != -1) {
            Record temp = getRecord(pos_long);
            if (strcmp(temp.id, key) < 0) {
                mqueue.push(temp);
                pos_long = temp.right;
            } else {
                pos_long = temp.left;
            }
        }
        if (!mqueue.empty()) {
            n = mqueue.front();
        }
        return n;
    }

    bool find(long post_root, const char* key) {
        if (post_root == -1)
            return false;
        Record Found = getRecord(post_root);
        if (strcmp(Found.id, key) == 0)
            return true;
        if (strcmp(Found.id, key) < 0)
            return find(Found.right, key);
        return find(Found.left, key);
    }

    Record findRecord(long post_root, const char* key) {
        if (post_root == -1) {
            Record Empty;
            strcpy(Empty.id, "-1");
            return Empty;
        }
        Record Found = getRecord(post_root);
        if (strcmp(Found.id, key) == 0)
            return Found;
        if (strcmp(Found.id, key) < 0)
            return findRecord(Found.right, key);
        return findRecord(Found.left, key);
    }

    long end_of_file() {
        ifstream inFile(filename, ios::binary | ios::ate);
        return inFile.tellg() / sizeof(Record);
    }

    int height(long post_root) {
        if (post_root == -1)
            return -1;
        Record Found = getRecord(post_root);
        return Found.height;
    }

    int updateHeight(Record& record) {
        return 1 + max(height(record.left), height(record.right));
    }

    int balancingFactor(long post_root) {
        if (post_root == -1)
            return 0;
        Record Found = getRecord(post_root);
        return height(Found.left) - height(Found.right);
    }

    Record getRecord(long post) {
        Record Found;
        ifstream inFile(filename, ios::binary);
        inFile.seekg(post * sizeof(Record));
        inFile.read(reinterpret_cast<char*>(&Found), sizeof(Record));
        inFile.close();
        return Found;
    }

    void updateRecord(long pos, Record record) {
        ofstream outFile(filename, ios::binary | ios::in | ios::out);
        outFile.seekp(pos * sizeof(Record));
        outFile.write(reinterpret_cast<const char*>(&record), sizeof(Record));
        outFile.close();
    }

    long right_rota(long post_root) {
        Record record = getRecord(post_root);
        long child = record.left;
        Record record2 = getRecord(child);
        long t2 = record2.right;

        record2.right = post_root;
        record.left = t2;

        record.height = updateHeight(record);
        record2.height = updateHeight(record2);

        updateRecord(post_root, record);
        updateRecord(child, record2);

        return child;
    }

    long left_rota(long post_root) {
        Record record = getRecord(post_root);
        long child = record.right;
        Record record2 = getRecord(child);
        long t2 = record2.left;

        record2.left = post_root;
        record.right = t2;

        record.height = updateHeight(record);
        record2.height = updateHeight(record2);

        updateRecord(post_root, record);
        updateRecord(child, record2);

        return child;
    }

    int countRecords(long pos_root){
        if(pos_root == -1)
            return 0;

        Record record = getRecord(pos_root);
        int left = countRecords(record.left);
        int right = countRecords(record.right);

        return 1 + left + right;    
    }

    long insert(long post_root, Record record) {
        if (post_root == -1) {
            long end_pos = end_of_file();
            updateRecord(end_pos, record);
            return end_pos;
        }

        Record Found = getRecord(post_root);
        if (strcmp(record.id, Found.id) < 0)
            Found.left = insert(Found.left, record);
        else if (strcmp(record.id, Found.id) > 0)
            Found.right = insert(Found.right, record);
        else
            return post_root;

        Found.height = updateHeight(Found);
        updateRecord(post_root, Found);

        int balance = balancingFactor(post_root);

        if(balance > 1 && strcmp(record.id, getRecord(Found.left).id) < 0)
            return right_rota(post_root);

        if(balance < -1 && strcmp(record.id, getRecord(Found.right).id) > 0)
            return left_rota(post_root);

        if(balance > 1 && strcmp(record.id, getRecord(Found.left).id) > 0){
            Found.left = left_rota(Found.left);
            updateRecord(post_root, Found);
            return right_rota(post_root);
        }

        if(balance < -1 && strcmp(record.id, getRecord(Found.right).id) < 0){
            Found.right = right_rota(Found.right);
            updateRecord(post_root, Found);
            return left_rota(post_root);
        }

        return post_root;
    }

    vector<Record> inorder(long post_root) {
        vector<Record> result;
        if (post_root != -1) {
            vector<Record> leftResult = inorder(getRecord(post_root).left);
            result.insert(result.end(), leftResult.begin(), leftResult.end());
            result.push_back(getRecord(post_root));
            vector<Record> rightResult = inorder(getRecord(post_root).right);
            result.insert(result.end(), rightResult.begin(), rightResult.end());
        }
        return result;
    }

    long minimo(long post_root){
        Record Found = getRecord(post_root);
        while(Found.left != -1){
            post_root = Found.left;
            Found = getRecord(post_root);
        }
        return post_root;
    }

    long remove(long post_root, const char* key, bool eliminated) {
        if (post_root == -1){
            eliminated = false;
            return post_root;
        }

        Record Found = getRecord(post_root);
        if (strcmp(key, Found.id) < 0)
            Found.left = remove(Found.left, key, eliminated);
        else if (strcmp(key, Found.id) > 0)
            Found.right = remove(Found.right, key, eliminated);
        else {
            eliminated = true;
            if (Found.left == -1 || Found.right == -1) {
                long temp = (Found.left == -1) ? Found.right : Found.left;
                if(temp == -1){
                    temp = post_root;
                    post_root = -1;
                }
                else{
                    post_root = temp;
                }
            } else {
                long temp = minimo(post_root);
                Record temporal = getRecord(temp);
                strncpy(Found.id, temporal.id, sizeof(Found.id));
                strncpy(Found.name, temporal.name, sizeof(Found.name));
                strncpy(Found.popularity, temporal.popularity, sizeof(Found.popularity));
                strncpy(Found.duration_ms, temporal.duration_ms, sizeof(Found.duration_ms));
                strncpy(Found.explicit_, temporal.explicit_, sizeof(Found.explicit_));
                strncpy(Found.artists, temporal.artists, sizeof(Found.artists));
                strncpy(Found.id_artists, temporal.id_artists, sizeof(Found.id_artists));
                strncpy(Found.release_date, temporal.release_date, sizeof(Found.release_date));
                strncpy(Found.danceability, temporal.danceability, sizeof(Found.danceability));
                strncpy(Found.energy, temporal.energy, sizeof(Found.energy));
                strncpy(Found.key_str, temporal.key_str, sizeof(Found.key_str));
                strncpy(Found.loudness, temporal.loudness, sizeof(Found.loudness));
                strncpy(Found.mode, temporal.mode, sizeof(Found.mode));
                strncpy(Found.speechiness, temporal.speechiness, sizeof(Found.speechiness));
                strncpy(Found.acousticness, temporal.acousticness, sizeof(Found.acousticness));
                strncpy(Found.instrumentalness, temporal.instrumentalness, sizeof(Found.instrumentalness));
                strncpy(Found.liveness, temporal.liveness, sizeof(Found.liveness));
                strncpy(Found.valence, temporal.valence, sizeof(Found.valence));
                strncpy(Found.tempo, temporal.tempo, sizeof(Found.tempo));
                strncpy(Found.time_signature, temporal.time_signature, sizeof(Found.time_signature));
                Found.right = remove(Found.right, temporal.id, eliminated);
            }
        }

        if (post_root == -1)
            return post_root;

        Found.height = updateHeight(Found);
        updateRecord(post_root, Found);

        int balance = balancingFactor(post_root);

        if (balance > 1 && balancingFactor(Found.left) >= 0)
            return right_rota(post_root);

        if (balance < -1 && balancingFactor(Found.right) <= 0)
            return left_rota(post_root);

        if (balance > 1 && balancingFactor(Found.left) < 0) {
            Found.left = left_rota(Found.left);
            updateRecord(post_root, Found);
            return right_rota(post_root);
        }

        if (balance < -1 && balancingFactor(Found.right) > 0) {
            Found.right = right_rota(Found.right);
            updateRecord(post_root, Found);
            return left_rota(post_root);
        }

        return post_root;
    }

    vector<Record> rangeSearch(long post_root, const char* begin_key, const char* end_key) {
        vector<Record> records;
        if (post_root == -1)
            return records;

        Record Found = getRecord(post_root);
        if (strcmp(begin_key, Found.id) < 0) {
            vector<Record> leftRecords = rangeSearch(Found.left, begin_key, end_key);
            records.insert(records.end(), leftRecords.begin(), leftRecords.end());
        }

        if (strcmp(begin_key, Found.id) <= 0 && strcmp(end_key, Found.id) >= 0)
            records.push_back(Found);

        if (strcmp(end_key, Found.id) > 0) {
            vector<Record> rightRecords = rangeSearch(Found.right, begin_key, end_key);
            records.insert(records.end(), rightRecords.begin(), rightRecords.end());
        }

        return records;
    }
};


vector<Record> leerCSV(const string& filename) {
    ifstream file(filename);
    vector<Record> records;
    string line;

    if (!file.is_open()) {
        cerr << "No se pudo abrir el archivo.\n";
        return records;
    }

    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        Record record;

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



/*
void readFile(string filename) {
    AVLFile<int> avlFile(filename);
    vector<Record> records = leerCSV("datos.csv");
    for (auto& record : records) {
        avlFile.insert(record);
    }
    cout << "------------------------------------------\n";
    bool passed = true;
    for (auto& record : records) {
        Record foundRecord = avlFile.findRecord(record.codigo);
        
        if (!(foundRecord == record)) {
            passed = false;
            cout << "Error en el record con codigo: " << record.codigo << "\n";
            cout << "Se esperaba: \n";
            record.showData();
            cout << "\nSe obtuvo: \n";
            foundRecord.showData();
            cout << "\n";
            break;
        }
    }
    if (passed) {
        cout << "Todos los records fueron leidos correctamente\n";
    } else {
        cout << "Hubo errores en la lectura de algunos records\n";
    }
  }

*/
int main() {

    AVLFile<Record>avlFile("proyecto.dat");


    //Leer el archivo y contar los registros que tiene
    try {
        vector<Record> records = leerCSV("dataMergeLimpioFinal.csv");
        for (const auto& record : records) {
            avlFile.insert(record);
        }
        int c = avlFile.countRecords();    
        cout << "El archivo tiene " << c << " registros" << endl;
    }catch (const char* msg){
        cerr << msg << endl;
    }


    Record prueba;
    strncpy(prueba.id, "2Be2FRskn33cqPyeSdTqFd", sizeof(prueba.id));
    prueba.id[sizeof(prueba.id) - 1] = '\0';

    strncpy(prueba.name, "No Me Olvides - Remasterizado", sizeof(prueba.name));
    prueba.name[sizeof(prueba.name) - 1] = '\0';

    strncpy(prueba.popularity, "0", sizeof(prueba.popularity));
    prueba.popularity[sizeof(prueba.popularity) - 1] = '\0';

    strncpy(prueba.duration_ms, "155093", sizeof(prueba.duration_ms));
    prueba.duration_ms[sizeof(prueba.duration_ms) - 1] = '\0';

    strncpy(prueba.explicit_, "0", sizeof(prueba.explicit_));
    prueba.explicit_[sizeof(prueba.explicit_) - 1] = '\0';

    strncpy(prueba.artists, "['Ignacio Corsini']", sizeof(prueba.artists));
    prueba.artists[sizeof(prueba.artists) - 1] = '\0';

    strncpy(prueba.id_artists, "['5LiOoJbxVSAMkBS2fUm3X2']", sizeof(prueba.id_artists));
    prueba.id_artists[sizeof(prueba.id_artists) - 1] = '\0';

    strncpy(prueba.release_date, "1922-03-21", sizeof(prueba.release_date));
    prueba.release_date[sizeof(prueba.release_date) - 1] = '\0';

    strncpy(prueba.danceability, "0.484", sizeof(prueba.danceability));
    prueba.danceability[sizeof(prueba.danceability) - 1] = '\0';

    strncpy(prueba.energy, "0.0558", sizeof(prueba.energy));
    prueba.energy[sizeof(prueba.energy) - 1] = '\0';

    strncpy(prueba.key_str, "8", sizeof(prueba.key_str));
    prueba.key_str[sizeof(prueba.key_str) - 1] = '\0';

    strncpy(prueba.loudness, "-25.33", sizeof(prueba.loudness));
    prueba.loudness[sizeof(prueba.loudness) - 1] = '\0';

    strncpy(prueba.mode, "0", sizeof(prueba.mode));
    prueba.mode[sizeof(prueba.mode) - 1] = '\0';

    strncpy(prueba.speechiness, "0.0575", sizeof(prueba.speechiness));
    prueba.speechiness[sizeof(prueba.speechiness) - 1] = '\0';

    strncpy(prueba.acousticness, "0.996", sizeof(prueba.acousticness));
    prueba.acousticness[sizeof(prueba.acousticness) - 1] = '\0';

    strncpy(prueba.instrumentalness, "0.278", sizeof(prueba.instrumentalness));
    prueba.instrumentalness[sizeof(prueba.instrumentalness) - 1] = '\0';

    strncpy(prueba.liveness, "0.202", sizeof(prueba.liveness));
    prueba.liveness[sizeof(prueba.liveness) - 1] = '\0';

    strncpy(prueba.valence, "0.636", sizeof(prueba.valence));
    prueba.valence[sizeof(prueba.valence) - 1] = '\0';

    strncpy(prueba.tempo, "99.555", sizeof(prueba.tempo));
    prueba.tempo[sizeof(prueba.tempo) - 1] = '\0';

    strncpy(prueba.time_signature, "4", sizeof(prueba.time_signature));
    prueba.time_signature[sizeof(prueba.time_signature) - 1] = '\0';

    //agregar Registro    
    try {
        avlFile.insert(prueba);
        cout << "Registro " << prueba.id << " agregado exitosamente" << '\n';
    }catch (const char* msg){
        cerr << msg << endl;
    }

    //Ver la cantidad de registros despues de las inserciones
    try {
        int c = avlFile.countRecords();    
        cout << "El archivo tiene " << c << " registros" << endl;;
    }catch (const char* msg){
        cerr << msg << endl;
    }

    //buscar Registro
    try {
        bool founded = avlFile.find("2Be2FRskn33cqPyeSdTqFd");
        if(founded){
            cout << "Registro encontrado: ";
            Record founded = avlFile.findRecord("2Be2FRskn33cqPyeSdTqFd");
            founded.print();
        }
    }catch (const char* msg){
        cerr << msg << endl;
    }

    //Eliminar un registro
    try {
        bool eliminated = avlFile.remove("2Be2FRskn33cqPyeSdTqFd");
        if(eliminated){
            cout << "Registro eliminado" << endl;
            bool attempt = avlFile.find("2Be2FRskn33cqPyeSdTqFd");
            if(attempt)
                cout << "Registro no encontrado";
        }
    }catch (const char* msg){
        cerr << msg << endl;
    }

    //Ver la cantidad de registros despues de la eliminacion
    try {
        int c = avlFile.countRecords();    
        cout << "El archivo tiene " << c << " registros" << endl;;
    }catch (const char* msg){
        cerr << msg << endl;
    }

    //Búsqueda por rango
    try
    {
        vector<Record> founded = avlFile.rangeSearch("0Dd9ImXtAtGwsmsAD69KZT","0kCB1bDVBC8gWCFcnJyIZc");
        cout << "Registros encontrados exitosamente" << endl;
        for (const auto& record : founded) {
            record.print();
        }
    }
    catch(const char* msg)
    {
        cerr << msg << endl;
    }
    return 0; 
}
