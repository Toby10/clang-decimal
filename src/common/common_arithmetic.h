#ifndef ARITHMETIC_H
#define ARITHMETIC_H

#include "common.h"

// -=-=-=- SIMPLE ARITHMETIC -=-=-=-

int s21_decimal_add_aligned(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);
int s21_decimal_add_digit(s21_decimal *dec, int digit);
int s21_decimal_sub_aligned(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_decimal_div_by_10(s21_decimal *value);
int s21_decimal_mul_by_10(s21_decimal *value);
int s21_decimal_shift_left_n(s21_decimal *dec, int n);

// -=-=-=- SCALE FUNCTIONS -=-=-=-

int s21_decimal_inc_scale(s21_decimal* value);
void s21_decimal_dec_scale(s21_decimal* value, int shift);
int s21_decimal_align_scale(s21_decimal* value1, s21_decimal* value2);

#endif