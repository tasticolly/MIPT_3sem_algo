#include <iostream>
#include <vector>
#include <limits>
uint64_t gcd(uint64_t first,uint64_t second){
  if (first < second){
    std::swap(first,second);
  }
  while (second){
    first %= second;
    std::swap(first, second);
  }
  return first;
}

int main(){
  long long n;
  std::cin>>n;
  int num_of_one = 0;
  std::vector<uint64_t> array(n);
  for (uint64_t& elem: array){
    std::cin>>elem;
    if (elem == 1){
      ++num_of_one;
    }
  }
  if (num_of_one){
    std::cout << n - num_of_one;
    return 0;
  }

  long long ans = std::numeric_limits<int>::max();
  uint64_t curr;
  for (long long i = 0; i < n; ++i){
    long long j = i + 1;
    curr = array[i];
    while (j < n && curr != 1){
      curr = gcd(curr,array[j]);
      ++j;
    }
    if (curr == 1){
      ans = std::min(ans, j - i);
    }
  }
  std::cout<<(ans == std::numeric_limits<int>::max() ? -1 : n - 1 + ans - 1);


}