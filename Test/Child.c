#include <stdio.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
  while(1 == 1)
  {
    printf("Soy el hijo y mi padre dice %s\n",argv[1]);
    sleep(3);
  }

  return 0;
}
