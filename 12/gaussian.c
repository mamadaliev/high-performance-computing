#include <stdio.h>
#include <conio.h>
#include <math.h>
#define N 4

int main(void)
{
    const int n=4;
    double r;
    int i, j, k, z, max, tmp;
    int masI[n];  //Массив индексов
 
    double mas[n][n + 1] =
    { { 45, 5, 7, 7, 3 },
      { 2, 0, 4, 4, 6 },
      { 7, 56, 7, -5, 2 },
      { 6000, 3, 4, 6, 1 }};
 
    for(i=0; i<n; ++i) masI[i]= i;
    //Вывод введенной системы
 
    printf("Sistema\n");
    for (i = 0; i < n; i++)
    {
     for (j = 0; j < n + 1; j++) printf("%7.2f ", mas[i][j]);
     printf("\n");
    };
 
    for (k = 0; k < n; k++)
    {
        max = fabs(mas[masI[k]][k]);
        z = k;
        for (int i = k + 1; i < n; i++)
            if (fabs(mas[masI[i]][k]) > max)
            {
             max = fabs(mas[masI[i]][k]);
             z = i;
            };
 
        if (z!=k)
        {
         tmp= masI[k];
         masI[k]= masI[z];
         masI[z]= tmp;
        };
 
        r= mas[masI[k]][k];
        for (j = k; j < n+1; j++) mas[masI[k]][j]/= r;
        for (i = 0; i < n; i++)
        {
         r = mas[i][k];
         if(i!=masI[k]) for (j = k; j < n+1; j++) mas[i][j]-= mas[masI[k]][j] * r;
        };
    };
 
    for (i = 0; i < n; i++)
    {
     for (j = 0; j < n + 1; j++) printf("%7.4f ", mas[masI[i]][j]);
     printf("\n");
    };
 
    _getch();
}
