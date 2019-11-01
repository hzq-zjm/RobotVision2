/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created: Fri Jan 4 17:19:33 2013
**      by: Qt User Interface Compiler version 4.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCommandLinkButton>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QLabel *m_imgLabel;
    QToolButton *toolButton;
    QCommandLinkButton *commandLinkButton;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(465, 245);
        m_imgLabel = new QLabel(Widget);
        m_imgLabel->setObjectName(QString::fromUtf8("m_imgLabel"));
        m_imgLabel->setEnabled(true);
        m_imgLabel->setGeometry(QRect(10, 0, 320, 240));
        QFont font;
        font.setBold(false);
        font.setItalic(false);
        font.setWeight(50);
        m_imgLabel->setFont(font);
        m_imgLabel->setMargin(5);
        m_imgLabel->setIndent(-1);
        m_imgLabel->setOpenExternalLinks(false);
        toolButton = new QToolButton(Widget);
        toolButton->setObjectName(QString::fromUtf8("toolButton"));
        toolButton->setGeometry(QRect(350, 0, 111, 51));
        commandLinkButton = new QCommandLinkButton(Widget);
        commandLinkButton->setObjectName(QString::fromUtf8("commandLinkButton"));
        commandLinkButton->setGeometry(QRect(350, 60, 111, 41));

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", 0, QApplication::UnicodeUTF8));
        m_imgLabel->setText(QApplication::translate("Widget", "TextLabel", 0, QApplication::UnicodeUTF8));
        toolButton->setText(QApplication::translate("Widget", "hello", 0, QApplication::UnicodeUTF8));
        commandLinkButton->setText(QApplication::translate("Widget", "JASON", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
