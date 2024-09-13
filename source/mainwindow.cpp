#include <QtMath>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fixed_point_calculations.h"
#include "number_repr.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initGUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getParams()
{
    range_min = ui->lineEdit_minRange->text().toInt();
    range_max = ui->lineEdit_maxRange->text().toInt();
    size = ui->comboBox_size->currentText().toUInt();
    scale = ui->spinBoxInputScale->text().toInt();
}

void MainWindow::setDecActive()
{
    ui->lineEdit_InputValue_Dec->setStyleSheet("QLineEdit { background-color: rgb(150, 200, 227);}");
    ui->lineEdit_InputValue_Scaled->setStyleSheet("QLineEdit { background-color: rgb(255, 255, 255);}");
    isDecActive = true;
}

void MainWindow::setDecInactive()
{
    ui->lineEdit_InputValue_Dec->setStyleSheet("QLineEdit { background-color: rgb(255, 255, 255);}");
    ui->lineEdit_InputValue_Scaled->setStyleSheet("QLineEdit { background-color: rgb(150, 200, 227);}");
    isDecActive = false;
}

// ---------- GUI events ----------
void MainWindow::on_checkBox_RangeAuto_stateChanged()
{
    SetRangeMode();
}

void MainWindow::on_checkBox_signed_stateChanged(int arg1)
{
    isSigned = arg1;

    QString hexValue = ui->lineEdit_InputValue_Hex->text();
    double scaled = 0.0;
    if(isSigned)
    {
        switch (size) {
        case 8:  scaled = static_cast<int8_t> (hexValue.toInt(NULL, 16)); break;
        case 16: scaled = static_cast<int16_t>(hexValue.toInt(NULL, 16)); break;
        case 32: scaled = static_cast<int32_t>(hexValue.toLongLong(NULL, 16)); break;
        case 64: scaled = static_cast<int64_t>(hexValue.toULongLong(NULL, 16)); break;
        }
    }
    else
    {
        switch (size) {
        case 8:  scaled = static_cast<uint8_t> (hexValue.toInt(NULL, 16)); break;
        case 16: scaled = static_cast<uint16_t>(hexValue.toInt(NULL, 16)); break;
        case 32: scaled = static_cast<uint32_t>(hexValue.toLongLong(NULL, 16)); break;
        case 64: scaled = static_cast<uint64_t>(hexValue.toULongLong(NULL, 16)); break;
        }
    }

    if(!isDecActive)
    {
        ui->lineEdit_InputValue_Scaled->setText(QString::number(scaled));
        num_repr.Scaled = scaled;
    }

    if(ui->checkBox_RangeAuto->checkState())
        SetRangeMode();
    checkRange();
    getFixedPoint();


    getParams();
    getNumberRepr(0);
}
void MainWindow::on_lineEdit_minRange_textChanged(const QString &arg1)
{
    if(!ui->checkBox_RangeAuto->checkState()){
        // manual range
        if(arg1.toDouble() > 0){
            ui->lineEdit_minRange->setText("0");
        }
        checkRange();
    }
    getFixedPoint();
}

void MainWindow::on_lineEdit_maxRange_textChanged()
{
    if(!ui->checkBox_RangeAuto->checkState())
        checkRange();
    getFixedPoint();
}

/// Size changed
void MainWindow::on_comboBox_size_currentTextChanged(const QString &arg1)
{
    size = arg1.toUInt();
    if(size > 64)
    {
        size = 64;
        ui->comboBox_size->setCurrentText("64");
    }
    if(ui->spinBoxInputScale->text().toUInt() > size - ui->checkBox_signed->isChecked())
    {
        ui->spinBoxInputScale->setValue(size - ui->checkBox_signed->isChecked());
        ui->lineEdit_InputValue_Hex->setText("");
    }

    // Update Hex and Bin input validator
    QRegExp hex_onlyNbit(QString("[0-9a-fA-F]{0,%1}$+").arg(int(size/4)));
    QRegExp binary_only(QString("[+]?[0-1]{0,%1}$+").arg(size));
    ui->lineEdit_InputValue_Hex->setValidator(new QRegExpValidator(hex_onlyNbit, this));
    ui->lineEdit_InputValue_Bin->setValidator(new QRegExpValidator(binary_only, this));

    getFixedPoint();
    getParams();
    getNumberRepr(0);

}
// --------------------------------

// ---------- GUI update functions ----------
void MainWindow::initGUI(){
    QString windowTitle = "Fixed Point Converter ";
    //windowTitle += version_string;
    this->setWindowTitle(windowTitle);
    size = 32;
    isScaleAuto = ui->checkBox_ScaleAuto->isChecked();
    isSigned = ui->checkBox_signed->isChecked();

    QRegExp binary_only(QString("[+]?[0-1]{0,%1}$+").arg(size));
    QRegExp hex_only("[0-9a-fA-F]+");
    QRegExp hex_onlyNbit(QString("[0-9a-fA-F]{0,%1}$+").arg(int(size/4)));
    QRegExp numbers_only("[+-]?([0-9]*[.])?[0-9]+");
    QRegExp pos_numbers_only("[+]?([0-9]*[.])?[0-9]+");
    QRegExp pos_dec_numbers_only("^\\d\\d?$");
    QRegExp neg_numbers_only("[-]?([0-9]*[.])?[0-9]+");

    ui->comboBox_size->setCurrentText(QString::number(size));
    ui->lineEdit_InputValue_Dec->setText("3.14159265359");
    ui->lineEdit_InputValue_Dec->setValidator(new QRegExpValidator(numbers_only, this));
    ui->lineEdit_InputValue_Scaled->setValidator(new QRegExpValidator(numbers_only, this));
    ui->lineEdit_InputValue_Hex->setValidator(new QRegExpValidator(hex_onlyNbit, this));
    ui->lineEdit_InputValue_Bin->setValidator(new QRegExpValidator(binary_only, this));
    ui->lineEdit_maxRange->setValidator(new QRegExpValidator(pos_numbers_only, this));
    ui->lineEdit_minRange->setValidator(new QRegExpValidator(neg_numbers_only, this));
    ui->comboBox_size->setValidator(new QRegExpValidator(pos_dec_numbers_only, this));
    ui->label_error->setVisible(false);

    SetRangeMode();
    getFixedPoint();
    setDecActive();

    num_repr.update(ui->lineEdit_InputValue_Dec->text().toDouble());

    ui->lineEdit_InputValue_Dec->addCopyButton();
    ui->lineEdit_InputValue_Scaled->addCopyButton();
    ui->lineEdit_InputValue_Hex->addCopyButton();
    ui->lineEdit_InputValue_Bin->addCopyButton();

    ui->lineEdit_InputValue_Dec->setText(num_repr.getDec());
    ui->lineEdit_InputValue_Scaled->setText(num_repr.getScaled());
    ui->lineEdit_InputValue_Hex->setText(num_repr.getHex().right(size/4));
    ui->lineEdit_InputValue_Bin->setText(num_repr.getBin().right(size));
}

void MainWindow::SetRangeMode()
{
    // enable/disable lineEdits
    bool range_mode = ui->checkBox_RangeAuto->checkState(); // true: auto  false: manual
    ui->lineEdit_minRange->setEnabled(!range_mode);
    ui->lineEdit_maxRange->setEnabled(!range_mode);

    // update lineEdits values
    if(range_mode)
    {   // auto range
        ui->checkBox_signed->setEnabled(true);
        range newRange = fxp_getRange(ui->lineEdit_InputValue_Dec->text().toDouble(), ui->checkBox_signed->isChecked());
        ui->lineEdit_minRange->setText(QString::number(newRange.min));
        ui->lineEdit_maxRange->setText(QString::number(newRange.max));
    }
    else
    {
        // manual mode
        ui->checkBox_signed->setEnabled(false);
    }
}

void MainWindow::checkRange()
{
    double input_value = ui->lineEdit_InputValue_Dec->text().toDouble();
    double range_min = ui->lineEdit_minRange->text().toDouble();
    double range_max = ui->lineEdit_maxRange->text().toDouble();

    if((input_value < range_min) || (input_value > range_max))
    {
        ui->label_error->setText("Input value out of input range!");
        ui->label_error->setVisible(true);
    }
    else
    {
        ui->label_error->setVisible(false);
    }
}

void MainWindow::getFixedPoint()
{
    QString fxp_type_string;
    double input_value = ui->lineEdit_InputValue_Dec->text().toDouble();
    getParams();
    fxp_info result = fxp_getFixedPoint(range_min, range_max, input_value, size);

    if(result.out_of_size)
    {
        ui->label_error->setText("Input value out of input size!");
        ui->label_error->setVisible(true);

        fxp_type_string += "int_" + QString::number(result.dataSize) + "." + QString::number(result.fractLenght);
        ui->label_type->setText(fxp_type_string);
        ui->lineEdit_fxp_minRange->setText(QString::number(result.min));
        ui->lineEdit_fxp_maxRange->setText(QString::number(result.max));
        ui->lineEdit_fxp_precision->setText(QString::number(result.precision));
        ui->lineEdit_fxp_value_dec->setText("");
        ui->lineEdit_fxp_value_hex->setText("");
        ui->lineEdit_fxp_value_bin->setText("");
        ui->lineEdit_fxp_error_abs->setText("");
        ui->lineEdit_fxp_value->setText("");
        ui->lineEdit_fxp_error_rel->setText("");
    }
    else
    {
        ui->label_error->setVisible(false);
        if(!result.is_signed) fxp_type_string += "u";
        fxp_type_string += "int_" + QString::number(result.dataSize) + "." + QString::number(result.fractLenght);
        fractLenght = result.fractLenght;
        ui->label_type->setText(fxp_type_string);
        ui->lineEdit_fxp_minRange->setText(QString::number(result.min));
        ui->lineEdit_fxp_maxRange->setText(QString::number(result.max));
        ui->lineEdit_fxp_precision->setText(QString::number(result.precision));
        ui->lineEdit_fxp_value_dec->setText(QString::number(result.fxp_scaled));
        ui->lineEdit_fxp_value_hex->setText(QString("%1").arg(static_cast<qulonglong>(result.fxp_scaled),0,16).right(size/4));
        ui->lineEdit_fxp_value_bin->setText(QString("%1").arg(static_cast<qulonglong>(result.fxp_scaled),0,2).right(size));
        ui->lineEdit_fxp_error_abs->setText(QString::number(result.error_abs));
        ui->lineEdit_fxp_value->setText(QString().setNum(result.fxp_value, 'f', 25));
        ui->lineEdit_fxp_error_rel->setText(QString::number(round(result.error_rel*1000)/1000) + " %");
    }
}
// --------------------------------

void MainWindow::getNumberRepr(int editNumber)
{
    if (isDecActive)
        num_repr.update();
    else
        num_repr.update(num_repr.Scaled / qPow(2, num_repr.Scale));

    if (editNumber != 1)
        ui->lineEdit_InputValue_Dec->setText(num_repr.getDec());
    if (editNumber != 2)
        ui->lineEdit_InputValue_Scaled->setText(num_repr.getScaled());

    SetRangeMode();
    checkRange();

    fxp_info res_target = fxp_getFixedPoint(range_min, range_max, num_repr.Dec, size, scale);
    if(res_target.out_of_size)
    {
        ui->lineEdit_value->setText("Input value out of input size!");
        ui->lineEdit_value->setStyleSheet("color: red;");
    }
    else
    {
        ui->label_error->setVisible(false);
        if (editNumber != 3)
            ui->lineEdit_InputValue_Hex->setText(num_repr.getHex().right(size/4));
        if (editNumber != 4)
        ui->lineEdit_InputValue_Bin->setText(num_repr.getBin().right(size));
        ui->lineEdit_value->setText(QString().setNum(res_target.fxp_value, 'f', 25));
        ui->lineEdit_value->setStyleSheet("color: black;");
    }

    ui->lineEdit_error_abs->setText(QString::number(res_target.error_abs));
    ui->lineEdit_error_rel->setText(QString::number(round(res_target.error_rel*1000)/1000) + " %");
}

/// Scale changed
void MainWindow::on_spinBoxInputScale_valueChanged()
{
    getParams();
    bool isUnderHalf = 0;
    if(ui->lineEdit_InputValue_Dec->text().toDouble() <= 0.5)
        isUnderHalf = 1;
    if( scale > size - ui->checkBox_signed->isChecked() + isUnderHalf)
    {
        scale = size - ui->checkBox_signed->isChecked() + isUnderHalf;
        ui->spinBoxInputScale->setValue(scale);
    }
    num_repr.setScale(scale);

    if (!isScaleAuto)
        getNumberRepr(0);
    else
        getNumberRepr(1);
}

/// Dec value edited
void MainWindow::on_lineEdit_InputValue_Dec_textEdited(const QString &arg1)
{
    num_repr.Dec = arg1.toDouble();
    getParams();
    getNumberRepr(1);
}

/// Scaled value edited
void MainWindow::on_lineEdit_InputValue_Scaled_textEdited(const QString &arg1)
{
    num_repr.Scaled = arg1.toDouble();
    getParams();
    getNumberRepr(2);
}

/// Hex value edited
void MainWindow::on_lineEdit_InputValue_Hex_textEdited(const QString &arg1)
{
    getParams();
    setDecInactive();

    if(isSigned)
    {
        switch (size) {
        case 8:  num_repr.Scaled = static_cast<int8_t>(arg1.toInt(NULL, 16)); break;
        case 16: num_repr.Scaled = static_cast<int16_t>(arg1.toInt(NULL, 16)); break;
        case 32: num_repr.Scaled = static_cast<int32_t>(arg1.toLongLong(NULL, 16)); break;
        case 64: num_repr.Scaled = static_cast<int64_t>(arg1.toULongLong(NULL, 16)); break;
        }
    }
    else
    {
        switch (size) {
        case 8:  num_repr.Scaled = static_cast<uint8_t>(arg1.toInt(NULL, 16)); break;
        case 16: num_repr.Scaled = static_cast<uint16_t>(arg1.toInt(NULL, 16)); break;
        case 32: num_repr.Scaled = static_cast<uint32_t>(arg1.toLongLong(NULL, 16)); break;
        case 64: num_repr.Scaled = static_cast<uint64_t>(arg1.toULongLong(NULL, 16)); break;
        }
    }

    getNumberRepr(3);
}

/// Bin value edited
void MainWindow::on_lineEdit_InputValue_Bin_textEdited(const QString &arg1)
{
    getParams();
    setDecInactive();
    if(isSigned)
    {
        switch (size) {
        case 8:  num_repr.Scaled = static_cast<int8_t>(arg1.toInt(NULL, 2)); break;
        case 16: num_repr.Scaled = static_cast<int16_t>(arg1.toInt(NULL, 2)); break;
        case 32: num_repr.Scaled = static_cast<int32_t>(arg1.toLongLong(NULL, 2)); break;
        case 64: num_repr.Scaled = static_cast<int64_t>(arg1.toULongLong(NULL, 2)); break;
        }
    }
    else
    {
        switch (size) {
        case 8:  num_repr.Scaled = static_cast<uint8_t>(arg1.toInt(NULL, 2)); break;
        case 16: num_repr.Scaled = static_cast<uint16_t>(arg1.toInt(NULL, 2)); break;
        case 32: num_repr.Scaled = static_cast<uint32_t>(arg1.toLongLong(NULL, 2)); break;
        case 64: num_repr.Scaled = static_cast<uint64_t>(arg1.toULongLong(NULL, 2)); break;
        }
    }
    getNumberRepr(4);
}

/// Input Dec changed
void MainWindow::on_lineEdit_InputValue_Dec_textChanged()
{
    SetRangeMode();
    checkRange();
    getFixedPoint();
    if (isScaleAuto)
        ui->spinBoxInputScale->setValue(fractLenght);
}

void MainWindow::on_lineEdit_InputValue_Dec_focussed()
{
    setDecActive();
}

void MainWindow::on_lineEdit_InputValue_Scaled_focussed()
{
    setDecInactive();
}


void MainWindow::on_pushButton_clicked()
{
    ui->spinBoxInputScale->setValue(fractLenght);
}

void MainWindow::on_checkBox_ScaleAuto_stateChanged(int arg1)
{
    ui->spinBoxInputScale->setEnabled(!arg1);
    ui->spinBoxInputScale->setValue(fractLenght);
    isScaleAuto = arg1;
}
