#include "dbmanager.h"

DbManager::DbManager(QObject *parent) : QObject (parent)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("maze.db");
}

DbManager::~DbManager()
{
    if (m_db.isOpen()){
        m_db.close();
    }
}

bool DbManager::isEmpty()
{
    return wasEmptyFlag;
}

void DbManager::writeDataToDb(QVector<QVector<bool> > &_state)
{
    QByteArray dataBlob;
    _state.data();
}

QVector<QVector<bool> > DbManager::readDataFromDb()
{

}

bool DbManager::createTable()
{
    QSqlQuery qry;
    qry.prepare( "CREATE TABLE IF NOT EXISTS maze "
    "(id INTEGER PRIMARY KEY, data BLOB)" );
    if( !qry.exec() )
        return false;
    return true;
}
