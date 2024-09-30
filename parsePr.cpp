#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <vector>
#include "parser.hh"
// #include "ISAM.h"
using namespace std;

int main(int argc, char* argv[]) {
    string sentencia;
    while (true) {
        cout << "Inserte su sentencia SQL: " << endl;
        // cin.ignore(); 
      getline(cin, sentencia);

        // cout << "Sentencia ingresada: " << sentencia << endl;

        Scanner scanner(sentencia + " ");
        Parser parser(&scanner);
        
        try {
            parser.parse();
        } catch (const exception& e) {
            cout << "Error durante el parseo: " << e.what() << endl;
        }
        
        if (!error_message.empty()) {
            cout << "Mensaje de error: " << error_message << endl;
        } else {
          cout << endl << endl;
        }
    }
    return 0;
}
