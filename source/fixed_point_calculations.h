#ifndef FIXED_POINT_CALCULATIONS_H
#define FIXED_POINT_CALCULATIONS_H
#include <cmath>

struct range
{
    int min;
    int max;
};
// SIGNint_N.K (SIGN: u for unsigned or blank for signed, N: length of data, K: length of fractional part, L: length of decimal part)
struct fxp_info
{
    unsigned int dataSize;          ///< length of data
    unsigned int fractLenght;       ///< length of fractional part
    unsigned int decLenght;         ///< length of decimal part
    bool is_signed;                 ///< is signed
    double min;                     ///< min value
    double max;                     ///< max value
    double precision;
    double fxp_value;
    double fxp_scaled;         ///< fixed point value auto scaled
    double error_abs;               ///< absolute error
    double error_rel;               ///< relative error
    bool out_of_size;
};

range fxp_getRange(double input_value, bool input_signed);
fxp_info fxp_getFixedPoint(int range_min, int range_max, double input_value, unsigned int size);
fxp_info fxp_getFixedPoint(int range_min, int range_max, double input_value, unsigned int size, unsigned int scale);

#endif // FIXED_POINT_CALCULATIONS_H
