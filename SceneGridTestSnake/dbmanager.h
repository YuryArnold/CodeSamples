#ifndef DBMANAGER_H
#define DBMANAGER_H
#include <QObject>
#include <QtSql>
#include <QSqlQuery>
#

/**
 * @brief The DbManager class
 * This class have to implement mvc
 * architecture. But for now I don't
 * do it. Because of mvc I have to rework
 * Architecture of application. Add new classes,
 * but it takes more time.
 * Good architecture desigions it's better
 * to expand application. For simplicity
 * I used blob approch.
 */

/** WARNING
 *  Logick fo work with db manager are not accomplised,
 *  This application was send besause of very shot shot limit
 *  of time during my work. I can accomplish it with a time.
 * */




class DbManager : public QObject
{
    Q_OBJECT
public:
    DbManager(QObject* parent = nullptr);
    ~DbManager();
    bool isEmpty();
    void writeDataToDb(QVector<QVector<bool>>& _state);
    QVector<QVector<bool>> readDataFromDb();

private:
    bool createTable();
    QSqlDatabase m_db;
    bool wasEmptyFlag;


};

#endif // DBMANAGER_H
