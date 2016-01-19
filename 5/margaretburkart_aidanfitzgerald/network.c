#include "lib.h"

int sock_write_n(int sock, char *str, int n) {
  int status;

  // Write four bytes for size
  n = htoni(n);
  status = write(sock, &n, sizeof(n));

  if (status < sizeof(n)) {
    
    return -2;
  }

  // Write the string itself
  status = write(sock, str, n);
  return status;
}
