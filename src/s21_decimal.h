#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

typedef struct {
  unsigned int bits[4]; // bits[0]–[2] = value, bits[3] = scale & sign
} s21_decimal;

#define DECIMAL_MAX_SCALE 28
#define DECIMAL_MIN_FLOAT 1e-28f
#define DECIMAL_MAX_FLOAT 79228162514264337593543950335.0f

// Convertors and parsers
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

// Comparison Operators
int s21_is_less(s21_decimal, s21_decimal);                // <
int s21_is_less_or_equal(s21_decimal, s21_decimal);       // <=
int s21_is_greater(s21_decimal, s21_decimal);             // >
int s21_is_greater_or_equal(s21_decimal, s21_decimal);    // >=
int s21_is_equal(s21_decimal, s21_decimal);               // ==
int s21_is_not_equal(s21_decimal, s21_decimal);           // !=

/*

// Arithmetic Operators
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result); // +
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result); // -
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result); // *
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result); // /

// Convertors and parsers
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

// Other functions
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);

*/

#endif  // S21_DECIMAL_H
