#include <SFML/Graphics.hpp>
#include <cmath>
#include <concepts>
#include <iostream>
#include <stdexcept>
#include <utility>
#include <vector>

template <typename T>
concept SupportLessThan = requires(T x) {
  { x < x } -> std::convertible_to<bool>;
};

template <typename T>
concept SupportMultiply = requires(T x) {
  { x *x } -> std::convertible_to<T>;
};

template <typename T>
concept SupportCastToDouble = requires(T x) {
  { static_cast<double>(x) } -> std::convertible_to<double>;
};

struct ShapeType {
  static constexpr int Rectangle = 0;
  static constexpr int Square = 1;
  static constexpr int Circle = 2;
  static constexpr int Ellipse = 3;
};

template <typename T>
  requires std::copyable<T> && SupportLessThan<T> && SupportMultiply<T> &&
           SupportCastToDouble<T>
class Canvas;

// Abstract base class providing a common interface for all shape types; not
// intended for direct instantiation.
template <typename T>
  requires std::copyable<T> && SupportLessThan<T> && SupportMultiply<T> &&
           SupportCastToDouble<T>
class Shape {
protected:
  Shape() {}; // throw an error if the shape object is created by user directly.
  double xCord;
  double yCord;
  Canvas<T> *myCanvas;

public:
  std::pair<double, double> getCoordinates() const { return {xCord, yCord}; }

  Canvas<T> *getCanvas() const { return myCanvas; }

  void setCanvas(Canvas<T> *canvas) { myCanvas = canvas; }

  virtual int getShape() const = 0; // pure virtual function

  virtual double getArea() const = 0;
  virtual T getRadii() const {
    throw std::runtime_error("Cannot call getRadii() on abstract Shape class");
  }

  virtual T getXLength() const {
    throw std::runtime_error(
        "Cannot call getXLength() on abstract Shape class");
  }

  virtual T getYLength() const {
    throw std::runtime_error(
        "Cannot call getYLength() on abstract Shape class");
  }

  virtual bool isCircle() const {
    throw std::runtime_error("Cannot call isCircle() on abstract Shape class");
  }

  virtual T getLength() const {
    throw std::runtime_error("Cannot call getLength() on abstract Shape class");
  }

  virtual T getBreadth() const {
    throw std::runtime_error(
        "Cannot call getBreadth() on abstract Shape class");
  }

  virtual bool isSquare() const {
    throw std::runtime_error("Cannot call isSquare() on abstract Shape class");
  }

  virtual T getSide() const {
    throw std::runtime_error("Cannot call getSide() on abstract Shape class");
  }

  virtual void draw(sf::RenderWindow &) const {
    throw std::runtime_error("Cannot call draw() on abstract Shape class");
  }

  virtual ~Shape() = default; 	// virtual destructor : very important so that 
  								// derived classes can be cleaned up properly
								// when deleted through a base class pointer

};

template <typename T>
  requires std::copyable<T> && SupportLessThan<T> && SupportMultiply<T> &&
           SupportCastToDouble<T>
class Ellipse : public Shape<T> {
protected:
  T a;
  T b;

public:
  Ellipse(const T a, const T b, double xCord = 0, double yCord = 0)
      : Shape<T>(), a(a), b(b) {
    this->xCord = xCord;
    this->yCord = yCord;
  }

  virtual int getShape() const override { return ShapeType::Ellipse; }

  virtual T getXLength() const override { return a; }
  virtual T getYLength() const override { return b; }

  virtual double getArea() const override { return M_PI * a * b; }

  virtual bool isCircle() const override { return a == b; }

  void draw(sf::RenderWindow &window) const override {
    sf::CircleShape ellipseShape(50); // fixed radius workaround
    ellipseShape.setScale(static_cast<float>(a) / 50,
                          static_cast<float>(b) / 50);
    ellipseShape.setPosition(this->xCord, this->yCord);
    ellipseShape.setFillColor(sf::Color::Blue);
    window.draw(ellipseShape);
  }

  virtual T getRadii() const override {
    throw std::runtime_error(
        "Cannot call getRadii() on abstract Ellipse class");
  }
};

template <typename T>
  requires std::copyable<T> && SupportLessThan<T> && SupportMultiply<T> &&
           SupportCastToDouble<T>
class Circle : public Ellipse<T> {

protected:
  using Ellipse<T>::a;

public:
  Circle(const T r, double xCord = 0, double yCord = 0)
      : Ellipse<T>(r, r, xCord, yCord) {}

  virtual int getShape() const override { return ShapeType::Circle; }

  T getRadii() const override { return a; }

  void draw(sf::RenderWindow &window) const override {
    sf::CircleShape circleShape(static_cast<float>(a)); // radius
    circleShape.setPosition(this->xCord, this->yCord);
    circleShape.setFillColor(sf::Color::Red);
    window.draw(circleShape);
  }
};

template <typename T>
  requires std::copyable<T> && SupportLessThan<T> && SupportMultiply<T> &&
           SupportCastToDouble<T>
class Rectangle : public Shape<T> {
protected:
  T length;
  T breadth;

public:
  Rectangle(const T l, const T b, double xCord = 0, double yCord = 0)
      : Shape<T>(), length(l), breadth(b) {
    this->xCord = xCord;
    this->yCord = yCord;
  }

  virtual int getShape() const override { return ShapeType::Rectangle; }

  virtual T getLength() const override { return length; }

  virtual T getBreadth() const override { return breadth; }

  virtual double getArea() const override {
    return static_cast<double>(length) * static_cast<double>(breadth);
  }

  virtual bool isSquare() const override { return (length == breadth); }
  virtual T getSide() const override {
    throw std::runtime_error(
        "Cannot call getSide() on abstract Rectangle class");
  }

  void draw(sf::RenderWindow &window) const override {
    sf::RectangleShape rectShape(
        sf::Vector2f(static_cast<float>(length), static_cast<float>(breadth)));
    rectShape.setPosition(this->xCord, this->yCord);
    rectShape.setFillColor(sf::Color::Green);
    window.draw(rectShape);
  }
};

template <typename T>
  requires std::copyable<T> && SupportLessThan<T> && SupportMultiply<T> &&
           SupportCastToDouble<T>
class Square : public Rectangle<T> {
protected:
  using Rectangle<T>::length;

public:
  Square(const T l, double xCord = 0, double yCord = 0)
      : Rectangle<T>(l, l, xCord, yCord) {}

  virtual int getShape() const override { return ShapeType::Square; }

  virtual T getSide() const override { return length; }
};

template <typename T>
  requires std::copyable<T> && SupportLessThan<T> && SupportMultiply<T> &&
           SupportCastToDouble<T>
class Canvas {
private:
  std::vector<Shape<T> *> shapes;

public:
  void addShape(Shape<T> *shapePtr) {
    shapes.push_back(shapePtr);
    shapePtr->setCanvas(this);
  }
  bool removeShape(Shape<T> *shapePtr) {
    auto it = std::find(shapes.begin(), shapes.end(), shapePtr);
    if (it != shapes.end()) {
      shapes.erase(it);
      shapePtr->setCanvas(nullptr);
      return true;
    }
    return false;
  }
  bool removeShape(int index) {
    if (index < 0 || index >= shapes.size()) {
      return false;
    }
    shapes[index]->setCanvas(nullptr);
    shapes.erase(shapes.begin() + index);
    return true;
  }
  Shape<T> *getShape(int index) const {
    if (index < 0 || index >= shapes.size()) {
      throw std::out_of_range("Index out of range");
    }
    return shapes[index];
  }
  double getTotalArea() const {
    double totalArea = 0.0;
    for (const auto &shape : shapes) {
      totalArea += shape->getArea();
    }
    return totalArea;
  }
  void listShapes() const {
    for (int i = 0; i < shapes.size(); ++i) {
      const auto &shape = shapes[i];
      std::cout << "Shape " << i << ": Type " << shape->getShape()
                << ", Coordinates: (" << shape->getCoordinates().first << ", "
                << shape->getCoordinates().second << ")\n";
    }
    return;
  }
  void clear() {
    for (auto &shape : shapes) {
      shape->setCanvas(nullptr);
    }
    shapes.clear();
  }

  void displayCanvas() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Canvas");

    while (window.isOpen()) {
      sf::Event event;
      while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
          window.close();
      }

      window.clear(sf::Color::White);
      for (const auto &shape : shapes) {
        shape->draw(window); // polymorphic call
      }
      window.display();
    }
  }

  ~Canvas() { clear(); }
};
