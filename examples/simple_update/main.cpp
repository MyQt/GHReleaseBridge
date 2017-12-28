#include <QCoreApplication>
#include "../../GHReleaseBridge.hpp"

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    // This must be read from file!
    QJsonObject Configuration {
        {"username", "antony-jr"},
        {"repo"	, "QArchive"},
        {"version", "0.0.1"},
        {"assetFormat", "QArchive-{version}.tar.gz"},
        {"installationPath", "./"},
        {"debug", false}
    };

    GHReleaseBridge Bridge(Configuration);

    QObject::connect(&Bridge, &GHReleaseBridge::updatesLatest,
    [&](QJsonDocument Updates) {
        if(Updates.isNull()) {
            qDebug() << "No Updates Available!";
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
            qDebug() << "Installation Complete!";
            app.quit();
        }
        return;
    });
    Bridge.CheckForUpdates();
    return app.exec();
}
