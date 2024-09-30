#include "parser.hh"
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

      // if(isNumber(getLexema()))
      //   return new Token(Token::NUMBER, getLexema());

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
  
void Parser::showV(){
    for(auto it : values){
      cout<<it.first<<": "<<it.second<<endl;
    }
}
  
void Parser::parseInsert(){
    try {
        if(tabla.second == "record1") {
            char id[24];
            char name[530];
            char popularity[4];
            char duration_ms[8];
            char explicit_[2];
            char artists[935];
            char id_artists[1509];
            char release_date[11];
            char danceability[7];
            char energy[9];
            char key_str[3];
            char loudness[8];
            char mode[2];
            char speechiness[7];
            char acousticness[9];
            char instrumentalness[9];
            char liveness[8];
            char valence[9];
            char tempo[8];
            char time_signature[2];

            // strncpy(id, values[0].first.c_str(), sizeof(id) - 1);
            // id[sizeof(id) - 1] = '\0';
            // strncpy(name, values[1].first.c_str(), sizeof(name) - 1);
            // name[sizeof(name) - 1] = '\0';
            // strncpy(popularity, values[2].first.c_str(), sizeof(popularity) - 1);
            // popularity[sizeof(popularity) - 1] = '\0';
            // strncpy(duration_ms, values[3].first.c_str(), sizeof(duration_ms) - 1);
            // duration_ms[sizeof(duration_ms) - 1] = '\0';
            // strncpy(explicit_, values[4].first.c_str(), sizeof(explicit_) - 1);
            // explicit_[sizeof(explicit_) - 1] = '\0';
            // strncpy(artists, values[5].first.c_str(), sizeof(artists) - 1);
            // artists[sizeof(artists) - 1] = '\0';
            // strncpy(id_artists, values[6].first.c_str(), sizeof(id_artists) - 1);
            // id_artists[sizeof(id_artists) - 1] = '\0';
            // strncpy(release_date, values[7].first.c_str(), sizeof(release_date) - 1);
            // release_date[sizeof(release_date) - 1] = '\0';
            // strncpy(danceability, values[8].first.c_str(), sizeof(danceability) - 1);
            // danceability[sizeof(danceability) - 1] = '\0';
            // strncpy(energy, values[9].first.c_str(), sizeof(energy) - 1);
            // energy[sizeof(energy) - 1] = '\0';
            // strncpy(key_str, values[10].first.c_str(), sizeof(key_str) - 1);
            // key_str[sizeof(key_str) - 1] = '\0';
            // strncpy(loudness, values[11].first.c_str(), sizeof(loudness) - 1);
            // loudness[sizeof(loudness) - 1] = '\0';
            // strncpy(mode, values[12].first.c_str(), sizeof(mode) - 1);
            // mode[sizeof(mode) - 1] = '\0';
            // strncpy(speechiness, values[13].first.c_str(), sizeof(speechiness) - 1);
            // speechiness[sizeof(speechiness) - 1] = '\0';
            // strncpy(acousticness, values[14].first.c_str(), sizeof(acousticness) - 1);
            // acousticness[sizeof(acousticness) - 1] = '\0';
            // strncpy(instrumentalness, values[15].first.c_str(), sizeof(instrumentalness) - 1);
            // instrumentalness[sizeof(instrumentalness) - 1] = '\0';
            // strncpy(liveness, values[16].first.c_str(), sizeof(liveness) - 1);
            // liveness[sizeof(liveness) - 1] = '\0';
            // strncpy(valence, values[17].first.c_str(), sizeof(valence) - 1);
            // valence[sizeof(valence) - 1] = '\0';
            // strncpy(tempo, values[18].first.c_str(), sizeof(tempo) - 1);
            // tempo[sizeof(tempo) - 1] = '\0';
            // strncpy(time_signature, values[19].first.c_str(), sizeof(time_signature) - 1);
            // time_signature[sizeof(time_signature) - 1] = '\0';
            
            char *campos[] = {id, name, popularity, duration_ms, explicit_, artists, id_artists, release_date,
                                danceability, energy, key_str, loudness, mode, speechiness, acousticness,
                                instrumentalness, liveness, valence, tempo, time_signature};
            int longitudes[] = {24, 530, 4, 8, 2, 935, 1509, 11, 7, 9, 3, 8, 2, 7, 9, 9, 8, 9, 8, 2};
            
            for (int i = 0; i < 20; i++) {
                strncpy(campos[i], values[i].first.c_str(), longitudes[i] - 1);
                // campos[i][longitudes[i] - 1] = '\0';
            }
            Record1 record(id, name, popularity, duration_ms, explicit_, artists, id_artists, release_date,
                            danceability, energy, key_str, loudness, mode, speechiness, acousticness, instrumentalness,
                            liveness, valence, tempo, time_signature);


            record.print();
            bool ins = metodo->add(record);
            if (!ins) {
                error_message = "Ya existe este elemento";
            } else {
                cout << "Insertado" << endl;
                error_message = "";
            }
            cout << "Insertando" << endl;
        } else {
          char id[7];
          char date[11];
          char customer[5];
          char model[14];
          char price[8];
          char quantity[2];
          char storelocation[13];
          char salespersonid[4];
          char paymentmethod[12];
          char customerage[3];
          char customergender[7];

          char *campos[] = {id, date, customer, model, price, quantity, storelocation, salespersonid, paymentmethod, customerage, customergender};
          int longitudes[] = {7, 11, 5, 14, 8, 2, 13, 4, 12, 3, 7};
          for (int i = 0; i < 11; i++) {
              strncpy(campos[i], values[i].first.c_str(), longitudes[i] - 1);
            //   campos[i][longitudes[i] - 1] = '\0';
          }

        //   strncpy(id, values[0].first.c_str(), sizeof(id));
        // //   id[sizeof(id) - 1] = '\0';
        //   strncpy(date, values[1].first.c_str(), sizeof(date));
        // //   date[sizeof(date) - 1] = '\0';
        //   strncpy(customer, values[2].first.c_str(), sizeof(customer));
        // //   customer[sizeof(customer) - 1] = '\0';
        //   strncpy(model, values[3].first.c_str(), sizeof(model));
        // //   model[sizeof(model) - 1] = '\0';
        //   strncpy(price, values[4].first.c_str(), sizeof(price));
        // //   price[sizeof(price) - 1] = '\0';
        //   strncpy(quantity, values[5].first.c_str(), sizeof(quantity));
        // //   quantity[sizeof(quantity) - 1] = '\0';
        //   strncpy(storelocation, values[6].first.c_str(), sizeof(storelocation));
        // //   storelocation[sizeof(storelocation) - 1] = '\0';
        //   strncpy(salespersonid, values[7].first.c_str(), sizeof(salespersonid));
        // //   salespersonid[sizeof(salespersonid) - 1] = '\0';
        //   strncpy(paymentmethod, values[8].first.c_str(), sizeof(paymentmethod));
        // //   paymentmethod[sizeof(paymentmethod) - 1] = '\0';
        //   strncpy(customerage, values[9].first.c_str(), sizeof(customerage));
        // //   customerage[sizeof(customerage) - 1] = '\0';
        //   strncpy(customergender, values[10].first.c_str(), sizeof(customergender));
        //   customergender[sizeof(customergender) - 1] = '\0';

          Record2 record(id, date, customer, model, price, quantity, storelocation, salespersonid, paymentmethod, customerage, customergender);
          bool ins = metodo2->add(record);
          if (!ins) {
              error_message = "Ya existe este elemento";
          } else {
              cout << "Insertado" << endl;
              error_message = "";
          }
          cout << "Insertando" << endl;
        }

    } catch (const std::exception& e) {
        cout << "\tValores no validos" << e.what() << endl;
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
        Token* temp = current;
        if (previous) delete previous;
        current = scanner->nextToken();
        previous = temp;
        if (check(Token::ERR)) {
        error_message = "\tParse error, no se reconoce el caracter";
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
              if (!existeTabla(previous->lexema)) {
                
                parseCreateTable(temp, previous->lexema);
                return;
              } 
              error_message = "\tYa existe la tabla " + previous->lexema;
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
            string table_Name = previous->lexema;
            if(!existeTabla(table_Name)){
                error_message = "\tNo existe esa tabla";
                return;
            }
            if(match(Token::VALUES)){
                bool res = parseValuesSentence(table_Name);
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
            error_message = "\tEsperaba \'";
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
    error_message = "\tSintaxis incorrecta....";
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

        cout << "Se selecciono la tabla " << tableName << endl;
        // cout<<tabla.second<<endl;

        if(tabla.second == "record1")
          registros1 = metodo->load();
        else
          registros2 = metodo2->load();

        cout << registros1.size() << endl;
        cout << registros2.size() << endl;
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
  error_message = "\tError de sintaxis_";
}

void Parser::parseEliminarTabla(){
    if(match(Token::FROM)){
        if(match(Token::ID)){
        string nomTable = previous->lexema;
        if(!existeTabla(nomTable)){
            error_message = "\tNo existe la tabla "+ nomTable;
            return;
        }
        bool r = parseDelete(nomTable);
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
                cout<<" Eliminar"<<endl;
                eliminarValor(tableName);
                cout << "se elimino" << endl;
                error_message = "";
                return true;
            }
            error_message = "\tSe esperaba ;";
            return false;
            }
            return false;
        }
        error_message = "\tSe esperaba '=' se encontro ";
        }
        error_message = "\tSe esperaba ID" ;
        return false;
    }
    error_message = "\tSe esperaba WHERE";
    return false;
}

bool Parser::existeTabla(string tableName){
    // cout << "holaaaa" << endl;
    // cout<<tablas[0].first<<endl;
    // cout<<tablas[1].first<<endl;
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
        if(tabla.second == "record1"){
            cout << "entro1" << endl;
            char id[23];
            strncpy(id, value.first.c_str(), 22);
            id[22] = '\0';
            eliminado = metodo->remove(id);
            cout << "Si se ha eliminado1" << endl;
        } else if(tabla.second == "record2"){
            cout << "entro2" << endl;
            char id[7];
            strncpy(id, value.first.c_str(), 6);
            id[6] = '\0';
            eliminado = metodo2->remove(id);
            cout << "Si se ha eliminado2" << endl;
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
        error_message = "\tSe esperaba filename";
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
    // cout << archivo << endl;
    if(match(Token::AVL)){
        if(match(Token::SEMICOLON)){
            if(archivo == "musica.csv"){
                remove("avl_data.dat");
                metodo = new AVLFile<Record1>(fileName);
                if (tableName == tablas[1].first) {
                    tablas[1].first = "";
                }
                tablas[0] = make_pair(tableName, "record1");
                tabla = tablas[0];
                insertData(fileName);
              cout << "Tabla con musica.csv creada" << endl;
            } 
            else if(archivo == "bike.csv"){
                remove("avl_data.dat");
                metodo2 = new AVLFile<Record2>(fileName);
                if (tableName == tablas[0].first) {
                    tablas[0].first = "";
                }
                tablas[1] = make_pair(tableName, "record2");
                tabla = tablas[1];
                insertData2(fileName);
              cout << "Tabla con bike.csv creada" << endl;
            }
            cout << "Se uso AVL para crear la tabla" << endl;
            error_message = "";
            return;
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
                // metodo = new SequentialFile<Record1>(fileName, comparator, printer);
                if (tableName == tablas[1].first) {
                    tablas[1].first = "";
                }
                tablas[0] = make_pair(tableName, "record1");
                tabla = tablas[0];
                // insertData(fileName);
              cout << "Tabla con musica.csv creada" << endl;
            } 
            else if(archivo == "bike.csv"){
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
                // metodo2 = new SequentialFile<Record2>(fileName, comparator, printer);
                if (tableName == tablas[0].first) {
                    tablas[0].first = "";
                }
                tablas[1] = make_pair(tableName, "record2");
                tabla = tablas[1];
                // insertData2(fileName);
              cout << "Tabla con bike.csv creada" << endl;
            }
            cout << "Se uso Sequential para crear la tabla" << endl;
            error_message = "";
            return;
        }
        error_message = "\tSintaxis incorrecta, ;";
        return;
    } else if(match(Token::ISAM)){
        // cout << archivo << endl;
        // cout << "tae" << endl;
        if(match(Token::SEMICOLON)){
            // cout << archivo << endl;
            if(archivo == "musica.csv"){
                remove("index1_musica.bin");
                remove("index2_musica.bin");
                remove("index3_musica.bin");
                delete metodo;
                metodo = new ISAM<Record1>(fileName, true);
                if (tableName == tablas[1].first) {
                    tablas[1].first = "";
                }
                tablas[0] = make_pair(tableName, "record1");
                tabla = tablas[0];
                // insertData(fileName);
              cout << "Tabla con musica.csv creada" << endl;
            } 
            else if(archivo == "bike.csv"){
                // cout<<"entro"<<endl;
                remove("index1_bike.bin");
                remove("index2_bike.bin");
                remove("index3_bike.bin");
                metodo2 = new ISAM<Record2>(fileName, false);
                if (tableName == tablas[0].first) {
                    tablas[0].first = "";
                }
                tablas[1] = make_pair(tableName, "record2");
                tabla = tablas[1];
                // insertData2(fileName);
              cout << "Tabla con bike.csv creada" << endl;
            }
            cout << "Se uso ISAM para crear la tabla, funcionando" << endl;
            error_message = "";
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
        error_message = "\tSintaxis incorrecta__";
        return;
      }
      
      return;
    }
    else if(match(Token::BETWEEN)){
        cout<<"Between"<<endl;
        searchRange(tableName);
        cout << "fin de rango" << endl;
    }
    else
        error_message = "\tSintaxis incorrecta____";
    return;
}


bool Parser::parseBuscarValor(string tableName){
          // cout << "holaaa" << endl;
    try{
        if(tabla.second == "record2"){
          cout << "holaaa" << endl;
            registros2.clear();
            char id[7];
            strncpy(id, value.first.c_str(), 6);
            id[6] = '\0';
            pair<Record2, bool> result = metodo2->search(id);
            result.first.print();
            if(!result.second){
                error_message = "\tNo existe el valor dicho.";
                return false;
            } else {
                registros2.push_back(result.first);
                return true;
            }
        } else {
            registros1.clear();
            char id[23];
            strncpy(id, value.first.c_str(), 22);
            id[22] = '\0';
            pair<Record1, bool> result = metodo->search(id);
            result.first.print();
            if(!result.second){
                error_message = "\tNo existe el valor dicho.";
                return false;
            } else {
                registros1.push_back(result.first);
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
      // cout << dato << endl;
          if(!match(Token::QUOTE)){
              error_message = "\tError de sintaxis, esperaba un \"";
              return false;
          }
          value = make_pair(dato, "char");
          return true;
        }
        error_message = "\tSintaxis incorrecta_.";
        return false;
    }
    error_message  = "\tSintaxis incorrecta-_";
    return false;
}

void Parser::searchRange(string tableName) {
    try {
        if (tabla.second == "record1") {
            string beginS, endS;
            if (match(Token::QUOTE)) {
                if (match(Token::ID)) {
                    beginS = previous->lexema;
                    if (!match(Token::QUOTE)) {
                        error_message = "\tError de sintaxis, esperaba un '";
                        return;
                    }
                }

                if (match(Token::AND)) {
                    if (match(Token::QUOTE)) {
                        if (match(Token::ID)) {
                            endS = previous->lexema;
                            if (!match(Token::QUOTE)) {
                                error_message = "\tError de sintaxis, esperaba un '";
                                return;
                            }

                            if (match(Token::SEMICOLON)) {
                                registros1.clear();
                                cout << "Busqueda por rango" << endl;
                                char begin[23], end[23];
                                strncpy(begin, beginS.c_str(), 22);
                                begin[22] = '\0';
                                strncpy(end, endS.c_str(), 22);
                                end[22] = '\0';

                                registros1 = metodo->rangeSearch(begin, end);
                                if (registros1.empty()) {
                                    cout << "No se encontraron registros en el rango especificado." << endl;
                                } else {
                                    for (int i = 0; i < registros1.size(); i++) {
                                        registros1[i].print();
                                    }
                                }
                                cout << "Se buscó" << endl;
                                return;
                            }
                        }
                        error_message = "\tSintaxis incorrecta";
                        return;
                    }
                    error_message = "\tSintaxis incorrecta";
                    return;
                }
                error_message = "\tSintaxis incorrecta";
                return;
            }
        } else {
            string beginS, endS; 
            if (match(Token::QUOTE)) {
                if (match(Token::ID)) {
                    beginS = previous->lexema; 
                    if (!match(Token::QUOTE)) {
                        error_message = "\tError de sintaxis, esperaba un '";
                        return;
                    }
                }

                if (match(Token::AND)) {
                    if (match(Token::QUOTE)) {
                        if (match(Token::ID)) {
                            endS = previous->lexema; 
                            if (!match(Token::QUOTE)) {
                                error_message = "\tError de sintaxis, esperaba un '";
                                return;
                            }

                            if (match(Token::SEMICOLON)) {
                                registros2.clear();
                                cout << "Busqueda por rango" << endl;
                                char begin[7], end[7];
                                strncpy(begin, beginS.c_str(), 6);
                                begin[6] = '\0'; 
                                strncpy(end, endS.c_str(), 6);
                                end[6] = '\0';

                                registros2 = metodo2->rangeSearch(begin, end);
                                if (registros2.empty()) {
                                    cout << "No se encontraron registros en el rango especificado." << endl;
                                } else {
                                    for (int i = 0; i < registros2.size(); i++) {
                                        registros2[i].print();
                                        // cout << "mmmm" << endl;
                                    }
                                }
                                cout << "Se buscó" << endl;
                                return;
                            }
                        }
                        error_message = "\tSintaxis incorrecta";
                        return;
                    }
                    error_message = "\tSintaxis incorrecta";
                    return;
                }
                error_message = "\tSintaxis incorrecta";
                return;
            }
        }
    } catch (const std::exception &e) {
        error_message = "\tError al buscar por rango.";
    }
}

// void Parser::searchRange(string tableName){
//     if(match(Token::QUOTE)){
//         if(match(Token::ID)){
//             string beginS = previous->lexema;
//             char* begin = new char[beginS.length() + 1];
//             strcpy(begin, beginS.c_str());
//             if(!match(Token::QUOTE)){
//                 error_message = "\tError de sintaxis, esperaba un \'";
//             }
//             value = make_pair(begin, "char");
//         }
//         if(match(Token::AND)){
//             if(match(Token::QUOTE)){
//                 if(match(Token::ID)){
//                     string endS = previous->lexema;
//                     char* end = new char[endS.length() + 1];
//                     strcpy(end, endS.c_str());
//                     if(!match(Token::QUOTE)){
//                         error_message = "\tError de sintaxis, esperaba un \'";
//                     }
//                     value = make_pair(end, "char");
//                     if(match(Token::SEMICOLON)){
//                         registros2.clear();
//                         cout << "Busqueda por rango" << endl;
//                         registros2 = metodo2->rangeSearch(begin, end);
//                         return ;
//                     }
//                 }
//                 error_message = "\tSintaxis incorrecta";
//                 return ;
//             }
//             error_message = "\tSintaxis incorrecta";
//             return ;
//         }
//         error_message = "\tSintaxis incorrecta";
//         return ;
//     }
// }


void Parser::insertData(string path){
    cout << path << endl;
    ifstream archivo(path);
    cout << "path: " << path << endl;
    if (!archivo.is_open()) {cout << "\npath incorrecto.";return;}

    string linea;
    vector<string> campos;
    istringstream lineaStream;
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


        // Creamos el objeto Record2 con los valores extraídos del CSV
        Record2 record2(id, date, customer, model, price, quantity, storelocation, salespersonid, paymentmethod, customerage, customergender);
        
        bool result = metodo2->add(record2);

        if (counter == 100000)
            break;
        counter++;
        campos.clear();
    }
}

