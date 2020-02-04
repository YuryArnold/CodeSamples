#ifndef SMARTBUFFER_H
#define SMARTBUFFER_H

#include <QObject>
#include <atomic>
#include <QMutex>

/**
 * @brief The SmartBuffer class
 * class of smart buffer,
 * control signal slot for cv
 * analogue technique for reading
 * and writing.
 */
class SmartBuffer : public QObject
{
    Q_OBJECT
public:
    explicit SmartBuffer(QObject *parent = nullptr);
    ~SmartBuffer();
    bool canRead();
    bool canWrite();
    bool isEmpty();
    /*Pair of this two methods
     * realize analogue a pair
     * of condition variables to
     * work with dynamically
     * alocated buffer*/
    void getBurst(QByteArray&);
    void setBurst(QByteArray&);
    void setBufferSize(int _size = 100);
    void reset();
    void setSizeOfFile(int _sizeOfFile);
    int getSizeOfFile();
    void setSizeOfBurst(int _sizeOfBurst);
    int getSizeOfBurst();

signals:
    void startRead();
    void startWrite();

private:
    QList<QByteArray> messages;
    std::atomic<int> numbOfUsedBursts;
    std::atomic<int> sizeOfFile;
    QMutex mtx;
    int bufferSize;
    int sizeOfBurst;
};

#endif // SMARTBUFFER_H
