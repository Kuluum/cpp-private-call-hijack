#include "step2.h"

// Explicit instantiation of the template only
// needs to happen in a single translation unit.
template
void Company::update_employee<OnSiteEmployeePolicy>(int);


int main() {

    Company c;
    c.update_employee<OnSiteEmployeePolicy>(1);
    // c.update_employee<OtherEmployeePolicy>(1); // Cant see this: step2.cpp:(.text+0x39): undefined reference to `void Company::update_employee<OtherEmployeePolicy>(int)'
    return 1;
}