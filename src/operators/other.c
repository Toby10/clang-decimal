#include "../common/common.h"

// -=-=-=- Main functions -=-=-=-

int s21_floor(s21_decimal value, s21_decimal *result) {  // Надо вычитание :()
  int ret_val = TRUE;

  return ret_val;
}
int s21_round(s21_decimal value,
              s21_decimal *result) {  // Надо вычитание и сложение :()
  int ret_val = TRUE;

  return ret_val;
}
int s21_truncate(s21_decimal value, s21_decimal *result) {
  int ret_val = is_s21_decimal_valid(&value) && is_s21_decimal_valid(result);
  unsigned long long leftover = 0;
  if (ret_val) {
    int scale = s21_decimal_get_scale(&value);
    for (int i = 0; i < scale; i++) {
      s21_div_by_ten(&value);
    }
    s21_decimal_set_scale(&value, s21_decimal_get_scale(&value) - scale);
    *result = value;
  }
  return ret_val;
}
int s21_negate(s21_decimal value, s21_decimal *result) {
  int ret_val = is_s21_decimal_valid(&value) && is_s21_decimal_valid(result);
  if (ret_val) {
    *result = value;
    s21_decimal_set_sign(result, !s21_decimal_get_sign(result));
  }
  ret_val = is_s21_decimal_valid(result);
  return ret_val;
}