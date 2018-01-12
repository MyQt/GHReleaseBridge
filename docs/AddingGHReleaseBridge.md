---
id: AddingGHReleaseBridge
title: Add GHReleaseBridge to Your Qt Project
sidebar_label: Adding to Your Qt Project
---

|	        |             				                                       |		
|-----------|------------------------------------------------------------------|
|  Header:  | #include "GHReleaseBridge/GHReleaseBridge.hpp"                 |
|   qmake:  | LIBS += -larchive	                                               |
|           | QT += core network xml concurrent                                  |
|	        | HEADERS += GHReleaseBridge/GHReleaseBridge.hpp                 |
|           | HEADERS += GHReleaseBridge/QArchive/QArchive.hpp                |
|           | HEADERS += GHReleaseBridge/QEasyDownloader/QEasyDownloader.hpp  |
|Inherits:  | [QObject](http://doc.qt.io/qt-5/qobject.html)                    |

**GHReleaseBridge** is just a header and all you have to do after installation is to add   
it in your **.pro** file ( **Qt Project file** ).

### Qt Project file (**.pro**)

Append these **lines** to your **Qt Project file**.

```
LIBS += -larchive
QT += core network xml concurrent
HEADERS += GHReleaseBridge/GHReleaseBridge.hpp \
           GHReleaseBridge/QArchive/QArchive.hpp \
           GHReleaseBridge/QEasyDownloader/QEasyDownloader.hpp
```

### Including GHReleaseBridge in your Source

Whenever you want to use **GHReleaseBridge** , you just need to include it!

```
#include "GHReleaseBridge/GHReleaseBridge.hpp"
```

