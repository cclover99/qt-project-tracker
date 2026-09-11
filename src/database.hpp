#pragma once

#include <QString>
#include <QVector>

struct Project {
    int id;
    QString name;
    QString createdAt;
};

class Database {
public:
    static bool initialize();

    static QVector<Project> getProjects();
    static Project createProject(const QString& name);
    static bool renameProject(int id, const QString& name);
    static bool deleteProject(int id);
};