#ifndef COMMON_H
#define COMMON_H

#include "../s21_decimal.h"
#include "common_arithmetic.h"

// -=-=-=- RESPONSE CODES -=-=-=-

#define SUCCESS 0
#define FAILURE 1
#define TRUE 1
#define FALSE 0
#define S21_TOO_BIG 1
#define S21_TOO_SMALL 2
#define S21_DIV_BY_ZERO 3

// -=-=-=- S21_DECIMAL INTERFACE -=-=-=-

void *s21_decimal_init(s21_decimal *dst);
int s21_decimal_get_sign(s21_decimal *decimal);
int s21_decimal_get_scale(s21_decimal *dec);
int s21_decimal_get_power(s21_decimal *dec);
void s21_decimal_set_sign(s21_decimal *dst, int sign);
void s21_decimal_set_scale(s21_decimal *dst, int scale);

// -=-=-=- ACCESSORIES -=-=-=-

int is_s21_decimal_valid(s21_decimal *dec);
int is_s21_decimal_zero(s21_decimal *dec);

#define s21_get_sign(arg) ((arg < 0) ? 1 : 0) // 0 – positive
#define s21_abs(arg) ((arg < 0) ? -arg : arg)
#define s21_min(a, b) ((a < b) ? a : b)
#define s21_max(a, b) ((a > b) ? a : b)

int s21_get_bit(s21_decimal *dec, const int position);
void s21_set_bit(s21_decimal *dec, const int position, const int newBit);
void s21_set_bit_int(unsigned int *value, const int position, const int newBit);

#endif //COMMON_H
