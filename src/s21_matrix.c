/* create_matrix (создание)
   remove_matrix (очистка и уничтожение)
   eq_matrix (сравнение)
   sum_matrix (сложение)
   sub_matrix (вычитание)
   mult_matrix (умножение)
   mult_number (умножение на число)
   transpose (транспонирование)
   determinant (вычисление определителя)
   calc_complements (вычисление матрицы алгебраических дополнений)
   inverse_matrix (поиск обратной матрицы)
   */

// Проверяемая точность дробной части - максимум 6 знаков после запятой.

/*Сравнение матриц (eq_matrix)
#define SUCCESS 1
#define FAILURE 0*/

/*
Все операции (кроме сравнения матриц) должны возвращать результирующий код:

0 - OK
1 - Ошибка, некорректная матрица
2 - Ошибка вычисления (несовпадающие размеры матриц; матрица, для которой нельзя
провести вычисления и т.д.)
*/

#include "s21_matrix.h"

// create_matrix (создание)
int s21_create_matrix(int rows, int columns, matrix_t *result) {
  int error = -1;  // Код ошибки, который мы будем возвращать
  if (rows < 1 || columns < 1) {
    error = 1;
  } else {
    result->rows = rows;
    result->columns = columns;
    result->matrix = (double **)calloc(rows, sizeof(double *));
  }
  if (result->matrix != NULL) {
    for (int row = 0; row < rows; row++) {
      result->matrix[row] = (double *)calloc(columns, sizeof(double));
    }
    error = 0;
  }
  return error;
}

// remove_matrix (очистка и уничтожение)
void s21_remove_matrix(matrix_t *A) {
  if (A->matrix != NULL) {
    for (int row = 0; row < A->rows; row++) {
      free(A->matrix[row]);
    }
    free(A->matrix);
  }
  A->rows = 0;
  A->columns = 0;
  A->matrix = NULL;
}

// eq_matrix (сравнение)
int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  int error = SUCCESS;

  if (s21_is_eq_size(*A, *B) == 1 && A->matrix != NULL && B->matrix != NULL) {
    int r = A->rows;
    int c = A->columns;
    for (int row = 0; row < r; ++row) {
      for (int col = 0; col < c; ++col) {
        if (fabs(A->matrix[row][col] - B->matrix[row][col]) >= 0.0000001) {
          error = FAILURE;
        }
      }
    }
  } else {
    error = FAILURE;
  }
  return error;
}

// sum_matrix (сложение)
int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int error = -1;
  if (s21_is_eq_size(*A, *B) == 1 && s21_check_matrix(A) == 0 &&
      s21_check_matrix(B) == 0 &&
      s21_create_matrix(A->rows, A->columns, result) == 0) {
    int r = A->rows;
    int c = A->columns;
    for (int row = 0; row < r; ++row) {
      for (int col = 0; col < c; ++col) {
        result->matrix[row][col] = A->matrix[row][col] + B->matrix[row][col];
      }
    }
    error = 0;
  } else if (s21_check_matrix(A) == 1 || s21_check_matrix(B) == 1) {
    error = 1;
  } else {
    error = 2;
  }
  return error;
}

// sub_matrix (вычитание)
int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int error = -1;
  if (s21_is_eq_size(*A, *B) == 1 && s21_check_matrix(A) == 0 &&
      s21_check_matrix(B) == 0 &&
      s21_create_matrix(A->rows, A->columns, result) == 0) {
    int r = A->rows;
    int c = A->columns;
    for (int row = 0; row < r; row++) {
      for (int col = 0; col < c; col++) {
        result->matrix[row][col] = A->matrix[row][col] - B->matrix[row][col];
      }
    }
    error = 0;
  } else if (s21_check_matrix(A) == 1 || s21_check_matrix(B) == 1) {
    error = 1;
  } else {
    error = 2;
  }
  return error;
}

// mult_number (умножение на число)
int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  int error = -1;
  if (s21_check_matrix(A) == 0 && A->rows > 0 && A->columns > 0 &&
      s21_create_matrix(A->rows, A->columns, result) == 0) {
    int r = A->rows;
    int c = A->columns;
    for (int row = 0; row < r; row++) {
      for (int col = 0; col < c; col++) {
        result->matrix[row][col] = A->matrix[row][col] * number;
      }
    }
    error = 0;
  } else {
    error = 1;
  }
  return error;
}

// mult_matrix (умножение)
int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int error = -1;
  if (A->columns != B->rows && s21_check_matrix(A) == 0 &&
      s21_check_matrix(B) == 0) {
    error = 2;
  } else if (s21_check_matrix(A) == 1 || s21_check_matrix(B) == 1) {
    error = 1;
  } else if (A->columns == B->rows && A->matrix != NULL && B->matrix != NULL &&
             s21_create_matrix(A->rows, B->columns, result) == 0) {
    int k = A->columns;
    for (int row = 0; row < result->rows; ++row) {
      for (int col = 0; col < result->columns; ++col) {
        for (int m = 0; m < k; ++m) {
          result->matrix[row][col] += A->matrix[row][m] * B->matrix[m][col];
        }
      }
    }
    error = 0;
  }
  return error;
}

// transpose (транспонирование)
int s21_transpose(matrix_t *A, matrix_t *result) {
  int error = -1;
  if (s21_check_matrix(A) == 0 && A->rows > 0 && A->columns > 0 &&
      s21_create_matrix(A->columns, A->rows, result) == 0) {
    for (int row = 0; row < result->rows; row++) {
      for (int col = 0; col < result->columns; col++) {
        result->matrix[row][col] = A->matrix[col][row];
      }
    }
    error = 0;
  } else {
    error = 1;
  }
  return error;
}

// determinant (вычисление определителя)
int s21_determinant(matrix_t *A, double *result) {
  int error = -1;
  if (A->rows != A->columns && A->matrix != NULL) {
    error = 2;
  } else if (s21_check_matrix(A) == 0) {
    *result = s21_calc_det(A);
    error = 0;
  } else {
    error = 1;
  }
  return error;
}

int s21_calc_complements(matrix_t *A, matrix_t *result) {
  int error = -1;
  if (A->rows != A->columns && A->matrix != NULL) {
    error = 2;
  } else if (s21_check_matrix(A) == 0 &&
             s21_create_matrix(A->rows, A->columns, result) == 0) {
    s21_calc_minor(A, result);
    for (int row = 0; row < result->rows; row++) {
      for (int col = 0; col < result->columns; col++) {
        result->matrix[row][col] *= pow(-1, (row + col));
      }
    }
    error = 0;
  } else {
    error = 1;
  }
  return error;
}

// inverse_matrix (поиск обратной матрицы)
int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  int error = -1;
  if (s21_check_matrix(A)) {
    error = 1;
  } else {
    double det_A;
    if (s21_determinant(A, &det_A) || det_A == 0) {
      error = 2;
    } else {
      matrix_t M;
      matrix_t M_transpose;
      if (s21_calc_complements(A, &M) || s21_transpose(&M, &M_transpose)) {
        error = 2;
      } else {
        if (s21_mult_number(&M_transpose, 1.0 / det_A, result)) {
          error = 2;
        }
      }
      s21_remove_matrix(&M_transpose);
      s21_remove_matrix(&M);
    }
  }
  return error;
}
