#include <stdio.h>
#include "method1_init.h"
int matr_product(int n1, int m1, double *matr1, int n2, int m2, double *matr2, double *res)
{
    if (m1 != n2)
        return -1;
    for (int i = 0; i < n1; i++)
    {
        for (int j = 0; j < m2; j++)
        {
            res[i * m2 + j] = 0;
            for (int r = 0; r < m1; r++)
            {
                res[i * m2 + j] += matr1[i * m1 + r] * matr2[r * m2 + j];
            }
        }
    }
    return 0;
}

int method1_init_axis_x(int n_x, double a, double b, double *g)
{
    double h;
    h = (b - a) / (n_x - 1);
    for (int i = 0; i < n_x; i++)
    {
        for (int j = 0; j < n_x; j++)
        {
            g[i * n_x + j] = 0;
        }
    }
    for (int i = 1; i < n_x - 1; i++)
    {
        g[i * n_x + i + 1] = 1 / (2*h);
        g[i * n_x + i - 1] = -1 / (2*h);
    }
    return 0;
}

int method1_init_axis_y(int n_y, double a, double b, double *g)
{
    double h;
    h = (b - a) / (n_y - 1);
    for (int i = 0; i < n_y; i++)
    {
        for (int j = 0; j < n_y; j++)
        {
            g[i * n_y + j] = 0;
        }
    }
    for (int i = 1; i < n_y - 1; i++)
    {
        g[(i - 1) * n_y + i] = -1 / (2*h);
        g[(i + 1) * n_y + i] = 1 / (2*h);
    }
    return 0;
}

int method1_init(int n_appr_x, double a_x, double b_x, int n_appr_y, double a_y, double b_y, double *values, double *derivatives, double **state)
{
    double *F_x = (double *)malloc(n_appr_x * n_appr_y * sizeof(double));
    double *F_y = (double *)malloc(n_appr_x * n_appr_y * sizeof(double));
    double *F_xy = (double *)malloc(n_appr_x * n_appr_y * sizeof(double));
    double *G_x = (double *)malloc(n_appr_x * n_appr_x * sizeof(double));
    double *G_y = (double *)malloc(n_appr_y * n_appr_y * sizeof(double));
    double *Gamma_x = (double *)malloc(4 * 4 * sizeof(double));
    double *Gamma_y = (double *)malloc(4 * 4 * sizeof(double));
    double *Gamma_buf = (double *)malloc(4 * 4 * sizeof(double));
    double *F_ij = (double *)malloc(4 * 4 * sizeof(double));
    method1_init_axis_x(n_appr_x, a_x, b_x, G_x);
    method1_init_axis_y(n_appr_y, a_y, b_y, G_y);
    matr_product(n_appr_x, n_appr_x, G_x, n_appr_x, n_appr_y, values, F_x);
    matr_product(n_appr_x, n_appr_y, values, n_appr_y, n_appr_y, G_y, F_y);
    matr_product(n_appr_x, n_appr_y, F_x, n_appr_y, n_appr_y, G_y, F_xy);
    for (int i = 0; i < n_appr_y; i++)
    {
        F_x[i] = derivatives[2 * i];
        F_x[n_appr_y * (n_appr_x - 1) + i] = derivatives[2 * i + 1];
        F_xy[i] = derivatives[2 * i];
        F_xy[n_appr_y * (n_appr_x - 1) + i] = derivatives[2 * i + 1];
    }
    for (int i = 0; i < n_appr_x; i++)
    {
        F_y[i * n_appr_y] = derivatives[2 * n_appr_y + 2 * i];
        F_y[i * n_appr_y + n_appr_y - 1] = derivatives[2 * n_appr_y + 2 * i + 1];
        F_xy[i * n_appr_y] = derivatives[2 * n_appr_y + 2 * i];
        F_xy[i * n_appr_y + n_appr_y - 1] = derivatives[2 * n_appr_y + 2 * i + 1];
    }
    double h_x = (b_x - a_x) / (n_appr_x - 1);
    double h_y = (b_y - a_y) / (n_appr_y - 1);
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            Gamma_x[i * 4 + j] = 0;
            Gamma_y[i * 4 + j] = 0;
            Gamma_buf[i * 4 + j] = 0;
        }
    }

    Gamma_x[0] = 1;
    Gamma_x[5] = 1;

    Gamma_x[8] = -3 / (h_x * h_x * h_x);
    Gamma_x[9] = -2 / (h_x * h_x);
    Gamma_x[10] = 3 / (h_x * h_x * h_x);
    Gamma_x[11] = -1 / (h_x * h_x);

    Gamma_x[12] = 2 / (h_x * h_x * h_x);
    Gamma_x[13] = 1 / (h_x * h_x);
    Gamma_x[14] = -2 / (h_x * h_x * h_x);
    Gamma_x[15] = 1 / (h_x * h_x);

    Gamma_y[0] = 1;
    Gamma_y[5] = 1;

    Gamma_y[2] = -3 / (h_x * h_x * h_x);
    Gamma_y[6] = -2 / (h_x * h_x);
    Gamma_y[10] = 3 / (h_x * h_x * h_x);
    Gamma_y[14] = -1 / (h_x * h_x);

    Gamma_y[3] = 2 / (h_y * h_y * h_y);
    Gamma_y[7] = 1 / (h_y * h_y);
    Gamma_y[11] = -2 / (h_y * h_y * h_y);
    Gamma_y[15] = 1 / (h_y * h_y);
    for (int i = 0; i < n_appr_x - 1; i++)
    {
        for (int j = 0; j < n_appr_y - 1; j++)
        {
            F_ij[0] = values[i * n_appr_y + j];
            F_ij[1] = F_y[i * n_appr_y + j];
            F_ij[2] = values[i * n_appr_y + j + 1];
            F_ij[3] = F_y[i * n_appr_y + j + 1];

            F_ij[4] = F_x[i * n_appr_y + j];
            F_ij[5] = F_xy[i * n_appr_y + j];
            F_ij[6] = F_x[i * n_appr_y + j + 1];
            F_ij[7] = F_xy[i * n_appr_y + j + 1];

            F_ij[8] = values[(i + 1) * n_appr_y + j];
            F_ij[9] = F_y[(i + 1) * n_appr_y + j];
            F_ij[10] = values[(i + 1) * n_appr_y + j + 1];
            F_ij[11] = F_y[(i + 1) * n_appr_y + j + 1];

            F_ij[12] = F_x[(i + 1) * n_appr_y + j];
            F_ij[13] = F_xy[(i + 1) * n_appr_y + j];
            F_ij[14] = F_x[(i + 1) * n_appr_y + j + 1];
            F_ij[15] = F_xy[(i + 1) * n_appr_y + j + 1];
            matr_product(4, 4, Gamma_x, 4, 4, F_ij, Gamma_buf);
            matr_product(4, 4, Gamma_buf, 4, 4, Gamma_y, state[i * n_appr_y + j]);
        }
    }
    for (int i = 0; i < n_appr_y; i++)
    {
        state[(n_appr_x - 1) * n_appr_y + i][0] = values[(n_appr_x - 1) * n_appr_y + i];
    }

    for (int i = 0; i < n_appr_x; i++)
    {
        state[i * n_appr_y + n_appr_y - 1][0] = values[i * n_appr_y + n_appr_y - 1];
    }
    free(F_x);
    free(F_y);
    free(F_xy);
    free(G_x);
    free(G_y);
    free(Gamma_x);
    free(Gamma_y);
    free(Gamma_buf);
    free(F_ij);
    return 0;
}