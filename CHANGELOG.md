# 01/20/2017
- conf-file is no more provided from install, but created on first run of *californium* if it not exists

# 01/19/2017
- The predefined menu-file and directory-files are moved to its own repository. Existing configurations will still work. New Configurations needs to install [sulfur](https://github.com/kernladung/sulfur) to get the predefined set and point to this set in the conf-file.
- Changed build-system from qmake/make to ninja
- Changed licence from GPLv3 to MIT

# 01/18/2017
- Changed default theme path to `/usr/share/kernladung/californium/themes/[themename]/[themename].qss` so each theme has its own directory. Existing configurations will still work, because the theme is configured as an absolut path in the conf-file. However, i suggest to use this new approach.