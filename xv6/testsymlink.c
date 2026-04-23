#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int
main(void)
{
  int fd;
  char buf[64];

  unlink("file");
  unlink("link1");
  unlink("link2");
  unlink("a");
  unlink("b");

  fd = open("file", O_CREATE | O_WRONLY);
  if(fd < 0){
    printf(1, "testsymlink: failed to create file\n");
    exit();
  }
  write(fd, "hello", 5);
  close(fd);

  if(symlink("file", "link1") < 0){
    printf(1, "testsymlink: failed to create link1\n");
    exit();
  }

  fd = open("link1", O_RDONLY);
  if(fd < 0){
    printf(1, "testsymlink: failed to open link1\n");
    exit();
  }

  memset(buf, 0, sizeof(buf));
  read(fd, buf, 5);
  close(fd);

  if(strcmp(buf, "hello") != 0){
    printf(1, "testsymlink: wrong contents through symlink: %s\n", buf);
    exit();
  }

  if(symlink("link1", "link2") < 0){
    printf(1, "testsymlink: failed to create link2\n");
    exit();
  }

  fd = open("link2", O_RDONLY);
  if(fd < 0){
    printf(1, "testsymlink: failed to open chained link\n");
    exit();
  }
  close(fd);

  if(symlink("b", "a") < 0){
    printf(1, "testsymlink: failed to create a->b\n");
    exit();
  }
  if(symlink("a", "b") < 0){
    printf(1, "testsymlink: failed to create b->a\n");
    exit();
  }

  fd = open("a", O_RDONLY);
  if(fd >= 0){
    printf(1, "testsymlink: cycle should have failed\n");
    close(fd);
    exit();
  }

  printf(1, "testsymlink: OK\n");
  exit();
}
