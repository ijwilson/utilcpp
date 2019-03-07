#include "Stirling.H"
// Note the code here is used from polpak - original code at end
// of file

Stirling::Stirling(int n):nmax(n)
{
  if ( n <= 0 ) {
    throw std::range_error("n should be greater than 1 for stirling");
  }

  mat = new double[n*n];

  mat[0] = 1;
  for (int j = 1; j < n; j++ ) {
    mat[j*n] = 0;
  }

  for ( int i = 1; i < n; i++ ) {
    mat[i] = - i * mat[i-1];
    for ( int j = 1; j < n; j++ ) {
      mat[i+j*n] = mat[i-1+(j-1)*n] - i * mat[i-1+j*n];
    }
  }
}


// int *stirling1 ( int n, int m )

// //****************************************************************************80
// //
// //  Purpose:
// //
// //    STIRLING1 computes the Stirling numbers of the first kind.
// //
// //  Discussion:
// //
// //    The absolute value of the Stirling number S1(N,M) gives the number
// //    of permutations on N objects having exactly M cycles, while the
// //    sign of the Stirling number records the sign (odd or even) of
// //    the permutations.  For example, there are six permutations on 3 objects:
// //
// //      A B C   3 cycles (A) (B) (C)
// //      A C B   2 cycles (A) (BC)
// //      B A C   2 cycles (AB) (C)
// //      B C A   1 cycle  (ABC)
// //      C A B   1 cycle  (ABC)
// //      C B A   2 cycles (AC) (B)
// //
// //    There are
// //
// //      2 permutations with 1 cycle, and S1(3,1) = 2
// //      3 permutations with 2 cycles, and S1(3,2) = -3,
// //      1 permutation with 3 cycles, and S1(3,3) = 1.
// //
// //    Since there are N! permutations of N objects, the sum of the absolute
// //    values of the Stirling numbers in a given row,
// //
// //      sum ( 1 <= I <= N ) abs ( S1(N,I) ) = N!
// //
// //  First terms:
// //
// //    N/M:  1     2      3     4     5    6    7    8
// //
// //    1     1     0      0     0     0    0    0    0
// //    2    -1     1      0     0     0    0    0    0
// //    3     2    -3      1     0     0    0    0    0
// //    4    -6    11     -6     1     0    0    0    0
// //    5    24   -50     35   -10     1    0    0    0
// //    6  -120   274   -225    85   -15    1    0    0
// //    7   720 -1764   1624  -735   175  -21    1    0
// //    8 -5040 13068 -13132  6769 -1960  322  -28    1
// //
// //  Recursion:
// //
// //    S1(N,1) = (-1)**(N-1) * (N-1)! for all N.
// //    S1(I,I) = 1 for all I.
// //    S1(I,J) = 0 if I < J.
// //
// //    S1(N,M) = S1(N-1,M-1) - (N-1) * S1(N-1,M)
// //
// //  Properties:
// //
// //    sum ( 1 <= K <= M ) S2(I,K) * S1(K,J) = Delta(I,J)
// //
// //    X_N = sum ( 0 <= K <= N ) S1(N,K) X**K
// //    where X_N is the falling factorial function.
// //
// //  Modified:
// //
// //    25 August 2004
// //
// //  Author:
// //
// //    John Burkardt
// //
// //  Parameters:
// //
// //    Input, int N, the number of rows of the table.
// //
// //    Input, int M, the number of columns of the table.
// //
// //    Output, int STIRLING1[N*M], the Stirling numbers of the first kind.
// //
// {
//   int i;
//   int j;
//   int *s1;

//   if ( n <= 0 )
//   {
//     return NULL;
//   }

//   if ( m <= 0 )
//   {
//     return NULL;
//   }
 
//   s1 = new int[n*m];

//   s1[0+0*n] = 1;
//   for ( j = 2; j <= m; j++ )
//   {
//     s1[0+(j-1)*n] = 0;
//   }

//   for ( i = 2; i <= n; i++ )
//   {
//     s1[i-1+0*n] = - ( i - 1 ) * s1[i-2+0*n];
//     for ( j = 2; j <= m; j++ )
//     {
//       s1[i-1+(j-1)*n] = s1[i-2+(j-2)*n] - ( i - 1 ) * s1[i-2+(j-1)*n];
//     }

//   }

//   return s1;
// }

