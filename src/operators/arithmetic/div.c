#include "../common_arithmetic.h"

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  if (!is_s21_decimal_valid(&value_1) || !is_s21_decimal_valid(&value_2))
    return FAILURE;
  if (is_s21_decimal_zero(&value_2)) return S21_DIV_BY_ZERO;

  // Initialization
  s21_decimal *inter_result = {0};
  s21_decimal *remainder = {0};

  // Integer calculation
  for (int position = 95; position >= 0; position--) {
    s21_decimal_shift_left_n(remainder, 1);
    s21_set_bit(remainder, 0, s21_get_bit(&value_1, position));

    if (s21_is_greater_or_equal(*remainder, value_2)) {
      s21_decimal_sub_aligned(*remainder, value_2, remainder);
      s21_set_bit(inter_result, position, 1);
    }
  }

  // Fractional calculation
  int scale = 0;
  while (!is_s21_decimal_zero(remainder) && scale < 28) {
    s21_decimal_mul_by_10(remainder);

    int digit = 0;
    while (s21_is_greater_or_equal(*remainder, value_2)) {
      s21_decimal_sub_aligned(*remainder, value_2, remainder);
      digit++;
    }

    s21_decimal_mul_by_10(inter_result);
    s21_decimal_add_digit(inter_result, digit);

    scale++;
  }

  int too_small = FALSE;
  if (is_s21_decimal_zero(inter_result) && !is_s21_decimal_zero(remainder))
    too_small = TRUE;

  if (!too_small) {
    result = inter_result;
    int sign = s21_decimal_get_sign(&value_1) ^ s21_decimal_get_sign(&value_2);
    s21_decimal_set_sign(result, sign);
    s21_decimal_set_scale(result, scale);
  }

  return (too_small) ? 0 : S21_TOO_SMALL;
}