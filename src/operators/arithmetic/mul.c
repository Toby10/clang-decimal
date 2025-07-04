#include "../../common/common_arithmetic.h"

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *output) {
  s21_decimal result = {0};
  int status = 0;
  int finalScale =
      s21_decimal_get_scale(&value_1) + s21_decimal_get_scale(&value_2);
  int finalSign = 0;
  finalSign =
      (s21_decimal_get_sign(&value_1) + s21_decimal_get_sign(&value_2)) % 2;
  for (int i = 0; i < s21_decimal_get_power(&value_1) + 1; i++) {
    if (s21_get_bit(&value_1, i) == 1) {
      status |= s21_add(value_2, result, &result);
    }
    status |= s21_decimal_shift_left_n(&value_2, 1);
  }
  s21_decimal_set_sign(&result, finalSign);
  s21_decimal_set_scale(&result, finalScale);
  if (status != 0) {
    status = finalSign + 1;
  } else {
    s21_decimal_init(output);
    *output = result;
  }
  return status;
}