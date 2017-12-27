#include <QCoreApplication>
#include "../../GHReleaseBridge.hpp"

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    GHReleaseBridge Bridge(
		    "antony-jr",
		    "QArchive",
		    "v0.0.1",
		    "QArchive-{version}.tar.gz",
		    "./",
		    true
    );



    QObject::connect(&Bridge , &GHReleaseBridge::updatesInstalled,
    [&](){
	qDebug() << "Successfully Installed Update!";
	app.quit();
	return;
    });
    QObject::connect(&Bridge , &GHReleaseBridge::updatesDownloaded,
    [&](){
	qDebug() << "Updates Downloaded Successfully!";
	qDebug() << "Starting installation... ";
	Bridge.InstallUpdates();
    });
    QObject::connect(&Bridge , &GHReleaseBridge::updatesLatest ,
    [&](QStringList update)
    {
    	if(update.isEmpty())
	{
		qDebug() << "No Updates Available Dude!";
		app.quit();
		return;
	}

	qDebug() << "New Updates Available!";
	qDebug() << "Installing " << update.at(GH_REPO) << " Version " << update.at(GH_VERSION);
	Bridge.DownloadUpdates();
    });

    Bridge.CheckForUpdates();

    return app.exec();
}
