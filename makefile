######  BUILD  ######

INCLUDES = -Isrc -I/usr/include -I/usr/include/qt -I/usr/include/qt/QtWidgets -I/usr/include/qt/QtGui -I/usr/include/qt/QtDBus -I/usr/include/qt/QtCore
COMPILER = clang -std=c++11 -Wall -O3 -fPIC -MMD $(INCLUDES) -c $< -o $@
MOC = moc $(INCLUDES) -o $@ $<
LINKER = g++ -o $@ $^ -lQt5Xdg -lQt5Widgets -lQt5Gui -lQt5DBus -lQt5Core
MKDIR = mkdir -p $(dir $@)


# this prevents make from deleting the generated moc_*.cpp files
# so that the including of the dep-files will not fail
.SECONDARY:


data/usr/bin/californium: build/californium.o build/main.o build/moc_californium.o
	$(MKDIR)
	$(LINKER)

build/%.o: src/%.cpp
	$(MKDIR)
	$(COMPILER)

build/moc_%.o: build/moc_%.cpp
	$(MKDIR)
	$(COMPILER)

build/moc_%.cpp: src/%.h
	$(MKDIR)
	$(MOC)

-include build/*.d


######  CLEAN  ######

clean:
	rm -f build/*
	rm -f data/usr/bin/californium


######  INSTALL  ######

install:
	cp -r data/* /


######  UNINSTALL  ######

uninstall:
	rm -f -r /usr/share/californium
	rm -f /usr/bin/californium


.PHONY: clean install uninstall