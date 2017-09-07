//draw.h

#ifndef DRAW_H
#define DRAW_H

class DrawLine {
  public:
    DrawLine( SDL_Surface *screen,  int x0, int y0 );
    void moveTo ( int x, int y );
    void lineTo( int x1, int y1, Uint16 color);
  private:
    int x0, y0;         //current position
    SDL_Surface *surf;
};

#endif
