#include "vmem_access.h"

int main(int argc, char* argv[]){
      bool ps = (argc < 2) ? false : true;
      pid_t pid;
      if(ps)pid = atoi(argv[1]);
      void* addr; void* pa = 0x0;
      int val, pv = 0;
      struct lock_container lc;
      lock_container_init(&lc, 5); 
      puts("enter q at any time to kill all locks");
      do{
            if(!ps){
                  puts("enter pid, addr, val");
                  fscanf(stdin, "%i %p %i", &pid, &addr, &val);
            }
            else{
                  puts("enter addr, val");
                  fscanf(stdin, "%p %i", &addr, &val);
            }
            if(pa != addr && pv != val){
                  create_lock(&lc, pid, &addr, &val, NULL, 1, false, true, NULL);
                  printf("address %p in proccess %i locked to %i\n", addr, pid, val);
            }
            pa = addr; pv = val;
      }while(getchar() != 'q');
      free_locks(&lc);
      return 0;
}
