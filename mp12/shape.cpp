// Anna Miller (annam4)
// ECE 220 - BD2

/* The shape constructor is used to set the value of name_. The member getName within
  Shape returns this private variable for public use. Rectangle, Circle, Sphere, and
  RectPrim  are derived classes of Shape. They inherit the public elements of Shape.
  The constructors set the appropriate dimensions (width, length, height, radius)
  GetArea returns the area, and getVolume returns the volume. The operator overload for +
  adds the corresponding dimensions using the input instance, then creates a new instance
  with the new dimensions. The operator overload for - subtracts the corresponding
  dimensions.Resulrs less than zero are set to zero. Then, the new dimensions are used
  to return a new instance. CreateShapes reads the input file using ifstream. Based on the
  number of objects in the file, it will assign names and dimensions in a for loop. within
  the loop, it determines the type of shape from the name read, then it sets the appropriate
  dimensions (width, length, height, radius) from the read file. Determining the type of
  shape first is important to retain all of the information. Once the dimensions are
  read, a new instance of the proper shape is created and assigned to a temporary pointer.
  This pointer is then pushed to the back of a vector. Finally, this vector containing the
  pointers is returned. MaxArea goes through all objects in an input vector and returns the
  maximum area of the list. MaxVolume does the same as maxArea, except with the volume.
*/

#include "shape.hpp"
#define PI 3.14159265359


//Base class
//Please implement Shape's member functions
//constructor, getName()
//
//Base class' constructor should be called in derived classes'
//constructor to initizlize Shape's private variable
Shape::Shape(string name){
  name_ = name;
}
string Shape::getName(){
  return name_;
}

//Rectangle
//Please implement the member functions of Rectangle:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here
//constructor
Rectangle::Rectangle(double width, double length):Shape("Rectangle"){
    width_ = width;
    length_ = length;
}
// area function
double Rectangle::getArea() const{
  return length_ * width_;
}
// volume function
double Rectangle::getVolume() const{
  return 0;
}
// operator +
Rectangle Rectangle::operator +(const Rectangle& rec){
  double w = width_ + rec.getWidth();
  double l = length_ + rec.getLength();
  return Rectangle(w, l);
}
// operator -
Rectangle Rectangle::operator -(const Rectangle& rec){
  double w = width_ - rec.getWidth();
  if(w < 0){
    w = 0;
  }
  double l = length_ - rec.getLength();
  if(l < 0){
    l = 0;
  }
  return Rectangle(w, l);
}

double Rectangle::getWidth()const{return width_;}
double Rectangle::getLength()const{return length_;}


//Circle
//Please implement the member functions of Circle:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here
Circle::Circle(double radius):Shape("Circle"){
    radius_ = radius;
}
// area function
double Circle::getArea() const{
  return (radius_ * radius_)*PI;
}
// volume function
double Circle::getVolume() const{
  return 0;
}
// operator +
Circle Circle::operator +(const Circle& cir){
  double r = radius_ + cir.getRadius();
  return Circle(r);
}
// operator -
Circle Circle::operator -(const Circle& cir){
  double r = radius_ - cir.getRadius();
  if(r < 0){
    r = 0;
  }
  return Circle(r);
}

double Circle::getRadius()const{return radius_;}

//Sphere
//Please implement the member functions of Sphere:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here
Sphere::Sphere(double radius):Shape("Sphere"){
  radius_ = radius;
}
// area function
double Sphere::getArea() const{
  return 4 * PI * (radius_ * radius_);
}
// volume function
double Sphere::getVolume() const{
  return (4.0/3.0) * (radius_ * radius_ * radius_) * PI;
}
// operator +
Sphere Sphere::operator +(const Sphere& sph){
  double r = radius_ + sph.getRadius();
  return Sphere(r);
}
// operator -
Sphere Sphere::operator -(const Sphere& sph){
  double r = radius_ - sph.getRadius();
  if(r < 0){
    r = 0;
  }
  return Sphere(r);
}

double Sphere::getRadius()const{return radius_;}

//Rectprism
//Please implement the member functions of RectPrism:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here
RectPrism::RectPrism(double width, double length, double height):Shape("RectPrism"){
  width_ = width;
  length_ = length;
  height_ = height;
}
// area function
double RectPrism::getArea() const{
  return 2*(length_ * width_ + length_ * height_ + width_ * height_);
}
// volume function
double RectPrism::getVolume() const{
  return length_ * width_ * height_;
}
// operator +
RectPrism RectPrism::operator +(const RectPrism& rectp){
  double w = width_ + rectp.getWidth();
  double l = length_ + rectp.getLength();
  double h = height_ + rectp.getHeight();
  return RectPrism(w, l, h);
}
// operator -
RectPrism RectPrism::operator -(const RectPrism& rectp){
  double w = width_ - rectp.getWidth();
  if(w < 0){
    w = 0;
  }
  double l = length_ - rectp.getLength();
  if(l < 0){
    l = 0;
  }
  double h = height_ - rectp.getHeight();
  if(h < 0){
    h = 0;
  }
  return RectPrism(w, l, h);
}

double RectPrism::getWidth()const{return width_;}
double RectPrism::getHeight()const{return height_;}
double RectPrism::getLength()const{return length_;}



// Read shapes from test.txt and initialize the objects
// Return a vector of pointers that points to the objects
vector<Shape*> CreateShapes(char* file_name){
	//@@Insert your code here
  // open the file and read it
  ifstream ifs (file_name, std::ifstream::in);

  // vector of pointers
  vector<Shape*> shapes;

  // number of objects, shape name, dimensions, and temporary pointer for new shape
  int numObj = 0;
  string name;
  double w, l, h, r;
  Shape* shape_ptr = NULL;

  // record the number of objects in the file
  ifs >> numObj;

  // read through all objects in the file
  for(int i = 0; i < numObj; i++){
    ifs >> name;
    // depending on the type of the shape, save the dimensions
    if(name == "Rectangle"){
      ifs >> w;
      ifs >> l;
      shape_ptr = new Rectangle(w, l);
    }
    if(name == "Circle"){
      ifs >> r;
      shape_ptr = new Circle(r);
    }
    if(name == "Sphere"){
      ifs >> r;
      shape_ptr = new Sphere(r);
    }
    if(name == "RectPrism"){
      ifs >> w;
      ifs >> l;
      ifs >> h;
      shape_ptr = new RectPrism(w, l, h);
    }
    // add the new shape to the end of the vector
    shapes.push_back(shape_ptr);
  }
  // close the file
  ifs.close();
	return shapes; // please remeber to modify this line to return the correct value
}

// call getArea() of each object
// return the max area
double MaxArea(vector<Shape*> shapes){
	double max_area = 0;
	//@@Insert your code here
  // go through the area of each element in the vector
  for(int i = 0; i < shapes.size(); i++){
    // if the area is greater than the current max, replace the max
    if(shapes[i]->getArea() > max_area){
      max_area = shapes[i]->getArea();
    }
  }
	return max_area;
}


// call getVolume() of each object
// return the max volume
double MaxVolume(vector<Shape*> shapes){
	double max_volume = 0;
	//@@Insert your code here
  // go through the area of each element in the vector
  for(int i = 0; i < shapes.size(); i++){
    // if the area is greater than the current max, replace the max
    if(shapes[i]->getVolume() > max_volume){
      max_volume = shapes[i]->getVolume();
    }
  }

	return max_volume;
}
