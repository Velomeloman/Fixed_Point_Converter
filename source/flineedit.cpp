#include "flineedit.h"
#include <QIcon>
#include <QAction>
#include <QClipboard>
#include <QApplication>

fLineEdit::fLineEdit(QWidget *parent) : QLineEdit(parent)
{}

fLineEdit::~fLineEdit()
{}

void fLineEdit::addCopyButton()
{
    QAction *copyAction = this->addAction(QIcon("source/icon/copy.png"), QLineEdit::TrailingPosition);
    connect(copyAction, &QAction::triggered, this, &fLineEdit::onCopyTriggered);
}

void fLineEdit::focusInEvent(QFocusEvent *e)
{
    QLineEdit::focusInEvent(e);
    emit(focussed(true));
}

void fLineEdit::focusOutEvent(QFocusEvent *e)
{
    QLineEdit::focusOutEvent(e);
    emit(focussed(false));
}

void fLineEdit::onCopyTriggered()
{
    if (QClipboard *c = QApplication::clipboard())
        c->setText(this->text());
}
