#include <QCoreApplication>
#include <QProcess>
#include <QFile>
#include <QHostAddress>
#include <QHostInfo>
#include <time.h>
#include "SServer.h"

int getRandomPort()
{
    return rand() % 55000 + 10000;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    srand (time(NULL));
    SServer* server = new SServer();

    int portNumber;
    int trialCount = 10, n=0;
    bool ok = false;

    QString localhostIP;
    QString localhostname =  QHostInfo::localHostName();
    QList<QHostAddress> hostList = QHostInfo::fromName(localhostname).addresses();
    foreach (const QHostAddress& address, hostList) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address.isLoopback() == false) {
            localhostIP = address.toString();
        }
    }

    qDebug() << "Local IP: " << localhostIP << "\n";


    QString path = "";
    QString program = "upnpc-shared.exe";


    while (!ok && (n++ < trialCount))
    {
        portNumber = getRandomPort();

        QProcess *myProcess = new QProcess();

        QStringList args;
        args << "-d";
        args << QString::number(portNumber);
        args << "tcp";

        myProcess->start(path+program, args);
        bool isOk = myProcess->waitForStarted();
        isOk = myProcess->waitForFinished();

        qDebug() << myProcess->readAllStandardOutput() << "\n";
        if(!isOk) qDebug() << "   Errors!!!  " << myProcess->readAllStandardError();


        args.clear();
        myProcess = new QProcess();

        args << "-a";
        args << localhostIP;
        args << QString::number(portNumber);
        args << QString::number(portNumber);
        args << "tcp";

        myProcess->start(path+program, args);
        isOk = myProcess->waitForStarted();
        isOk = myProcess->waitForFinished();

        qDebug() << myProcess->readAllStandardOutput() << "\n";
        if(!isOk) qDebug() << "   Errors!!!  " << myProcess->readAllStandardError();

        auto result = myProcess->readAllStandardOutput();
        myProcess->deleteLater();

        server->port = portNumber;
        ok = server->startServer();
    }

    return a.exec();
}
