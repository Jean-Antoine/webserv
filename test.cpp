#include <iostream>
using namespace std;
class Base {
   public :
   int func() {
      static int a;
      static int b = 12;
      cout << "The default value of static variable a is: " << a;
      cout << "\nThe value of static variable b is: " << b;
	  b++;
	  return 0;
   }
};
int main() {
   Base b;
   b.func();
   b.func();
   b.func();
   b.func();
   b.func();
   Base a;
   a.func();
   return 0;
}
