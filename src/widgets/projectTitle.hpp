#pragma once

#include <QLineEdit>

class ProjectTitle : public QLineEdit {
Q_OBJECT

public:
    explicit ProjectTitle(
        const QString& text,
        QWidget* parent = nullptr
    );

    void setTitle(const QString& text);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

signals:
    void renameRequested(
        const QString& oldName,
        const QString& newName
    );

protected:
    void focusInEvent(QFocusEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    QString originalText;
    bool cancelEdit = false;
};