#include "common.h"

#include <stdlib.h>
#include <string.h>

// Сюда скидывать функции, которые могут пригодиться в других файлах

// -=-=-=- S21_DECIMAL INTERFACE -=-=-=-

void s21_decimal_init(s21_decimal *dec) {
  memset(dec, 0, sizeof(s21_decimal));  // set 0
}

void s21_decimal_set_sign(s21_decimal *dec, int sign) {
  if (sign)
    dec->bits[3] |= 0x80000000;  // 1 – negative
  else
    dec->bits[3] &= 0x7FFFFFFF;  // 0 – positive
}

void s21_decimal_set_scale(s21_decimal *dec, int scale) {
  dec->bits[3] &= 0xFF00FFFF;              // erase sign
  dec->bits[3] |= ((scale & 0xFF) << 16);  // set sign
}

int s21_decimal_get_sign(s21_decimal *dec) {
  return (dec->bits[3] & 0x80000000) == 0x80000000;
}

int s21_decimal_get_scale(s21_decimal *dec) {
  return (dec->bits[3] >> 16) & 0xFF;
}

// -=-=-=- ACCESSORIES -=-=-=-

int is_s21_decimal_valid(s21_decimal *dec) {
  if (!dec) return FALSE;

  int is_valid = (dec->bits[3] & 0x7F00FFFF) == 0;
  if (is_valid) is_valid = (s21_decimal_get_scale(dec) <= DECIMAL_MAX_SCALE);
  return is_valid;
}

void s21_div_by_ten(s21_decimal *value) {
  unsigned int leftover = 0;
  for (int j = 2; j >= 0; j--) {
    unsigned int cur_bit = (leftover << 31) | value->bits[j];
    value->bits[j] = cur_bit / 10;
    leftover = cur_bit % 10;
  }
}
int s21_mul_by_ten(s21_decimal *value) {
  int res = 0;
  unsigned int leftover = 0, tmp[3] = {0};
  for (int j = 0; j < 3; j++) {
    tmp[j] = value->bits[j] * 10u + leftover;
    leftover = tmp[j] >> 31;
    tmp[j] &= (1ull << 31) - 1;
  }
  if (leftover != 0)
    res = 1;
  else {
    for (int i = 0; i < 3; i++) value->bits[i] = tmp[i];
  }
  return res;
}

// -=-=-=- SCALE FUNCTIONS -=-=-=-

int s21_inc_scale(s21_decimal *value) {
  s21_decimal_set_scale(value, s21_decimal_get_scale(value) + 1);
  return s21_mul_by_ten(value);
}
void s21_dec_scale(s21_decimal *value, int shift) {
  for (int i = 0; i < shift; i++) s21_div_by_ten(value);
  s21_decimal_set_scale(value, s21_decimal_get_scale(value) - shift);
}

int s21_align_scale(s21_decimal *value1, s21_decimal *value2) {
  int high, low;
  high = s21_decimal_get_scale(value1);
  low = s21_decimal_get_scale(value2);
  if (high != low) {
    if (high < low)
      high = s21_align_scale(value2, value1);
    else {
      while (high - low++ > 0) s21_inc_scale(value2);
    }
  }
  return high;
}