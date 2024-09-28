#include "ISAM.h"

template <typename RecordTipo>
void pruebaISAM(string csv_path){
        bool tipo=true;
        ISAM<RecordTipo> isam(csv_path,tipo);
        cout << "ISAM construido exitosamente." << endl;
        const char* busca = "1h4TGS0ytq2EujrSjsbopD";
        vector<RecordTipo> resultado = isam.search(busca);
        
        cout << "Resultados de la búsqueda:" << endl;
        for (const auto& record : resultado) {
            cout << "ID: " << record.id << ", Nombre: " << record.name << endl;
        }

        // Ejemplo de búsqueda por rango
        const char* start_id = "5HlbF07r4waO8aEZn5pP6l";
        const char* end_id =   "5L3asAbitjZc0uZsAyqv42";
        vector<RecordTipo> range_result = isam.range_search(start_id, end_id);
        
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


//TRUE RECORD 1 //FALSE RECORD 2
template <typename RecordTipo>
void pruebaISAM2(string csv_path){
        bool tipo=false;
        ISAM<RecordTipo> isam(csv_path,tipo);
        // isam.print_index_files();
        cout << "ISAM construido exitosamente." << endl;
        const char* busca = "000010";
        vector<RecordTipo> resultado = isam.search(busca);
        
        cout << "Resultados de la búsqueda:" << endl;
        for (const auto& record : resultado) {
            cout << "ID: " << record.id << ", Nombre: " << record.customer << endl;
        }

        // Ejemplo de búsqueda por rango
        const char* start_id = "000019";
        const char* end_id =   "000030";
        vector<RecordTipo> range_result = isam.range_search(start_id, end_id);
        
        cout << "\nResultados de la búsqueda por rango:" << endl;
        for (const auto& record : range_result) {
            cout << "ID: " << record.id << ", Nombre: " << record.customer << endl;
        }

        // Ejemplo de eliminación
        const char* remove_id = "000010";
        bool removed = isam.remove(remove_id);
        if (removed) {
            cout << "\nRegistro con ID " << remove_id << " eliminado exitosamente." << endl;
        } else {
            cout << "\nNo se encontró el registro con ID " << remove_id << " para eliminar." << endl;
        }
    
}


void prubita(){
   
    try {
        string csv_path = "bike.csv";
        bool tipo = false;
        ISAM<Record2> isam(csv_path, tipo);
        cout << "ISAM construido exitosamente." << endl;

        Record2 new_record;
        strcpy(new_record.id, "100001");
        strcpy(new_record.date, "2022-01-01");
        strcpy(new_record.customer, "Juan");
        strcpy(new_record.model, "Modelo 1");
        strcpy(new_record.price, "100.00");
        strcpy(new_record.quantity, "1");
        strcpy(new_record.storelocation, "Tienda 1");
        strcpy(new_record.salespersonid, "001");
        strcpy(new_record.paymentmethod, "Efectivo");
        strcpy(new_record.customerage, "25");
        strcpy(new_record.customergender, "Masculino");

        bool added = isam.add(new_record);
        if (added) {
            cout << "Registro agregado exitosamente." << endl;
        } else {
            cout << "No se pudo agregar el registro." << endl;
        }

        const char* busca = "100001";
        vector<Record2> resultado = isam.search(busca);
        
        cout << "Resultados de la búsqueda:" << endl;
        for (const auto& record : resultado) {
            cout << "ID: " << record.id << ", Nombre: " << record.customer << endl;
        }


    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

}

int main(){
    pruebaISAM<Record1>("sincomas.csv");
    pruebaISAM2<Record2>("bike.csv");
    prubita();
}