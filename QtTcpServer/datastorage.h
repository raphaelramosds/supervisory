#ifndef DATASTORAGE_H
#define DATASTORAGE_H
#include <QDateTime>
#include <QHostAddress>
#include <QMutex>
#include <QMap>
#include <vector>

using namespace std;

typedef struct{
    qint64 theTime;
    float measurement;
} Entry;

class DataStorage{

public:

    DataStorage();

    /**
    * @brief getData fetch stored data of a host
    * @param address the host's IP
    * @param lastn how many data the client wants to fetch
    */
    vector<Entry> getData(QHostAddress address, unsigned int lastn=2);

    void addData(QHostAddress address, qint64 time, float measurement);
    void deleteHost(quint32 address);
    vector<QHostAddress> getHostList();

private:
    QMutex mutex;
    /**
    * @brief data stores a map
    * @details quint32 for host ip, vector<Entry> for time/data
    */
    QMap<quint32, vector<Entry>> data;
    QMap<quint32, vector<Entry>>::iterator dataIterator;
    Q_DISABLE_COPY(DataStorage)
};

#endif // DATASTORAGE_H
