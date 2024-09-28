#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <vector>
#include "parser_sql.cpp"


using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString sql = ui->lineEdit->text();
    QString sql2 = ui->lineEdit_2->text();
    string query = sql.toStdString();
    string n = sql2.toStdString();

    string sentencia = query;
    while (true) {
        cout << "Inserte su sentencia SQL: " << endl;
        cin.ignore();  // Limpiar el buffer
        getline(cin, sentencia);
        Scanner scanner(sentencia + " ");
        Parser parser(&scanner);
        parser.parse();

    }
}

