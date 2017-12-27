/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2017, Antony jr
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * -----------------------------------------------------------------------------
 *  @filename           : GHReleaseBridge.hpp
 *  @description        : A simple Github Release Bridge to Download latest
 *  			  release and update them Automatically , writen
 *  			  in C++ using Qt5. Best suitable for your Qt Project
 *  			  which requires Autoupdate on new release!
 * -----------------------------------------------------------------------------
*/
#if !defined(GH_RELEASE_BRIDGE_HPP_INCLUDED)
#define GH_RELEASE_BRIDGE_HPP_INCLUDED
#include <QtCore>

#include "QArchive/QArchive.hpp"
#include "QEasyDownloader/QEasyDownloader.hpp"


/*
 * Error Codes
 * -----------
*/
#define NETWORK_ERROR -1

/*
 * Package update helper codes for QStringList.
*/
#define GH_USERNAME	 0
#define GH_REPO     	 1
#define GH_VERSION  	 2
#define GH_DOWNLOAD_LINK 3

/*
 * Class GHReleaseBridge  <- Inherits QObject.
 * ---------------------
 *
 *  The main class which handles everything and do stuff with github
 *  like a pro.
 *
 *  Constructors:
 *
 *  	GHReleaseBridge(QObject *p = NULL)  - Sets parent  if the user needs it.
 *  	GHReleaseBridge(
 *  			const QString &username,
 *                      const QString &description,
 *                      const QString &repo,
 *                      const QString &version,
 *                      const QString &installationPath,
 *                      bool debug
 *      )				    - Sets the configuration for GHReleaseBridge.
 *
 *
*/
class GHReleaseBridge : public QObject
{
    Q_OBJECT
public:
    explicit GHReleaseBridge(QObject *p = NULL)
    {
        connect(&DownloadManager, &QEasyDownloader::Error,
        [&](QNetworkReply::NetworkError errorCode, QUrl url, QString fileName) {
            (void)url; // These are useless anyways!
            (void)fileName;
            (void)errorCode;

            if(debug) {
                qDebug() << "GHReleaseBridge::Cannot connect with github.";
            }
            emit error(NETWORK_ERROR, "cannot connect with github.");
            return;
        });

    }

    explicit GHReleaseBridge(
        const QString &username,
        const QString &repo,
        const QString &version,
        const QString &assetFormat,
        const QString &installationPath,
        bool debug
    )
        : QObject(NULL),
          username(username),
          repo(repo),
          version(version),
          assetFormat(assetFormat),
          installationPath(installationPath),
          debug(debug)

    {
        connect(&DownloadManager, &QEasyDownloader::Error,
        [&](QNetworkReply::NetworkError errorCode, QUrl url, QString fileName) {
            (void)url; // These are useless anyways!
            (void)fileName;
            (void)errorCode;

            if(debug) {
                qDebug() << "GHReleaseBridge::Cannot connect with github.";
            }
            emit error(NETWORK_ERROR, "cannot connect with github.");
            return;
        });

        // Show configuration
        showConfiguration();
    }

    void setConfiguration(
        const QString &username,
        const QString &repo,
        const QString &version,
        const QString &assetFormat,
        const QString &installationPath,
        bool debug
    )
    {
        this->username = username;
        this->repo = repo;
        this->version = version;
        this->assetFormat = assetFormat;
        this->installationPath = installationPath;
        this->debug |= debug;
        showConfiguration();
        return;
    }

    void showConfiguration()
    {
        if(debug) {
            qDebug() << "**** Configuration ****";
            qDebug() << "Username/Organization:: " << ((username.isEmpty()) ? "Empty!" : username);
            qDebug() << "Repo                 :: " << ((repo.isEmpty()) ? "Empty!" : repo);
            qDebug() << "Version/Tag	      :: " << ((version.isEmpty()) ? "Empty!" : version);
            qDebug() << "Installation Path    :: " << ((installationPath.isEmpty()) ? "Empty!" : installationPath);
            qDebug() << "Debug                :: True";
            qDebug() << "***********************";
        }
        return;
    }

    ~GHReleaseBridge() { }

private slots:

    void CheckGitHubReleases(const QString &content)
    {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(content.toUtf8());
        QJsonObject jsonObject = jsonResponse.object();
        QJsonArray assetsArray = jsonObject["assets"].toArray();
        QString version = jsonObject["tag_name"].toString();
        QString toDownload(assetFormat);
        toDownload.replace("{version}", version);
        QString downloadLink;
        QVector<QJsonObject> assets;

        if(debug) {
            qDebug() << "GHReleaseBridge::Latest Version:: " << version;
            qDebug() << "GHReleaeeBrdige::Asset Required:: " << toDownload;
        }

        if(this->version == version) {
            if(debug) {
                qDebug() << "GHReleaseBridge::No new updates available.";
            }
            emit updatesLatest(Updates);
            return;
        }

        // Parse the array in the assets vector!
        foreach (const QJsonValue &value, assetsArray) {
            assets.push_back(value.toObject());
        }

        for(int i = 0; i < assets.size(); ++i) {
            if(debug) {
                qDebug() << "GHReleaseBridge::Checking Asset::" << assets.at(i)["name"].toString();
            }
            if(assets.at(i)["name"].toString() == toDownload) {
                downloadLink = assets.at(i)["browser_download_url"].toString();
                if(debug) {
                    qDebug() << "GHReleaseBridge::Latest Package::" << downloadLink;
                }
                break;
            }
        }

        // append the latest update
        Updates << this->username
                << this->repo
                << version
                << downloadLink;

        disconnect(&DownloadManager, SIGNAL(GetResponse(const QString&)),
                   this,SLOT(CheckGitHubReleases(const QString&)));

        emit updatesLatest(Updates);
        return;
    }

    bool isEmptyConfiguration()
    {
        return (
                   username.isEmpty() ||
                   repo.isEmpty() ||
                   version.isEmpty() ||
                   assetFormat.isEmpty() ||
                   installationPath.isEmpty()
               );
    }

public slots:
    void CheckForUpdates()
    {
        if(debug) {
            qDebug() << "GHReleaseBridge::Checking for updates.";
            qDebug() << "GHReleaseBridge::Checking the releases for latest assets.";
        }

        if(isEmptyConfiguration()) {
            if(debug) {
                qDebug() << "GHReleaseBridge::Empty Configuration given.";
            }
            return;
        }

        /*
         * Build the URL.
        */
        QUrl latestRelease(
            QString("https://api.github.com/repos/") +
            username +
            QString("/") +
            repo +
            QString("/releases/latest")
        );

        connect(&DownloadManager, SIGNAL(GetResponse(const QString&)),
                this,SLOT(CheckGitHubReleases(const QString&)));

        DownloadManager.Debug(debug);
        DownloadManager.Get(latestRelease);
        return;
    }

    void DownloadUpdates()
    {
        if(Updates.isEmpty() || TempFile != NULL) {
            return;
        }

        connect(&DownloadManager, &QEasyDownloader::Finished,
        [&]() {
            emit updatesDownloaded();
        });

        connect(&DownloadManager, &QEasyDownloader::DownloadProgress,
                [&](qint64 bytesReceived,
                    qint64 bytesTotal,
                    int percent,
                    double speed,
                    const QString &unit,
                    const QUrl &url,
        const QString &fileName) {
            emit updatesDownloadProgress(bytesReceived, bytesTotal, percent, speed, unit, url,fileName);
            return;
        });

        DownloadManager.Debug(debug);
        DownloadManager.ResumeDownloads(false);

        TempFile = new QTemporaryFile;
        TempFile->open();

        DownloadManager.Download(Updates.at(GH_DOWNLOAD_LINK), TempFile->fileName());
        return;
    }

    void InstallUpdates()
    {
        if(TempFile == NULL) {
            return;
        }
        connect(&Archiver, &QArchive::Extractor::status,
        [&](const QString& Archive,const QString& file) {
            (void)Archive;
            emit updatesInstalling(file);
            return;
        });

        connect(&Archiver, &QArchive::Extractor::error,
        [&](short errorCode, const QString& Archive) {
            emit error(errorCode, Archive);
            return;
        });

        connect(&Archiver, &QArchive::Extractor::finished,
        [&]() {
            TempFile->remove();
            TempFile = NULL; // Dereference
            emit updatesInstalled();
            return;
        });
        Archiver.addArchive(TempFile->fileName());
        Archiver.setDestination(this->installationPath);
        Archiver.start();
        return;
    }

    void AbortDownload()
    {
        DownloadManager.Pause();
        TempFile->remove();
        TempFile = NULL;
        emit DownloadAborted();
        return;
    }

    void AbortInstallation()
    {
        if(Archiver.isRunning()) {
            Archiver.requestInterruption();
            Archiver.wait();
        }
        TempFile->remove();
        TempFile = NULL;
        Updates.clear();
        emit InstallationAborted();
        return;
    }

signals:
    void error(short, const QString&);
    void updatesLatest(const QStringList&);
    void updatesDownloadProgress(qint64 bytesReceived,
                                 qint64 bytesTotal,
                                 int percent,
                                 double speed,
                                 const QString &unit,
                                 const QUrl &url,
                                 const QString &fileName);
    void updatesInstalling(const QString&);
    void updatesDownloaded();
    void updatesInstalled();
    void DownloadAborted();
    void InstallationAborted();

private:
    QString username,
            repo,
            version,
            assetFormat,
            installationPath;
    bool debug = false;
    QTemporaryFile *TempFile = NULL;
    QStringList Updates;
    QArchive::Extractor Archiver;
    QEasyDownloader DownloadManager;
}; // Class GHReleaseBridge Ends
#endif // GH_RELEASE_BRIDGE_HPP_INCLUDED
