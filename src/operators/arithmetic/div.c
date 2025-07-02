#include <stdio.h>

#include "../../common/common_arithmetic.h"

int s21_div(s21_decimal src, s21_decimal divider, s21_decimal *output) {
  if (!is_s21_decimal_valid(&src) || !is_s21_decimal_valid(&divider))
    return FAILURE;
  if (is_s21_decimal_zero(&divider)) return S21_DIV_BY_ZERO;

  s21_decimal buffer = {0}, result = {0}, ten = {{10, 0, 0, 0}};
  int position = 96, scale = 0, too_small = 0, too_big = 0;

  // shift
  
  while (position-- > 0 || (!is_s21_decimal_zero(&buffer) && scale < 28)) {
    if (position < 0) {
      scale++;
      s21_mul(buffer, ten, &buffer);
      s21_mul(result, ten, &result);
    } else {
      s21_decimal_shift_left_n(&buffer, 1);
      s21_decimal_shift_left_n(&result, 1);
    }

    if (!scale) {
      s21_set_bit(&buffer, 0, s21_get_bit(&src, position));
      // too_big?
    } else {
      float w;
      s21_from_decimal_to_float(buffer, &w);
      printf("scale: %d ? %.1f\n", scale, w);
    }

    int count = 0;
    while (s21_is_greater_or_equal(buffer, divider)) {
      s21_decimal_sub_aligned(buffer, divider, &buffer);
      if (!scale) {
        s21_decimal_inc(&result);
      } else {
        count++;
      }   
    }
    if (count) s21_decimal_add_digit(&result, count);
  }

  // round

  too_small = (is_s21_decimal_zero(&result) && !is_s21_decimal_zero(&buffer));
  if (!too_small) {
    *output = result;
    int sign = s21_decimal_get_sign(&src) ^ s21_decimal_get_sign(&divider);
    s21_decimal_set_sign(output, sign);
    s21_decimal_set_scale(output, scale);

    float a, b, c;
    s21_from_decimal_to_float(src, &a);
    s21_from_decimal_to_float(divider, &b);
    s21_from_decimal_to_float(*output, &c);
    printf("%.1f/%.1f = %.1f\n", a, b, c);
  }

  return (too_small) ? S21_TOO_SMALL : (too_big) ? S21_TOO_BIG : SUCCESS;
}