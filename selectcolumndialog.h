#ifndef SELECTCOLUMNDIALOG_H
#define SELECTCOLUMNDIALOG_H

#include <QDialog>

namespace Ui {
class SelectColumnDialog;
}

class SelectColumnDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectColumnDialog(QWidget *parent = nullptr);
    ~SelectColumnDialog();

private:
    Ui::SelectColumnDialog *ui;
};

#endif // SELECTCOLUMNDIALOG_H
