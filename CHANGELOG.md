# 01/26/2017
- removed prefix "mlde" from name to make the component independent from a project name.
- default settings-path changed from `~/.config/mlde/californium.conf` to `~/.config/californium/californium.conf`. Please change your config-path.
- default theme-path changed from `/usr/share/mlde-californium/...` to `/usr/share/californium/...`. Please change your theme-path.
- changed namespace to Californium and renamed class Californium to Menu so the namespace and class names are clean

# 01/24/2017
- changed built-tool back to makefile
- bugfixes

# 01/23/2017
- added install/uninstall support

# 01/21/2017
- changed namespace from kernladung to mlde

# 01/20/2017
- conf-file is no more provided from install, but created on first run of *californium* if it not exists

# 01/19/2017
- The predefined menu-file and directory-files are moved to its own repository. Existing configurations will still work. New Configurations needs to install [sulfur](https://github.com/mlde/sulfur) to get the predefined set and point to this set in the conf-file.
- Changed build-system from qmake/make to ninja
- Changed licence from GPLv3 to MIT

# 01/18/2017
- Changed default theme path to `/usr/share/mlde/californium/themes/[themename]/[themename].qss` so each theme has its own directory. Existing configurations will still work, because the theme is configured as an absolut path in the conf-file. However, i suggest to use this new approach.