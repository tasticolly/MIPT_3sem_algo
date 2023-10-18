#include <iostream>
#include <vector>
const long long module = 1000000000 + 7;

long long binPow(long long elem, long long pow){
  long long ans = 1;
  while (pow){
    if (pow % 2 == 0){
      elem *= elem  % module;
      elem %= module;
      pow /= 2;
    } else {
      ans *= elem  % module;
      ans %= module;
      --pow;
    }
  }
  return ans % module;
}

long long reverse(long long elem){
  return binPow(elem, module - 2) ;
}


int main(){
  std::vector<long long> input(4);
  long long tmp;
  for (long long& i: input){
    std::cin>>tmp;
    if (tmp < 0){
      tmp += module;
    }
    i = tmp;
  }
  long long sum = (input[0] * input[3]  % module + input[1] * input[2]  % module) % module;
  long long a = (input[1] * input[3])  % module;
  std::cout << sum * binPow(a, module - 2) % module;
}