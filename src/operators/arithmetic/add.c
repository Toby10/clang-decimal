#include "../../common/common_arithmetic.h"
#include <stdio.h>

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  int status = SUCCESS;
  int sign1 = s21_decimal_get_sign(&value_1);
  int sign2 = s21_decimal_get_sign(&value_2);
  int res_scale = 0;
  int ctl = 0;
  if (sign1 == sign2) {  // Both numbers either positive or negative
    for (int i = 0; i < 4; i++) result->bits[i] = 0;
    res_scale = s21_decimal_align_scale(&value_1, &value_2);
    ctl = s21_decimal_add_aligned(value_1, value_2, result);
    if (!ctl && !sign1)
      status = S21_TOO_BIG;
    else if (!ctl && sign1)
      status = S21_TOO_SMALL;
    s21_decimal_set_scale(result, res_scale);
    s21_decimal_set_sign(result, sign1);
  } else if (sign1 && !sign2) {  // First number negative, second - positive
    s21_decimal_set_sign(&value_1, 0);
    status = s21_sub(value_2, value_1, result);
  } else {  // First number positive second - negative
    s21_decimal_set_sign(&value_2, 0);
    status = s21_sub(value_1, value_2, result);
  }
  return status;
}
