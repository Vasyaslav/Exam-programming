#include <QVector>

void sift(QVector<double> &A, int i, int m)
{
  int j = i, k = i*2+1;	 // левый сын
  while (k <= m)
  {
    if (k<m && A[k]<A[k+1]) k++; // больший сын
    if (A[j] < A[k]){
        double c;
        c = A[j];
        A[j] = A[k];
        A[k] = c;
        j = k; k = k*2+1;
    }
    else break;
  }
}

void heap_sort(QVector<double> &A, int n, int steps)
{
  int i, m;
  // построение пирамиды
  for (i = n/2; i >= 0; i--)
    sift(A, i, n-1);
  // сортировка массива
  for (m = n-1; m >= 1; m--)
  {
    double c;
    c = A[0];
    A[0] = A[m];
    A[m] = c;
    sift(A, 0, m-1);
    if (n - m == steps)
        break;
  }
}
