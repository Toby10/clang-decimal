#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>

#include "common.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  if (!dst) return FAILURE;

  int sign = s21_get_sign(src);  // 0 – positive

  s21_decimal_init(dst);

  if (sign && src == INT_MIN) {
    dst->bits[0] = (unsigned int)src;
  } else {
    dst->bits[0] = (unsigned int)s21_abs(src);
  }

  s21_decimal_set_sign(dst, sign);

  return SUCCESS;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  if (!dst || isnan(src) || isinf(src)) return FAILURE;
  if (fabsf(src) > DECIMAL_MAX_FLOAT) return FAILURE;
  if (fabsf(src) > 0 && fabsf(src) < DECIMAL_MIN_FLOAT) return FAILURE;

  int sign = signbit(src);
  src = fabsf(src);

  int scale = 0;

  float integral_part;
  modff(src, &integral_part);
  while (scale < 7 && src != integral_part) {
    src *= 10.0f;
    scale++;
    modff(src, &integral_part);
  }

  unsigned long long int_val = (unsigned long long)roundf(src);
  if (int_val == 0) scale = 0;  // squeeze in case of src < 5e-8

  s21_decimal_init(dst);
  dst->bits[0] = (unsigned int)(int_val & 0xFFFFFFFF);
  dst->bits[1] = (unsigned int)((int_val >> 32) & 0xFFFFFFFF);
  dst->bits[2] = 0;

  s21_decimal_set_scale(dst, scale);
  s21_decimal_set_sign(dst, sign);

  return SUCCESS;
}

// TODO: replace with real division
void temp_div_by_10(s21_decimal *dec) {
  // by gpt. idk is it safe
  if (!dec) return;

  unsigned int result[3] = {0};
  unsigned long rem = 0;

  for (int i = 2; i >= 0; i--) {
    unsigned long long part = ((unsigned long long)rem << 32) | dec->bits[i];
    result[i] = (unsigned int)(part / 10);
    rem = part % 10;
  }

  dec->bits[0] = result[0];
  dec->bits[1] = result[1];
  dec->bits[2] = result[2];
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  if (!dst || !is_s21_decimal_valid(&src)) return FAILURE;

  s21_decimal limit_low = {0}, limit_high = {0};
  s21_from_int_to_decimal(INT_MIN, &limit_low);
  s21_from_int_to_decimal(INT_MAX, &limit_high);
  if (s21_is_greater(src, limit_high) || s21_is_less(src, limit_low))
    return FAILURE;

  s21_decimal temp_dec = src;
  int scale = s21_decimal_get_scale(&temp_dec);
  while (scale--) {
    temp_div_by_10(&temp_dec);
  }  // TODO: replace with div(dec, 10^scale)

  int sign = s21_decimal_get_sign(&temp_dec);
  uint32_t raw_value = (uint32_t)temp_dec.bits[0];

  if (sign && raw_value == ((uint32_t)INT_MAX + 1U)) {
    *dst = INT_MIN;
  } else {
    *dst = (int)raw_value;
    if (sign) *dst *= -1;
  }

  return SUCCESS;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  if (!dst || !is_s21_decimal_valid(&src)) return FAILURE;

  int sign = s21_decimal_get_sign(&src);
  int scale = s21_decimal_get_scale(&src);

  float raw_value = 0.0f;

  raw_value += (float)src.bits[0];
  raw_value += (float)src.bits[1] * powf(2.0f, 32);
  raw_value += (float)src.bits[2] * powf(2.0f, 64);

  raw_value /= powf(10.0f, scale);
  if (sign) raw_value = -raw_value;

  *dst = raw_value;
  return SUCCESS;
}