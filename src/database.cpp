#include "database.hpp"

#include <QDir>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QDebug>

bool Database::initialize() {
    QString dataDir = QStandardPaths::writableLocation(
        QStandardPaths::AppDataLocation
    );

    QDir().mkpath(dataDir);

    QString databasePath = dataDir + "/projects.db";

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(databasePath);

    if (!db.open()) {
        qCritical() << "Failed to open database:"
                    << db.lastError().text();
        return false;
    }

    QSqlQuery query;

    if (!query.exec(R"(
        CREATE TABLE IF NOT EXISTS projects (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            created_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP
        )
    )")) {
        qCritical() << "Failed to create projects table:"
                    << query.lastError().text();
        return false;
    }

    return true;
}

QVector<Project> Database::getProjects() {
    QVector<Project> projects;

    QSqlQuery query(
        "SELECT id, name, created_at "
        "FROM projects "
        "ORDER BY created_at"
    );

    while (query.next()) {
        projects.append({
            query.value(0).toInt(),
            query.value(1).toString(),
            query.value(2).toString()
        });
    }

    return projects;
}

Project Database::createProject(const QString& name) {
    QSqlQuery query;

    query.prepare(
        "INSERT INTO projects (name) VALUES (?)"
    );

    query.addBindValue(name);

    if (!query.exec()) {
        qCritical() << query.lastError().text();
        return {};
    }

    int id = query.lastInsertId().toInt();

    QSqlQuery projectQuery;
    projectQuery.prepare(
        "SELECT id, name, created_at "
        "FROM projects WHERE id = ?"
    );

    projectQuery.addBindValue(id);
    projectQuery.exec();
    projectQuery.next();

    return {
        projectQuery.value(0).toInt(),
        projectQuery.value(1).toString(),
        projectQuery.value(2).toString()
    };
}

bool Database::deleteProject(int id) {
    QSqlQuery query;

    query.prepare("DELETE FROM projects WHERE id = ?");
    query.addBindValue(id);

    if (!query.exec()) {
        qCritical() << "Failed to delete project:"
                    << query.lastError().text();
        return false;
    }

    return true;
}

bool Database::renameProject(int id, const QString& name) {
    QSqlQuery query;

    query.prepare("UPDATE projects SET name = ? WHERE id = ?");
    query.addBindValue(name);
    query.addBindValue(id);

    if (!query.exec()) {
        qCritical() << "Failed to rename project:"
                    << query.lastError().text();
        return false;
    }

    return true;
}