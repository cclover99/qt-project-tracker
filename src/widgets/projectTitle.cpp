#include "projectTitle.hpp"

#include <QFocusEvent>
#include <QKeyEvent>


ProjectTitle::ProjectTitle(const QString& text, QWidget* parent): QLineEdit(text, parent), originalText(text) {
    setFrame(false);

    setStyleSheet(R"(
        QLineEdit {
            padding: 2px 2px;

            background: transparent;
            border: none;
            margin-top: 1px;
        }

        QLineEdit:focus {
            background-color: rgba(255, 255, 255, 20);
            border-radius: 4px;
        }
        
    )");

    QFont titleFont = font();
    titleFont.setPointSize(12);
    titleFont.setWeight(QFont::DemiBold);
    setFont(titleFont);

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    connect(this, &QLineEdit::textChanged, this, [this]() {
        updateGeometry();
    });
}

void ProjectTitle::setTitle(const QString& text) {
    originalText = text;
    setText(text);
}

QSize ProjectTitle::sizeHint() const {
    QFontMetrics metrics(font());

    return {
        metrics.horizontalAdvance(text()) + 5 + 4,
        QLineEdit::sizeHint().height()
    };
}

void ProjectTitle::focusInEvent(QFocusEvent* event) {
    originalText = text();
    cancelEdit = false;

    QLineEdit::focusInEvent(event);
}

void ProjectTitle::focusOutEvent(QFocusEvent* event) {
    QLineEdit::focusOutEvent(event);

    if (cancelEdit) {
        setText(originalText);
        cancelEdit = false;
        return;
    }

    QString newName = text().trimmed();

    if (newName.isEmpty()) {
        setText(originalText);
        return;
    }

    setText(newName);

    if (newName == originalText)
        return;

    QString oldName = originalText;
    originalText = newName;

    emit renameRequested(oldName, newName);
}

void ProjectTitle::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape) {
        cancelEdit = true;
        clearFocus();
        return;
    }

    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        clearFocus();
        return;
    }

    QLineEdit::keyPressEvent(event);
}