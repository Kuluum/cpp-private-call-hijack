/**
 * Puzzle piece 3: Passing a member-function pointer as a non-type template parameterIn C++, 
 * template arguments are usually types, but there is some support for non-type template parameters if they are of integral or pointer type. 
 * 2 Conveniently enough, it’s perfectly legal to pass a pointer-to-member-function as a template argument.
 *
 **/
#include <iostream>

class SpaceShip {
 public:
  void dock(int i) {
      std::cout << "dock" << i << std::endl;
  };
  // ...
};

// Member function alias that matches the
// signature of SpaceShip::dock()
using SpaceShipFun = void (SpaceShip::*)(int);

// spaceship_fun is a pointer-to-member-function
// value which is baked-in to the type of the
// SpaceStation template at compile time.
template <SpaceShipFun spaceship_fun>
class SpaceStation {
  public:
  SpaceStation() {
      SpaceShip ship;
      (ship.*spaceship_fun)(1);
  }
};

// The intermediate SpaceShipFun
// alias hampers the genericity of the SpaceStation template, so we can move the type of the 
// pointer-to-member-function into the template arguments too
template <
  typename SpaceShipFun2,
  SpaceShipFun2 spaceship_fun
>
class SpaceStation2 {
    public:
  SpaceStation2() {
      SpaceShip ship;
      (ship.*spaceship_fun)(2);
  }
};


void test1() {
    // Instantiate a SpaceStation and pass in a
    // pointer to member function statically as a
    // template argument.
    SpaceStation<&SpaceShip::dock> space_station{};

    SpaceStation2<
    void (SpaceShip::*)(int),
    &SpaceShip::dock
    > space_station2{};
    
    // We can take that a step further and have the compiler deduce the type of the member function for us:
    SpaceStation2<
    decltype(&SpaceShip::dock),
    &SpaceShip::dock
    > space_station3{};
}




class Widget {
 public:
 private:
  void forbidden() {
      std::cout << "im forbidden" << std::endl;
  };
};


// The first template parameter is the type
// signature of the pointer-to-member-function.
// The second template parameter is the pointer
// itself.
template <
  typename ForbiddenFun,
  ForbiddenFun forbidden_fun
>
struct HijackImpl {
  static void apply(Widget& w) {
    // Calls a private method of Widget
    (w.*forbidden_fun)();
  }
};

// Explicit instantiation is allowed to refer to
// `Widget::forbidden` in a scope where it's not
// normally permissible.
template struct HijackImpl<
  decltype(&Widget::forbidden),
  &Widget::forbidden
>;

/**
 * Brilliant! We’ve instantiated a template that is able to reach in and call the forbidden() member function on any Widget that we care to pass in. 
 * So we just have to write the free function, hijack and we can go back to watching cat videos on YouTube, right?
 * 
 * The only problem is that it doesn’t work. The compiler sees through our ruse and raps us smartly on the knuckles:
 * 
 * The use of the HijackImpl template inside the hijack function is not an explicit template instantiation. 
 * It’s just a ‘normal’ implicit instantiation. So the loophole doesn’t apply. 
 * It’s time to phone a friend for help with solving the next piece of the puzzle.
 * 
 */
void hijack(Widget& w) {

    // error: 'forbidden' is a private member of 'Widget'
    // HijackImpl<decltype(&Widget::forbidden),
    // &Widget::forbidden>::hijack(w);

    // HijackImpl<
    //   decltype(&Widget::forbidden),
    //   &Widget::forbidden
    // >::apply(w);

  }


int main() {

    test1();

    return 1;
}