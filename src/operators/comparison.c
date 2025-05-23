#include "../s21_decimal.h"

/*
typedef struct {
  unsigned int bits[4];
} s21_decimal;

int s21_is_less(s21_decimal, s21_decimal);                // <
int s21_is_less_or_equal(s21_decimal, s21_decimal);       // <=
int s21_is_greater(s21_decimal, s21_decimal);             // >
int s21_is_greater_or_equal(s21_decimal, s21_decimal);    // >=
int s21_is_equal(s21_decimal, s21_decimal);               // ==
int s21_is_not_equal(s21_decimal, s21_decimal);           // !=
 */


//вынести в common
int s21_get_bit(const s21_decimal value, const int position) {
    return (value.bits[position / 32] >> (position % 32)) & 1;
}

void s21_set_bit(s21_decimal *value, const int position, const int newBit) {
    int index = position / 32;
    int bit_pos = position % 32;
    if (newBit) {
        value->bits[index] |= (1u << bit_pos);
    } else {
        value->bits[index] &= ~(1u << bit_pos);
    }
}

void s21_set_bit_int(unsigned int *value, const int position, const int newBit) {
    if (newBit) {
        *value |= (1u << position);
    } else {
        *value &= ~(1u << position);
    }
}


int s21_is_zero(const s21_decimal value) {
    int result = 1;
    for (int i = 0; i < 3; i++) {
        if (value.bits[i] != 0) {
            result = 0;
        }
    }
    return result;
}

int s21_get_exp(const s21_decimal value) {
    return (value.bits[3] >> (16)) & 255;
}

int s21_is_negative(const s21_decimal value) {
    return (value.bits[3] >> (31)) & 1;
}

int s21_calc_power(const s21_decimal value) {
    int i = 95;
    if (s21_is_zero(value)) {
        i = 0;
    } else {
        while (s21_get_bit(value,i) == 0) {
            i--;
        }
    }
    return i;
}


//оставить тут
int s21_is_equal(s21_decimal num1, s21_decimal num2) {
    int result = 1;
    if (!(s21_is_zero(num1) && s21_is_zero(num2))) {
        for (int i = 0; i < 96; i++) {
            if (s21_get_bit(num1, i) != s21_get_bit(num2, i)) {
                result = 0;
            }
        }
        if (s21_get_exp(num1) != s21_get_exp(num2)) {
            result = 0;
        }
        if (s21_is_negative(num1) != s21_is_negative(num2)) {
            result = 0;
        }
    }
    return result;
}

int s21_is_less(s21_decimal num1, s21_decimal num2) {
    int result = -1;
    int power1 = s21_calc_power(num1), power2 = s21_calc_power(num2);
    int exp1 = s21_get_exp(num1), exp2 = s21_get_exp(num2);
    if (s21_is_equal(num1, num2)) {
        result = 0;
    } else if (s21_is_negative(num1) ^ s21_is_negative(num2)) {
        result = s21_is_negative(num1);
    } else {
        if (power1 - exp1 < power2 - exp2) {
            result = 1;
        } else if (power1 - exp1 > power2 - exp2) {
            result = 0;
        } else if (power1 - exp1 == power2 - exp2) {
            int i = power1, j = power2;
            while (i > -1 && j > -1 && s21_get_bit(num1, i) == s21_get_bit(num2, j)) {
                i--;
                j--;
            }
            if (s21_get_bit(num1, i) == s21_get_bit(num2, j)) {
                result = power1 < power2;
            } else {
                result = s21_get_bit(num2, j);
            }
        }
        result ^= s21_is_negative(num1);
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

//временные тесты
#include <stdio.h>


// Тесты для s21_is_equal
int test_equal_zeros() {
    s21_decimal a = {{0, 0, 0, 0}};  // Все биты = 0
    s21_decimal b = {{0, 0, 0, 0}};

    // Установка масштабов (биты 16-23)
    a.bits[3] = 2 << 16;  // scale = 2
    b.bits[3] = 5 << 16;  // scale = 5

    int result = s21_is_equal(a, b);
    if (result != 1) {
        printf("Test equal_zeros failed: expected 1, got %d\n", result);
        return 1;
    }
    return 0;
}

int test_equal_diff_sign() {
    s21_decimal a = {{123, 0, 0, 0}};
    s21_decimal b = {{123, 0, 0, 0}};

    // Установка масштаба 2 и разных знаков
    a.bits[3] = (2 << 16) | (0 << 31);  // scale=2, positive
    b.bits[3] = (2 << 16) | (1 << 31);  // scale=2, negative

    int result = s21_is_equal(a, b);
    if (result != 0) {
        printf("Test equal_diff_sign failed: expected 0, got %d\n", result);
        return 1;
    }
    return 0;
}

// Тесты для s21_is_less
int test_less_positive() {
    s21_decimal a = {{1, 0, 0, 0}};  // 1.0
    s21_decimal b = {{2, 0, 0, 0}};  // 2.0

    a.bits[3] = 1 << 16;  // scale=1
    b.bits[3] = 1 << 16;

    int result = s21_is_less(a, b);
    if (result != 1) {
        printf("Test less_positive failed: expected 1, got %d\n", result);
        return 1;
    }
    return 0;
}

int test_less_negative() {
    s21_decimal a = {{2, 0, 0, 0}};
    s21_decimal b = {{1, 0, 0, 0}};

    // Оба отрицательные, масштаб=0
    a.bits[3] = (1 << 31);  // -2
    b.bits[3] = (1 << 31);  // -1

    int result = s21_is_less(a, b);
    if (result != 1) {
        printf("Test less_negative failed: expected 1, got %d\n", result);
        return 1;
    }
    return 0;
}

int test_equal_max_values() {
    s21_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
    s21_decimal b = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
    a.bits[3] = 0 << 16;    // max value, scale 0
    b.bits[3] = 5 << 16;    // max value, scale 5
    int result = s21_is_equal(a, b);
    if (result != 0) {  // Разные масштабы -> разные значения
        printf("Test equal_max_values failed: got %d\n", result);
        return 1;
    }
    return 0;
}

int test_less_negative_with_scale() {
    s21_decimal a = {{1234, 0, 0, 0}};   // -1.234 (scale=3)
    s21_decimal b = {{12345, 0, 0, 0}};  // -1.2345 (scale=4)
    a.bits[3] = (3 << 16) | (1u << 31);
    b.bits[3] = (4 << 16) | (1u << 31);
    int result = s21_is_less(a, b);      // -1.234 > -1.2345 -> false
    if (result != 0) {
        printf("Test less_negative_with_scale failed: %d\n", result);
        return 1;
    }
    return 0;
}

int test_equal_high_bits() {
    s21_decimal a = {{0, 0xABCDEF12, 0x12345678, 0}};
    s21_decimal b = {{0, 0xABCDEF12, 0x12345678, 0}};
    a.bits[3] = 10 << 16;
    b.bits[3] = 10 << 16;
    int result = s21_is_equal(a, b);  // Значения и масштабы совпадают
    if (result != 1) {
        printf("Test equal_high_bits failed: %d\n", result);
        return 1;
    }
    return 0;
}

int test_less_overflow_risk() {
    s21_decimal a = {{0xFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}}; // Почти max
    s21_decimal b = {{1, 0, 0, 0}};                           // 1.0
    a.bits[3] = 1 << 16;    // scale=1 (очень большое число)
    b.bits[3] = 28 << 16;   // scale=28 (маленькое число)
    int result = s21_is_less(a, b);  // 7922816251426433759354395033.5 < 0.000000001?
    if (result != 0) {               // Ожидаем 0 (false)
        printf("Test less_overflow_risk failed: %d\n", result);
        return 1;
    }
    return 0;
}

int test_equal_negative_zero() {
    s21_decimal a = {{0, 0, 0, 0}};
    s21_decimal b = {{0, 0, 0, 0}};
    a.bits[3] = (1u << 31);  // -0
    b.bits[3] = 0;           // +0
    int result = s21_is_equal(a, b);  // -0 == +0?
    if (result != 1) {                // Зависит от реализации, обычно 1
        printf("Test equal_negative_zero failed: %d\n", result);
        return 1;
    }
    return 0;
}

int test_less_extreme_scales() {
    s21_decimal a = {{123, 0, 0, 0}};    // 0.000000000000000000000000123 (scale=28)
    s21_decimal b = {{1230, 0, 0, 0}};   // 0.000000000000000000000001230 (scale=28)
    a.bits[3] = 28 << 16;
    b.bits[3] = 28 << 16;
    int result = s21_is_less(a, b);  // 123 < 1230
    if (result != 1) {
        printf("Test less_extreme_scales failed: %d\n", result);
        return 1;
    }
    return 0;
}

int test_less_sign_and_scale() {
    s21_decimal a = {{100, 0, 0, 0}};   // -1.00 (scale=2)
    s21_decimal b = {{10000, 0, 0, 0}}; // +0.10000 (scale=5)
    a.bits[3] = (2 << 16) | (1u << 31);
    b.bits[3] = 5 << 16;
    int result = s21_is_less(a, b);  // -1 < 0.1 -> true
    if (result != 1) {
        printf("Test less_sign_and_scale failed: %d\n", result);
        return 1;
    }
    return 0;
}



int test_less_mixed_bits() {
    s21_decimal a = {{0x11111111, 0x22222222, 0x0, 0}};
    s21_decimal b = {{0x11111111, 0x22222222, 0x1, 0}};
    a.bits[3] = 2 << 16;
    b.bits[3] = 2 << 16;
    int result = s21_is_less(a, b);  // 0x2222222211111111 < 0x12222222211111111
    if (result != 1) {
        printf("Test less_mixed_bits failed: %d\n", result);
        return 1;
    }
    return 0;
}

int main() {
    int fail_count = 0;
    fail_count += test_equal_zeros();
    fail_count += test_equal_diff_sign();
    fail_count += test_less_positive();
    fail_count += test_less_negative();
    fail_count += test_equal_max_values();
    fail_count += test_less_negative_with_scale();
    fail_count += test_equal_high_bits();
    fail_count += test_less_overflow_risk();
    fail_count += test_equal_negative_zero();
    fail_count += test_less_extreme_scales();
    fail_count += test_less_sign_and_scale();
    fail_count += test_less_mixed_bits();

    printf("Total failed tests: %d\n", fail_count);
    return fail_count;
}