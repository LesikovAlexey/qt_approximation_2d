#include "method_compute.h"
double method_compute(int n_x, double a_x, double b_x, double x, int n_y, double a_y, double b_y, double y, double **state)
{
    int buf_x = 0, buf_y = 0;
    double res = 0;
    double h_x;
    double h_y;
    h_x = (b_x - a_x) / (n_x - 1);
    h_y = (b_y - a_y) / (n_y - 1);
    for (int i = 0; i < n_x; i++)
    {
        for (int j = 0; j < n_y; j++)
        {
            if (x + 1.e-6 < a_x + (i + 1) * h_x && x + 1.e-6 > a_x + i * h_x && y + 1.e-6 < a_y + (j + 1) * h_y && y + 1.e-6 > a_y + j * h_y)
            {
                buf_x = i;
                buf_y = j;
                break;
            }
        }
    }
    
    for (int i = 0; i < 4; i++) 
    {
        for(int j = 0; j < 4; j++)
        {
            res += state[buf_x * n_y + buf_y][i * 4 + j] * pow((x - (a_x + h_x * (double)buf_x)), i) * pow((y - (a_y + h_y * (double)buf_y)), j);
            // if (x < -9.5f && x > -10.5f && y < -9.5f && y > -10.5f) printf("i = %d, j = %d, state = %lf, res = %lf, pow1 = %lf = %lf - (%lf + %lf * %d), pow2 = %lf = %lf - (%lf + %lf * %d)\n", i, j, state[buf_x * n_y + buf_y][i * 4 + j], res, pow((x - (a_x + h_x * buf_x)), i), x, a_x, h_x, buf_x, pow(y - (a_y + h_y * buf_y), j), y, a_y, h_y, buf_y);
            // if (x < -9.5f && x > -10.5f && y < 10.5f && y > 9.5f) printf("i = %d, j = %d, state = %lf, res = %lf, pow1 = %lf = %lf - (%lf + %lf * %d), pow2 = %lf = %lf - (%lf + %lf * %d)\n", i, j, state[buf_x * n_y + buf_y][i * 4 + j], res, pow((x - (a_x + h_x * buf_x)), i), x, a_x, h_x, buf_x, pow(y - (a_y + h_y * buf_y), j), y, a_y, h_y, buf_y);
            if (x < 0.5f && x > -0.5f && y < 10.5f && y > 9.5f) printf("i = %d, j = %d, state = %lf, res = %lf, pow1 = %lf = %lf - (%lf + %lf * %d), pow2 = %lf = %lf - (%lf + %lf * %d)\n", i, j, state[buf_x * n_y + buf_y][i * 4 + j], res, pow((x - (a_x + h_x * buf_x)), i), x, a_x, h_x, buf_x, pow(y - (a_y + h_y * buf_y), j), y, a_y, h_y, buf_y);

        }
    }
    return res;
}