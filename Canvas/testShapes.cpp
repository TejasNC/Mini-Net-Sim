#include "shapes.cpp"
#include <iostream>
#include <cmath>

int main() {
    Canvas<double> canvas;

    // Create shapes dynamically
    Circle<double>* c1 = new Circle<double>(5.0, 10, 10);
    Ellipse<double>* e1 = new Ellipse<double>(3.0, 4.0, 2, 3);
    Rectangle<double>* r1 = new Rectangle<double>(4.0, 6.0, 5, 5);
    Square<double>* s1 = new Square<double>(3.0, 0, 0);

    // Add shapes to the canvas
    canvas.addShape(c1);
    canvas.addShape(e1);
    canvas.addShape(r1);
    canvas.addShape(s1);

    std::cout << "Initial Canvas Contents:\n";
    canvas.listShapes();

    std::cout << "\nTotal Area: " << canvas.getTotalArea() << "\n";

    // Test removal by index
    std::cout << "\nRemoving shape at index 1 (Ellipse)...\n";
    if (canvas.removeShape(1)) {
        std::cout << "Shape removed successfully.\n";
    } else {
        std::cout << "Failed to remove shape.\n";
    }

    std::cout << "\nCanvas After Removal:\n";
    canvas.listShapes();

    std::cout << "\nUpdated Total Area: " << canvas.getTotalArea() << "\n";

    // Clean up manually allocated memory
    // Only delete shapes that are still on the canvas
    delete c1;
    delete e1;
    delete r1;
    delete s1;

    return 0;
}
