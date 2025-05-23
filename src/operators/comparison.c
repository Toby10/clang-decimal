#include "../s21_decimal.h"
#include "../common/common.h"

int s21_is_equal(s21_decimal num1, s21_decimal num2) {
    int result = 1;
    if (!(is_s21_decimal_zero(&num1) && is_s21_decimal_zero(&num2))) {
        for (int i = 0; i < 96; i++) {
            if (s21_get_bit(&num1, i) != s21_get_bit(&num2, i)) {
                result = 0;
            }
        }
        if (s21_decimal_get_scale(&num1) != s21_decimal_get_scale(&num2)) {
            result = 0;
        }
        if (s21_decimal_get_sign(&num1) != s21_decimal_get_sign(&num2)) {
            result = 0;
        }
    }
    return result;
}

int s21_is_less(s21_decimal num1, s21_decimal num2) {
    int result = -1;
    int power1 = s21_decimal_get_power(&num1), power2 = s21_decimal_get_power(&num2);
    int exp1 = s21_decimal_get_scale(&num1), exp2 = s21_decimal_get_scale(&num2);
    if (s21_is_equal(num1, num2)) {
        result = 0;
    } else if (s21_decimal_get_sign(&num1) ^ s21_decimal_get_sign(&num2)) {
        result = s21_decimal_get_sign(&num1);
    } else {
        if (power1 - exp1 < power2 - exp2) {
            result = 1;
        } else if (power1 - exp1 > power2 - exp2) {
            result = 0;
        } else if (power1 - exp1 == power2 - exp2) {
            int i = power1, j = power2;
            while (i > -1 && j > -1 && s21_get_bit(&num1, i) == s21_get_bit(&num2, j)) {
                i--;
                j--;
            }
            if (s21_get_bit(&num1, i) == s21_get_bit(&num2, j)) {
                result = power1 < power2;
            } else {
                result = s21_get_bit(&num2, j);
            }
        }
        result ^= s21_decimal_get_sign(&num1);
    }
    return result;
}

int s21_is_less_or_equal(s21_decimal num1, s21_decimal num2) {
    return s21_is_equal(num1, num2) || s21_is_less(num1, num2);
}

int s21_is_greater(s21_decimal num1, s21_decimal num2) {
    return !(s21_is_equal(num1, num2)) && !(s21_is_less(num1, num2));
}

int s21_is_greater_or_equal(s21_decimal num1, s21_decimal num2) {
    return !(s21_is_less(num1, num2));
}

int s21_is_not_equal(s21_decimal num1, s21_decimal num2) {
    return !(s21_is_equal(num1, num2));
}
