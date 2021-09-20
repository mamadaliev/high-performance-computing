#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
// vvod razmernosti n
int n,m,w;
cout << "Vvedite razmer matrizi n: ";
cin >> n;
m=n+n;
w=n+1;

float array1[n][m];
float array2[n][w];

float a,b;
int i,j,k,l;

// vvod elementov matrizi n*n
for (i = 1; i <= n; i++)
{
for (j = 1; j <= n+1; j++)
{
cout << "array[" << i << "," << j << "]=" ;
cin >> array1[i][j];
array2[i][j]=array1[i][j];
}
}

//dopolnenie k osnovnoi matrize
for (i = 1; i <= n; i++)
{
for (j = 1; j <= n; j++)
{
if (j==i)
{
array1[i][j+n]=1;
}
else
{
array1[i][j+n]=0;
}
}
}


// pruamoi hod
for (k = 1; k < n; k++)
{
//poisk i perestanovka nenulevih elementov
while (array1[k][k]==0)
{
l=k;
do
{
l=k+1;
if (l>n) goto end; //vihod esli v stolbze vse nuli
}
while (array1[l][k]==0);

for (j = k; j <= m; j++)
{
b = array1[k][j];
array1[k][j]=array1[l][j];
array1[l][j]= b;
}
}

// privedenie k treugolnomu vidu
a=array1[k][k];
for (i = k+1; i <= n; i++)
{
b=array1[i][k]/a;
for (j = k; j <= m; j++)
{
array1[i][j]= array1[i][j] - (b*array1[k][j]);
}
}
}

// obratniy hod

for (k = n; k >= 1; k--)
{
if (array1[k][k]==0) goto end; // vihod esli sistema he imeet reshenia
b=array1[k][k] / array1[k][k];
for (i = k-1; i >= 1; i--)
{
a=array1[i][k] / array1[k][k];
for (j = n+1; j <= m; j++)
{
array1[i][j]= array1[i][j]-(array1[k][j]*a);
}
array1[i][k]= array1[i][k]-(array1[i][k]*b);
}
}



// vichislenie opredelitelua osnovnoy matrizi

a=1;
for (i = 1; i <= n; i++)
{
a=a*array1[i][i];
}

cout << "det=" << a << endl;


// normirovka

for (i = 1; i <= n; i++)
{
for (j = n+1; j <= m; j++)
{
array1[i][j]=array1[i][j]/array1[i][i];
}
array1[i][i]=array1[i][i]/array1[i][i];
}



// peremeshenie

for (i = 1; i <= n; i++)
{
for (j = 1; j <= n; j++)
{
a=array1[i][j];
array1[i][j]=array1[i][j+n];
array1[i][j+n]=a;
}
}

// umnozenie

for (i = 1; i <= n; i++)
{
a=0;
for (j = 1; j <= n; j++)
{
a=a+array1[i][j]*array2[j][n+1];
}
array1[i][n+1]=a;
}


// vivod reshenia

cout << "reshenie" << endl;

for (i = 1; i <= n; i++)
{
cout << "x[" << i << "]=" << array1[i][n+1] << endl;
}

//proverka-esli vse nuli to pravilno
cout << "test" << endl;

for (i = 1; i <= n; i++)
{
a=0;
for (j = 1; j <= n; j++)
{
a=a+array2[i][j]*array1[j][n+1];
}
b=a-array2[i][n+1];
cout << "test[" << i << "]=" << b << endl;
}


//cout << "test" << endl;

//for (i = 1; i <= n; i++)
//{
//for (j = 1; j <= n; j++)
//{
//cout << "array1[" << i << "," << j << "]=" << array1[i][j] << endl;
//}
//}

// konez

end: system("pause");
return 0;
}
