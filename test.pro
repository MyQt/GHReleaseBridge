TEMPLATE=app
TARGET=test
LIBS += -larchive
QT+=core network xml
SOURCES += test.cpp 
HEADERS += GHReleaseBridge.hpp \
	   QArchive/QArchive.hpp \
	   QEasyDownloader/QEasyDownloader.hpp
