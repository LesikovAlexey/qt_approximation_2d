#include <stdlib.h>
int method1_init(int n_appr_x, double a_x, double b_x, int n_appr_y, double a_y, double b_y, double *values, double *derivatives, double **state);
int method1_init_axis_x(int n_x, double a, double b, double *g);
int method1_init_axis_y(int n_y, double a, double b, double *g);
int matr_product(int n1, int m1, double *matr1, int n2, int m2, double *matr2, double *res);
