#include "driver.hpp"

int main(int argc, const char *argv[]) {
  if (!Driver::setup(argv, argc)) {
    return -1;
  }

  if (!Driver::run()) {
    return -1;
  }

  return 0;
}
