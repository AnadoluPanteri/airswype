/*
 * Copyright (c) 2011-2014 Microsoft Mobile.
 */

#include <QtSql>
#include "databasemanager.h"

DatabaseManager::DatabaseManager(QObject *parent) :
    QObject(parent),
    m_CurrentShapeId(0)
{
    m_DB = new QSqlDatabase;
    *m_DB = QSqlDatabase::addDatabase("QSQLITE");
}


DatabaseManager::~DatabaseManager()
{
    if(m_DB) {
        if(m_DB->isOpen()) {
            m_DB->close();
        }
        delete m_DB;
        m_DB = NULL;
    }
}


QString DatabaseManager::lastError() const
{
    return m_DB->lastError().text();
}


bool DatabaseManager::openDB(const QString &name)
{
    m_DB->setDatabaseName(name);
    return m_DB->open();
}


int DatabaseManager::shapeCount()
{
    int shapeCount = 0;

    QSqlQuery query("select count(shape_id) from shape");

    if(query.next()) {
        shapeCount = query.value(0).toInt();
    }

    return shapeCount;
}


Shape DatabaseManager::shape(int shapeId)
{
    Shape shape;

    QString sql = QString("SELECT "
                                "s.level, "
                                "s.time_to_show, "
                                "c.x, "
                                "c.y, "
                                "c.z "
                          "FROM "
                                "shape s, "
                                "coordinate c "
                          "WHERE "
                                "s.shape_id = c.shape_id AND "
                                "s.shape_id = %1 "
                          "ORDER BY "
                                "c.coordinate_id").arg(shapeId);

    QSqlQuery query(sql);
    while (query.next()) {
        shape.level = query.value(0).toInt();
        shape.timeToShow = query.value(1).toReal();

        Coordinate m;
        m.pos[0] = query.value(2).toReal();
        m.pos[1] = query.value(3).toReal();
        m.pos[2] = query.value(4).toReal();

        shape.coordinates.push_back(m);
    }

    return shape;
}


int DatabaseManager::firstShapeId()
{
    QSqlQuery query("SELECT "
                        "shape_id "
                    "FROM "
                        "shape "
                    "ORDER BY "
                        "[order], "
                        "shape_id");

    if(query.next()) {
        return query.value(0).toInt();
    }

    return 0;
}

int DatabaseManager::orderOfShape()
{
    int currentOrder = -1;
    QString sql = QString("SELECT "
                                "[order]"
                          "FROM "
                                "shape "
                          "WHERE "
                                "shape_id = %1").arg(m_CurrentShapeId);

    QSqlQuery query(sql);
    if(query.next()) {
        currentOrder = query.value(0).toInt();
    }

    return currentOrder;
}


Shape DatabaseManager::nextShape()
{
    // Get the order of the current shape
    int currentOrder = orderOfShape();

    QString sql = QString("SELECT "
                                "shape_id "
                          "FROM "
                                "shape "
                          "WHERE "
                                "shape_id <> %1 AND "
                                "[order] >= %2 "
                          "ORDER BY "
                                "[order], "
                                "shape_id").arg(m_CurrentShapeId).arg(currentOrder);

    QSqlQuery query(sql);

    if(query.next()) {
        // Gets the next shape id from DB
        m_CurrentShapeId = query.value(0).toInt();
    }
    else {
        QString s = query.lastError().text();
        // All of the shapes has already gone through, get the first one again
        m_CurrentShapeId = firstShapeId();
    }

    return shape(m_CurrentShapeId);
}
