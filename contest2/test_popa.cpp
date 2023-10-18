#include <iostream>

const long long module = 998244353;
long long binPow(long long elem, long long pow) {
  long long ans = 1;
  while (pow) {
    if (pow % 2 == 0) {
      elem *= elem % module;
      elem %= module;
      pow /= 2;
    } else {
      ans *= elem % module;
      ans %= module;
      --pow;
    }
  }
  return ans % module;
}

long long reverse(long long elem) {
  return binPow(elem, module - 2);
}
int main(){
  std::cout<<binPow(31,1<<23) ;

}