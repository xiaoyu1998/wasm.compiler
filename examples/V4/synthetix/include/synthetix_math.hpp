#pragma once

#include <synthetix_constants.hpp>

namespace synthetix {

    //fixmd:rounding
    template<typename T>
    uint128_t divide_decimal(uint128_t a, uint128_t b, T precision){
        uint128_t tmp = 10 * a * precision  / b;
        return (tmp + 5) / 10;
    }

    template<typename T>
    uint128_t multiply_decimal(uint128_t a, uint128_t b, T precision){
        uint128_t tmp = 10 * a * b / precision;
        return (tmp + 5) / 10;
    }

    #define synthetix_divide_decimal(a, b) divide_decimal(a, b, PRECISION_1)
    #define synthetix_multiply_decimal(a, b) multiply_decimal(a, b, PRECISION_1)

}