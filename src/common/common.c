#include "common.h"

#include <stdlib.h>
#include <string.h>

// Сюда скидывать функции, которые могут пригодиться в других файлах

// -=-=-=- S21_DECIMAL INTERFACE -=-=-=-

void s21_decimal_init(s21_decimal *dec) {
  memset(dec, 0, sizeof(s21_decimal));  // set 0
}

int s21_decimal_get_sign(s21_decimal *dec) {
  return (dec->bits[3] & 0x80000000) == 0x80000000;
}

int s21_decimal_get_scale(s21_decimal *dec) {
  return (dec->bits[3] >> 16) & 0xFF;
}

int s21_decimal_get_power(s21_decimal *dec) {
  int i = 95;
  if (is_s21_decimal_zero(dec)) {
    i = 0;
  } else {
    while (s21_get_bit(dec,i) == 0) {
      i--;
    }
  }
  return i;
}

void s21_decimal_set_sign(s21_decimal *dec, int sign) {
  if (sign)
    dec->bits[3] |= 0x80000000;  // 1 – negative
  else
    dec->bits[3] &= 0x7FFFFFFF;  // 0 – positive
}

void s21_decimal_set_scale(s21_decimal *dec, int scale) {
  dec->bits[3] &= 0xFF00FFFF;              // erase scale
  dec->bits[3] |= ((scale & 0xFF) << 16);  // set scale
}


// -=-=-=- ACCESSORIES -=-=-=-

int is_s21_decimal_valid(s21_decimal *dec) {
  if (!dec) return FALSE;

  int is_valid = (dec->bits[3] & 0x7F00FFFF) == 0;
  if (is_valid)
  	is_valid = (s21_decimal_get_scale(dec) <= DECIMAL_MAX_SCALE);
  return is_valid;
}

int is_s21_decimal_zero(s21_decimal *dec) {
  int result = 1;
  for (int i = 0; i < 3; i++) {
    if (dec->bits[i] != 0) {
      result = 0;
    }
  }
  return result;
}

int s21_get_bit(s21_decimal *dec, const int position) {
  return (dec->bits[position / 32] >> (position % 32)) & 1;
}

void s21_set_bit(s21_decimal *dec, const int position, const int newBit) {
  int index = position / 32;
  int bit_pos = position % 32;
  if (newBit) {
    dec->bits[index] |= (1u << bit_pos);
  } else {
    dec->bits[index] &= ~(1u << bit_pos);
  }
}

void s21_set_bit_int(unsigned int *value, const int position, const int newBit) {
  if (newBit) {
    *value |= (1u << position);
  } else {
    *value &= ~(1u << position);
  }
}