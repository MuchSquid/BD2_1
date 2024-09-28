#ifndef AVL_H
#define AVL_H

#include "selector.h"

template <typename R>
class AVLFile: public SeleccionarMetodo<R> {
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

    void insert(Record1 record) {
        pos_root = insert(pos_root, record);
    }

    vector<Record1> inorder() {
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

    Record1 findRecord(const char* key) {
        return findRecord(pos_root, key);
    }

    Record1 if_not_found_succesor(const char* key) {
        return if_not_found_succesor(pos_root, key);
    }

    Record1 if_not_found_predecessor(const char* key) {
        return if_not_found_predecessor(pos_root, key);
    }

    vector<Record1> rangeSearch(const char* begin_key, const char* end_key) {
        if (!find(begin_key))
            begin_key = if_not_found_succesor(begin_key).id;
        if (!find(end_key))
            end_key = if_not_found_predecessor(end_key).id;
        return rangeSearch(pos_root, begin_key, end_key);
    }

    Record1 if_not_found_succesor(long pos_long, const char* key) {
        Record1 n;
        stack<Record1> mstack;
        while (pos_long != -1) {
            Record1 temp = getRecord(pos_long);
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

    Record1 if_not_found_predecessor(long pos_long, const char* key) {
        Record1 n;
        queue<Record1> mqueue;
        while (pos_long != -1) {
            Record1 temp = getRecord(pos_long);
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
        Record1 Found = getRecord(post_root);
        if (strcmp(Found.id, key) == 0)
            return true;
        if (strcmp(Found.id, key) < 0)
            return find(Found.right, key);
        return find(Found.left, key);
    }

    Record1 findRecord(long post_root, const char* key) {
        if (post_root == -1) {
            Record1 Empty;
            strcpy(Empty.id, "-1");
            return Empty;
        }
        Record1 Found = getRecord(post_root);
        if (strcmp(Found.id, key) == 0)
            return Found;
        if (strcmp(Found.id, key) < 0)
            return findRecord(Found.right, key);
        return findRecord(Found.left, key);
    }

    long end_of_file() {
        ifstream inFile(filename, ios::binary | ios::ate);
        return inFile.tellg() / sizeof(Record1);
    }

    int height(long post_root) {
        if (post_root == -1)
            return -1;
        Record1 Found = getRecord(post_root);
        return Found.height;
    }

    int updateHeight(Record1& record) {
        return 1 + max(height(record.left), height(record.right));
    }

    int balancingFactor(long post_root) {
        if (post_root == -1)
            return 0;
        Record1 Found = getRecord(post_root);
        return height(Found.left) - height(Found.right);
    }

    Record1 getRecord(long post) {
        Record1 Found;
        ifstream inFile(filename, ios::binary);
        inFile.seekg(post * sizeof(Record1));
        inFile.read(reinterpret_cast<char*>(&Found), sizeof(Record1));
        inFile.close();
        return Found;
    }

    void updateRecord(long pos, Record1 record) {
        ofstream outFile(filename, ios::binary | ios::in | ios::out);
        outFile.seekp(pos * sizeof(Record1));
        outFile.write(reinterpret_cast<const char*>(&record), sizeof(Record1));
        outFile.close();
    }

    long right_rota(long post_root) {
        Record1 record = getRecord(post_root);
        long child = record.left;
        Record1 record2 = getRecord(child);
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
        Record1 record = getRecord(post_root);
        long child = record.right;
        Record1 record2 = getRecord(child);
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

        Record1 record = getRecord(pos_root);
        int left = countRecords(record.left);
        int right = countRecords(record.right);

        return 1 + left + right;
    }

    long insert(long post_root, Record1 record) {
        if (post_root == -1) {
            long end_pos = end_of_file();
            updateRecord(end_pos, record);
            return end_pos;
        }

        Record1 Found = getRecord(post_root);
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

    vector<Record1> inorder(long post_root) {
        vector<Record1> result;
        if (post_root != -1) {
            vector<Record1> leftResult = inorder(getRecord(post_root).left);
            result.insert(result.end(), leftResult.begin(), leftResult.end());
            result.push_back(getRecord(post_root));
            vector<Record1> rightResult = inorder(getRecord(post_root).right);
            result.insert(result.end(), rightResult.begin(), rightResult.end());
        }
        return result;
    }

    long minimo(long post_root){
        Record1 Found = getRecord(post_root);
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

        Record1 Found = getRecord(post_root);
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
                Record1 temporal = getRecord(temp);
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

    vector<Record1> rangeSearch(long post_root, const char* begin_key, const char* end_key) {
        vector<Record1> records;
        if (post_root == -1)
            return records;

        Record1 Found = getRecord(post_root);
        if (strcmp(begin_key, Found.id) < 0) {
            vector<Record1> leftRecords = rangeSearch(Found.left, begin_key, end_key);
            records.insert(records.end(), leftRecords.begin(), leftRecords.end());
        }

        if (strcmp(begin_key, Found.id) <= 0 && strcmp(end_key, Found.id) >= 0)
            records.push_back(Found);

        if (strcmp(end_key, Found.id) > 0) {
            vector<Record1> rightRecords = rangeSearch(Found.right, begin_key, end_key);
            records.insert(records.end(), rightRecords.begin(), rightRecords.end());
        }

        return records;
    }
};


vector<Record1> leerCSV(const string& filename) {
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
};


#endif // AVL_H
