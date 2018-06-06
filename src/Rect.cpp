#include "Rect.h"

Rect::Rect(double x, double y, double size) : x(x), y(y), size(size) {}

Rect::~Rect()
{

}

Rect Rect::GetQuadrant(int q) const {
    double halfSize = size*0.5;
    if(q == 0)  //Top left
        return Rect(x, y+halfSize, halfSize);
    else if(q == 1) //Top right
        return Rect(x+halfSize, y+halfSize, halfSize);
    else if(q == 2) //Bottom right
        return Rect(x+halfSize, y, halfSize);
    else    //Bottom left
        return Rect(x, y, halfSize);
}
