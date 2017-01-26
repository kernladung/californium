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


#include "menu.h"
#include <QKeyEvent>
#include <qt5xdg/XdgMenuWidget>
#include <QDebug>
#include <QtDBus>
#include <QIcon>


namespace Californium {


const QString DBUSNAME = "californium.menu";


Menu::Menu(int &argc, char** argv):
    QApplication(argc, argv),
    settings("californium", "californium")
{
	loadSettings();
    loadTheme();

    readMenu();
    buildMenu();

    registerToggle();

    watchSettings();
}


Menu::~Menu()
{
    delete menu;
}


void
Menu::loadSettings()
{	
	qInfo() << "Menu::loadSettings() " << settings.fileName();

	QFile settingsFile(settings.fileName());

	if (!settingsFile.exists())
	{
		qDebug() << "Menu::loadSettings() " << settings.fileName() << " does not exists, created it with default values:";
		settings.setValue("theme", "/usr/share/californium/themes/default/default.qss");
		settings.setValue("menu", "/etc/xdg/menus/sulfur-applications.menu");
		settings.sync();
	}
}


void
Menu::watchSettings()
{    
	watcher.addPath(settings.fileName());
	watcher.addPath(settings.value("theme").toString());
    // xdgmenu watches changes to menu-file itself!

    connect(&watcher, SIGNAL(fileChanged(QString)), this, SLOT(fileChanged(QString)));
}


void
Menu::fileChanged(QString fileName)
{
    qInfo() << "Menu::fileChanged() " << fileName;


    if ( fileName == settings.value("theme").toString() )
    {
        loadTheme();
    }
    else if ( fileName == settings.fileName() )
    {
        // reloads settings from file
        settings.sync();

        loadTheme();
        readMenu();
        buildMenu();
    }

    
    // some editors dont update but delete/create the config file, in this 
    // case our watcher loses the file so we have to add it again
    
    watcher.addPath(fileName);
}


void
Menu::loadTheme()
{
	QString theme = settings.value("theme").toString();

	qInfo() << "Menu::loadTheme() " << theme;
    

    QFile qssFile(theme);

    if (!qssFile.exists())
    {
        qCritical() << "Menu::loadTheme() file " << theme << " does not exists";
        return;
    }

    if (!qssFile.open(QIODevice::ReadOnly))
    {
        qCritical() << "Menu::loadTheme() unable to open file " << theme;
        return;
    }


    // change cwd so urls in qss can be relative
    QDir::setCurrent(QFileInfo(qssFile).canonicalPath());

    QString qss = QLatin1String(qssFile.readAll());

    setStyleSheet(qss);
}


void
Menu::readMenu()
{
	QString menu = settings.value("menu").toString();
	qInfo() << "Menu::readMenu() " << menu;


    QFile menuFile(menu);
    
    if (!menuFile.exists())
    {
        qCritical() << "Menu::readMenu() file " << menu << " does not exists";
        return;
    }

    if (!menuFile.open(QIODevice::ReadOnly))
    {
        qCritical() << "Menu::loadTheme() unable to open file " << menu;
        return;
    }


    xdgMenu.setEnvironments(QStringList() << qgetenv("XDG_CURRENT_DESKTOP"));

    if (!xdgMenu.read(menu))
    {
        qCritical() << "Menu::readMenu() error reading file " << menu << " : " << xdgMenu.errorString();
        return;
    }

    connect(&xdgMenu, SIGNAL(changed()), this, SLOT(buildMenu()));
}


void
Menu::buildMenu()
{
    if(menu)
    {
        delete menu;
    }

    menu = new XdgMenuWidget(xdgMenu, "Californium");

    menu->ensurePolished();
    menu->installEventFilter(this);
    menu->setAttribute(Qt::WA_TranslucentBackground);


    foreach(QAction* action, menu->actions())
    {
        if (action->menu())
        {
            action->menu()->installEventFilter(this);
            action->menu()->setAttribute(Qt::WA_TranslucentBackground);
        }
    }
}



void
Menu::toggle()
{
    if (!menu)
    {
        qCritical() << "Menu::toggle() menu is null";
        return;
    }

    if (menu->isVisible())
    {
        menu->hide();
    }
    else
    {
        // with some input devices, the menu hides immediatly, maybe because the buttons
        // are badly debounced, so to avoid this, wait a short time
        QTimer::singleShot(100, this, SLOT(show()));
    }
}

void
Menu::show()
{
    if (!menu)
    {
        qCritical() << "Menu::show() menu is null";
        return;
    }

    menu->popup(QCursor::pos());
}


void
Menu::registerToggle()
{
    QDBusConnection bus = QDBusConnection::sessionBus();

    if (!bus.isConnected())
    {
        qCritical() << "Menu::registerToggle() unable to connect to session bus";
        return;
    }

    if ( !bus.connect(
             QString(),
             QString(),
             DBUSNAME,
             "toggle",
             this,
             SLOT(toggle())))
    {
        qCritical() << "Menu::registerToggle() unable to connect to signal";
        return;
    }
}


int
Menu::sendToggle()
{
    QDBusConnection bus = QDBusConnection::sessionBus();

    if (!bus.isConnected())
    {
        qCritical() << "Menu::sendToggle() unable to connect to session bus";
        return false;
    }


    QDBusMessage m = QDBusMessage::createSignal("/", DBUSNAME, "toggle");

    if (!bus.send(m))
    {
        qCritical() << "Menu::sendToggle() sending signal failed: " << m.errorMessage();
        return false;
    }

    return true;
}


struct MatchAction
{
    MatchAction(QString key):key_(key) {}
    bool operator()(QAction* action) { return action->text().startsWith(key_, Qt::CaseInsensitive); }
    QString key_;
};


bool
Menu::eventFilter(QObject *obj, QEvent *event)
{
    QMenu* menu = qobject_cast<QMenu*>(obj);

    if(menu && event->type() == QEvent::KeyPress)
    {
        // if our shortcut key is pressed while the menu is open, close the menu
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

        if (keyEvent->modifiers() & ~Qt::ShiftModifier)
        {
            menu->hide();
            return true;
        }

        else // go to the menu item starts with the pressed key
        {
            QString key = keyEvent->text();
            if(key.isEmpty())
                return false;

            QAction* action = menu->activeAction();
            QList<QAction*> actions = menu->actions();
            QList<QAction*>::iterator it = std::find(actions.begin(), actions.end(), action);

            it = std::find_if(it + 1, actions.end(), MatchAction(key));

            if(it == actions.end())
                it = std::find_if(actions.begin(), it, MatchAction(key));

            if(it != actions.end())
                menu->setActiveAction(*it);
        }
    }

    return false;
}


} // namespace