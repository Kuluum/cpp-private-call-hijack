#pragma once
#include <iostream>

class Widget {
 private:
  void forbidden() {
      std::cout << "im forbidden" << std::endl;
  };
};
