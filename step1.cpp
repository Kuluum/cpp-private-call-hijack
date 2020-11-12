#include <iostream>

/**
* Puzzle piece 1: Pointers to private member functions can be called from any scope
* Suppose the author of the Widget class had helpfully provided a means to obtain a pointer to the Widget::forbidden() member function.
* Once we have that pointer, we are able to call it from any scope where we have a Widget available (Listing 2).
*
* That’s useful to know, but most classes don’t offer to hand out pointers to their private member functions. 
* We need to find a sneakier way to get hold of one from outside of the class scope.
*
* There’s a curious loophole in the C++ standard around the use of explicit template instantiation which allows us to refer to private class members. 
* That gives us the second piece of the Folly puzzle.
**/

class Widget {
 public:
// Public pointer to provate function
  static auto forbidden_fun() {
    return &Widget::forbidden;
  }
 private:
  void forbidden() {
      std::cout << "im forbidden" << std::endl;
  };
};

void hijack(Widget& w) {
  using ForbiddenFun = void (Widget::*)();
  ForbiddenFun const forbidden_fun =
    Widget::forbidden_fun();

  // Calls a private member function on the Widget
  // instance passed in to the function.
  (w.*forbidden_fun)();
}

int main() {

    Widget w;

    hijack(w);

    return 0;
}