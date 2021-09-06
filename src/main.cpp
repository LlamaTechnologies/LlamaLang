#include "driver.hpp"
#include "file_utils.hpp"

int main(int argc, const char *argv[]) {
  Driver driver = Driver();

  if (!driver.setup(argv, argc)) {
    return -1;
  }

  if (!driver.run()) {
    return -1;
  }

  return 0;
}
