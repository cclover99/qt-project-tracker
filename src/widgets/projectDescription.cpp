#include "projectDescription.hpp"

#include <QFocusEvent>
#include <QKeyEvent>
#include <QTextBlock>
#include <QAbstractTextDocumentLayout>
#include <QFontMetricsF>
#include <QtMath>


ProjectDescription::ProjectDescription(const QString& text, QWidget* parent): QPlainTextEdit(text, parent), originalText(text) {
    // setFrame(false);

    setStyleSheet(R"(
        QPlainTextEdit {            
            border: none;
            border-radius 4px;
            background-color: rgba(255, 255, 255, 10);
        }        

        QPlainTextEdit {
            border-radius: 4px;
        }
    )");

    setPlaceholderText("Add description...");

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    connect(this, &QPlainTextEdit::textChanged, this, [this]() {
        updateGeometry();
    });
}

void ProjectDescription::setDescription(const QString& text) {
    originalText = text;
    setPlainText(text);
}

QSize ProjectDescription::sizeHint() const {
    auto* layout = document()->documentLayout();
    const qreal margin = document()->documentMargin();

    qreal contentHeight = margin;

    for (QTextBlock block = document()->begin();
         block.isValid();
         block = block.next()) {
        contentHeight += layout->blockBoundingRect(block).height();
    }

    const qreal lineHeight = qCeil(QFontMetricsF(document()->defaultFont()).height());
    const qreal minimumHeight = 3 * lineHeight + 2 * margin;

    return {
    QPlainTextEdit::sizeHint().width(),
    qCeil(qMax(contentHeight, minimumHeight)) + 2 * frameWidth() + 1
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