/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QLabel *m_imgLabel;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->setWindowModality(Qt::NonModal);
        Widget->resize(797, 523);
        Widget->setAcceptDrops(false);
        m_imgLabel = new QLabel(Widget);
        m_imgLabel->setObjectName(QString::fromUtf8("m_imgLabel"));
        m_imgLabel->setEnabled(true);
        m_imgLabel->setGeometry(QRect(40, 0, 640, 480));
        m_imgLabel->setMinimumSize(QSize(640, 480));
        m_imgLabel->setMaximumSize(QSize(9999, 9999));
        QFont font;
        font.setBold(false);
        font.setItalic(false);
        font.setWeight(50);
        m_imgLabel->setFont(font);
        m_imgLabel->setScaledContents(true);
        m_imgLabel->setMargin(5);
        m_imgLabel->setIndent(-1);
        m_imgLabel->setOpenExternalLinks(false);
        pushButton = new QPushButton(Widget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(700, 130, 91, 31));
        pushButton_2 = new QPushButton(Widget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(700, 320, 91, 31));

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Iva Fatigue Driving Real-time Detection System", 0, QApplication::UnicodeUTF8));
        m_imgLabel->setText(QApplication::translate("Widget", "TextLabel", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("Widget", "START", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("Widget", "QUIT", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
