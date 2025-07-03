#include "../../common/common_arithmetic.h"
#define is_dec_zero(dec) is_s21_decimal_zero(dec)

typedef struct {
  s21_decimal buffer;
  int position;
  s21_decimal ten;
  int shift;
  int shifted_down_by;
  s21_decimal increase_shifted;
  int count;

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

  ctx.shift = scale_2 - scale_1;  // Align decimals
  if (scale_1 > scale_2) {
    ctx.shifted_down_by = -1 * ctx.shift;
    ctx.shift = 0;
    // TODO: optimize by MIN/MAX
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

  while (ctx.position-- > 0 || (!is_dec_zero(&ctx.buffer) && ctx.scale <= 28)) {
    if (ctx.position < 0) {
      (ctx.scale)++;
      s21_mul(ctx.buffer, ctx.ten, &ctx.buffer);
      if (ctx.scale <= 28) s21_mul(ctx.result, ctx.ten, &ctx.result);
    } else {
      s21_decimal_shift_left_n(&ctx.buffer, 1);
      ctx.too_big = s21_decimal_shift_left_n(&ctx.result, 1);
    }

    if (!ctx.scale && !ctx.too_big) {
      s21_set_bit(&ctx.buffer, 0, s21_get_bit(&src, ctx.position));
    }

    ctx.count = 0;
    while (s21_is_greater_or_equal(ctx.buffer, divider) && !ctx.too_big) {
      s21_decimal_sub_aligned(ctx.buffer, divider, &ctx.buffer);
      if (!ctx.scale)
        s21_add(ctx.result, ctx.increase_shifted, &ctx.result);
      else
        (ctx.count)++;
    }
    if (ctx.count && !ctx.too_big && ctx.scale <= 28)
      s21_decimal_add_digit(&ctx.result, ctx.count);
  }

  ctx.too_big = s21_decimal_round_bank(&ctx.result, ctx.count) || ctx.too_big;

  if (!ctx.too_big) {
    ctx.too_small =
        (is_s21_decimal_zero(&ctx.result) && !is_s21_decimal_zero(&ctx.buffer));
    if (!ctx.too_small) {
      *output = ctx.result;
      s21_decimal_set_sign(output, ctx.sign);
      s21_decimal_set_scale(output, s21_min(ctx.scale, 28) + ctx.shifted_down_by);
    }
  }

  return (ctx.too_small * 2) + (ctx.too_big * (ctx.sign + 1));
}