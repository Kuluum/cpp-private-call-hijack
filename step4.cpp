/**
 * Puzzle piece 4: A friend comes to our aid 
 * Because we’re not allowed to refer to Widget::forbidden inside our hijack function, we must solve the conundrum of accessing the value of the ForbiddenFun 
 * template parameter withou t making any direct reference to the HijackImpl<...> template. This apparently unreasonable requirement is easily solved with a shrewd 
 * application of the friend keyword.
 * 
 * Let’s take another step back from the task in hand and look at some of the different effects one can achieve when marking a free function as a friend of a class.
 *  The behaviour depends on whether the class contains only a declaration of the function (i.e. function signature only), or whether the complete definition 
 * (including the function body) appears inside the class.
 * 
 */

/**
 * ‘friend’ function declarations
 * Most C++ developers will be familiar with the pattern of placing a free function declaration inside of a class definition and marking it as a friend . 
 * The definition of the free-function still lives outside of the class, but is now allowed to access private members of the class. (See Listing 8.)
 **/

#include <iostream>

class Gadget {
  // Friend declaration gives `frobnicate` access
  // to Gadget's private members.
  friend void frobnicate();

 private:
  void internal() {
    std::cout << "Gadget" << std::endl;
  }
};

// Definition as a normal free function
void frobnicate() {
  Gadget g;
  // OK because `frobnicate()` is a friend of
  // `Gadget`.
  g.internal();
}

// If we could make hijack be a friend of Widget then the compiler would allow us to refer to Widget::forbidden inside the hijack function.
// Alas, this option is unavailable because the rules of our game don’t allow us to modify Widget . Let’s try something else.


/**
 * Inline ‘friend’ function definitions
 * 
 * It’s also possible to define a friend function inside a class (as opposed to just declaring it there). This isn’t something seen as often in C++ code. 
 * Probably because when we try to call the free function, the compiler is unable to find it! Here’s the compile error:
 * 
**/

class Gadget2 {
  // Free function declared as a friend of Gadget
  friend void frobnicate2() {
    Gadget2 g;
    g.internal(); // Still OK
  }

 private:
   void internal();
};

/**
 * ‘frobnicate2’ was not declared in this scope
 * 
 * As before, frobnicate() is still a free function that lives in the global namespace, but it behaves quite differently under name lookup now that 
 * it is defined inside the Gadget class. A friend function defined inside a class is sometimes known as a ‘hidden friend’ [ JSS19 ] [ Saks18 ]. 
 * Hidden friends can only be found through Argument Dependent Lookup (ADL) and ADL only works if one of the arguments to the function is of the enclosing class type.
 *  In the above example frobnicate() takes no arguments, so argument dependent lookup won’t happen. 
 * The result is that frobnicate() can’t be called from anywhere. Not even from within frobnicate() itself!
**/



class Gadget3 {

  friend void frobnicate3(Gadget3& gadget) {
    gadget.internal();
  }

 private:
   void internal() {
       std::cout << "Gadget3" << std::endl;
   };
};

/**
 * Making hidden friends visible
 * The hidden-friend ADL trick can be very useful; it’s an ideal tool when writing operator overloads for user-defined types. 
 * But we’ll use a slightly bigger hammer for our hijack function. 
 * There’s another way of allowing the compiler to find hidden friends, and that is to put a declaration of the function in the enclosing namespace 
 * 
 */
class Gadget4 {
  // Definition stays inside the Gadget class
  friend void frobnicate4() {
    Gadget4 g;
    g.internal();
  }

 private:
   void internal() {
       std::cout << "Gadget4" << std::endl;
   };
};

// An additional namespace-scope declaration makes
// the function available for normal name lookup.
void frobnicate4();



/**
 * Using friends to pilfer template parameters I trust you will be at least a little unsettled to discover that the program
 * 
 * What’s happening is that the observe() function is not defined until the point at which the SpookyAction template is instantiated
 * (by its use in the main function). There is a single definition of the observe() function, because there is a single instantiation of the SpookyAction template.
 * The really useful part is that the observe() function gains access to the template parameter of the SpookyAction<42> instantiation that caused it to be defined
 * Of course things go wrong very quickly if we try to instantiate any more versions of the SpookyAction template, as each one results in a redefinition 
 * of the observe() function and an angry compiler.
 * Provided we use it carefully, we now have the last piece of our puzzle – a way to access the template parameters of a class from a scope external to that class.
 */

template <int N>
class SpookyAction {
  friend int observe() {
    return N;
  }
};

int observe();


int main() {
    frobnicate();

    // frobnicate2(); // ‘frobnicate2’ was not declared in this scope
    
    Gadget3 g;
    frobnicate3(g);

    frobnicate4();

    SpookyAction<42>{};
    std::cout << observe() << '\n';  // Prints 42
    return 1;
}