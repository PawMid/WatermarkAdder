#ifndef DIRECTORYSELECTOR_H
#define DIRECTORYSELECTOR_H

#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QGridLayout>
#include "DirectoryViewer.h"

#include <QString>
#include <QUrl>
#include <QObject>
#include <QEvent>
using std::string;

class DirectorySelector : public QObject
{
    Q_OBJECT
    QString selectorLabel;
    QString selectedDirectory;
    QString buttonLabel;
    QLabel * directoryLabel;

    QString trimLabel();
    QString expandLabel();
    DirectoryViewer* viewer;

    private slots:
        void on_button_clicked();
        void on_mouse_enter();
        void on_mouse_leave();

    public:
        DirectorySelector(QString selectorLabel, DirectoryViewer* viewer = nullptr, QString buttonLabel="Select", QString defaultDirectory = "C:\\");
        ~DirectorySelector();
        QWidget * getWidget();
        QString* getDirectory();
};

#endif // DIRECTORYSELECTOR_H
