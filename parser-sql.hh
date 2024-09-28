#ifndef PARSER_SQL_H
#define PARSER_SQL_H

#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <cctype>
#include <string>
#include <fstream>
#include <vector>
#include <list>
#include <unordered_map>
// #include "avl.cpp"
#include "isam.cpp"
#include "selector.h"
#include "sequential.cpp"

using namespace std;

inline SeleccionarMetodo<Record1>* metodo = new SeleccionarMetodo<Record1>();
inline SeleccionarMetodo<Record2>* metodo2 = new SeleccionarMetodo<Record2>();
// inline SeleccionarMetodo<Record1>* metodo = nullptr;
// inline SeleccionarMetodo<Record2>* metodo2 = nullptr;
inline vector<Record1> registros1;
inline vector<Record2> registros2;
inline pair<string, string> tabla;
inline vector<pair<string, string>> tablas(2);

class Token{
    public: 
    enum Type {SELECT, CREATE, TABLE, FROM, FILE, WHERE, AND, INSERT, DELETE, BETWEEN, INTO, END, ERR, LPAREN, RPAREN, COMMA, SEMICOLON, ALL, FILENAME, NUMBER, EQUAL, QUOTE, DOBLEQUOTE, FLOAT, ID, VALUES, INDEX, AVL, SEQUENTIAL, ISAM, USING };
    static const char* token_names[31]; //CAMBIARRR
    Type type;
    string lexema;
    Token(Type);
    Token(Type, const string source);
};


class Scanner{
public:
    Scanner(const string& s);
    Token* nextToken();
    ~Scanner();
private:
    string input;
    int first, current;
    unordered_map<string, Token::Type> reserved;
    Token::Type checkReserved(const string& lexema);
    char nextChar();
    void rollBack();
    void startLexema();
    string getLexema();
    bool isNumber(const string& s);
};


class Parser{
    Scanner* scanner;
    Token *current, *previous;
    vector<string> datos;
    vector<pair<string, string>> values;
    pair<string, string> value;
    bool match(Token::Type ttype);
    bool check(Token::Type ttype);
    bool advance();
    bool isAtEnd();
    bool findTable(string tableName);
    bool parseCondition(string tableName);
    bool parseEqual();
    bool search_key(string tableName);
    bool parse_range(string tableName);
    bool parseDelete2(string tableName);
    bool existeTabla(const string& tableName);

    bool parseValuesSentence(string tableName);
    bool parseValuesList();
    bool parseValues();
public:
    // Parser(Scanner* scanner);
    // Parser(Scanner* sc):scanner(sc) {
    //     previous = current = NULL;
    //     return;
    // }
    Parser(Scanner* &sc): scanner(sc), previous(nullptr), current(nullptr) {}
    // ~Parser();
    void parse();
    void mostrar();
    void parseStatement();
    void parseExpression();
    void parseCreateTable(Token* temp, string tableName);
    void parseSelect();
    void parseInsert();
    void parseDelete();

    void eliminarValor(string tableName);
    void parseIndex(string fileName, string tableName);
    void parseTipoMetodo(string fileName, string tableName);
    void parseFile(string tableName);
    void parseCondition2(string tableName);
    void insertData(string path);
    void insertData2(string path);
};

#endif