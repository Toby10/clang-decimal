#include "../../common/common_arithmetic.h"

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int status = SUCCESS;
  int sign1 = s21_decimal_get_sign(&value_1);
  int sign2 = s21_decimal_get_sign(&value_2);
  int res_scale = 0;
  int ctl = 0;
  if (!sign1 && !sign2) {  // Both numbers positive
    for (int i = 0; i < 4; i++) result->bits[i] = 0;
    res_scale = s21_decimal_align_scale(&value_1, &value_2);
    if (s21_is_greater(value_1, value_2)) {
      ctl = s21_decimal_sub_aligned(value_1, value_2, result);
    } else if (s21_is_greater(value_2, value_1)) {
      ctl = s21_decimal_sub_aligned(value_2, value_1, result);
    } else
      ctl = -1;
    if (ctl >= 0) {
      s21_decimal_set_scale(result, res_scale);
      s21_decimal_set_sign(result, sign1);
    }
  } else if (sign1 && !sign2) {  // First number negative, second - positive
    s21_decimal_set_sign(&value_1, 0);
    status = s21_add(value_1, value_2, result);
    s21_decimal_set_sign(result, 1);
  } else if (!sign1 && sign2) {  // First number positive, second - negative
    s21_decimal_set_sign(&value_2, 0);
    status = s21_add(value_1, value_2, result);
  } else {  // Both numbers negative
    s21_decimal_set_sign(&value_2, 0);
    s21_decimal_set_sign(&value_1, 0);
    status = s21_sub(value_2, value_1, result);
  }
  return status;
}
