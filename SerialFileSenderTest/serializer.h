#ifndef SERIALIZER_H
#define SERIALIZER_H
#include <QObject>
#include <QList>
#include <QByteArray>
#include <QThread>
#include <QFile>

class SmartBuffer;
/**
 * @brief The Serializer class
 * Class of serialization interesting file
 * into bytestream and pack it into bursts.
 */
class Serializer : public QObject
{
    Q_OBJECT
public:
    explicit Serializer(QObject *parent = nullptr);
    ~Serializer();
    void setSmartBuffer(SmartBuffer* _buffer);

public slots:
    void burstLoop();
    void setPathToSendedFileSlot(QString _pathTofile);
    void closeFileWithPort();

signals:
    void errorOpenFileString(QString _error);
    void sizeOfFileSignal(int _size);

private:
    QByteArray serializedFile;
    SmartBuffer* buffer = nullptr;
    QFile file;
    int sizeOfBurst;
};

#endif // PARSER_H
