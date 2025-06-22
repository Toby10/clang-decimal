#ifndef COMMON_H
#define COMMON_H

#include "../s21_decimal.h"

// -=-=-=- RESPONSE CODES -=-=-=-

#define SUCCESS 0
#define FAILURE 1
#define TRUE 1
#define FALSE 0

#define S21_TOO_BIG 1
#define S21_TOO_SMALL 2

// -=-=-=- S21_DECIMAL INTERFACE -=-=-=-

void s21_decimal_init(s21_decimal *dst);
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

int s21_get_bit(s21_decimal dec, const int position);
void s21_set_bit(s21_decimal *dec, const int position, const int newBit);
void s21_set_bit_int(unsigned int *value, const int position, const int newBit);

void s21_div_by_ten(s21_decimal *value);
int s21_decimal_mul_by_two(s21_decimal *dst);
int s21_inc_scale(s21_decimal* value);
void s21_dec_scale(s21_decimal* value, int shift);
int s21_align_scale(s21_decimal* value1, s21_decimal* value2);

#endif //COMMON_H

