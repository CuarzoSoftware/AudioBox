#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  int pid = fork();

  if(pid == 0)
  {
    execlp("./Child", "Child", " que soy un pendejo", NULL, NULL);
  }
  else
  {
    while(1 == 1)
    {
      printf("Soy el padre\n");
      sleep(3);
    }
  }

  return 0;
}
