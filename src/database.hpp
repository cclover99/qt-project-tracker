#pragma once

#include <QString>
#include <QVector>

struct Project {
    int id;
    QString name;
    QString description;
    QString createdAt;
};

class Database {
public:
    static bool initialize();

    static QVector<Project> getProjects();
    static Project createProject();
    static bool renameProject(int id, const QString& name);
    static bool updateDescription(int id, const QString& description);
    static bool deleteProject(int id);
};