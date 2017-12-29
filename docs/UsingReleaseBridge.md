---
id: UsingReleaseBridge
title: Using Github Release Bridge for a simple AutoUpdate!
sidebar_label: Simple Github Release Bridge Usage
---

This is a simple updater which updates my small header **QArchive**.   
Taken from the **examples** tree from the official repo.

## main.cpp

```
#include <QCoreApplication>
#include "GHReleaseBridge/GHReleaseBridge.hpp"

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    GHReleaseBridge Bridge({
        {"username", "antony-jr"},
        {"repo"	, "QArchive"},
        {"version", "0.0.1"},
        {"assetFormat", "QArchive-{version}.tar.gz"},
        {"installationPath", "./"},
        {"debug", false}
    });
    QObject::connect(&Bridge, &GHReleaseBridge::updatesLatest,
    [&](QJsonDocument Updates) {
        if(Updates.isNull()) {
            qDebug() << "No Updates Available!"; // Execute App Here
            return;
        }
        auto root = Updates.object();
        qDebug() << "New Update!";
        qDebug() << root["repo"].toString() << " ::version:: " << root["version"].toString();

        Bridge.DownloadUpdates();
        return;
    });
    QObject::connect(&Bridge, &GHReleaseBridge::updatesDownloaded, [&]() {
        Bridge.InstallUpdates();
        return;
    });
    QObject::connect(&Bridge, &GHReleaseBridge::newConfiguration,
    [&](QJsonDocument newConfig) {
        // lets store it for later use!
        QFile jsonFile("GHRelease.json");
        if(jsonFile.open(QIODevice::WriteOnly)) {
            jsonFile.write(newConfig.toJson());
            jsonFile.close();
            qDebug() << "Installation Complete!"; // Restart here
            app.quit();
        }
        return;
    });
    Bridge.CheckForUpdates(); // Check
    return app.exec();
}
```

## simple_update.pro

```
TEMPLATE =   app
TARGET   =   update
LIBS     += -larchive
QT       +=  core network
SOURCES  +=  main.cpp 
HEADERS  +=  GHReleaseBridge/GHReleaseBridge.hpp \
             GHReleaseBridge/QArchive/QArchive.hpp \
             GHReleaseBridge/QEasyDownloader/QEasyDownloader.hpp
```

## Compilation and Execution

```
 $ mkdir build
 $ cd build
 $ qmake ../simple_update.pro
 $ make -j4
 $ ./update
 $ # Now you should have a file named GHRelease.json
 $ # and the package in QArchive Directory.
```

The above is a very simple example so please refer the **[Class Documentation](ClassGHReleaseBridge.md)**
