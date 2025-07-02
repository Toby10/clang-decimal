// TODO: remove – #include <stdio.h>

#include "../../common/common_arithmetic.h"

typedef struct {
  s21_decimal buffer;
  int position;
  s21_decimal ten;
  int shift;
  int shifted_down_by;
  s21_decimal increase_shifted;
  
  s21_decimal result;
  int scale;
  int sign;
  
  int too_small;
  int too_big;
} s21_div_context;

s21_div_context s21_div_initialize(s21_decimal *dec_1, s21_decimal *dec_2) {
  s21_div_context ctx = {0};
  ctx.position = 96;
  ctx.ten.bits[0] = 10;

  int scale_1, scale_2;
  scale_1 = s21_decimal_get_scale(dec_1);
  scale_2 = s21_decimal_get_scale(dec_2);

  ctx.shift = scale_2 - scale_1; // Align decimals
  if (scale_1 > scale_2) {
    ctx.shifted_down_by = -1 * ctx.shift; // TODO: test it, idk at all
    ctx.shift = 0;
    // TODO: optimize by MIN/MAX if it works
  }

  if (ctx.shift) {
    ctx.increase_shifted = ctx.ten;
    for (int i = ctx.shift; i > 1; i--) {
      s21_decimal_mul_by_10(&ctx.increase_shifted);
    }
  } else {
    ctx.increase_shifted.bits[0] = 1;
  }

  ctx.sign = s21_decimal_get_sign(dec_1) ^ s21_decimal_get_sign(dec_2);

  // Make decimals as positive integers
  dec_1->bits[3] = 0;
  dec_2->bits[3] = 0;

  return ctx;
}

int s21_div(s21_decimal src, s21_decimal divider, s21_decimal *output) {
  if (!is_s21_decimal_valid(&src) || !is_s21_decimal_valid(&divider))
    return FAILURE;
  if (is_s21_decimal_zero(&divider)) return S21_DIV_BY_ZERO;

  s21_div_context ctx = s21_div_initialize(&src, &divider);

  while (ctx.position-- > 0 || (!is_s21_decimal_zero(&ctx.buffer) && ctx.scale < 28)) {
    if (ctx.position < 0) {
      (ctx.scale)++;
      s21_mul(ctx.buffer, ctx.ten, &ctx.buffer);
      ctx.too_big = s21_mul(ctx.result, ctx.ten, &ctx.result) > 0;
    } else {
      s21_decimal_shift_left_n(&ctx.buffer, 1);
      s21_decimal_shift_left_n(&ctx.result, 1);
    }

    if (!ctx.scale && !ctx.too_big) {
      s21_set_bit(&ctx.buffer, 0, s21_get_bit(&src, ctx.position));
    }

    int count = 0;
    while (s21_is_greater_or_equal(ctx.buffer, divider) && !ctx.too_big) {
      s21_decimal_sub_aligned(ctx.buffer, divider, &ctx.buffer);
      if (!ctx.scale)
        s21_add(ctx.result, ctx.increase_shifted, &ctx.result);
      else
        count++;
    }
    if (count && !ctx.too_big) s21_decimal_add_digit(&ctx.result, count);
  }

  // TODO: ctx.too_big = round() > 0

  if (!ctx.too_big) {
    ctx.too_small = (is_s21_decimal_zero(&ctx.result) && !is_s21_decimal_zero(&ctx.buffer));
    if (!ctx.too_small) {
      *output = ctx.result;
      s21_decimal_set_sign(output, ctx.sign);
      s21_decimal_set_scale(output, ctx.scale + ctx.shifted_down_by);

      float a, b, c;
      s21_from_decimal_to_float(src, &a);
      s21_from_decimal_to_float(divider, &b);
      s21_from_decimal_to_float(*output, &c);
      // TODO: remove – printf("%.1f/%.1f = %.1f\n", a, b, c);
    }
  }

  return (ctx.too_small) ? S21_TOO_SMALL : (ctx.too_big) ? (S21_TOO_BIG + ctx.sign) : SUCCESS;
}