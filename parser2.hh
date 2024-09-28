#ifndef PARSER_SQL_H
#define PARSER_SQL_H
#include <cctype>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <fstream>
#include <map>
#include "avl.h"
#include "avl2.h"
#include "ISAM.h"
#include "selector.h"
#include "sequential.cpp"

using namespace std;

inline SeleccionarMetodo<Record1>* metodo = new SeleccionarMetodo<Record1>();
inline SeleccionarMetodo<Record2>* metodo2 = new SeleccionarMetodo<Record2>();
inline vector<Record1> registros1;
inline vector<Record2> registros2;
inline string error_message = "";
inline vector<pair<string,string> > tablas(2);
inline pair<string, string> tabla;


class Token {
public:
  enum Type { SELECT, CREATE, TABLE, FROM, ALL, WHERE, DELETE, EQUAL, BETWEEN, AND, INSERT, INTO, VALUES, FILE, LPAREN, RPAREN, INDEX, USING, ISAM, AVL, SEQUENTIAL, END, ERR, SEMICOLON, COMMA, ID, EOL, NUMBER, VALUE, QUOTE, FILENAME, TRUE, FALSE, FLOAT};
  inline static const char* token_names[34];
  Type type;
  string lexema;
  Token(Type type):type(type) { lexema = ""; }
  Token(Type, char c);
  Token(Type type, const string source):type(type) {
    lexema = source;
  }
};

//palabras reservadas
class Reserved {
  map<string, Token::Type> reserved;

public:
  Reserved(){
    reserved.insert(make_pair("SELECT", Token::SELECT));
    reserved.insert(make_pair("CREATE", Token::CREATE)); 
    reserved.insert(make_pair("TABLE", Token::TABLE));
    reserved.insert(make_pair("FROM", Token::FROM));
    reserved.insert(make_pair("WHERE", Token::WHERE));
    reserved.insert(make_pair("DELETE", Token::DELETE));
    reserved.insert(make_pair("EQUAL", Token::EQUAL));
    reserved.insert(make_pair("BETWEEN", Token::BETWEEN));
    reserved.insert(make_pair("AND", Token::AND));
    reserved.insert(make_pair("INSERT", Token::INSERT));
    reserved.insert(make_pair("INTO", Token::INTO));
    reserved.insert(make_pair("VALUES", Token::VALUES));
    reserved.insert(make_pair("FILE", Token::FILE));
    reserved.insert(make_pair("INDEX", Token::INDEX));
    reserved.insert(make_pair("USING", Token::USING));
    reserved.insert(make_pair("ISAM", Token::ISAM));
    reserved.insert(make_pair("AVL", Token::AVL));
    reserved.insert(make_pair("SEQUENTIAL", Token::SEQUENTIAL));
    reserved.insert(make_pair("TRUE", Token::TRUE));
    reserved.insert(make_pair("FALSE", Token::FALSE));
  }

  Token::Type search(string lexema){
    auto it = reserved.find(lexema);
    if(it != reserved.end()) return it->second;
    return Token::VALUE;
  }
};


class Scanner {
public:
  Scanner(string _input) :input(_input),first(0),current(0) { };
  Token* nextToken();
  ~Scanner(){};  
private:
  string input;
  Reserved reserved;
  int first, current;
  int state;

  char nextChar();
  void rollBack();
  string getLexema();
  void startLexema();

  bool isNumber(const string& s);
  void incrStartLexema() {
    first++;
  };
};

class Parser {
  Scanner* scanner;
  Token* current, * previous;
  vector<string> datos;
  vector<pair<string, string>> values;
  pair<string, string> value;
public:
  Parser(Scanner* sc):scanner(sc) {
    previous = current = NULL;
    return;
  }
  void parse();
private:
  void show();
  void showV();
  void parseInsert();
  bool match(Token::Type type);
  bool check(Token::Type ttype);
  bool advance();
  bool isAtEnd();

  void parseStatement();
  bool parseValuesSentence(string tableName);
  bool parseValuesList();
  bool parseValues();
  void parseExpression();
  void parseCreateTable(Token* temp, string tableName);
  void parseEliminarTabla();
  bool existeTabla(string tableName);

  void eliminarValor(string tableName);
  bool parseDelete(string tableName);

  void parseIndex(string fileName, string tableName);
  void parseTipoMetodo(string fileName, string tableName);
  void parseOp1(string tableName);

  void parseFile(string tableName);
  bool parseEqual();

  void parseOp2(string tableName);
  bool parseBuscarValor(string tableName);
  void searchRange(string tableName);
  void insertData(string path);
  void insertData2(string path);
};


#endif
