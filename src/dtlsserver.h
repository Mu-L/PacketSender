
// Copyright (C) 2018 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause
#ifndef SERVER_H
#define SERVER_H

#include <QtCore>
#include <QtNetwork>

#include <vector>
#include <memory>
#include "packet.h"

//! [0]
class DtlsServer : public QObject
{
    Q_OBJECT

public:
    static bool closeNotifyReceived;  // Initialize static member


    QSslCertificate certificate;
    QSslKey privateKey;
    QSslCertificate caCertificate;

    QSslConfiguration serverConfiguration;
    DtlsServer();
    ~DtlsServer();



    bool listen(const QHostAddress &address, quint16 port);
    bool isListening() const;
    void close();
    Packet createPacket(const std::vector<QString>& packetInfo, const QByteArray& dgram);
    std::vector<QString> createInfoVect(const QHostAddress &fromAddress, quint16 fromPort, const QHostAddress &toAddress, quint16 toPort);
    bool serverResonse(QDtls* dtlsServer);
    void loadKeyLocalCertCaCert();
    void setConfiguration();
    QSsl::EncodingFormat getCertFormat(QFile& certFile);
    QSslKey getPrivateKey(QFile& keyFile);
    QString getFullPathToCaCert();


    QString getIPmode();
    bool IPv4Enabled();
    bool IPv6Enabled();
    QHostAddress resolveDNS(QString hostname);







    //bool closeNotifyRecieved;
    QUdpSocket serverSocket;
    QByteArray smartData;


signals:
    void serverPacketReceived(Packet);
    void serverPacketSent(Packet);


    void errorMessage(const QString &message);
    void warningMessage(const QString &message);
    void infoMessage(const QString &message);

    //void serverDatagramReceived(const QString &peerInfo, const QByteArray &cipherText, const QByteArray &plainText);

private slots:
    void readyRead();
    void pskRequired(QSslPreSharedKeyAuthenticator *auth);

public slots:
    void on_signedCert_textChanged();
    //void serverReceivedDatagram(const QString& peerInfo, const QByteArray &clientMessage, const QByteArray& dgram);

private:
    void handleNewConnection(const QHostAddress &peerAddress, quint16 peerPort,
                             const QByteArray &clientHello);

    void doHandshake(QDtls *newConnection, const QByteArray &clientHello);
    void sendAck(QDtls *connection, const QByteArray &clientMessage);
    void shutdown();

    bool listening = false;

    QDtlsClientVerifier cookieSender;
    std::vector<std::unique_ptr<QDtls>> knownClients;


    Q_DISABLE_COPY(DtlsServer)
};
//! [0]

#endif // SERVER_H

