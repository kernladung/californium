A standalone application menu based on xdg and qt, notably for openbox, awesome, i3 and other window managers without a automatic xdg menu.

##### californium with default-theme and arc-icon-theme
![Screenshot of californium](https://github.com/nukura/californium/blob/master/screenshot.png)



### Features
- creates a main application menu automatically 
- creates the menu based on the freedesktop xdg specification
- updates itself automatically for new installed applications
- menu is easily customizable via the standard menu-files
- based on QT and easily themable with qss files
- supports transparency
- easy to use with key and mouse-bindings



# Get it

- clone the repository with `$ git clone https://github.com/kernladung/sulfur`

**or**

- download as zip and extract it



# Build it

*californium* uses [clang](http://clang.llvm.org/) as compiler, [g++](https://gcc.gnu.org/) as linker and [ninja](https://github.com/ninja-build/ninja) as build-tool. 

To build *lead* open terminal in the root folder of the repository and:

    $ ninja

### Dependencies
- Qt5Xdg
- Qt5Widgets 
- Qt5Gui 
- Qt5DBus 
- Qt5Core

### Build

A build `kernladung.californium` is provided in the build dir. It was build on my arch64 system.


# Install it

(in development, currently does not work!)

    $ sudo ninja install


# Configure it

*californium* will look for the following conf-files. The first one found will be used. 

    ~/.config/kernladung/californium.conf
    /etc/xdg/kernladung/californium.conf

If none of these exists, it will create `~/.config/kernladung/californium.conf` with default values:

    menu=/etc/xdg/menus/sulfur-applications.menu
    theme=/usr/share/kernladung/californium/themes/default.qss

Used config-files and theme-files are monitored and changes are applied automatically.


### Predefined menu-file

[sulfur](https://github.com/kernladung/sulfur) provides a predefined menu-file and directory-files that work well with californium. To use an existing or new menu-file simply point to this menu-file in the conf-file.

Menu-files are typically placed in `/etc/xdg/menus/`. Directory-files are typically placed in `/usr/share/desktop-directories`.


### Themes
In the default-conf the path to the theme is set to `/usr/share/kernladung/californium/themes/default/default.qss`. You can simply change that path in the conf-file ie `~/.config/kernladung/californium.conf`. Any url's in the qss may be relative to the path of the qss-file.

See http://doc.qt.io/qt-5/stylesheet-examples.html#customizing-qmenu for specific and http://doc.qt.io/qt-5/stylesheet-syntax.html for general reference.


### Icons
Per default californium uses the global icon theme which is set via your desktop manager or via qt5ct.

See https://wiki.archlinux.org/index.php/qt#Configuration_of_Qt5_apps_under_environments_other_than_KDE_Plasma




# Run it

*californium* provides the binary `kernladung.californium` which starts a QT Application without a visible GUI, but with the application menu. It then listens to a specific DBus Signal to toggle this menu. The signal can be send with `kernladung.californium toggle`, the menu pops up.

Start *californium* as background process:

    $ kernladung.californium &

Send signal to toggle/show menu:

    $ kernladung.californium toggle



# Openbox

To replace the build-in menu in openbox with *californium*, do this:

### Autostart

Add entry to `~/.config/openbox/autostart`:

    kernladung.californium &

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
                <command>kernladung.californium toggle</command>
            </action>
        </context>
        ....
    </context>


### Key bindings

To add a keybind for ie Super-Key (windows key), add this to `~/.config/openbox/rc.xml`:

    <keyboard>
        ....
        <keybind key="Super_L">
            <action name="Execute">
                <command>kernladung.californium toggle</command>
            </action>
        </keybind>
        ....
    </keyboard>

See http://openbox.org/wiki/Help:Configuration for reference.



# Credits
*californium* is inspired and based on the great work of the LXQt team. I really liked their new LXQt Desktop, especially the main menu plugin from their lxqt-panel repository. However, i'm using a openbox setup without a panel. So i created a simple standalone main menu based on the main menu plugin.

http://lxqt.org/
