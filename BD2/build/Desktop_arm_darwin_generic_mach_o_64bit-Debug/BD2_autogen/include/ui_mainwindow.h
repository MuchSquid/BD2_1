/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QTabWidget *tabWidget;
    QWidget *tab;
    QLabel *label;
    QLabel *label_2;
    QWidget *tab_2;
    QTableWidget *tableWidget;
    QLineEdit *lineEdit;
    QPushButton *pushButton;
    QLineEdit *lineEdit_2;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        MainWindow->setStyleSheet(QString::fromUtf8("background-color: rgb(18, 18, 18)"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setGeometry(QRect(10, 10, 781, 521));
        tabWidget->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        tabWidget->setStyleSheet(QString::fromUtf8("QTabWidget::pane { \n"
"    border: 0; \n"
"}\n"
"\n"
"QTabBar::tab {\n"
"    background-color: #333; \n"
"    color: white;\n"
"    padding: 7px;\n"
"    border-radius: 10px;\n"
"	margin-top: 10px;\n"
" 	margin-left: 20px;\n"
"}\n"
"\n"
"QTabBar::tab:selected {\n"
"    background-color: #1DB954; \n"
"    color: black;\n"
"}\n"
"\n"
"QTabBar::tab:hover {\n"
"    background-color: #1DB954; \n"
"}\n"
"\n"
"QTabBar::tab:!selected {\n"
"    background-color: #333;\n"
"    color: white;\n"
"}\n"
"\n"
"QTabBar::tab:disabled {\n"
"    background-color: #777;  \n"
"    color: #999;\n"
"}\n"
""));
        tabWidget->setTabPosition(QTabWidget::TabPosition::North);
        tab = new QWidget();
        tab->setObjectName("tab");
        label = new QLabel(tab);
        label->setObjectName("label");
        label->setGeometry(QRect(300, 40, 201, 41));
        label->setStyleSheet(QString::fromUtf8("font: 32pt;"));
        label_2 = new QLabel(tab);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(60, 100, 671, 331));
        label_2->setStyleSheet(QString::fromUtf8("QLabel::pixmap {\n"
"    border-radius: 30px;\n"
"}\n"
""));
        label_2->setPixmap(QPixmap(QString::fromUtf8(":/media/music.png")));
        label_2->setScaledContents(true);
        label_2->setWordWrap(false);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/media/Home.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        tabWidget->addTab(tab, icon, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        tableWidget = new QTableWidget(tab_2);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setGeometry(QRect(20, 80, 741, 401));
        tableWidget->setStyleSheet(QString::fromUtf8("border-radius: 10px;\n"
"background-color: rgb(30, 30, 30);"));
        lineEdit = new QLineEdit(tab_2);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(20, 30, 341, 31));
        lineEdit->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    border: 1px solid rgba(27, 28, 28, 194);\n"
"    border-radius: 10px;\n"
"    padding: 5px;\n"
"    background-color: rgb(30, 30, 30);\n"
"}"));
        pushButton = new QPushButton(tab_2);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(370, 30, 31, 31));
        pushButton->setStyleSheet(QString::fromUtf8("border: 1px solid rgba(27, 28, 28, 194);\n"
"    border-radius: 10px;\n"
"    padding: 5px;\n"
"    background-color: rgb(30, 30, 30);"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/media/zoom.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        pushButton->setIcon(icon1);
        lineEdit_2 = new QLineEdit(tab_2);
        lineEdit_2->setObjectName("lineEdit_2");
        lineEdit_2->setGeometry(QRect(410, 30, 161, 31));
        lineEdit_2->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    border: 1px solid rgba(27, 28, 28, 194);\n"
"    border-radius: 10px;\n"
"    padding: 5px;\n"
"    background-color: rgb(30, 30, 30);\n"
"}"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/media/File.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        tabWidget->addTab(tab_2, icon2, QString());
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 38));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Proyecto BD2", nullptr));
        label_2->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "Home", nullptr));
        lineEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "Inserte su consulta SQL", nullptr));
        pushButton->setText(QString());
        lineEdit_2->setPlaceholderText(QCoreApplication::translate("MainWindow", "Cantidad de registros", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "SQL", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
