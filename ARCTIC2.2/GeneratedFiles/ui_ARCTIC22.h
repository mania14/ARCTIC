/********************************************************************************
** Form generated from reading UI file 'ARCTIC22.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ARCTIC22_H
#define UI_ARCTIC22_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ARCTIC22Class
{
public:
    QAction *actionTransformPosition;
    QAction *actionTransformRotation;
    QAction *actionTransformScale;
    QWidget *MainLayout;
    QGridLayout *gridLayout;
    QTabWidget *RendererTabWidget;
    QWidget *PlayTabWidget;
    QGridLayout *gridLayout_2;
    QWidget *Debug;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuTool;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QDockWidget *MenuDock;
    QWidget *dockWidgetContents;

    void setupUi(QMainWindow *ARCTIC22Class)
    {
        if (ARCTIC22Class->objectName().isEmpty())
            ARCTIC22Class->setObjectName(QStringLiteral("ARCTIC22Class"));
        ARCTIC22Class->resize(1065, 717);
        actionTransformPosition = new QAction(ARCTIC22Class);
        actionTransformPosition->setObjectName(QStringLiteral("actionTransformPosition"));
        actionTransformRotation = new QAction(ARCTIC22Class);
        actionTransformRotation->setObjectName(QStringLiteral("actionTransformRotation"));
        actionTransformScale = new QAction(ARCTIC22Class);
        actionTransformScale->setObjectName(QStringLiteral("actionTransformScale"));
        MainLayout = new QWidget(ARCTIC22Class);
        MainLayout->setObjectName(QStringLiteral("MainLayout"));
        gridLayout = new QGridLayout(MainLayout);
        gridLayout->setSpacing(0);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        RendererTabWidget = new QTabWidget(MainLayout);
        RendererTabWidget->setObjectName(QStringLiteral("RendererTabWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(RendererTabWidget->sizePolicy().hasHeightForWidth());
        RendererTabWidget->setSizePolicy(sizePolicy);
        PlayTabWidget = new QWidget();
        PlayTabWidget->setObjectName(QStringLiteral("PlayTabWidget"));
        gridLayout_2 = new QGridLayout(PlayTabWidget);
        gridLayout_2->setSpacing(0);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        RendererTabWidget->addTab(PlayTabWidget, QString());
        Debug = new QWidget();
        Debug->setObjectName(QStringLiteral("Debug"));
        RendererTabWidget->addTab(Debug, QString());

        gridLayout->addWidget(RendererTabWidget, 0, 0, 1, 1);

        ARCTIC22Class->setCentralWidget(MainLayout);
        menuBar = new QMenuBar(ARCTIC22Class);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1065, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QStringLiteral("menuEdit"));
        menuTool = new QMenu(menuBar);
        menuTool->setObjectName(QStringLiteral("menuTool"));
        ARCTIC22Class->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ARCTIC22Class);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        ARCTIC22Class->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(ARCTIC22Class);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        ARCTIC22Class->setStatusBar(statusBar);
        MenuDock = new QDockWidget(ARCTIC22Class);
        MenuDock->setObjectName(QStringLiteral("MenuDock"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(MenuDock->sizePolicy().hasHeightForWidth());
        MenuDock->setSizePolicy(sizePolicy1);
        MenuDock->setMinimumSize(QSize(300, 38));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        MenuDock->setWidget(dockWidgetContents);
        ARCTIC22Class->addDockWidget(static_cast<Qt::DockWidgetArea>(2), MenuDock);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuTool->menuAction());
        mainToolBar->addAction(actionTransformPosition);
        mainToolBar->addAction(actionTransformRotation);
        mainToolBar->addAction(actionTransformScale);

        retranslateUi(ARCTIC22Class);

        RendererTabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ARCTIC22Class);
    } // setupUi

    void retranslateUi(QMainWindow *ARCTIC22Class)
    {
        ARCTIC22Class->setWindowTitle(QApplication::translate("ARCTIC22Class", "ARCTIC22", Q_NULLPTR));
        actionTransformPosition->setText(QApplication::translate("ARCTIC22Class", "Position", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionTransformPosition->setShortcut(QApplication::translate("ARCTIC22Class", "P", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionTransformRotation->setText(QApplication::translate("ARCTIC22Class", "Rotation", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionTransformRotation->setShortcut(QApplication::translate("ARCTIC22Class", "R", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionTransformScale->setText(QApplication::translate("ARCTIC22Class", "Scale", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionTransformScale->setShortcut(QApplication::translate("ARCTIC22Class", "S", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        RendererTabWidget->setTabText(RendererTabWidget->indexOf(PlayTabWidget), QApplication::translate("ARCTIC22Class", "Tab 1", Q_NULLPTR));
        RendererTabWidget->setTabText(RendererTabWidget->indexOf(Debug), QApplication::translate("ARCTIC22Class", "Tab 2", Q_NULLPTR));
        menuFile->setTitle(QApplication::translate("ARCTIC22Class", "File", Q_NULLPTR));
        menuEdit->setTitle(QApplication::translate("ARCTIC22Class", "Edit", Q_NULLPTR));
        menuTool->setTitle(QApplication::translate("ARCTIC22Class", "Tool", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ARCTIC22Class: public Ui_ARCTIC22Class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ARCTIC22_H
