#include "projectDescription.hpp"

#include <QFocusEvent>
#include <QKeyEvent>


ProjectDescription::ProjectDescription(const QString& text, QWidget* parent): QPlainTextEdit(text, parent), originalText(text) {
    // setFrame(false);

    setStyleSheet(R"(
        QPlainTextEdit {
            background-color: rgba(255, 255, 255, 20);
            border-radius: 4px;
            border: none;
        }

        QPlainTextEdit:focus {
            border: none;
        }
        
    )");

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    setPlaceholderText("Add description...");

    connect(this, &QPlainTextEdit::textChanged, this, [this]() {
        updateGeometry();
    });
}

void ProjectDescription::setDescription(const QString& text) {
    originalText = text;
    setPlainText(text);
}

QSize ProjectDescription::sizeHint() const {
    QFontMetrics metrics(font());

    return {
        metrics.horizontalAdvance(toPlainText()) + 5 + 4,
        QPlainTextEdit::sizeHint().height()
    };
}

void ProjectDescription::focusInEvent(QFocusEvent* event) {
    originalText = toPlainText();

    QPlainTextEdit::focusInEvent(event);
}

void ProjectDescription::focusOutEvent(QFocusEvent* event) {
    QPlainTextEdit::focusOutEvent(event);

    QString newDescription = toPlainText().trimmed();

    setPlainText(newDescription);

    if (newDescription == originalText)
        return;

    QString oldDescription = originalText;
    originalText = newDescription;

    emit descriptionChanged(oldDescription, newDescription);
}

void ProjectDescription::keyPressEvent(QKeyEvent* event) {

    if (event->key() == Qt::Key_Escape) {
        clearFocus();
        return;
    }

    QPlainTextEdit::keyPressEvent(event);
}