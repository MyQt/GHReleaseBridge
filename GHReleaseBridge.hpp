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

class GHReleaseBridge : public QObject
{
    Q_OBJECT
public:

    /*
     * error codes
    */
    enum {
        NETWORK_ERROR = -1,
        JSON_PARSE_ERROR = -2
    };

    explicit GHReleaseBridge(QObject *p = NULL)
        : QObject(p)
    {
        connectDefaultErrorSlot();
    }

    explicit GHReleaseBridge(
        const QString &username,
        const QString &repo,
        const QString &version,
        const QString &assetFormat,
        const QString &installationPath,
        bool debug
    )
        : QObject(NULL)
    {
        connectDefaultErrorSlot();
        setConfiguration(username, repo, version, assetFormat, installationPath, debug);
    }


    explicit GHReleaseBridge(const QJsonDocument& configuration)
        : QObject(NULL)
    {
        connectDefaultErrorSlot();
        setConfiguration(configuration);
    }


    explicit GHReleaseBridge(const QJsonObject& configuration)
        : QObject(NULL)
    {
        connectDefaultErrorSlot();
        setConfiguration(QJsonDocument(configuration));
    }

    void setConfiguration(const QJsonDocument& configuration)
    {

        this->configuration = configuration;
        auto root = configuration.object();
        if(
            root["username"].isString() &&
            root["repo"].isString() &&
            root["version"].isString() &&
            root["assetFormat"].isString() &&
            root["installationPath"].isString() &&
            root["debug"].isBool()
        ) {
            debug = root["debug"].toBool();
            showConfiguration();
        } else {
            emit error(JSON_PARSE_ERROR, "some error in the given json document.");
        }
        return;
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
        QJsonObject ConfigObject {
            {"username", username},
            {"repo", repo},
            {"version", version},
            {"assetFormat", assetFormat},
            {"installationPath", installationPath},
            {"debug", debug}
        };
        setConfiguration(QJsonDocument(ConfigObject));
        return;
    }

    void showConfiguration()
    {
        if(debug) {
            auto root = configuration.object();
            qDebug() << "**** Configuration ****";
            qDebug() << "Username/Organization:: " << ((root["username"].toString().isEmpty()) ? "Empty!" : root["username"].toString());
            qDebug() << "Repo                 :: " << ((root["repo"].toString().isEmpty()) ? "Empty!" : root["repo"].toString());
            qDebug() << "Version/Tag	      :: " << ((root["version"].toString().isEmpty()) ? "Empty!" : root["version"].toString());
            qDebug() << "Installation Path    :: " << (
                         (
                             root["installationPath"].toString().isEmpty()) ? "Empty!"
                         :
                         root["installationPath"].toString()
                     );
            qDebug() << "Debug                :: True";
            qDebug() << "***********************";
        }
        return;
    }

    ~GHReleaseBridge() { }

private slots:

    void CheckGitHubReleases(const QString &content)
    {
        auto root = configuration.object();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(content.toUtf8());
        QJsonObject jsonObject = jsonResponse.object();
        QJsonArray assetsArray = jsonObject["assets"].toArray();
        QString version = jsonObject["tag_name"].toString();
        QString toDownload(root["assetFormat"].toString());
        toDownload.replace("{version}", version);
        QString downloadLink;
        QVector<QJsonObject> assets;

        if(debug) {
            qDebug() << "GHReleaseBridge::Latest Version:: " << version;
            qDebug() << "GHReleaeeBrdige::Asset Required:: " << toDownload;
        }

        if(root["version"].toString() == version) {
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
        QJsonObject UpdateObject {
            {"username", root["username"]},
            {"repo", root["repo"]},
            {"version", version},
            {"downloadLink", downloadLink}
        };
        Updates = QJsonDocument(UpdateObject);

        disconnect(&DownloadManager, SIGNAL(GetResponse(const QString&)),
                   this,SLOT(CheckGitHubReleases(const QString&)));

        emit updatesLatest(Updates);
        return;
    }

    void connectDefaultErrorSlot()
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

        return;
    }

    bool isEmptyConfiguration()
    {
        auto root = configuration.object();
        return (
                   (
                       root["username"].toString().isEmpty() ||
                       root["repo"].toString().isEmpty() ||
                       root["version"].toString().isEmpty() ||
                       root["assetFormat"].toString().isEmpty() ||
                       root["installationPath"].toString().isEmpty()
                   ) &&
                   (
                       root["username"].isNull() ||
                       root["repo"].isNull() ||
                       root["version"].isNull() ||
                       root["assetFormat"].isNull() ||
                       root["installationPath"].isNull() ||
                       root["debug"].isNull()
                   )
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
            qDebug() << "GHReleaseBridge::Empty Configuration given.";
            return;
        }

        // Set the latest update information to cache
        if(!Updates.isNull()) {
            setConfiguration(Updates);
        }

        // Get Our Configuration.
        auto root = configuration.object();

        /*
         * Build the URL.
        */
        QUrl latestRelease(
            QString("https://api.github.com/repos/") +
            root["username"].toString() +
            QString("/") +
            root["repo"].toString() +
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
        if(Updates.isNull() || TempFile != NULL) {
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

        auto root = Updates.object();
        DownloadManager.Download(root["downloadLink"].toString(), TempFile->fileName());
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

            // Set new version
            auto root = configuration.object();
            auto rootU = Updates.object();
            QJsonObject newConfig {
                {"username" , root["username"]},
                {"repo"     , root["repo"]},
                {"version"  , rootU["version"]},
                {"installationPath" , root["installationPath"]},
                {"assetFormat" , root["assetFormat"]},
                {"debug"    , root["debug"]}
            };
            
            configuration = QJsonDocument(newConfig);
            
            // emit signals
            emit updatesInstalled();
            emit newConfiguration(configuration);
            return;
        });

        auto root = configuration.object();
        Archiver.addArchive(TempFile->fileName());
        Archiver.setDestination(root["installationPath"].toString());
        Archiver.start();
        return;
    }

    void AbortDownload()
    {
        if(TempFile == NULL) {
            return;
        }
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
        } else {
            return;
        }
        TempFile->remove();
        TempFile = NULL;
        Updates = QJsonDocument();
        emit InstallationAborted();
        return;
    }

signals:
    void error(short, const QString&);
    void updatesLatest(const QJsonDocument&);
    void updatesDownloadProgress(qint64 bytesReceived,
                                 qint64 bytesTotal,
                                 int percent,
                                 double speed,
                                 const QString &unit,
                                 const QUrl &url,
                                 const QString &fileName);
    void updatesInstalling(const QString&);
    void newConfiguration(const QJsonDocument& newConfig);

    void updatesDownloaded();
    void updatesInstalled();
    void DownloadAborted();
    void InstallationAborted();

private:
    /*
     * This is the main configuration which
     * will hold all required data and the
     * new configuration.
    */
    QJsonDocument configuration;
    QJsonDocument Updates;
    QTemporaryFile *TempFile = NULL;
    bool debug = false;
    QArchive::Extractor Archiver;
    QEasyDownloader DownloadManager;
}; // Class GHReleaseBridge Ends
#endif // GH_RELEASE_BRIDGE_HPP_INCLUDED
