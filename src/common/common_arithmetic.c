#include "common_arithmetic.h"
#include <stdio.h>
// -=-=-=- SIMPLE ARITHMETIC -=-=-=-

int s21_decimal_add_aligned(s21_decimal value_1, s21_decimal value_2,
                            s21_decimal *result) {
  unsigned long long sum = 0;
  unsigned int leftover = 0;
  int res = 0;
  for (int i = 0; i < 3; i++) {
    sum = (unsigned long long)value_1.bits[i] + (unsigned long long)value_2.bits[i] + leftover;
    result->bits[i] = (unsigned int) (sum & 0xFFFFFFFF);
    leftover = (unsigned int)(sum >> 32);
    res = leftover ? 1 : 0;
  }
  if(res) res &= !s21_decimal_round_bank(result, leftover);
  return !res;
}

int s21_decimal_add_digit(s21_decimal *dec, int digit) {
  if (!(digit >= 0 && digit <= 9)) return FAILURE;

  s21_decimal inter_result = *dec;
  unsigned int carry = digit;
    for (int i = 0; i < 3 && carry > 0; i++) {
    unsigned int sum = inter_result.bits[i] + carry;
    carry = (sum < inter_result.bits[i]) ? 1 : 0;
    inter_result.bits[i] = sum;
  }

  if (!carry) *dec = inter_result;
  return (carry) ? FAILURE : SUCCESS;
}

int s21_decimal_sub_aligned(s21_decimal value_1, s21_decimal value_2,
                            s21_decimal *result) {
  unsigned long diff = 0;
  unsigned long borrow = 0;
  int res = 0;
  for (int i = 0; i < 3; i++) {
    diff = value_1.bits[i] - value_2.bits[i] - borrow;
    result->bits[i] = (diff & ((1u << 31) - 1));
    borrow = (diff >> (32 * 2 - 1)) & 1;
    res |= borrow ? 1 : 0;
  }
  if (res) {
    s21_decimal_round_bank(result, borrow);
  }
  return !res;
}

int s21_decimal_div_by_10(s21_decimal *value) {
  unsigned int leftover = 0;

  for (int j = 2; j >= 0; j--) {
    unsigned int cur_bit = (leftover << 31) | value->bits[j];
    value->bits[j] = cur_bit / 10;
    leftover = cur_bit % 10;
  }

  return leftover ? FAILURE : SUCCESS;
}

int s21_decimal_mul_by_10(s21_decimal *value) {
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

int s21_decimal_shift_left_n(s21_decimal *dec, int n) {
  unsigned carry = 0;

  for (int i = 0; i < n && !carry; i++) {
    for (int j = 0; j < 3; j++) {
      unsigned new_carry = (dec->bits[j] & 0x80000000) ? 1 : 0;
      dec->bits[j] = (dec->bits[j] << 1) | carry;
      carry = new_carry;
    }
  }

  return (carry) ? FAILURE : SUCCESS;
}

int s21_decimal_round_bank(s21_decimal *dec, unsigned int leftover) {
  if (!dec || leftover > 9) return FAILURE;
  int overflow = FALSE;
  if (leftover > 5) {
    overflow = s21_decimal_inc(dec);
  } else if (leftover == 5) {
    if ((dec->bits[0] & 1) != 0) overflow = s21_decimal_inc(dec);
  }
  return overflow;
}

// -=-=-=- SCALE FUNCTIONS -=-=-=-

int s21_decimal_inc_scale(s21_decimal *value) {
  s21_decimal_set_scale(value, s21_decimal_get_scale(value) + 1);
  return s21_decimal_mul_by_10(value);
}

void s21_decimal_dec_scale(s21_decimal *value, int shift) {
  for (int i = 0; i < shift; i++) s21_decimal_div_by_10(value);
  s21_decimal_set_scale(value, s21_decimal_get_scale(value) - shift);
}

int s21_decimal_align_scale(s21_decimal *value1, s21_decimal *value2) {
  int high, low;
  high = s21_decimal_get_scale(value1);
  low = s21_decimal_get_scale(value2);
  if (high != low) {
    if (high < low)
      high = s21_decimal_align_scale(value2, value1);
    else {
      while (high - low++ > 0) s21_decimal_inc_scale(value2);
    }
  }
  return high;
}