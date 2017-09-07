




#include <iostream>

using namespace std;

void buildKnots ( int m, int n, float knot[] )
{
  if ( n < m  ) return;         //not enough control points
  for ( int i = 0; i < n + m; ++i ){
    if (i < m) knot[i] = 0.0;
    else if (i < n) knot[i] = i - m + 1;        //i is at least m here
    else knot[i] = n - m + 1;
  }
}

float bSpline ( int k, int m, float u, float knot[] )
{
  float d1, d2, sum = 0.0;

  if ( m == 1 )
    return ( knot[k] < u &&  u <= knot[k+1] );   //1 or 0

  //m larger than 1, so recurse
   d1 = knot[k+m-1] - knot[k];
  if ( d1 != 0 )
    sum = (u - knot[k]) * bSpline(k,m-1,u, knot) / d1;
  d2 = knot[k+m] - knot[k+1];
  if ( d2 != 0 )
    sum += (knot[k+m] - u) * bSpline(k+1, m-1, u, knot) / d2;

  return sum;
}

int main()
{
  float knots[12];

  buildKnots (4, 8, knots);

  for (int i = 0; i < 12; i++)
    cout << knots[i] << " " << endl;

  cout << endl << bSpline(5, 4, 0.8, knots);

  cout << endl << endl;
}
