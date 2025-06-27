#include "shapes.cpp"
#include <SFML/Graphics.hpp>
// #include <algorithm>
#include <cmath>
// #include <iostream>
// #include <stdexcept>
// #include <vector>


int main() {
    Canvas<double> c;
    c.addShape(new Rectangle<double>(100, 50, 100, 100));
    c.addShape(new Square<double>(60, 300, 100));
    c.addShape(new Circle<double>(40, 500, 100));
    c.addShape(new Ellipse<double>(30, 60, 100, 300));

    c.displayCanvas();
}
