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
  if (is_valid)
  	is_valid = (s21_decimal_get_scale(dec) <= DECIMAL_MAX_SCALE);
  return is_valid;
}