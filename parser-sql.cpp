#include "parser-sql.hh"

const char* Token::token_names[31] = {
    "SELECT", "CREATE", "TABLE", "FROM", "FILE", "WHERE", "AND", 
    "INSERT", "DELETE", "BETWEEN", "INTO", "END", "ERR", "NUMBER", 
    "LPAREN", "RPAREN", "COMMA", "SEMICOLON", "ALL", "FILENAME", 
    "EQUAL", "QUOTE", "DOBLEQUOTE","FLOAT","ID", "VALUES" , "INDEX", "AVL", 
    "SEQUENTIAL", "ISAM","USING"
    };

Token::Token(Type type):type(type) { lexema = ""; }

Token::Token(Type type, const string source):type(type) {
  lexema = source;
}

std::ostream& operator << ( std::ostream& outs, const Token & tok )
{
  if (tok.lexema.empty())
    return outs << Token::token_names[tok.type];
  else
    return outs << Token::token_names[tok.type] << "(" << tok.lexema << ")";
}

std::ostream& operator << ( std::ostream& outs, const Token* tok ) {
  return outs << *tok;
}

Scanner::Scanner(const string& s) : input(s), first(0), current(0) {
    reserved["SELECT"] = Token::SELECT;
    reserved["CREATE"] = Token::CREATE;
    reserved["TABLE"] = Token::TABLE;
    reserved["FROM"] = Token::FROM;
    reserved["FILE"] = Token::FILE;
    reserved["WHERE"] = Token::WHERE;
    reserved["AND"] = Token::AND;
    reserved["INSERT"] = Token::INSERT;
    reserved["DELETE"] = Token::DELETE;
    reserved["BETWEEN"] = Token::BETWEEN;
    reserved["INTO"] = Token::INTO;
    reserved["END"] = Token::END;
    reserved["FILENAME"] = Token::FILENAME;
    reserved["AVL"] = Token::AVL;
    reserved["SEQUENTIAL"] = Token::SEQUENTIAL;
    reserved["ISAM"] = Token::ISAM;
    reserved["INDEX"] = Token::INDEX;
    reserved["VALUES"] = Token::VALUES;
    reserved["USING"] = Token::USING;
}

Token::Type Scanner::checkReserved(const string& lexema) {
  std::unordered_map<std::string,Token::Type>::const_iterator it = reserved.find (lexema);
  if (it == reserved.end())
    return Token::ERR;
 else
   return it->second;
}

Scanner::~Scanner() {
  
}

char Scanner::nextChar() {
  if (current < input.size())
    return input[current++];
  else
    return '\0';
}

void Scanner::rollBack() {
  current --;
}

void Scanner::startLexema() {
  first = current;
}

string Scanner::getLexema() {
  return input.substr(first, current - first);
}

bool Scanner::isNumber(const string& s) {
  for (int i = 0; i < s.size(); i++) 
    if (!isdigit(s[i]))
      return false;
  return true;
}

Token* Scanner::nextToken(){
    Token* token;
    char c;
    c = nextChar();
    while(c == ' ' || c == '\t' ) c = nextChar();
    if(c == '\n'){
        c = nextChar();
        while(c == '\n') c = nextChar();
    }
    if(c == '\0') return new Token(Token::END);

    startLexema();
    if(isdigit(c)||isalpha(c)){
        c = nextChar();
        while(isalpha(c) || isdigit(c)) c = nextChar();
        // if(c == ':'){
        // c = nextChar();
    //     while(strchr("/.-_", c)) c = nextChar();  // Manejo del filename
    //     rollBack();
    //     token = new Token(Token::FILENAME, getLexema());
    //     } else 
        if(isdigit(c)){
            c = nextChar();
            while(isdigit(c)) c = nextChar();
            rollBack();
            token = new Token(Token::NUMBER, getLexema());
        } else if(isNumber(getLexema().substr(0,getLexema().length()-1)) && c == '.'){
            c = nextChar();
            while (isdigit(c)){
            c = nextChar();
            }
            if(isalpha(c))
            return new Token(Token::ERR, getLexema());
            rollBack();
            string a = getLexema();
            return new Token(Token::FLOAT, getLexema());
        }

        else if(isalpha(c)){
            c = nextChar();
            while(isalpha(c)) c = nextChar();
            rollBack();
            token = new Token(checkReserved(getLexema()), getLexema());
        // para lectura de csv con funcion leer archivo csv
        } else if(c == '"'|| c == '\''){
            char quote = c;
            c = nextChar();
            while(c != quote && c != '\0') {  
                c = nextChar();
            }
            if (c == quote) {
                token = new Token(Token::FILENAME, getLexema());
            } else {
                token = new Token(Token::ERR, getLexema());
            }
        }
        return new Token(Token::ID, getLexema());
    } else if(strchr("(),;*\'\"=", c)){
        switch(c){
            case '(': token = new Token(Token::LPAREN); break;
            case ')': token = new Token(Token::RPAREN); break;
            case ',': token = new Token(Token::COMMA); break;
            case ';': token = new Token(Token::SEMICOLON); break;
            case '*': token = new Token(Token::ALL); break;
            case '=': token = new Token(Token::EQUAL); break;
            case '\'': token = new Token(Token::QUOTE); break;
            case '\"': token = new Token(Token::DOBLEQUOTE); break;
            default: cout<<"No deberia llegar hasta aca"<<endl;
        }
    } else {
        token = new Token(Token::ERR, getLexema());
    }
    return token;
}


// ----------------Parser----------------

void Parser::mostrar(){
    for(auto const &d: datos){
        cout<<d<<", ";
    }
    cout << endl;
}

bool Parser::match(Token::Type ttype){
    if(check(ttype)){
        advance();
        return true;
    }
    return false;
}

bool Parser::existeTabla(const string& tableName) {
    for (const auto& table : tablas) {
        if (table.first == tableName) {
            return true;
        }
    }
    return false;
}

bool Parser::findTable(string tableName){
    cout<<tablas[0].first<<endl;
    cout<<tablas[1].first<<endl;
    if(tableName == tablas[0].first){
      tabla = tablas[0];
      return true;
    }
    else if(tableName == tablas[1].first){
      tabla = tablas[1];
      return true;
    }
    return false;
}

bool Parser::check(Token::Type ttype){
    if(isAtEnd()) return false;
    return current->type == ttype;
}

bool Parser::advance() {
  if (!isAtEnd()) {
    Token* temp = current;
    if (previous) delete previous;
    current = scanner->nextToken();
    previous = temp;
    if (check(Token::ERR)) {
      cout << "Parse error, unrecognised character: " << current->lexema << endl;
      exit(0);
    }
    return true;
  }
  return false;
} 

bool Parser::isAtEnd() {
  return (current->type == Token::END);
}

void Parser::parse(){
    current = scanner->nextToken();
    if(check(Token::ERR)){
        cout<<"Caracter invalido"<<endl;
        return;
    }
    parseStatement();
    return;
}

void Parser::parseStatement(){
    if(match(Token::SELECT)){
        if(match(Token::ID)){
            parseExpression();
        } else if(match(Token::ALL)){
            parseCreateTable(nullptr, "");
        } else {
            cout<<"Error de sintaxis"<<endl;
        }
    } else if(match(Token::CREATE)){
        if(match(Token::TABLE)){
            Token* temp = previous;
            if(match(Token::ID)){
                parseCreateTable(temp, previous->lexema);
                return;
            }
            cout << "Error en la crwacion de la tbla" << endl;
        }
        cout << "Error" << endl;
        return;
    } else if(match(Token::INSERT)){
        if(match(Token::INTO)){
            if(match(Token::ID)){
                string tableName = previous->lexema;
                if(match(Token::VALUES)){
                    if(match(Token::LPAREN)){
                        do {
                            if (match(Token::QUOTE)) {
                                if (match(Token::ID)) {
                                    string dato = previous->lexema;
                                    if (match(Token::QUOTE)) {
                                        values.push_back(make_pair(dato, "char"));
                                    } else {
                                        cout << "Error de sintaxis, se esperaba un \"." << endl;
                                        return;
                                    }
                                } else {
                                    cout << "Error de sintaxis, se esperaba un dato." << endl;
                                    return;
                                }
                            } else if (match(Token::NUMBER)) {
                                values.push_back(make_pair(previous->lexema, "int"));
                            } else if (match(Token::FLOAT)) {
                                values.push_back(make_pair(previous->lexema, "float"));
                            } else {
                                cout << "Sintaxis incorrecta." << endl;
                                return;
                            }
                        } while (match(Token::COMMA)); 
                        if(match(Token::RPAREN)){
                            if(match(Token::SEMICOLON)){
                                if(existeTabla(tableName)){
                                    parseInsert();
                                } else {
                                    cout << "Error en la tabla, no existe" << endl;
                                }
                            }
                            cout << "Error de sintaxis, se esperaba un ;" << endl;
                        }
                        cout << "Error de sintaxis, se esperaba un )" << endl;
                    }
                    cout << "Error de sintaxis, se esperaba un (" << endl;
                } 
                else if(!existeTabla(tableName)){
                    cout << "Error en la tabla, no existe" << endl;
            }
        }
    } else if(match(Token::DELETE)){
        if(match(Token::FROM)){
            parseDelete();
        } 
    } else {
        cout<<"Error de sintaxis"<<endl;
    }
}
}

void Parser::parseInsert() {
    try {
        if(tabla.second == "registros1") {
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

            char *campos[] = {id, name, popularity, duration_ms, explicit_, artists, id_artists, release_date,
                              danceability, energy, key_str, loudness, mode, speechiness, acousticness,
                              instrumentalness, liveness, valence, tempo, time_signature};
            int longitudes[] = {23, 529, 3, 7, 1, 934, 1508, 10, 6, 8, 2, 7, 1, 6, 8, 8, 7, 8, 7, 1};

            for (int i = 0; i < 20; i++) {
                strncpy(campos[i], values[i].first.c_str(), longitudes[i] - 1);
                campos[i][longitudes[i] - 1] = '\0';
            }
            Record1 record(id, name, popularity, duration_ms, explicit_, artists, id_artists, release_date,
                           danceability, energy, key_str, loudness, mode, speechiness, acousticness, instrumentalness,
                           liveness, valence, tempo, time_signature);

            bool ins = metodo->add(record);
            if (!ins) {
                cout << "Ya existe este elemento" << endl;
            } else {
                cout << "Insertado" << endl;
            }
        } else {
            char id[7];
            char date[10];
            char customer[4];
            char model[13];
            char price[7];
            char quantity[1];
            char storelocation[12];
            char salespersonid[3];
            char paymentmethod[11];
            char customerage[2];
            char customergender[6];

            char *campos[] = {id, date, customer, model, price, quantity, storelocation, salespersonid, paymentmethod, customerage, customergender};
            int longitudes[] = {7, 10, 4, 13, 7, 1, 12, 3, 11, 2, 6};
            for (int i = 0; i < 11; i++) {
                strncpy(campos[i], values[i].first.c_str(), longitudes[i] - 1);
                campos[i][longitudes[i] - 1] = '\0';
            }

            Record2 record(id, date, customer, model, price, quantity, storelocation, salespersonid, paymentmethod, customerage, customergender);
            bool ins = metodo2->add(record);
            if (!ins) {
                cout << "Ya existe este elemento" << endl;
            } else {
                cout << "Insertado" << endl;
            }
        }

    } catch (const std::exception& e) {
        cout << "\tValores incorrectos para la tabla: " << e.what() << endl;
    }
}

void Parser::parseCreateTable(Token* temp, string tableName){
    if(match(Token::FROM)){
        if(match(Token::ID)){
            tableName = previous->lexema;
            if(match(Token::SEMICOLON)){
                if(!findTable(tableName)){
                    cout << endl << "No existe la tabla "<< tableName;
                    return;
                }
                cout<<tabla.second<<endl;
                if(tabla.second == "registros1")
                    registros1 = metodo->load();
                else
                    registros2 = metodo2->load();
                datos.clear();
                cout << "---error----";
                return;
            }
                
        }
        else if(match(Token::FILE)){
            parseFile(tableName);
        }
        else {
            cout<<"Error de sintaxis"<<endl;
        }
    }
}

bool Parser::parseCondition(string tableName) {
    if (match(Token::WHERE)) {
        if (!match(Token::ID)) {
            cout << "Se esperaba un ID" << endl;
            return false;
        }
        if (match(Token::EQUAL)) {
            if (!parseEqual()) {
                cout << "Sintaxis incorrecta en la condición de igualdad" << endl;
                return false;
            }

            if (!match(Token::SEMICOLON)) {
                cout << "Sintaxis incorrecta: se esperaba un punto y coma" << endl;
                return false;
            }
            if (!search_key(tableName)) {
                cout << "No existe un elemento con la llave buscada" << endl;
                return false;
            }
            datos.clear();
            return true;
        } 
        else if (match(Token::BETWEEN)) {
            cout << "Between" << endl;
            return parse_range(tableName); 
        } 
        else {
            cout << "Sintaxis incorrecta: se esperaba un operador de comparación" << endl;
            return false;
        }
    }
    return false; 
}

void Parser::parseFile(string nombre_tabla) {
    string filename = "";
    char quote;
    if (match(Token::QUOTE) || match(Token::DOBLEQUOTE)) {
        quote = previous->lexema[0]; 
        if (match(Token::FILENAME)) {
            filename = previous->lexema;
            if (match(quote == '\'' ? Token::QUOTE : Token::DOBLEQUOTE)) {
                parseIndex(filename, nombre_tabla);
                return;
            }
            cout << "\tSe esperaba \" o \' para cerrar el nombre del archivo" << endl;
            return;
        }
        cout << "\tSe esperaba el nombre del archivo" << endl;
        return;
    }
    cout << "\tSe esperaba una comilla" << endl;
    return;
}

void Parser::parseIndex(string fileName, string tableName){
    if(match(Token::USING)){
        if(match(Token::INDEX)){
            parseTipoMetodo(fileName, tableName);
            return;
        }
    }
    cout<<"Sintaxis incorrecta"<<endl;
    return;
}

// por corregir, se insercionan los metodos
void Parser::parseTipoMetodo(string fileName, string tableName){
    size_t pos = fileName.find_last_of('/');
    string archivo = (pos == string::npos) ? fileName : fileName.substr(pos + 1);
    cout << "archivo: " << archivo << endl;

    if(match(Token::AVL)){
        if(match(Token::SEMICOLON)){
            if(archivo == "data.csv"){
                remove("avl_data.dat");
                remove("avl_index.dat");
                // metodo = new AVLFile<Record1, char*, 20>();
                if (tableName == tablas[1].first) {
                    tablas[1].first = "";
                }
                tablas[0] = make_pair(tableName, "registros1");
                tabla = tablas[0];
                insertData(fileName);
            } 
            else if(archivo == "data2.csv"){
                remove("avl_data.dat");
                remove("avl_index.dat");
                // metodo2 = new AVLFile<Record2, char*, 20>();
                if (tableName == tablas[0].first) {
                    tablas[0].first = "";
                }
                tablas[1] = make_pair(tableName, "registros2");
                tabla = tablas[1];
                insertData2(fileName);
            }
            cout << "Se uso AVL para crear la tabla" << endl;
        } cout << "Sintaxis incorrecta" << endl;
    } else if(match(Token::SEQUENTIAL)){
        if(match(Token::SEMICOLON)){
            if(archivo == "data.csv"){
                remove("sequential_data.dat");
                remove("sequential_aux.dat");
                auto comparator = [](const Record1& a, const Record1& b) {
                  return string(a.id) < string(b.id);
                };

                auto printer = [](const Record1& record) {
                  cout << "----- Registro -----\n";
                  cout << "Id: " << record.id << "\n";
                  cout << "Name: " << record.name << "\n";
                  cout << "---------------------\n\n";
                };
                metodo = new SequentialFile<Record1>(fileName, comparator, printer);
                if (tableName == tablas[1].first) {
                    tablas[1].first = "";
                }
                tablas[0] = make_pair(tableName, "registros1");
                tabla = tablas[0];
                insertData(fileName);
            } 
            else if(archivo == "data2.csv"){
                remove("sequential_data.dat");
                remove("sequential_aux.dat");
                // metodo2 = new SequentialFile<Record2, char*>();
                if (tableName == tablas[0].first) {
                    tablas[0].first = "";
                }
                tablas[1] = make_pair(tableName, "registros2");
                tabla = tablas[1];
                insertData2(fileName);
            }
            cout << "Se uso Sequential para crear la tabla" << endl;
        }
        cout << "Sintaxis incorrecta" << endl;
    } else if(match(Token::ISAM)){
        if(match(Token::SEMICOLON)){
            if(archivo == "data.csv"){
                remove("isam_data.dat");
                remove("isam_index.dat");
                // delete metodo;
                metodo = new ISAM<Record1>(fileName, true);
                if (tableName == tablas[1].first) {
                    tablas[1].first = "";
                }
                tablas[0] = make_pair(tableName, "registros1");
                tabla = tablas[0];
                insertData(fileName);
            } 
            else if(archivo == "data2.csv"){
                remove("isam_data.dat");
                remove("isam_index.dat");
                metodo2 = new ISAM<Record2>(fileName, true);
                if (tableName == tablas[0].first) {
                    tablas[0].first = "";
                }
                tablas[1] = make_pair(tableName, "registros2");
                tabla = tablas[1];
                insertData2(fileName);
            }
            cout << "Se uso ISAM para crear la tabla" << endl;
            return;
        }
        cout << "Sintaxis incorrecta" << endl;
        return;
    } 
    cout << "Sintaxis incorrecta" << endl;
    return;
}


void Parser::parseExpression(){
    datos.push_back(previous->lexema);
    if(match(Token::COMMA)){  
        advance();            
        parseExpression();    
    }
    else if (match(Token::ID)) { 
        datos.push_back(previous->lexema);
        parseExpression(); 
    }
    else {
        parseCreateTable(nullptr, ""); 
    }
}

void Parser::parseDelete(){
    if(match(Token::FROM)){
        if(match(Token::ID)){
            string tableName = previous->lexema;
            if(findTable(tableName)){
                parseDelete2(tableName);
            } else {
                cout<<"No existe la tabla"<<endl;
            }
        } else {
            cout<<"Se esperaba un nombre de tabla"<<endl;
        }
    } else {
        cout<<"Se esperaba FROM"<<endl;
    }
}

bool Parser::parseDelete2(string tableName) {
    if (match(Token::WHERE)) {
        if (match(Token::ID)) {
            if(match(Token::EQUAL)){
                if(!parseEqual()){
                    return false;
                } else {
                    if(match(Token::SEMICOLON)){
                        bool s = search_key(tableName);
                        if(!s){
                            cout << "No existe un elemento con la llave buscada" << endl;;
                            return false;
                        }
                        eliminarValor(tableName);
                        return true;
                    }
                }
            }
            else if(match(Token::BETWEEN)){
                cout<<"Between"<<endl;
                parse_range(tableName);
            }
            else {
                cout << "\tSintaxis incorrecta"<< endl; 
            }
        }
    }
    return false; 
}

bool Parser::parseEqual() {
    if (match(Token::NUMBER)) {
        value = make_pair(previous->lexema, "int");
        return true;
    } 
    else if (match(Token::QUOTE)) {
        if (match(Token::ID)) {
            string dato = previous->lexema;
            if (!match(Token::QUOTE)) {
                cout << "Error de sintaxis, esperaba un \"" << endl;
                return false;
            }
            value = make_pair(dato, "char");
            return true;
        }
        cout << "Sintaxis incorrecta: se esperaba un ID después de la comilla" << endl;
        return false;
    }
    cout << "Sintaxis incorrecta: se esperaba un número o una cadena" << endl;
    return false;
}

void Parser::eliminarValor(string tableName){
    try{
        bool eliminado;
        if(tabla.second == "registros1"){
            char id[23];
            strncpy(id, value.first.c_str(), 22);
            id[22] = '\0';
            eliminado = metodo->remove(id);
        } else if(tabla.second == "registros2"){
            char id[7];
            strncpy(id, value.first.c_str(), 6);
            id[6] = '\0';
            eliminado = metodo2->remove(id);
        }
    } catch(const std::exception &e){
        cout << "Error" << endl;
    }
}

bool Parser::search_key(string tableName) {
    try{
        if(tabla.second == "registros1"){
            registros1.clear();
            char id[23];
            strncpy(id, value.first.c_str(), 22);
            id[22] = '\0';
            pair<Record1, bool> result = metodo->search(id);
            if(!result.second){
                return false;
            } else {
                registros1.push_back(result.first);
                return true;
            }
        } else {
            registros2.clear();
            char id[7];
            strncpy(id, value.first.c_str(), 6);
            id[6] = '\0';
            pair<Record2, bool> result = metodo2->search(id);
            if(!result.second){
                return false;
            } else {
                registros2.push_back(result.first);
                return true;
            }
        }
    } catch(const std::exception &e){
        cout << "\tError al buscar." << endl;
    }
}

bool Parser::parse_range(string nombre_tabla) {
    if(match(Token::NUMBER)){
        string beginStr = previous->lexema;
        char* begin = new char[beginStr.length() + 1];
        strcpy(begin, beginStr.c_str());
        if(match(Token::AND)){
            if(match(Token::NUMBER)){
                string endStr = previous->lexema;
                char* end = new char[endStr.length() + 1];
                strcpy(end, endStr.c_str());
                if(match(Token::SEMICOLON)){
                    registros1.clear();
                    cout << "Busqueda por rango" << endl;
                    registros1 = metodo->rangeSearch(begin, end);
                    delete[] begin;
                    delete[] end;
                    return true;
                }
                cout << "\tSintaxis incorrecta" << endl;
                delete[] begin;
                delete[] end;
                return false;
            }
            cout << "\tSintaxis incorrecta" << endl;
            delete[] begin;
            return false;
        }
        cout << "\tSintaxis incorrecta" << endl;
        delete[] begin;
        return false;
    }
}

void Parser::insertData(string path){
    cout << path << endl;
    ifstream archivo(path);
    cout << "path: " << path << endl;
    if (!archivo.is_open()) {cout << "\npath incorrecto.";return;}

    string linea;
    vector<string> campos;
    int counter = 0;
    while (getline(archivo, linea)) {
        istringstream lineaStream(linea);
        string campo;

        while (getline(lineaStream, campo, ',')) {
            campos.push_back(campo);
        }

        char id[20];
        char name[50];
        char popularity[10];
        char duration_ms[10];
        char explicit_[5];
        char artists[100];
        char id_artists[50];
        char release_date[12];
        char danceability[10];
        char energy[10];
        char key_str[5];
        char loudness[10];
        char mode[5];
        char speechiness[10];
        char acousticness[10];
        char instrumentalness[10];
        char liveness[10];
        char valence[10];
        char tempo[10];
        char time_signature[5];

        // Asignamos los valores a cada campo
        strncpy(id, campos[0].c_str(), sizeof(id) - 1);
        id[sizeof(id) - 1] = '\0';

        strncpy(name, campos[1].c_str(), sizeof(name) - 1);
        name[sizeof(name) - 1] = '\0';

        strncpy(popularity, campos[2].c_str(), sizeof(popularity) - 1);
        popularity[sizeof(popularity) - 1] = '\0';

        strncpy(duration_ms, campos[3].c_str(), sizeof(duration_ms) - 1);
        duration_ms[sizeof(duration_ms) - 1] = '\0';

        strncpy(explicit_, campos[4].c_str(), sizeof(explicit_) - 1);
        explicit_[sizeof(explicit_) - 1] = '\0';

        strncpy(artists, campos[5].c_str(), sizeof(artists) - 1);
        artists[sizeof(artists) - 1] = '\0';

        strncpy(id_artists, campos[6].c_str(), sizeof(id_artists) - 1);
        id_artists[sizeof(id_artists) - 1] = '\0';

        strncpy(release_date, campos[7].c_str(), sizeof(release_date) - 1);
        release_date[sizeof(release_date) - 1] = '\0';

        strncpy(danceability, campos[8].c_str(), sizeof(danceability) - 1);
        danceability[sizeof(danceability) - 1] = '\0';

        strncpy(energy, campos[9].c_str(), sizeof(energy) - 1);
        energy[sizeof(energy) - 1] = '\0';

        strncpy(key_str, campos[10].c_str(), sizeof(key_str) - 1);
        key_str[sizeof(key_str) - 1] = '\0';

        strncpy(loudness, campos[11].c_str(), sizeof(loudness) - 1);
        loudness[sizeof(loudness) - 1] = '\0';

        strncpy(mode, campos[12].c_str(), sizeof(mode) - 1);
        mode[sizeof(mode) - 1] = '\0';

        strncpy(speechiness, campos[13].c_str(), sizeof(speechiness) - 1);
        speechiness[sizeof(speechiness) - 1] = '\0';

        strncpy(acousticness, campos[14].c_str(), sizeof(acousticness) - 1);
        acousticness[sizeof(acousticness) - 1] = '\0';

        strncpy(instrumentalness, campos[15].c_str(), sizeof(instrumentalness) - 1);
        instrumentalness[sizeof(instrumentalness) - 1] = '\0';

        strncpy(liveness, campos[16].c_str(), sizeof(liveness) - 1);
        liveness[sizeof(liveness) - 1] = '\0';

        strncpy(valence, campos[17].c_str(), sizeof(valence) - 1);
        valence[sizeof(valence) - 1] = '\0';

        strncpy(tempo, campos[18].c_str(), sizeof(tempo) - 1);
        tempo[sizeof(tempo) - 1] = '\0';

        strncpy(time_signature, campos[19].c_str(), sizeof(time_signature) - 1);
        time_signature[sizeof(time_signature) - 1] = '\0';

        // Creamos el objeto Record1 con los valores extraídos del CSV
        Record1 record1(id, name, popularity, duration_ms, explicit_, artists, id_artists, release_date, danceability, 
                        energy, key_str, loudness, mode, speechiness, acousticness, instrumentalness, 
                        liveness, valence, tempo, time_signature);

        // Agregamos el record a la base de datos o método correspondiente
        bool result = metodo->add(record1);

        if (counter == 1000) break;
        counter++;
        campos.clear();
    }
}

void Parser::insertData2(string path) {
    cout << path << endl;
    ifstream archivo(path);
    cout << "path: " << path << endl;
    if (!archivo.is_open()) {cout <<"\npath incorrecto.";return;}

    string linea;
    vector<string> campos;
    int counter = 0;
    while (getline(archivo, linea)) {
        istringstream lineaStream(linea);
        string campo;

        while (getline(lineaStream, campo, ',')) {
            campos.push_back(campo);
        }

        char id[7];
        char date[10];
        char customer[4];
        char model[13];
        char price[7];
        char quantity[1];
        char storelocation[12];
        char salespersonid[3];
        char paymentmethod[11];
        char customerage[2];
        char customergender[6];

        strncpy(id, campos[0].c_str(), sizeof(id) - 1);
        id[sizeof(id) - 1] = '\0';
        strncpy(date, campos[1].c_str(), sizeof(date) - 1);
        date[sizeof(date) - 1] = '\0';
        strncpy(customer, campos[2].c_str(), sizeof(customer) - 1);
        customer[sizeof(customer) - 1] = '\0';
        strncpy(model, campos[3].c_str(), sizeof(model) - 1);
        model[sizeof(model) - 1] = '\0';
        strncpy(price, campos[4].c_str(), sizeof(price) - 1);
        price[sizeof(price) - 1] = '\0';
        strncpy(quantity, campos[5].c_str(), sizeof(quantity) - 1);
        quantity[sizeof(quantity) - 1] = '\0';
        strncpy(storelocation, campos[6].c_str(), sizeof(storelocation) - 1);
        storelocation[sizeof(storelocation) - 1] = '\0';
        strncpy(salespersonid, campos[7].c_str(), sizeof(salespersonid) - 1);
        salespersonid[sizeof(salespersonid) - 1] = '\0';
        strncpy(paymentmethod, campos[8].c_str(), sizeof(paymentmethod) - 1);
        paymentmethod[sizeof(paymentmethod) - 1] = '\0';
        strncpy(customerage, campos[9].c_str(), sizeof(customerage) - 1);
        customerage[sizeof(customerage) - 1] = '\0';
        strncpy(customergender, campos[10].c_str(), sizeof(customergender) - 1);
        customergender[sizeof(customergender) - 1] = '\0';


        // Creamos el objeto Record1 con los valores extraídos del CSV
        Record2 record2(id, date, customer, model, price, quantity, storelocation, salespersonid, paymentmethod, customerage, customergender);
        
        bool result = metodo2->add(record2);

        if (counter == 100000)
            break;
        counter++;
        campos.clear();
    }
}