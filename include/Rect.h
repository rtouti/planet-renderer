#ifndef RECT_H
#define RECT_H


struct Rect {
    Rect(double x, double y, double size);
    ~Rect();
    Rect GetQuadrant(int q) const;

    double x, y, size;
};

#endif // RECT_H
