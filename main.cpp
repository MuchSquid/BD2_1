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
const size_t M = 50; // Tamaño de la página para índices
const size_t N = 100; // Tamaño de la página de datos
struct PageData {
    Record records[N];
    size_t count;
    long long nextPage = -1;

    PageData() : count(0) {}

    char* serialize() {
        return reinterpret_cast<char*>(this);
    }
    char *deserialize() { return reinterpret_cast<char *>(this); }
};

struct PageIndex {
    long long pages[M];
    char keys[M][22]; // Usando el tamaño del id como clave
    size_t count;

PageIndex() : count(0) {
        memset(pages, 0, sizeof(pages));
        memset(keys, 0, sizeof(keys));
    }
    char* serialize() {
        return reinterpret_cast<char*>(this);
    }
    char *deserialize() { return reinterpret_cast<char *>(this); }
};

class ISAM {
public:
    ISAM(const string& csv_path) : data_path("data.bin") {
        // Rutas de índice
        index_paths.push_back("index1.bin");
        index_paths.push_back("index2.bin");
        index_paths.push_back("index3.bin");

        if (!filesystem::exists(csv_path)) {
            throw runtime_error("El archivo CSV no existe.");
        }

        read_csv(csv_path);
        build(true);
    }

private:
    string data_path;
    vector<string> index_paths;
    vector<streamoff> free_page_addresses;

public:
    streamoff get_free_page_address() {
        if (free_page_addresses.empty()) {
            return -1;
        }
        streamoff address = free_page_addresses.back();
        free_page_addresses.pop_back();
        return address;
    }

    void read_csv(const string& csv_path) {
        ifstream csv_file(csv_path);
        ofstream data_file(data_path, ios::binary);
        string line;
        getline(csv_file, line); // Ignorar la primera línea (encabezados)

        while (getline(csv_file, line)) {
            Record record;
            vector<string> fields;
            size_t pos = 0;
            string token;
            while ((pos = line.find(',')) != string::npos) {
                token = line.substr(0, pos);
                fields.push_back(token);
                line.erase(0, pos + 1);
            }
            fields.push_back(line); // Agrega el último campo

            if (fields.size() == 20) { // Asegurarse de que hay 20 campos
                strncpy(record.id, fields[0].c_str(), sizeof(record.id) - 1);
                strncpy(record.name, fields[1].c_str(), sizeof(record.name) - 1);
                strncpy(record.popularity, fields[2].c_str(), sizeof(record.popularity) - 1);
                strncpy(record.duration_ms, fields[3].c_str(), sizeof(record.duration_ms) - 1);
                strncpy(record.explicit_, fields[4].c_str(), sizeof(record.explicit_) - 1);
                strncpy(record.artists, fields[5].c_str(), sizeof(record.artists) - 1);
                strncpy(record.id_artists, fields[6].c_str(), sizeof(record.id_artists) - 1);
                strncpy(record.release_date, fields[7].c_str(), sizeof(record.release_date) - 1);
                strncpy(record.danceability, fields[8].c_str(), sizeof(record.danceability) - 1);
                strncpy(record.energy, fields[9].c_str(), sizeof(record.energy) - 1);
                strncpy(record.key_str, fields[10].c_str(), sizeof(record.key_str) - 1);
                strncpy(record.loudness, fields[11].c_str(), sizeof(record.loudness) - 1);
                strncpy(record.mode, fields[12].c_str(), sizeof(record.mode) - 1);
                strncpy(record.speechiness, fields[13].c_str(), sizeof(record.speechiness) - 1);
                strncpy(record.acousticness, fields[14].c_str(), sizeof(record.acousticness) - 1);
                strncpy(record.instrumentalness, fields[15].c_str(), sizeof(record.instrumentalness) - 1);
                strncpy(record.liveness, fields[16].c_str(), sizeof(record.liveness) - 1);
                strncpy(record.valence, fields[17].c_str(), sizeof(record.valence) - 1);
                strncpy(record.tempo, fields[18].c_str(), sizeof(record.tempo) - 1);
                strncpy(record.time_signature, fields[19].c_str(), sizeof(record.time_signature) - 1);

                data_file.write(reinterpret_cast<char*>(&record), sizeof(Record));
            }
        }
        csv_file.close();
        data_file.close();
    }

    void build(bool drop_existing) {
        if (drop_existing) {
            for (const auto& index_path : index_paths) {
                filesystem::remove(index_path);
            }
        }
        build_bottom_level();
        build_upper_levels();
    }


void build_bottom_level() {
        fstream data_file(data_path, ios::in | ios::binary);
        fstream index_file(index_paths[2], ios::out | ios::binary);

        PageData data_page;
        Record record;
        long long page_address = 0;
        size_t total_records = 0;

        while (data_file.read(reinterpret_cast<char*>(&record), sizeof(record))) {
            if (data_page.count == N) {
                index_file.write(reinterpret_cast<char*>(&data_page), sizeof(data_page));
                page_address = index_file.tellp();
                data_page.count = 0;
            }
            data_page.records[data_page.count++] = record;
            total_records++;
        }
        if (data_page.count > 0) {
            index_file.write(reinterpret_cast<char*>(&data_page), sizeof(data_page));
        }

        cout << "Total de registros en el nivel inferior: " << total_records << endl;
        data_file.close();
        index_file.close();
    }

   
vector<Record> search(const char* search_id) {
    cout << "Buscando ID: " << search_id << endl;
    
    fstream root_index_file(index_paths[0], ios::in | ios::binary);
    PageIndex search_page;
    long long found_page_address = 0;
    bool found = false;

    while (root_index_file.read(reinterpret_cast<char*>(&search_page), sizeof(search_page))) {
        for (size_t i = 0; i < search_page.count; i++) {
            cout << "Comparando con key: " << search_page.keys[i] << endl;
            if (strcmp(search_id, search_page.keys[i]) <= 0) {
                found_page_address = search_page.pages[i];
                found = true;
                break;
            }
        }
        if (found) break;
    }

    if (!found) {
        found_page_address = search_page.pages[search_page.count - 1];
    }

    cout << "Página encontrada en el primer nivel: " << found_page_address << endl;

    fstream second_index_file(index_paths[1], ios::in | ios::binary);
    PageIndex second_index_page;
    second_index_file.seekg(found_page_address);
    long long found_second_page_address = 0;
    found = false;

    while (second_index_file.read(reinterpret_cast<char*>(&second_index_page), sizeof(second_index_page))) {
        for (size_t i = 0; i < second_index_page.count; i++) {
            cout << "Comparando con key del segundo nivel: " << second_index_page.keys[i] << endl;
            if (strcmp(search_id, second_index_page.keys[i]) <= 0) {
                found_second_page_address = second_index_page.pages[i];
                found = true;
                break;
            }
        }
        if (found) break;
    }

    if (!found) {
        found_second_page_address = second_index_page.pages[second_index_page.count - 1];
    }

    cout << "Página encontrada en el segundo nivel: " << found_second_page_address << endl;

    fstream data_index_file(index_paths[2], ios::in | ios::binary);
    PageData data_page;
    data_index_file.seekg(found_second_page_address);

    vector<Record> result;
    
    while (data_index_file.read(reinterpret_cast<char*>(&data_page), sizeof(data_page))) {
        for (size_t i = 0; i < data_page.count; i++) {
            cout << "Comparando con ID: " << data_page.records[i].id << endl;
            if (strcmp(search_id, data_page.records[i].id) == 0) {
                result.push_back(data_page.records[i]);
                cout << "Registro encontrado!" << endl;
            }
        }
        if (data_page.nextPage == -1) break;
        data_index_file.seekg(data_page.nextPage);
    }

    root_index_file.close();
    second_index_file.close();
    data_index_file.close();
    
    cout << "Número de resultados encontrados: " << result.size() << endl;
    return result;
}

    void build_upper_levels() {
        fstream data_index_file(index_paths[2], ios::in | ios::binary);
        fstream second_index_file(index_paths[1], ios::out | ios::binary);
        PageData data_index_page;
        PageIndex second_index_page;

        size_t data_index_page_address = 0;
        while (data_index_file.read(data_index_page.serialize(), sizeof(data_index_page))) {
            if (second_index_page.count == M) {
                second_index_file.write(second_index_page.serialize(), sizeof(second_index_page));
                second_index_page.count = 0;
            }
            second_index_page.pages[second_index_page.count] = data_index_page_address;
            strncpy(second_index_page.keys[second_index_page.count], data_index_page.records[data_index_page.count - 1].id, sizeof(second_index_page.keys[0]) - 1);
            second_index_page.count++;
            data_index_page_address = data_index_file.tellg();
        }

        if (second_index_page.count > 0) {
            second_index_file.write(second_index_page.serialize(), sizeof(second_index_page));
        }

        data_index_file.close();
        second_index_file.close();

        second_index_file.open(index_paths[1], ios::in | ios::binary);
        fstream first_index_file(index_paths[0], ios::out | ios::binary);
        PageIndex first_index_page;
        size_t second_index_page_address = 0;
        while (second_index_file.read(second_index_page.serialize(), sizeof(second_index_page))) {
            if (first_index_page.count == M) {
                first_index_file.write(first_index_page.serialize(), sizeof(first_index_page));
                first_index_page.count = 0;
            }
            first_index_page.pages[first_index_page.count] = second_index_page_address;
            strncpy(first_index_page.keys[first_index_page.count], second_index_page.keys[second_index_page.count - 1], sizeof(first_index_page.keys[0]) - 1);
            first_index_page.count++;
            second_index_page_address = second_index_file.tellg();
        }

        if (first_index_page.count > 0) {
            first_index_file.write(first_index_page.serialize(), sizeof(first_index_page));
        }

        second_index_file.close();
        first_index_file.close();
    }

    bool add(Record new_record) {
        fstream root_index_file(index_paths[0], ios::in | ios::binary);
        PageIndex search_page;
        size_t found_page_address = 0;
        bool found = false;
        while (!found && root_index_file.read(search_page.deserialize(), sizeof(search_page))) {
            for (int i = 0; i < search_page.count; i++) {
                if (strcmp(new_record.id, search_page.keys[i]) < 0) {
                    found_page_address = search_page.pages[i];
                    found = true;
                    break;
                }
            }
        }
        if (!found) {
            found_page_address = search_page.pages[search_page.count - 1];
        }

        fstream second_index_file(index_paths[1], ios::in | ios::binary);
        PageIndex second_index_page;
        second_index_file.seekg((streamoff) found_page_address);
        size_t found_second_page_address = 0;
        found = false;
        while (!found && second_index_file.read(second_index_page.deserialize(), sizeof(second_index_page))) {
            for (int i = 0; i < second_index_page.count; i++) {
                if (strcmp(new_record.id, second_index_page.keys[i]) < 0) {
                    found_second_page_address = second_index_page.pages[i];
                    found = true;
                    break;
                }
            }
        }
        if (!found) {
            found_second_page_address = second_index_page.pages[second_index_page.count - 1];
        }

        fstream data_index_file(index_paths[2], ios::in | ios::out | ios::binary);
        PageData data_page;
        data_index_file.seekg((streamoff) found_second_page_address);
        data_index_file.read(data_page.deserialize(), sizeof(data_page));
        size_t last_page_address = found_second_page_address;
        while (data_page.nextPage != -1) {
            data_index_file.seekg(data_page.nextPage);
            last_page_address = data_index_file.tellg();
            data_index_file.read(data_page.deserialize(), sizeof(data_page));
        }
        if (data_page.count == N) {
            PageData new_data_page;
            streamoff new_data_page_address = get_free_page_address();
            new_data_page.records[0] = new_record;
            new_data_page.count++;
            if (new_data_page_address == -1) {
                data_index_file.seekp(0, ios::end);
            } else {
                data_index_file.seekp(new_data_page_address);
            }
            data_page.nextPage = data_index_file.tellp();
            data_index_file.write(new_data_page.deserialize(), sizeof(new_data_page));
            data_index_file.seekp((streamoff) last_page_address);
            data_index_file.write(data_page.deserialize(), sizeof(data_page));
        } else {
            data_page.records[data_page.count] = new_record;
            data_page.count++;
            data_index_file.seekp((streamoff) last_page_address);
            data_index_file.write(data_page.deserialize(), sizeof(data_page));
        }

        root_index_file.close();
        second_index_file.close();
        data_index_file.close();
        return true;
    }

  
    vector<Record> range_search(const char* start_id, const char* end_id) {
        fstream root_index_file(index_paths[0], ios::in | ios::binary);
        PageIndex search_page;
        size_t found_page_address = 0;
        bool found = false;
        while (!found && root_index_file.read(search_page.deserialize(), sizeof(search_page))) {
            for (size_t i = 0; i < search_page.count; i++) {
                if (strcmp(start_id, search_page.keys[i]) < 0) {
                    found_page_address = search_page.pages[i];
                    found = true;
                    break;
                }
            }
        }
        if (!found) {
            found_page_address = search_page.pages[search_page.count - 1];
        }

        fstream second_index_file(index_paths[1], ios::in | ios::binary);
        PageIndex second_index_page;
        second_index_file.seekg((streamoff) found_page_address);
        size_t found_second_page_address = 0;
        found = false;
        while (!found && second_index_file.read(second_index_page.deserialize(), sizeof(second_index_page))) {
            for (int i = 0; i < second_index_page.count; i++) {
                if (strcmp(start_id, second_index_page.keys[i]) < 0) {
                    found_second_page_address = second_index_page.pages[i];
                    found = true;
                    break;
                }
            }
        }
        if (!found) {
            found_second_page_address = second_index_page.pages[second_index_page.count - 1];
        }

        fstream data_index_file(index_paths[2], ios::in | ios::binary);
        PageData data_page;
        data_index_file.seekg((streamoff) found_second_page_address);
        bool inside_range = true;
        vector<Record> records;
        while (inside_range && data_index_file.read(data_page.deserialize(), sizeof(data_page))) {
            for (int i = 0; i < data_page.count; i++) {
                if (strcmp(data_page.records[i].id, start_id) >= 0 && strcmp(data_page.records[i].id, end_id) <= 0) {
                    records.push_back(data_page.records[i]);
                } else if (strcmp(data_page.records[i].id, end_id) > 0) {
                    inside_range = false;
                    break;
                }
            }
            if (data_page.nextPage == -1) break;
            data_index_file.seekg(data_page.nextPage);
        }

        root_index_file.close();
        second_index_file.close();
        data_index_file.close();

        sort(records.begin(), records.end(), [](const Record &a, const Record &b) {
            return strcmp(a.id, b.id) < 0;
        });

        return records;
    }

    bool remove(const char * search_id) {
        fstream root_index_file(index_paths[0], ios::in | ios::binary);
        PageIndex search_page;
        size_t found_page_address = 0;
        bool found = false;
        while (!found && root_index_file.read(search_page.deserialize(), sizeof(search_page))) {
            for (int i = 0; i < search_page.count; i++) {
                if (strcmp(search_id, search_page.keys[i]) < 0) {
                    found_page_address = search_page.pages[i];
                    found = true;
                    break;
                }
            }
        }
        if (!found) {
            found_page_address = search_page.pages[search_page.count - 1];
        }

        fstream second_index_file(index_paths[1], ios::in | ios::binary);
        PageIndex second_index_page;
        second_index_file.seekg((streamoff) found_page_address);
        size_t found_second_page_address = 0;
        found = false;
        while (!found && second_index_file.read(second_index_page.deserialize(), sizeof(second_index_page))) {
            for (int i = 0; i < second_index_page.count; i++) {
                if (strcmp(search_id, second_index_page.keys[i]) < 0) {
                    found_second_page_address = second_index_page.pages[i];
                    found = true;
                    break;
                }
            }
        }
        if (!found) {
            found_second_page_address = second_index_page.pages[second_index_page.count - 1];
        }

        fstream data_index_file(index_paths[2], ios::in | ios::out | ios::binary);
        PageData data_page;
        data_index_file.seekg((streamoff) found_second_page_address);
        data_index_file.read(data_page.deserialize(), sizeof(data_page));
        size_t last_page_address = found_second_page_address;

        bool read_this_page = true;

        while (read_this_page) {
            for (int i = 0; i < data_page.count; i++) {
                if (strcmp(data_page.records[i].id, search_id) == 0) {
                    data_page.records[i] = data_page.records[data_page.count - 1];
                    data_page.count--;
                    data_index_file.seekp((streamoff) last_page_address);
                    data_index_file.write(data_page.deserialize(), sizeof(data_page));
                    if (data_page.count == 0 && data_page.nextPage != -1) {
                        free_page_addresses.push_back((streamoff) last_page_address);
                    }
                    root_index_file.close();
                    second_index_file.close();
                    data_index_file.close();
                    return true;
                }
            }
            if (data_page.nextPage != -1) {
                data_index_file.seekg(data_page.nextPage);
                last_page_address = data_index_file.tellg();
                data_index_file.read(data_page.deserialize(), sizeof(data_page));
            } else {
                read_this_page = false;
            }
        }
        root_index_file.close();
        second_index_file.close();
        data_index_file.close();
        return false;
    }
    void print_index_files() {
    for (size_t i = 0; i < index_paths.size(); ++i) {
        cout << "Contenido del archivo de índice " << i << ":" << endl;
        fstream index_file(index_paths[i], ios::in | ios::binary);
        if (i < 2) {  // Para los niveles de índice
            PageIndex page;
            while (index_file.read(reinterpret_cast<char*>(&page), sizeof(PageIndex))) {
                for (size_t j = 0; j < page.count; ++j) {
                    cout << "Key: " << page.keys[j] << ", Page: " << page.pages[j] << endl;
                }
                cout << "---" << endl;
            }
        } else {  // Para el nivel de datos
            PageData page;
            while (index_file.read(reinterpret_cast<char*>(&page), sizeof(PageData))) {
                for (size_t j = 0; j < page.count; ++j) {
                    cout << "ID: " << page.records[j].id << ", Name: " << page.records[j].name << endl;
                }
                
                cout << "Next Page: " << page.nextPage << endl;
                cout << "---" << endl;
                if(page.nextPage!=(-1)){
                    cout<<page.nextPage<<endl;
                    break;
                }
            }
        }
        index_file.close();
    }
}
};


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
