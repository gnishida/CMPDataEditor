/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindowClass
{
public:
    QAction *actionExit;
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionUndo;
    QAction *actionNext;
    QAction *actionPrevious;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menuFle;
    QMenu *menuEdit;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindowClass)
    {
        if (MainWindowClass->objectName().isEmpty())
            MainWindowClass->setObjectName(QStringLiteral("MainWindowClass"));
        MainWindowClass->resize(800, 853);
        actionExit = new QAction(MainWindowClass);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionOpen = new QAction(MainWindowClass);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionSave = new QAction(MainWindowClass);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        actionUndo = new QAction(MainWindowClass);
        actionUndo->setObjectName(QStringLiteral("actionUndo"));
        actionNext = new QAction(MainWindowClass);
        actionNext->setObjectName(QStringLiteral("actionNext"));
        actionPrevious = new QAction(MainWindowClass);
        actionPrevious->setObjectName(QStringLiteral("actionPrevious"));
        centralWidget = new QWidget(MainWindowClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        MainWindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindowClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 800, 21));
        menuFle = new QMenu(menuBar);
        menuFle->setObjectName(QStringLiteral("menuFle"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QStringLiteral("menuEdit"));
        MainWindowClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindowClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindowClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindowClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindowClass->setStatusBar(statusBar);

        menuBar->addAction(menuFle->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuFle->addAction(actionOpen);
        menuFle->addAction(actionNext);
        menuFle->addAction(actionPrevious);
        menuFle->addAction(actionSave);
        menuFle->addSeparator();
        menuFle->addAction(actionExit);
        menuEdit->addAction(actionUndo);

        retranslateUi(MainWindowClass);

        QMetaObject::connectSlotsByName(MainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowClass)
    {
        MainWindowClass->setWindowTitle(QApplication::translate("MainWindowClass", "CMP Data Editor", 0));
        actionExit->setText(QApplication::translate("MainWindowClass", "Exit", 0));
        actionOpen->setText(QApplication::translate("MainWindowClass", "Open", 0));
        actionOpen->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+O", 0));
        actionSave->setText(QApplication::translate("MainWindowClass", "Save", 0));
        actionSave->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+S", 0));
        actionUndo->setText(QApplication::translate("MainWindowClass", "Undo", 0));
        actionUndo->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+Z", 0));
        actionNext->setText(QApplication::translate("MainWindowClass", "Next", 0));
        actionNext->setShortcut(QApplication::translate("MainWindowClass", "Right", 0));
        actionPrevious->setText(QApplication::translate("MainWindowClass", "Previous", 0));
        actionPrevious->setShortcut(QApplication::translate("MainWindowClass", "Left", 0));
        menuFle->setTitle(QApplication::translate("MainWindowClass", "File", 0));
        menuEdit->setTitle(QApplication::translate("MainWindowClass", "Edit", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
