#ifndef COMMON_H
#define COMMON_H

#include "../s21_decimal.h"

// -=-=-=- RESPONSE CODES -=-=-=-

#define SUCCESS 0
#define FAILURE 1
#define TRUE 1
#define FALSE 0

// -=-=-=- S21_DECIMAL INTERFACE -=-=-=-

void s21_decimal_init(s21_decimal *dst);
void s21_decimal_set_sign(s21_decimal *dst, int sign);
void s21_decimal_set_scale(s21_decimal *dst, int scale);
int s21_decimal_get_sign(s21_decimal *decimal);
int s21_decimal_get_scale(s21_decimal *dec);

// -=-=-=- ACCESSORIES -=-=-=-

int is_s21_decimal_valid(s21_decimal *dec);
#define s21_get_sign(arg) ((arg < 0) ? 1 : 0) // 0 – positive
#define s21_abs(arg) ((arg < 0) ? -arg : arg)


#endif //COMMON_H
