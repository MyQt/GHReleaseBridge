---
id: ClassGHReleaseBridge
title: Class GHReleaseBridge
sidebar_label: Class GHReleaseBridge
---

This is the only **class** that is provided by **GHReleaseBridge** , this takes care of everything you   
want in a single class and thus its **easier** for you to **remember**.

> Warning: Your Update Asset should be a archive.

## Public Functions

|              |                                                                                                              |
|--------------|--------------------------------------------------------------------------------------------------------------|
| **explicit** | GHReleaseBridge(QObject *p = 0)                                                                              |
| **explicit** | GHReleaseBridge(const QString &username,const QString &repo,const QString &version,const QString &assetFormat,const QString &installationPath,bool debug) |
| **explicit** | GHReleaseBridge(const QJsonDocument& configuration)                                                          |
| **explicit** | GHReleaseBridge(const QJsonObject& configuration)                                                            |
| **void**     | setConfiguration(const QJsonDocument& configuration)                                                         |
| **void**     | setConfiguration(const QString &username,const QString &repo,const QString &version,const QString &assetFormat,const QString &installationPath,bool debug) |
| **void**     | showConfiguration(void)                                                                                      |


## Slots

|                       |                                       |
|-----------------------|---------------------------------------|
| **void**              | CheckForUpdates(void)                 |
| **void**              | DownloadUpdates(void)                 |
| **void**              | InstallUpdates(void)                  |
| **void**              | AbortDownload(void)                   |
| **void**              | AbortInstallation(void)               |

## Signals

|              |                                                                                                                                             |
|--------------|---------------------------------------------------------------------------------------------------------------------------------------------|
| **void**     | error(short **[erroCode](GHReleaseBridgeErrorCodes.md)** , const QString& what)                                                             |
| **void**     | updatesLatest(const QJsonDocument& Update)                                                                                                  |
| **void**     | updatesDownloadProgress(qint64 received,qint64 total,int percent,double speed,const QString &unit,const QUrl &url,const QString &fileName)  |
| **void**     | updatesDownloaded(void)                                                                                                                     |
| **void**     | updatesInstalling(const QString& pacakgeTempFileName)                                                                                       |
| **void**     | updatesInstalled(void)                                                                                                                      |
| **void**     | newConfiguration(const QJsonDocument& newConfig)                                                                                            |
| **void**     | DownloadAborted(void)                                                                                                                       |
| **void**     | InstallationAborted(void)                                                                                                                   |


## Member Functions Documentation

### explicit GHReleaseBridge(QObject *p = 0)

Only Constructs the **Class**. Sets the **QObject Parent** as NULL or **0** if the user does not give it!

### explicit GHReleaseBridge(const QString &username,const QString &repo,const QString &version,const QString &assetFormat,const QString &installationPath,bool debug)

Constructs and Sets the configuration for Github Release Bridge. 

> **username** : Your username or organization name.   
> **repo**     : Your repo name or address.   
> **version**  : Current version of your app   
> **assetFormat** : The format your update asset will follow. (Must be a archive).   
> **installationPath**: Where must the archive extract to.   
> **debug**: whether to debug or not?

**Example**:
```
username = "antony-jr";
repo     = "GHReleaseBridge";
version  = "v0.0.1";
assetFormat = "Updates-{version}.tar.gz";
// The {version} will be replaced with the latest version 

installationPath = "./";
debug = true;
```

### explicit GHReleaseBridge(const QJsonDocument& configuration)

Sets the configuration via a **json document** and thus makes it easier for you.

```
QFile fileIO("config.json");
if(!fileIO.open(QIODevice::ReadOnly)){
    return -1;
}
GHReleaseBridge Bridge(QJsonDocument::fromJson(fileIO.readAll()));
```

**config.json**
```
{
    "username" : "antony",
    "repo"     : "GHReleaseBridge",
    "version"  : "v0.0.1",
    "assetFormat" : "Updates-{version}.tar.gz",
    "installationPath" : "./",
    debug : true
}
```

### explicit GHReleaseBridge(const QJsonObject& configuration)

This is the easiest and the most efficient way to construct the **class**.
```
GHReleaseBridge Bridge({
       {"username", "antony-jr"},
       {"repo" , "QArchive"},
       {"version", "0.0.1"},
       {"assetFormat", "QArchive-{version}.tar.gz"},
       {"installationPath", "./"},
       {"debug", false}
   });
```

### void setConfiguration(const QJsonDocument& configuration)

Sets the configuration via a **json document** and thus makes it easier for you.

```
QFile fileIO("config.json");
if(!fileIO.open(QIODevice::ReadOnly)){
    return -1;
}
GHReleaseBridge Bridge;
Bridge.setConfiguration(QJsonDocument::fromJson(fileIO.readAll()));
fileIO.close();
```

**config.json**
```
{
    "username" : "antony",
    "repo"     : "GHReleaseBridge",
    "version"  : "v0.0.1",
    "assetFormat" : "Updates-{version}.tar.gz",
    "installationPath" : "./",
     debug : true
}
```

### void setConfiguration(const QString &username,const QString &repo,const QString &version,const QString &assetFormat,const QString &installationPath,bool debug)

Sets the configuration for Github Release Bridge. 

> **username** : Your username or organization name.   
> **repo**     : Your repo name or address.   
> **version**  : Current version of your app   
> **assetFormat** : The format your update asset will follow. (Must be a archive).   
> **installationPath**: Where must the archive extract to.   
> **debug**: whether to debug or not?

**Example**:
```
username = "antony-jr";
repo     = "GHReleaseBridge";
version  = "v0.0.1";
assetFormat = "Updates-{version}.tar.gz";
// The {version} will be replaced with the latest version 

installationPath = "./";
debug = true;
```

### void showConfiguration(void)

Shows the configuration if debug is set to **true**.

> **Note:** Automatically shows the configuration if debug is true.


### void CheckForUpdates(void)
<p align="right"> <b> [SLOT] </b> </p>

Checks for new updates from the GitHub Releases. Emits **updatesLatest(const QJsonDocument& update)** when finished.

### void DownloadUpdates(void)
<p align="right"> <b> [SLOT] </b> </p>

Downloads new updates if available , **Must Be Called After CheckForUpdates()**. Emits **updatesDownloaded()** when finished.

### void InstallUpdates(void)
<p align="right"> <b> [SLOT] </b> </p>

Installs the downloaded updates to the **installation path** , **Must Be Called After DownloadUpdates()** , Emits **updatesInstalled()** when finished.
and **newConfiguration(const QJsonDocument& newConfig) to set new configuration.

### void AbortDownload(void)
<p align="right"> <b> [SLOT] </b> </p>

Aborts the current downloads. Emits **DownloadAborted()** on success.

### void AbortInstallation(void)
<p align="right"> <b> [SLOT] </b> </p>

Aborts thhe current installation. Emits **InstallationAborted()** on success.


### void error(short **[erroCode](GHReleaseBridgeErrorCodes.md)** , const QString& what)
<p align="right"> <b> [SIGNAL] </b> </p>

Emitted when something goes wrong.

### void updatesLatest(const QJsonDocument& Update)
<p align="right"> <b> [SIGNAL] </b> </p>

Emitted when **CheckForUpdates()** is finished. If **Update.isNull()** is **true** then there is no new update!

**The QJsonDocument will follow this structure**
```
{
    "username" : "antony-jr",
    "repo"     :  "QArchive",
    "version"  :  "v0.0.2",
    "downloadLink" : "downloadLink"
}
```

### void updatesDownloadProgress(qint64 received,qint64 total,int percent,double speed,const QString &unit,const QUrl &url,const QString &fileName)
<p align="right"> <b> [SIGNAL] </b> </p>

Emitted on progress of the download for the updates.

> **received** : The bytes recived.   
> **total**    : Total bytes to be retrived.   
> **percent**  : The percent of file downloaded from the url.   
> **speed**    : The speed of the download of the current progress.   
> **unit**     : The unit that needs to be used to represent **speed**.(ex: kbps/s)   
> **url**      : The current download url.
> **filename** : The filename which is beign writen to.


### void updatesDownloaded(void)
<p align="right"> <b> [SIGNAL] </b> </p>

Emitted when all updates are downloaded.

### void updatesInstalling(const QString& pacakgeTempFileName)
<p align="right"> <b> [SIGNAL] </b> </p>

Emitted when installing a file. (i.e) Copying a single file.


### void updatesInstalled(void)
<p align="right"> <b> [SIGNAL] </b> </p>

Emitted when the installation is successfull.

### void newConfiguration(const QJsonDocument& newConfig)
<p align="right"> <b> [SIGNAL] </b> </p>

Emitted along with **updatesInstalled()** to give the user a option to save configuration!

### void DownloadAborted(void)
<p align="right"> <b> [SIGNAL] </b> </p>

Emitted when download was aborted successfully.

### void InstallationAborted(void)
<p align="right"> <b> [SIGNAL] </b> </p>

Emitted when installation was aborted successfully.
