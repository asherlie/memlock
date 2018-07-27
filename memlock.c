#include <vmem_access.h>

bool strtoi(const char* str, int* i){
      char* res;
      int r = (int)strtol(str, &res, 10);
      if(i)*i = r;
      return !*res;
}

bool strtop(const char* str, void** p){
      char* res;
      if(p)*p = (void*)strtoul(str, &res, 16);
      else strtoul(str, &res, 16);
      return !*res;
}

bool has_ac(pid_t pid){
      struct mem_rgn rgn = get_vmem_locations(pid, true);
      bool ret = !((!rgn.stack.start || !rgn.stack.end) && (!rgn.heap.start || !rgn.heap.end) && (rgn.n_remaining == 0));
      free_mem_rgn(&rgn);
      return ret;
}

bool is_root(){
      return has_ac(1);
}

int main(int argc, char* argv[]){
      if(!is_root())fprintf(stderr, "WARNING: root permissions are required\n");
      pid_t pid;
      char pid_s[10], addr_s[15], val_s[20];
      bool ps = argc >= 2 && strtoi(argv[1], &pid) && has_ac(pid);
      if(!ps && argc >= 2)puts("invalid pid entered, starting memlock in any-pid mode");
      void* addr = 0x0; void* pa = 0x0;
      int val = 0, pv = 0;
      struct lock_container lc;
      lock_container_init(&lc, 5); 
      puts("enter q at any time to kill all locks or Q to exit without killing locks");
      char ch = 0;
      while(1){
            if(!ps){
                  puts("enter pid, addr, val");
                  fscanf(stdin, "%s", pid_s);
                  if(*pid_s == 'q' || *pid_s == 'Q'){
                        ch = *pid_s;
                        break;
                  }
                  fscanf(stdin, "%s %s", addr_s, val_s);
                  if(!strtoi(pid_s, &pid) || !(strtoi(val_s, &val)) || !(strtop(addr_s, &addr)) || !has_ac(pid))continue;
            }
            else{
                  puts("enter addr, val");
                  fscanf(stdin, "%s", addr_s);
                  if(*addr_s == 'q' || *addr_s == 'Q'){
                        ch = *addr_s;
                        break;
                  }
                  fscanf(stdin, "%s", val_s);
                  if(!(strtoi(val_s, &val)) || !((strtop(addr_s, &addr))))continue;
            }
            if(pa != addr || pv != val){
                  create_lock(&lc, pid, &addr, &val, NULL, 1, false, true, NULL);
                  #pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
                  printf("address %p in proccess %i locked to %i\n", addr, pid, val);
            }
            pa = addr; pv = val;
      }
      if(ch == 'q')printf("%i locks have been removed\n", free_locks(&lc));
      else if(lc.n > 0){
            printf("%i locks in place\nto remove locks, enter the following:\nkill -9 ", lc.n);
            // can use n because removal is not possible in memlock
            for(unsigned int i = 0; i < lc.n; ++i)printf(" %i", lc.locks[i].pid);
            puts("");
      }
      return 0;
}
