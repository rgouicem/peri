#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
  int fd;

  if((fd = open("/dev/ledbtn", O_RDWR)) < 0) {
    perror("open");
    return -1;
  }
  char lo;

  write(fd, "lala", 5);

  read(fd, &lo, 1);

  close(fd);

  return 0;
}
