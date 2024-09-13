#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#define version_string "v2.0"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void getParams();
    int range_min;
    int range_max;
    unsigned int size;
    unsigned int scale;
    unsigned int fractLenght;
    bool isDecActive;
    bool isSigned;
    bool isScaleAuto;
    void setDecActive();
    void setDecInactive();

private slots:
    void initGUI();
    void SetRangeMode();
    void checkRange();
    void getFixedPoint();
    void getNumberRepr(int editNumber);
    void on_checkBox_RangeAuto_stateChanged();
    void on_lineEdit_minRange_textChanged(const QString &arg1);
    void on_lineEdit_maxRange_textChanged();
    void on_comboBox_size_currentTextChanged(const QString &arg1);
    void on_lineEdit_InputValue_Scaled_textEdited(const QString &arg1);
    void on_lineEdit_InputValue_Hex_textEdited(const QString &arg1);
    void on_lineEdit_InputValue_Bin_textEdited(const QString &arg1);
    void on_lineEdit_InputValue_Dec_textEdited(const QString &arg1);      
    void on_spinBoxInputScale_valueChanged();
    void on_lineEdit_InputValue_Dec_textChanged();
    void on_lineEdit_InputValue_Dec_focussed();
    void on_lineEdit_InputValue_Scaled_focussed();
    void on_pushButton_clicked();
    void on_checkBox_ScaleAuto_stateChanged(int arg1);
    void on_checkBox_signed_stateChanged(int arg1);
};

#endif // MAINWINDOW_H
