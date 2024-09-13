#include <QtMath>
#include "number_repr.h"

Number_repr num_repr;

Number_repr::Number_repr()
{
    Scale = 0;
}

Number_repr::Number_repr(double value)
{    
    Dec = value;
    Scaled = qPow(2, Scale) * Dec;
    Hex = QString::number(int(Scaled), 16).toUpper();
    Bin = QString::number(int(Scaled), 2);    
}

Number_repr::Number_repr(double value, int Scale)
{
    this->Scale = Scale;
    Dec = value;
    Scaled = qPow(2, Scale) * Dec;
    Hex = QString::number(int(Scaled), 16).toUpper();
    Bin = QString::number(int(Scaled), 2);    
}

QString Number_repr::getDec() const
{
    return QString().setNum(Dec, 'f', 16);
}

QString Number_repr::getScaled() const
{
    return QString().setNum(Scaled, 'f', 16);
}

QString Number_repr::getHex() const
{
    return Hex;
}

QString Number_repr::getBin() const
{
    return Bin;
}

void Number_repr::setScale(int value)
{
    Scale = value;
}

void Number_repr::setDec(double value)
{
    Dec = value;
}

void Number_repr::setScaled(double value)
{
    Scaled = qPow(2, Scale) * int(value);
}

void Number_repr::setHex(const int &value)
{
    Hex = QString::number(value, 16).toUpper();
}

void Number_repr::setBin(const int &value)
{
    Bin = QString::number(value, 2);
}

void Number_repr::update(double value)
{
    Dec = value;
    Scaled = qPow(2, Scale) * Dec;
    Hex = QString("%1").arg(static_cast<qlonglong>(Scaled),0,16);//.toUpper();
    Bin = QString::number(int(Scaled), 2);
}

void Number_repr::update()
{
    Scaled = qPow(2, Scale) * Dec;
    Hex = QString("%1").arg(static_cast<qlonglong>(Scaled),0,16);
    Bin = QString::number(int(Scaled), 2);
}

void Number_repr::update(fxp_info result)
{
    Dec = result.fxp_value;
    Scaled = result.fxp_scaled;
    Hex = QString("%1").arg(static_cast<qlonglong>(Scaled),0,16);
    Bin = QString("%1").arg(static_cast<qlonglong>(Scaled),0,2);
}

