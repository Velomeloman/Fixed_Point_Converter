#ifndef FLINEEDIT_H
#define FLINEEDIT_H

#include <QLineEdit>

class fLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    fLineEdit(QWidget *parent = 0);
    ~fLineEdit();
    void addCopyButton();

signals:
    void focussed(bool hasFocus);

protected:
    virtual void focusInEvent(QFocusEvent *e);
    virtual void focusOutEvent(QFocusEvent *e);

private slots:
    void onCopyTriggered();
};

#endif // FLINEEDIT_H
