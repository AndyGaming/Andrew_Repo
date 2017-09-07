#ifndef POINT_H
#define POINT_H
using namespace std;

class Point {
  public:
    int x;
    int y;
    Point () { x = y = 0; }
    Point ( int x0, int y0 ) { x = x0; y = y0; }
    void set ( int x0, int y0 ) { x = x0; y = y0; }
};

#endif
