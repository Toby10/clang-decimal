#include "../../common/common.h"

// Addition of decimals with aligned scales
int s21_add_aligned(s21_decimal value_1, s21_decimal value_2,
                    s21_decimal* result);

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  int status = SUCCESS;
  int sign1 = s21_decimal_get_sign(&value_1);
  int sign2 = s21_decimal_get_sign(&value_2);
  int res_scale = 0;
  int ctl = 0;
  if (sign1 == sign2) {  // Both numbers either positive or negative
    for (int i = 0; i < 4; i++) result->bits[i] = 0;
    res_scale = s21_align_scale(&value_1, &value_2);
    ctl = s21_add_aligned(value_1, value_2, result);
    if (!ctl && sign1)
      status = S21_TOO_BIG;
    else if (!ctl && !sign1)
      status = S21_TOO_SMALL;
    s21_decimal_set_scale(result, res_scale);
    s21_decimal_set_sign(result, sign1);
  } else if (!sign1 && sign2) {  // First number negative, second - positive
    s21_decimal_set_sign(&value_1, 0);
    status = s21_sub(value_2, value_1, result);
  } else {  // First number positive second - negative
    s21_decimal_set_sign(&value_2, 0);
    status = s21_sub(value_1, value_2, result);
  }
  return status;
}

int s21_add_aligned(s21_decimal value_1, s21_decimal value_2,
                    s21_decimal* result) {
  unsigned int sum = 0, leftover = 0;
  for (int i = 0; i < 3; i++) {
    sum = value_1.bits[i] + value_2.bits[i] + leftover;
    result->bits[i] = sum & ((1u << 31) - 1);
    leftover = sum >> 31;
  }
  int res = leftover ? 0 : 1;
  // if(!res) result = s21_bank(result, leftover); TODOOOOOTOTOTOO
  return res;
}