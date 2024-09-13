#include "fixed_point_calculations.h"
#include <QString>

range fxp_getRange(double input_value, bool input_signed)
{
    range result;

    // max value
    result.max = static_cast<int>(ceil(abs(input_value)));
    // min value
    if(input_signed)
    {
        // signed
        result.min = -result.max;
    }
    else
    {
        // unsigned
        result.min = 0;
    }

    return result;
}

fxp_info fxp_getFixedPoint(int range_min, int range_max, double input_value, unsigned int size)
{
    fxp_info result;

    result.dataSize = size;
    if(range_min < 0)
    {
        result.is_signed = true;
    }
    else
    {
        result.is_signed = false;
    }

    // find max absolute value of range
    int range_max_abs_value;
    if(result.is_signed)
    {
        // signed
        if(range_max > abs(range_min))
        {
            range_max_abs_value = range_max;
        }
        else
        {
            range_max_abs_value = abs(range_min);
            if (static_cast<int>(input_value) == range_max)
                range_max_abs_value += 1;
        }

    }
    else
    {
        // unsigned -> max absoulte value is range_max
        range_max_abs_value = range_max;
    }

    // length of decimal part
    result.decLenght = static_cast<unsigned int>(ceil(log2(range_max_abs_value)) + 1);
    if(!result.is_signed || fabs(input_value) < 0.5)
        result.decLenght -= 1;

    // length of fractional part
    if(result.decLenght < result.dataSize)
    {
        result.fractLenght = result.dataSize - result.decLenght;
    }
    else
    {
        result.fractLenght = 0;
    }
    // minimum value
    if(result.is_signed)
    {
        result.min = -pow(2,result.dataSize - 1 - result.fractLenght);
    }
    else
    {
        result.min = 0;
    }
    // maximum value
    result.max = pow(2,result.dataSize - result.is_signed - result.fractLenght)
            - result.is_signed * pow(2,-static_cast<int>(result.fractLenght));
    if (fabs(input_value) < 0.5)
    {
        result.min = -0.5;
        result.max =  0.5;
    }
    if(!result.is_signed && input_value == result.max)
    {
        result.decLenght += 1;
        result.fractLenght -=1;
    }

    // precision
    result.precision = pow(2,-static_cast<int>(result.fractLenght));
    // range check of input value
    if((input_value < result.min) || (input_value > result.max))
    {
        result.out_of_size = true;
        return result;
    }
    else
    {
        result.out_of_size = false;
    }

    // fixed point value in decimal
    if(input_value == (result.max + result.precision))
    {
        // workaround for positiv boundary number
        result.fxp_scaled = pow(2,result.dataSize - 1) - 1;
    }
    else if(input_value >= 0)
    {
        result.fxp_scaled = round(input_value/result.precision);
    }
    else if(input_value < 0)
    {
        result.fxp_scaled = pow(2,result.dataSize - 1) + round((pow(2,result.decLenght-1) + input_value)/result.precision);
        switch (size) {
        case 8:  result.fxp_scaled = static_cast<int8_t> ((QString("%1").arg(static_cast<qulonglong>(result.fxp_scaled),0,16)).toInt(NULL, 16)); break;
        case 16: result.fxp_scaled = static_cast<int16_t>((QString("%1").arg(static_cast<qulonglong>(result.fxp_scaled),0,16)).toInt(NULL, 16)); break;
        case 32: result.fxp_scaled = static_cast<int32_t>((QString("%1").arg(static_cast<qulonglong>(result.fxp_scaled),0,16)).toLongLong(NULL, 16)); break;
        case 64: result.fxp_scaled = static_cast<int64_t>((QString("%1").arg(static_cast<qulonglong>(result.fxp_scaled),0,16)).toULongLong(NULL, 16)); break;
        }
        if(!result.decLenght)
            result.fxp_scaled = round(input_value/result.precision);
    }
    // value
    if(result.is_signed)
    {
        if(result.fxp_scaled > pow(2,result.dataSize - 1)-1)
        {
            // negative number
            result.fxp_value = -pow(2,result.dataSize - 1 - result.fractLenght) + (result.fxp_scaled - pow(2,result.dataSize -1))*result.precision;
        }
        else
        {
            // positive number
            result.fxp_value = result.precision * result.fxp_scaled;
        }
    }
    else
    {
        result.fxp_value = result.precision * result.fxp_scaled;
    }

    // absolute error
    result.error_abs = input_value - result.fxp_value;
    // relative error
    result.error_rel = 100 - (input_value/result.fxp_value) * 100;

    return result;
}

fxp_info fxp_getFixedPoint(int range_min, int range_max, double input_value, unsigned int size, unsigned int scale)
{
    fxp_info result;

    result.dataSize = size;
    if(range_min < 0)
    {
        result.is_signed = true;
    }
    else
    {
        result.is_signed = false;
    }

    // find max absolute value of range
    int range_max_abs_value;
    if(result.is_signed)
    {
        // signed
        if(range_max > abs(range_min))
        {
            range_max_abs_value = range_max;
        }
        else
        {
            range_max_abs_value = abs(range_min);
        }

    }
    else
    {
        // unsigned -> max absoulte value is range_max
        range_max_abs_value = range_max;
    }

    // length of decimal part
    result.decLenght = static_cast<unsigned int>(ceil(log2(range_max_abs_value)) + 1);
    // length of fractional part
    if(result.decLenght < result.dataSize)
    {
        result.fractLenght = scale;
    }
    else
    {
        result.fractLenght = 0;
    }
    // minimum value
    if(result.is_signed)
    {
        result.min = -pow(2,result.dataSize - 1);
    }
    else
    {
        result.min = 0;
    }
    // maximum value
    int fix = result.is_signed;
    result.max = pow(2,result.dataSize - fix) - fix;
    // precision
    result.precision = pow(2,-static_cast<int>(result.fractLenght));

    // fixed point value in decimal
    if(input_value == (result.max + result.precision))
    {
        // workaround for positiv boundary number
        result.fxp_scaled = pow(2,result.dataSize - 1) - 1;
    }
    else if(input_value >= 0)
    {
        result.fxp_scaled = round(input_value/result.precision);
    }
    else if(input_value < 0)
    {
        result.fxp_scaled = round(input_value/result.precision);
    }
    // range check of fxp_scaled
    if((result.fxp_scaled < result.min) || (result.fxp_scaled > result.max + result.precision))
    {
        result.out_of_size = true;
        return result;
    }
    else
    {
        result.out_of_size = false;
    }
    // value
    if(result.is_signed)
    {
        if(result.fxp_scaled > pow(2,result.dataSize - 1)-1)
        {
            // negative number
            result.fxp_value = -pow(2,result.dataSize - 1 - scale) + (result.fxp_scaled - pow(2,result.dataSize -1))*result.precision;

        }
        else
        {
            // positive number
            result.fxp_value = result.precision * result.fxp_scaled;
        }
    }
    else
    {
        result.fxp_value = result.precision * result.fxp_scaled;
    }

    // absolute error
    result.error_abs = input_value - result.fxp_value;
    // relative error
    result.error_rel = 100 - (input_value/result.fxp_value) * 100;

    return result;
}
