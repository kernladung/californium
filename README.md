A standalone application menu based on xdg and qt, notably for openbox, awesome, i3 and other window managers without a automatic xdg menu.

##### californium with default-theme and arc-icon-theme
![Screenshot of californium](https://github.com/mlde/californium/blob/master/screenshot.png)



### Features
- creates a main application menu automatically 
- creates the menu based on the freedesktop xdg specification
- updates itself automatically for new installed applications
- menu is easily customizable via the standard menu-files
- based on QT and easily themable with qss files
- supports transparency
- easy to use with key and mouse-bindings



# Get it

- clone the repository with `$ git clone https://github.com/mlde/californium.git`

**or**

- download as zip and extract it



# Build it

[missing]

### Dependencies
- Qt5Xdg
- Qt5Widgets 
- Qt5Gui 
- Qt5DBus 
- Qt5Core

### Build

A build `californium` is provided in `data/usr/bin`. It was build on my arch64 system.


# Install it

[missing]


# Configure it

*californium* will look for the following conf-files. The first one found will be used. 

    ~/.config/californium/californium.conf
    /etc/xdg/californium/californium.conf

If none of these exists, it will create `~/.config/californium/californium.conf` with default values:

    menu=/etc/xdg/menus/sulfur-applications.menu
    theme=/usr/share/californium/themes/default.qss

Used config-files and theme-files are monitored and changes are applied automatically.


### Predefined menu-file

[europium](https://github.com/mlde/europium) provides a predefined menu-file and [holmium](https://github.com/mlde/holmium ) provides directory-files that work well with *californium*. To use an existing or new menu-file simply point to this menu-file in the conf-file.

Menu-files are typically placed in `/etc/xdg/menus/`. Directory-files are typically placed in `/usr/share/desktop-directories`.


### Themes
In the default-conf the path to the theme is set to the default theme `/usr/share/californium/themes/default/default.qss`. You can simply change that path in the conf-file ie `~/.config/californium/californium.conf`. Any url's in the qss may be relative to the path of the qss-file.

See http://doc.qt.io/qt-5/stylesheet-examples.html#customizing-qmenu for specific and http://doc.qt.io/qt-5/stylesheet-syntax.html for general reference.


### Icons
Per default californium uses the global icon theme which is set via your desktop manager or via qt5ct.

See https://wiki.archlinux.org/index.php/qt#Configuration_of_Qt5_apps_under_environments_other_than_KDE_Plasma




# Run it

*californium* provides the binary `californium` which starts a QT Application without a visible GUI, but with the application menu. It then listens to a specific DBus Signal to toggle this menu. The signal can be send with `californium toggle`, the menu pops up.

Start *californium* as background process:

    $ californium &

Send signal to toggle/show menu:

    $ californium toggle



# Openbox

To replace the build-in menu in openbox with *californium*, do this:

### Autostart

Add entry to `~/.config/openbox/autostart`:

    californium &

### Mouse bindings

Find this part in `~/.config/openbox/rc.xml`:

    <context name="Root">
        ....
        <mousebind button="Right" action="Press">
            <action name="ShowMenu"><menu>root-menu</menu></action>
        </mousebind>
        ....
    </context>

Change it to:

    <context name="Root">
        ....    
        <mousebind action="Press" button="Right">
            <action name="Execute">
                <command>californium toggle</command>
            </action>
        </mousebind>
        ....
    </context>


### Key bindings

To add a keybind for ie Super-Key (windows key), add this to `~/.config/openbox/rc.xml`:

    <keyboard>
        ....
        <keybind key="Super_L">
            <action name="Execute">
                <command>californium toggle</command>
            </action>
        </keybind>
        ....
    </keyboard>

See http://openbox.org/wiki/Help:Configuration for reference.



# Uninstall it

To uninstall *californium* open a terminal in the root folder of the repository and:

    $ sudo make uninstall

Of course, you have to reset any settings ie in openbox too.


# Credits
*californium* is inspired and based on the great work of the LXQt team. I really liked their new LXQt Desktop, especially the main menu plugin from their lxqt-panel repository. However, i'm using a openbox setup without a panel. So i created a simple standalone main menu based on the main menu plugin.

http://lxqt.org/
