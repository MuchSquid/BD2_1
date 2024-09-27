#include "Record.h"

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

        // cout << "Total de registros en el nivel inferior: " << total_records << endl;
        data_file.close();
        index_file.close();
    }

   
vector<Record> search(const char* search_id) {
    // cout << "Buscando ID: " << search_id << endl;
    
    fstream root_index_file(index_paths[0], ios::in | ios::binary);
    PageIndex search_page;
    long long found_page_address = 0;
    bool found = false;

    while (root_index_file.read(reinterpret_cast<char*>(&search_page), sizeof(search_page))) {
        for (size_t i = 0; i < search_page.count; i++) {
            // cout << "Comparando con key: " << search_page.keys[i] << endl;
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

    // cout << "Página encontrada en el primer nivel: " << found_page_address << endl;

    fstream second_index_file(index_paths[1], ios::in | ios::binary);
    PageIndex second_index_page;
    second_index_file.seekg(found_page_address);
    long long found_second_page_address = 0;
    found = false;

    while (second_index_file.read(reinterpret_cast<char*>(&second_index_page), sizeof(second_index_page))) {
        for (size_t i = 0; i < second_index_page.count; i++) {
            // cout << "Comparando con key del segundo nivel: " << second_index_page.keys[i] << endl;
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

    // cout << "Página encontrada en el segundo nivel: " << found_second_page_address << endl;

    fstream data_index_file(index_paths[2], ios::in | ios::binary);
    PageData data_page;
    data_index_file.seekg(found_second_page_address);

    vector<Record> result;
    
    while (data_index_file.read(reinterpret_cast<char*>(&data_page), sizeof(data_page))) {
        for (size_t i = 0; i < data_page.count; i++) {
            // cout << "Comparando con ID: " << data_page.records[i].id << endl;
            if (strcmp(search_id, data_page.records[i].id) == 0) {
                result.push_back(data_page.records[i]);
                // cout << "Registro encontrado!" << endl;
            }
        }
        if (data_page.nextPage == -1) break;
        data_index_file.seekg(data_page.nextPage);
    }

    root_index_file.close();
    second_index_file.close();
    data_index_file.close();
    
    // cout << "Número de resultados encontrados: " << result.size() << endl;
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


void pruebaISAM(string csv_path){
        ISAM isam(csv_path);
        cout << "ISAM construido exitosamente." << endl;
        const char* busca = "1h4TGS0ytq2EujrSjsbopD";
        vector<Record> resultado = isam.search(busca);
        
        cout << "Resultados de la búsqueda:" << endl;
        for (const auto& record : resultado) {
            cout << "ID: " << record.id << ", Nombre: " << record.name << endl;
        }

        // Ejemplo de búsqueda por rango
        const char* start_id = "5HlbF07r4waO8aEZn5pP6l";
        const char* end_id =   "5L3asAbitjZc0uZsAyqv42";
        vector<Record> range_result = isam.range_search(start_id, end_id);
        
        cout << "\nResultados de la búsqueda por rango:" << endl;
        for (const auto& record : range_result) {
            cout << "ID: " << record.id << ", Nombre: " << record.name << endl;
        }

        // Ejemplo de eliminación
        const char* remove_id = "1h4TGS0ytq2EujrSjsbopD";
        bool removed = isam.remove(remove_id);
        if (removed) {
            cout << "\nRegistro con ID " << remove_id << " eliminado exitosamente." << endl;
        } else {
            cout << "\nNo se encontró el registro con ID " << remove_id << " para eliminar." << endl;
        }
    
}