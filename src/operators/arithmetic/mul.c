#include "../../s21_decimal.h"
#include "../../common/common.h"

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int status = 1;
    int finalScale = s21_decimal_get_scale(value_1) + s21_decimal_get_scale(value_2);
    for (int i = 0; i < s21_decimal_get_power(value_1); i++) {
        if (s21_get_bit(&value_1, i) == 1) {
            status &= s21_add(value_2, *result, result);
        }
        status &= s21_decimal_mul_by_two(&value_2);
    }
    s21_decimal_set_scale(result, finalScale);
    return status;
}