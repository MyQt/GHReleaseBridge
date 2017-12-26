#include <QCoreApplication>
#include "GHReleaseBridge.hpp"

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    GHReleaseBridge Bridge(
		    "antony-jr",
		    "QArchive",
		    "v0.0.2",
		    "QArchive-{version}.tar.gz",
		    "./",
		    false
    );


    QObject::connect(&Bridge , &GHReleaseBridge::updatesList ,
    [&](QVector<GHPackageUpdate> &list)
    {
    	if(list.isEmpty())
	{
		qDebug() << "No Updates Available Dude!";
		app.quit();
		return;
	}

	qDebug() << "New Updates Available!";

	for(int i = 0; i < list.size() ; ++i)
	{
		qDebug() << "Download Link:: " << list.at(i).downloadLink;
	}
    });

    Bridge.CheckForUpdates();

    return app.exec();
}
