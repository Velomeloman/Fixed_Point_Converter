#ifndef NUMBER_REPR_H
#define NUMBER_REPR_H

#include <QString>
#include "fixed_point_calculations.h"

class Number_repr
{
public:
    Number_repr();
    Number_repr(double value);
    Number_repr(double value, int Scale);

    int Scale;      /// масштаб
    double Dec;     /// десятичное представление
    double Scaled;  /// масштабированное десятичное предстваление
    QString Hex;    /// шестнадцатиричное представление
    QString Bin;    /// двоичное представление
    fxp_info fxp;

    void setScale(int value);
    void setDec(double value);
    void setScaled(double value);
    void setBin(const int &value);
    void setHex(const int &value);
    void update(double value);
    void update();
    void update(fxp_info result);
    QString getDec() const;
    QString getScaled() const;
    QString getHex() const;
    QString getBin() const;
};

extern Number_repr num_repr;

#endif // NUMBER_REPR_H
