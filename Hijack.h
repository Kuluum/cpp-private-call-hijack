/**
 * Putting the puzzle pieces together
 * Let’s go back to our original Widget example, now that we’ve got all of the pieces that we need to be able to reach in and call its private member function, Widget::forbidden() . In summary:
 * 
 * We use the loophole in the explicit template instantiation rules to allow us to refer to Widget::forbidden() from outside of the Widget class.
 * We inject the address of Widget::forbidden() into our HijackImpl class as a template parameter.
 * We define the hijack() function directly inside of HijackImpl so that it can access the template parameter containing the address of Widget::forbidden() .
 * We mark hijack as a friend so that it becomes a free function and we provide a declaration of hijack at namespace scope so that it participates in name-lookup.
 * We can now invoke Widget::forbidden() on any Widget instance through the member-function address that is exposed to the hijack function.
 * 
 */
#pragma once
#include "Widget.h"

// HijackImpl is the mechanism for injecting the
// private member function pointer into the
// hijack function.
template <
  typename ForbiddenFun,
  ForbiddenFun forbidden_fun
>
class HijackImpl {
  // Definition of free function inside the class
  // template to give it access to the
  // forbidden_fun template argument.
  // Marking hijack as a friend prevents it from
  // becoming a member function.
  friend void hijack(Widget& w) {
    (w.*forbidden_fun)();
  }
};
// Declaration in the enclosing namespace to make
// hijack available for name lookup.
void hijack(Widget& w);

// Explicit instantiation of HijackImpl template
// bypasses access controls in the Widget class.
template class
HijackImpl<
  decltype(&Widget::forbidden),
  &Widget::forbidden
>;