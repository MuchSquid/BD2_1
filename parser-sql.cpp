#include "parser_sql.hh"
using namespace std;

inline static const char* token_names[34]= {"SELECT", "CREATE", "TABLE", "FROM", "ALL", "WHERE", 
  "DELETE", "EQUAL", "BETWEEN", "AND", "INSERT", "INTO", "VALUES", "FILE", "LPAREN", "RPAREN", 
  "INDEX", "USING", "END", "ERR", "SEMICOLON", "COMMA", "ID", "EOL",
   "NUMBER", "VALUE", "QUOTE", "FILENAME", "TRUE", "FALSE", "FLOAT", 
   "AVL", "SEQUENTIAL", "ISAM"};

inline std::ostream& operator << ( std::ostream& outs, const Token & tok )
{
  if (tok.lexema.empty())
    return outs << Token::token_names[tok.type];
  else
    return outs << Token::token_names[tok.type] << "(" << tok.lexema << ")";
}

inline std::ostream& operator << ( std::ostream& outs, const Token* tok ) {
  return outs << *tok;
}

//-------------------------Scanner--------------------------------

string Scanner::getLexema() {
    return input.substr(first,current-first);
  };

void Scanner::startLexema() {
    first = current-1;  
    return;
  };

void Scanner::rollBack() { 
    if (input[current] != '\0')
      current--;
};

char Scanner::nextChar() {
    int c = input[current];
    if (c != '\0') current++;
    return c;
};

bool Scanner::isNumber(const string& s) {
    for (char c : s) {
      if (!isdigit(c)) {
        return false;
      }
    }
    return true;
};


Token* Scanner::nextToken() {
    Token* token;
    char c;
    c = nextChar();
    while (c == ' ' || c == '\t' ) c = nextChar();
    if(c == '\n'){
      c = nextChar();
      while(c == '\n')
        c = nextChar();
    }
    startLexema();
    if (isdigit(c) || isalpha(c)) {
      c = nextChar();
      while (isalpha(c) || isdigit(c)) {
        c = nextChar();
      }

      if (c == ':') {
        c = nextChar();
        while (strchr("\\.:/-_",c) || isalpha(c) || isdigit(c)) {
          c = nextChar();
        }
        rollBack();
        string a = getLexema();
        return new Token(Token::FILENAME,a);
      }
      else if(isNumber(getLexema().substr(0,getLexema().length()-1)) && c == '.'){
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
      rollBack();
      string lexema = getLexema();
      if(current == input.length()){
        current-=1;
        lexema = getLexema();
        current++;
      }
      Token::Type ktype = reserved.search(lexema);
      if(ktype != Token::VALUE){
        return new Token(ktype);
      }

      if(isNumber(getLexema()))
        return new Token(Token::NUMBER, getLexema());

      return new Token(Token::ID, getLexema());    
    }
    else if (c == '\0') {
      return new Token(Token::END);
    }
    else if (c == '/' || c == '~'){
        c = nextChar();
      while (strchr("~/.-_",c) || isalpha(c) || isdigit(c)) {
          c = nextChar();
        }
        rollBack();
        string a = getLexema();
        return new Token(Token::FILENAME,a);
    }
    else if (strchr("()=,;*'",c)){
      switch (c) {
        case '(': return new Token(Token::LPAREN);
        case ')': return new Token(Token::RPAREN);
        case '=': return new Token(Token::EQUAL);
        case ',': return new Token(Token::COMMA);
        case ';': return new Token(Token::SEMICOLON);
        case '*': return new Token(Token::ALL);
        case '\'': return new Token(Token::QUOTE);
        default: cout << "No deberia llegar aca" << endl;
      }
    }
    else {
      return new Token(Token::ERR,getLexema());
    }
    return NULL;
  };



// -------------------------Parser--------------------------------


void Parser::parse(){
    current = scanner->nextToken();
    if(check(Token::ERR)){
        error_message = "\tCaracter invalido";
        return;
    }
    parseStatement();
    return;
}

void Parser::show(){
    cout<<"Atributos: "<<endl;
    for (auto const &v: datos){
        cout<<v<<", ";
    }
    cout<<endl;
}
  
//   void mostrarValues(){
//     for(auto it : values){
//       cout<<it.first<<": "<<it.second<<endl;
//     }
//   }
  
void Parser::parseInsert(){
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
                error_message = "Ya existe este elemento";
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
                error_message = "Ya existe este elemento";
            } else {
                cout << "Insertado" << endl;
            }
        }

    } catch (const std::exception& e) {
        cout << "\tValores incorrectos para la tabla: " << e.what() << endl;
    }
}

bool Parser::match(Token::Type ttype) {
    if (check(ttype)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::check(Token::Type ttype) {
    if (isAtEnd()) return false;
    return current->type == ttype;
}


bool Parser::advance() {
    if (!isAtEnd()) {
        Token* temp =current;
        if (previous) delete previous;
        current = scanner->nextToken();
        previous = temp;
        if (check(Token::ERR)) {
        error_message = "\tParse error, unrecognised character";
        return false;
        }
        return true;
    }
    return false;
} 

bool Parser::isAtEnd() {
    return (current->type == Token::END);
} 

  
void Parser::parseStatement(){
    if(match(Token::SELECT)){
        if(match(Token::ID)){
            parseExpression();
        }
        else if(match(Token::ALL))
            parseCreateTable(nullptr, "");
        else
            error_message =  "\tError esperaba los datos";
        return;
    }
    else if(match(Token::CREATE)){
        if(match(Token::TABLE)){
        Token* temp = previous; 
        if(match(Token::ID)){
            parseCreateTable(temp, previous->lexema);
            return;
        }
        error_message = "\tSe esperaba un nombre de tabla";
        return;
        }
        error_message = "\tError de sintaxis";
        return;
    }
    else if(match(Token::INSERT)){
        if(match(Token::INTO)){
        if(match(Token::ID)){
            string tabla_nombre = previous->lexema;
            if(!existeTabla(tabla_nombre)){
            error_message = "\tNo existe esa tabla";
            return;
            }
            if(match(Token::VALUES)){
            bool res = parseValuesSentence(tabla_nombre);
            return;
            }
            error_message = "\tEsperaba la sentencia VALUES";
            return;
        }
        error_message = "\tEsperaba el nombre de la tabla";
        return;
        }
        error_message = "\tEspera INTO en despues del INSERT.";
        return;
    }
    else if(match(Token::DELETE)){
        parseEliminarTabla();
        return;
    }
    error_message = "\tError esperaba una sentencia SQL.";
}

bool Parser::parseValuesSentence(string tableName){
    if(match(Token::LPAREN)){
        bool res = parseValuesList();
        if(!res) return false;
        if(match(Token::RPAREN)){
        if(match(Token::SEMICOLON)){
            parseInsert();
            return true;
        }
        error_message = "\tEsperaba ;";
        return false;
        }
        error_message = "\tEsperaba )";
        return false;
    }
    error_message = "\tSe esperaba (.";
    return false;
}

bool Parser::parseValuesList(){
    bool res = parseValues();
    while(match(Token::COMMA)){
        res = parseValues();
    }
    return res;
}

bool Parser::parseValues(){
    if(match(Token::QUOTE)){
        if(match(Token::ID)){
        string value = previous->lexema;
        if(match(Token::QUOTE)){
            values.push_back(make_pair(value, "string"));
            return true;
        }
        error_message = "\tEsperaba \"";
        return false;
        }
        error_message = "\tSe esperaba un dato";
        return false;
    }
    else if(match(Token::NUMBER)){
        values.push_back(make_pair(previous->lexema, "int"));
        return true;
    }
    else if(match(Token::FLOAT)){
        values.push_back(make_pair(previous->lexema, "float"));
        return true;
    }
    else if(match(Token::TRUE)){
        values.push_back(make_pair("true", "bool"));
        return true;
    }
    else if(match(Token::FALSE)){
        values.push_back(make_pair("false", "bool"));
        return true;
    }
    error_message = "\tSintaxis incorrecta";
    return false;
}


void Parser::parseExpression(){
    datos.push_back(previous->lexema);
    if(match(Token::COMMA)){
        advance();
        parseExpression();
    }
    else
        parseCreateTable(nullptr, "");
}
  
void Parser::parseCreateTable(Token* temp, string tableName){
    if(match(Token::FROM)){
        if(match(Token::ID)){
        tableName = previous->lexema;
        if(match(Token::SEMICOLON)){
            if(!existeTabla(tableName)){
            error_message = "\tNo existe la tabla " + tableName;
            return;
            }
            cout<<tabla.second<<endl;

            if(tabla.second == "record1")
            registros1 = metodo->load();
            else
            registros2 = metodo2->load();
            datos.clear();
            error_message = "";
            return;
        }
        if(tableName != tabla.first){
            error_message = "\tNo existe la tabla " + tableName;
            return;
        }
        parseOp1(tableName);
        return;
        }
        else if(temp!=nullptr && match(Token::FILE)){
        parseFile(tableName);
        return;
        }
        error_message = "\tError de sintaxis.";
        return;
    }
    error_message = "\tError de sintaxis";
}
void Parser::parseEliminarTabla(){
    if(match(Token::FROM)){
        if(match(Token::ID)){
        string nombre_table = previous->lexema;
        if(!existeTabla(nombre_table)){
            error_message = "\tNo existe la tabla "+ nombre_table;
            return;
        }
        bool r = parseDelete(nombre_table);
        return;
        }
        error_message  = "\tSe esperaba el nombre de la tabla.";
        return;
    }
    error_message = "\tSe esperaba la sentencia FROM, se obtuvo ";
}

bool Parser::parseDelete(string tableName){
    if (match(Token::WHERE)){
        if(match(Token::ID)){
        if(match(Token::EQUAL)){
            bool r = parseEqual();
            if(r){
            if(match(Token::SEMICOLON)){
                cout<<" Eliminacion"<<endl;
                eliminarValor(tableName);
                error_message = "";
                return true;
            }
            error_message = "\tSintaxis incorrecta, se esperaba ;";
            return false;
            }
            return false;
        }
        error_message = "\tSe esperaba la un '=' se encontro ";
        }
        error_message = "\tSe esperaba un id" ;
        return false;
    }
    error_message = "\tSe esperaba un WHERE";
    return false;
}

bool Parser::existeTabla(string tableName){
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
        if(!eliminado){
            error_message = "\tNo existe este valor.";
            return;
        }
    } catch(const std::exception &e){
        error_message = "\tError al eliminar.";
    }
}

void Parser::parseFile(string tableName){
    string fileName = "";
    if(match(Token::QUOTE)){
        if(match(Token::FILENAME)){
        fileName = previous->lexema;
        if(match(Token::QUOTE)){
            parseIndex(fileName, tableName);
            return;
        }
        error_message = "\tSe esperaba \"";
        return;
        }
        error_message = "\tSe esperaba el nombre del archivo";
        return;
    }
    error_message = "\tSe esperaba '";
    return;
}

void Parser::parseIndex(string fileName, string tableName){
    if(match(Token::USING)){
        if(match(Token::INDEX)){
        parseTipoMetodo(fileName, tableName);
        return;
        }
        error_message = "\tSintaxis incorrecta";
        return;
    }
    error_message = "\tSintaxis incorrecta";
    return;
}

void Parser::parseTipoMetodo(string fileName, string tableName){
    string archivo = "";
    int i = fileName.length()-1;
    while (fileName[i] != '/'){
        archivo = fileName[i] + archivo;
        i--;
    }
    if(match(Token::AVL)){
        if(match(Token::SEMICOLON)){
            if(archivo == "musica.csv"){
                remove("avl_data.dat");
                remove("avl_index.dat");
                metodo = new AVLFile<Record1>(fileName);
                if (tableName == tablas[1].first) {
                    tablas[1].first = "";
                }
                tablas[0] = make_pair(tableName, "registros1");
                tabla = tablas[0];
                insertData(fileName);
            } 
            else if(archivo == "bike.csv"){
                remove("avl_data.dat");
                remove("avl_index.dat");
                metodo2 = new AVLFile2<Record2>(fileName);
                if (tableName == tablas[0].first) {
                    tablas[0].first = "";
                }
                tablas[1] = make_pair(tableName, "registros2");
                tabla = tablas[1];
                insertData2(fileName);
            }
            error_message = "";
            cout << "Se uso AVL para crear la tabla" << endl;
        } 
        error_message = "\tSe esperaba un ;";
        return;
    } else if(match(Token::SEQUENTIAL)){
        if(match(Token::SEMICOLON)){
            if(archivo == "musica.csv"){
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
            else if(archivo == "musica.csv"){
                remove("sequential_data.dat");
                remove("sequential_aux.dat");
                auto comparator = [](const Record2& a, const Record2& b) {
                    return string(a.id) < string(b.id);
                };

                auto printer = [](const Record2& record) {
                    cout << "----- Registro -----\n";
                    cout << "Id: " << record.id << "\n";
                    cout << "Name: " << record.customer << "\n";
                    cout << "---------------------\n\n";
                };
                metodo2 = new SequentialFile<Record2>(fileName, comparator, printer);
                if (tableName == tablas[0].first) {
                    tablas[0].first = "";
                }
                tablas[1] = make_pair(tableName, "registros2");
                tabla = tablas[1];
                insertData2(fileName);
            }
            error_message = "";
            cout << "Se uso Sequential para crear la tabla" << endl;
            return;
        }
        error_message = "\tSintaxis incorrecta, ;";
        return;
    } else if(match(Token::ISAM)){
        if(match(Token::SEMICOLON)){
            if(archivo == "musica.csv"){
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
            else if(archivo == "bike.csv"){
                remove("isam_data.dat");
                remove("isam_index.dat");
                metodo2 = new ISAM<Record2>(fileName, false);
                if (tableName == tablas[0].first) {
                    tablas[0].first = "";
                }
                tablas[1] = make_pair(tableName, "registros2");
                tabla = tablas[1];
                insertData2(fileName);
            }
            error_message = "";
            cout << "Se uso ISAM para crear la tabla" << endl;
            return;
        }
        error_message = "\tSintaxis incorrecta, ;";
        return;
    } 
    error_message = "\tSintaxis incorrecta, falta index";
    return;
}

void Parser::parseOp1(string tableName){
    if (match(Token::WHERE)){
        if(match(Token::ID)){
        parseOp2(tableName);
        return;
        }
        error_message = "\tSe esperaba un id";
        return;
    }
    error_message = "\tSe esperaba un WHERE";
    return;
}

void Parser::parseOp2(string tableName){
    if(match(Token::EQUAL)){
        bool v = parseEqual();
        if(v){
        cout << previous->lexema << endl;
        if(match(Token::SEMICOLON)){
            bool s = parseBuscarValor(tableName);
            if(!s){
                error_message = "No existe un elemento con la llave buscada";
                return;
            }
            datos.clear();
            error_message = "";
            return;
        }
        error_message = "\tSintaxis incorrecta";
        return;
        }
        
        return;
    }
    else if(match(Token::BETWEEN)){
        cout<<"Between"<<endl;
        searchRange(tableName);
    }
    else
        error_message = "\tSintaxis incorrecta";
    return;
}


bool Parser::parseBuscarValor(string tableName){
    try{
        if(tabla.second == "registros1"){
            registros1.clear();
            char id[23];
            strncpy(id, value.first.c_str(), 22);
            id[22] = '\0';
            pair<Record1, bool> result = metodo->search(id);
            if(!result.second){
                error_message = "\tNo existe el valor dicho.";
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
                error_message = "\tNo existe el valor dicho.";
                return false;
            } else {
                registros2.push_back(result.first);
                return true;
            }
        }
    } catch(const std::exception &e){
        error_message = "\tError al eliminar.";
    }

}

bool Parser::parseEqual(){
    if(match(Token::NUMBER)){
        value = make_pair(previous->lexema, "int");
        return true;
    }
    else if(match(Token::QUOTE)){
        if(match(Token::ID)){
        string dato = previous->lexema;
        if(!match(Token::QUOTE)){
            error_message = "\tError de sintaxis, esperaba un \"";
            return false;
        }
        value = make_pair(dato, "char");
        return true;
        }
        error_message = "\tSintaxis incorrecta";
        return false;
    }
    error_message  = "\tSintaxis incorrecta";
    return false;
}

void Parser::searchRange(string tableName){
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
                    return ;
                }
                error_message = "\tSintaxis incorrecta";
                delete[] begin;
                delete[] end;
                return ;
            }
            error_message = "\tSintaxis incorrecta";
            delete[] begin;
            return ;
        }
        error_message = "\tSintaxis incorrecta";
        delete[] begin;
        return ;
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

