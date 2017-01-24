######  BUILD  ######

INCLUDES = -Isrc -I/usr/include -I/usr/include/qt -I/usr/include/qt/QtWidgets -I/usr/include/qt/QtGui -I/usr/include/qt/QtDBus -I/usr/include/qt/QtCore
COMPILER = clang -std=c++11 -Wall -O3 -fPIC -MMD $(INCLUDES) -c $< -o $@
MOC = moc $(INCLUDES) -o $@ $<
LINKER = g++ -o $@ $? -lQt5Xdg -lQt5Widgets -lQt5Gui -lQt5DBus -lQt5Core


# this prevents make from deleting the generated moc_*.cpp files
# so that the including of the dep-files will not fail
.SECONDARY:


data/usr/bin/mlde-californium: build/californium.o build/main.o build/moc_californium.o
	$(LINKER)

build/%.o: src/%.cpp
	$(COMPILER)

build/moc_%.o: build/moc_%.cpp
	$(COMPILER)

build/moc_%.cpp: src/%.h
	$(MOC)

-include build/*.d


######  CLEAN  ######

clean:
	rm build/*
	rm data/usr/bin/mlde-californium


######  INSTALL  ######

install:
	cp -r data/* /
	cp build/mlde-californium /usr/bin/


######  UNINSTALL  ######

uninstall:
	rm -r /usr/share/mlde-californium
	rm /usr/bin/mlde-californium


.PHONY: clean install uninstall