#ifndef AVL2_H
#define AVL2_H

#include "selector.h"

template <typename R>
class AVLFile2: public SeleccionarMetodo<R> {
private:
    string filename;
    long pos_root;

public:
    AVLFile2(string filename) {
        this->filename = filename;
        pos_root = -1;
    }

    bool find(const char* key) {
        return find(pos_root, key);
    }

    void insert(Record2 record) {
        pos_root = insert(pos_root, record);
    }

    vector<Record2> inorder() {
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

    Record2 findRecord(const char* key) {
        return findRecord(pos_root, key);
    }

    Record2 if_not_found_succesor(const char* key) {
        return if_not_found_succesor(pos_root, key);
    }

    Record2 if_not_found_predecessor(const char* key) {
        return if_not_found_predecessor(pos_root, key);
    }

    vector<Record2> rangeSearch(const char* begin_key, const char* end_key) {
        if (!find(begin_key))
            begin_key = if_not_found_succesor(begin_key).id;
        if (!find(end_key))
            end_key = if_not_found_predecessor(end_key).id;
        return rangeSearch(pos_root, begin_key, end_key);
    }

    Record2 if_not_found_succesor(long pos_long, const char* key) {
        Record2 n;
        stack<Record2> mstack;
        while (pos_long != -1) {
            Record2 temp = getRecord(pos_long);
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

    Record2 if_not_found_predecessor(long pos_long, const char* key) {
        Record2 n;
        queue<Record2> mqueue;
        while (pos_long != -1) {
            Record2 temp = getRecord(pos_long);
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
        Record2 Found = getRecord(post_root);
        if (strcmp(Found.id, key) == 0)
            return true;
        if (strcmp(Found.id, key) < 0)
            return find(Found.right, key);
        return find(Found.left, key);
    }

    Record2 findRecord(long post_root, const char* key) {
        if (post_root == -1) {
            Record2 Empty;
            strcpy(Empty.id, "-1");
            return Empty;
        }
        Record2 Found = getRecord(post_root);
        if (strcmp(Found.id, key) == 0)
            return Found;
        if (strcmp(Found.id, key) < 0)
            return findRecord(Found.right, key);
        return findRecord(Found.left, key);
    }

    long end_of_file() {
        ifstream inFile(filename, ios::binary | ios::ate);
        return inFile.tellg() / sizeof(Record2);
    }

    int height(long post_root) {
        if (post_root == -1)
            return -1;
        Record2 Found = getRecord(post_root);
        return Found.height;
    }

    int updateHeight(Record2& record) {
        return 1 + max(height(record.left), height(record.right));
    }

    int balancingFactor(long post_root) {
        if (post_root == -1)
            return 0;
        Record2 Found = getRecord(post_root);
        return height(Found.left) - height(Found.right);
    }

    Record2 getRecord(long post) {
        Record2 Found;
        ifstream inFile(filename, ios::binary);
        inFile.seekg(post * sizeof(Record2));
        inFile.read(reinterpret_cast<char*>(&Found), sizeof(Record2));
        inFile.close();
        return Found;
    }

    void updateRecord(long pos, Record2 record) {
        ofstream outFile(filename, ios::binary | ios::in | ios::out);
        outFile.seekp(pos * sizeof(Record2));
        outFile.write(reinterpret_cast<const char*>(&record), sizeof(Record2));
        outFile.close();
    }

    long right_rota(long post_root) {
        Record2 record = getRecord(post_root);
        long child = record.left;
        Record2 record2 = getRecord(child);
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
        Record2 record = getRecord(post_root);
        long child = record.right;
        Record2 record2 = getRecord(child);
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

        Record2 record = getRecord(pos_root);
        int left = countRecords(record.left);
        int right = countRecords(record.right);

        return 1 + left + right;
    }

    long insert(long post_root, Record2 record) {
        if (post_root == -1) {
            long end_pos = end_of_file();
            updateRecord(end_pos, record);
            return end_pos;
        }

        Record2 Found = getRecord(post_root);
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

    vector<Record2> inorder(long post_root) {
        vector<Record2> result;
        if (post_root != -1) {
            vector<Record2> leftResult = inorder(getRecord(post_root).left);
            result.insert(result.end(), leftResult.begin(), leftResult.end());
            result.push_back(getRecord(post_root));
            vector<Record2> rightResult = inorder(getRecord(post_root).right);
            result.insert(result.end(), rightResult.begin(), rightResult.end());
        }
        return result;
    }

    long minimo(long post_root){
        Record2 Found = getRecord(post_root);
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

        Record2 Found = getRecord(post_root);
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
                Record2 temporal = getRecord(temp);
                strncpy(Found.id, temporal.id, sizeof(Found.id));
                strncpy(Found.date, temporal.date, sizeof(Found.date));
                strncpy(Found.customer, temporal.customer, sizeof(Found.customer));
                strncpy(Found.model, temporal.model, sizeof(Found.model));
                strncpy(Found.price, temporal.price, sizeof(Found.price));
                strncpy(Found.quantity, temporal.quantity, sizeof(Found.quantity));
                strncpy(Found.storelocation, temporal.storelocation, sizeof(Found.storelocation));
                strncpy(Found.salespersonid, temporal.salespersonid, sizeof(Found.salespersonid));
                strncpy(Found.paymentmethod, temporal.paymentmethod, sizeof(Found.paymentmethod));
                strncpy(Found.customerage, temporal.customerage, sizeof(Found.customerage));
                strncpy(Found.customergender, temporal.customergender, sizeof(Found.customergender));
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

    vector<Record2> rangeSearch(long post_root, const char* begin_key, const char* end_key) {
        vector<Record2> records;
        if (post_root == -1)
            return records;

        Record2 Found = getRecord(post_root);
        if (strcmp(begin_key, Found.id) < 0) {
            vector<Record2> leftRecords = rangeSearch(Found.left, begin_key, end_key);
            records.insert(records.end(), leftRecords.begin(), leftRecords.end());
        }

        if (strcmp(begin_key, Found.id) <= 0 && strcmp(end_key, Found.id) >= 0)
            records.push_back(Found);

        if (strcmp(end_key, Found.id) > 0) {
            vector<Record2> rightRecords = rangeSearch(Found.right, begin_key, end_key);
            records.insert(records.end(), rightRecords.begin(), rightRecords.end());
        }

        return records;
    }
};

#endif // AVL2_H
