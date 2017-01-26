/*
MIT License

Copyright (c) 2017 Noah Andreas

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


#pragma once


#include <qt5xdg/XdgMenu>
#include <QMenu>
#include <QApplication>
#include <QSettings>
#include <QFileSystemWatcher>
#include <QXmlStreamReader>


class Californium : public QApplication
{
    Q_OBJECT

public:
    explicit Californium(int &argc, char** argv);
    ~Californium();

    int sendToggle();


protected:
    bool eventFilter(QObject *obj, QEvent *event);


private:
    QSettings settings;
    QMenu* menu = 0;
    XdgMenu xdgMenu;
    QFileSystemWatcher watcher;

    void loadSettings();
    void watchSettings();
    void loadTheme();
    void readMenu();
    void registerToggle();


public slots:
    void buildMenu();
    void toggle();
    void fileChanged(QString fileName);


private slots:
    void show();

};