/*
 * Copyright (c) 2011 Nokia Corporation.
 */

#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QList>
#include "datatypes.h"

class QSqlDatabase;

using namespace AS;

/*!
  \class DatabaseManager
  \brief Finds target shapes from the SQLite database
*/
class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    /*!
      Constructor
    */
    explicit DatabaseManager(QObject *parent = 0);
    ~DatabaseManager();

    /*!
      Opens database
    */
    bool openDB(const QString &name);

    /*!
      Gets error from database
    */
    QString lastError() const;

    /*!
      Count of different shapes in the database
    */
    int shapeCount();

    /*!
      Gets next shape
    */
    Shape nextShape();

    /*!
      Gets shape
    */
    Shape shape(int shapeId);

protected:
    int firstShapeId();
    int orderOfShape();

    QSqlDatabase *m_DB;
    int m_CurrentShapeId;
};

#endif // DATABASEMANAGER_H
