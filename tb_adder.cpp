#include <cassert>
#include <iostream>
#include "adder_kernel.h"

int main() {
  std::cout << "=== HLS C-Simulation Test ===" << std::endl;

  // Test case 1: Basic addition
  int out1 = 0;
  int a1 = 3, b1 = 5;
  std::cout << "Test 1: " << a1 << " + " << b1 << " = ?" << std::endl;
  adder_kernel(a1, b1, &out1);
  std::cout << "Result: " << out1 << std::endl;
  assert(out1 == 8);
  std::cout << "✓ Test 1 PASSED: " << a1 << " + " << b1 << " = " << out1 << std::endl;

  // Test case 2: Negative numbers
  int out2 = 0;
  int a2 = -10, b2 = 7;
  std::cout << "\nTest 2: " << a2 << " + " << b2 << " = ?" << std::endl;
  adder_kernel(a2, b2, &out2);
  std::cout << "Result: " << out2 << std::endl;
  assert(out2 == -3);
  std::cout << "✓ Test 2 PASSED: " << a2 << " + " << b2 << " = " << out2 << std::endl;

  // Test case 3: Zero values
  int out3 = 0;
  int a3 = 0, b3 = 0;
  std::cout << "\nTest 3: " << a3 << " + " << b3 << " = ?" << std::endl;
  adder_kernel(a3, b3, &out3);
  std::cout << "Result: " << out3 << std::endl;
  assert(out3 == 0);
  std::cout << "✓ Test 3 PASSED: " << a3 << " + " << b3 << " = " << out3 << std::endl;

  // Test case 4: Large numbers
  int out4 = 0;
  int a4 = 1000, b4 = 2000;
  std::cout << "\nTest 4: " << a4 << " + " << b4 << " = ?" << std::endl;
  adder_kernel(a4, b4, &out4);
  std::cout << "Result: " << out4 << std::endl;
  assert(out4 == 3000);
  std::cout << "✓ Test 4 PASSED: " << a4 << " + " << b4 << " = " << out4 << std::endl;

  std::cout << "\n=== All Tests PASSED! ===" << std::endl;
  std::cout << "C-Simulation completed successfully." << std::endl;

  return 0;  // 0 is csim OK
}
