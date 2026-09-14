#pragma once

#include <QPlainTextEdit>

class ProjectDescription : public QPlainTextEdit {
Q_OBJECT

public:
    explicit ProjectDescription(
        const QString& text,
        QWidget* parent = nullptr
    );

    void setDescription(const QString& text);

    QSize sizeHint() const override;

signals:
    void descriptionChanged(
        const QString& oldDescription,
        const QString& newDescription
    );

protected:
    void focusInEvent(QFocusEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    QString originalText;
};