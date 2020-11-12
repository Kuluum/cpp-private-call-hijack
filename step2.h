/** 
 * Puzzle piece 2: The explicit template instantiation loophole
 * The C++17 standard contains the following paragraph (with the parts of interest to us marked in bold):
 *
 * 17.7.2 (item 12)
 *
 * The usual access checking rules do not apply to names used to specify explicit instantiations. 
 * [Note: In particular, the template arguments and names used in the function declarator (including parameter types, return types and exception specifications)
 *  may be private types or objects which would normally not be accessible and the template may be a member template 
 * or member function which would not normally be accessible.]
 *
 * To understand the reason behind this curiosity, we need to discuss the explicit template instantiation mechanism for a moment.
 *
 * Suppose we’ve got a Company class with an internal private member function template, update_employee. 
 * Perhaps there is one particular template argument, OnSiteEmployeePolicy which is expensive to compile, but used regularly. 
 * We’d like to avoid the cost of instantiating that version of the template in lots of translation units. We can achieve this by explicitly instantiating
 * the member template in just one translation unit and marking it as extern everywhere else.
 * 
 * 
 * Brushing aside the question of how someone ever snuck such an awkward API design through code review, 
 * notice how the template instantiation mechanism needs to allow a reference to a private member of 
 * Company – Company::update_employee – in a context where we would not normally be able to (i.e. outside the scope of the Company class). 
 * That’s the reason for the exception in the C++ standard that allows for private types to appear in explicit template instantiations.
 * 
 * It’s also the crucial loophole that Folly takes advantage of. We can’t relax just yet, though. There’s still some work to be done.
 **/

#include <iostream>

class OnSiteEmployeePolicy {
  // ... contains daring and unfettered use of
  // ... hairy template meta-programming tricks.
  public:

    OnSiteEmployeePolicy() {
        std::cout << "OnSiteEmployeePolicy" << std::endl;
    }
};

class OtherEmployeePolicy {
  public:
    OtherEmployeePolicy() {
        std::cout << "OtherEmployeePolicy" << std::endl;
    }
};

class Company {

  public: // Maked public for testing template instantiation. private in paper.
  template <typename EmployeePolicy>
  void update_employee(int employee_id) {
    EmployeePolicy e;
  }
};

// Prevents implicit instantiation of a specific
// specialization.
extern template 
void Company::update_employee<OnSiteEmployeePolicy>(int);

extern template 
void Company::update_employee<OtherEmployeePolicy>(int);