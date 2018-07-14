#include "vmem_access.h"

int main(int a, char** b){
      if(a < 4)return -1;
      pid_t pid = atoi(b[1]);
      void* addr = (void*)strtoul(b[2], NULL, 16);
      int v = atoi(b[3]);
      struct lock_container lc;
      lock_container_init(&lc, 1);
      create_lock(&lc, pid, &addr, &v, NULL, 1, false, true, NULL);
      printf("address %p in proccess %i locked to %i\nlock has pid: %i\npress enter to remove lock", addr, pid, v, lc.locks[0].pid);
      while(getchar() != '\n');
      /*free_locks(&lc);*/
}
