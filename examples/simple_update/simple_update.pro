TEMPLATE=app
TARGET=update
LIBS += -larchive
QT+=core network
SOURCES += main.cpp 
HEADERS += ../../GHReleaseBridge.hpp \
	   ../../QArchive/QArchive.hpp \
	   ../../QEasyDownloader/QEasyDownloader.hpp
