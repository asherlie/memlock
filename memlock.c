#include <vmem_access.h>
#include <string.h>

bool strtoi(const char* str, int* i){
      char* res;
      int r = (int)strtol(str, &res, 10);
      if(*res)return false;
      if(i)*i = r;
      return true;
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

int main(int argc, char* argv[]){
      bool ir = false;
      if(!(ir = has_ac(1)))fprintf(stderr, "WARNING: root permissions are required\n");
      pid_t pid, pr_pid = -1;
      char pid_s[10], addr_s[15], val_s[20];
      bool ps = argc >= 2 && strtoi(argv[1], &pid) && has_ac(pid);
      if(!ps && argc >= 2){
            if(ir)fprintf(stderr, "WARNING: invalid pid entered, starting memlock in any-pid mode\n");
            else fprintf(stderr, "WARNING: pid entered may be invalid, starting memlock in any-pid mode\n");
      }
      void* addr = 0x0; void* pa = 0x0;
      int val = 0, pv = 0;
      struct lock_container lc;
      lock_container_init(&lc, 5); 
      puts("enter q at any time to kill all locks or Q to exit without killing locks");
      char ch = 0;
      bool integers;
      while(1){
            if(!ps){
                  puts("enter pid, addr, val");
                  fscanf(stdin, "%s", pid_s);
                  if(*pid_s == 'q' || *pid_s == 'Q'){
                        ch = *pid_s;
                        break;
                  }
                  fscanf(stdin, "%s %s", addr_s, val_s);
                  if(!strtoi(pid_s, &pid) || !strtop(addr_s, &addr) || addr == 0x0 || !has_ac(pid))continue;
            }
            else{
                  puts("enter addr, val");
                  fscanf(stdin, "%s", addr_s);
                  if(*addr_s == 'q' || *addr_s == 'Q'){
                        ch = *addr_s;
                        break;
                  }
                  fscanf(stdin, "%s", val_s);
                  if(!strtop(addr_s, &addr) || addr == 0x0)continue;
            }
            integers = strtoi(val_s, &val);
            if(pa != addr || pv != val){
                  #pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
                  char** vs = NULL;
                  if(!integers){
                        vs = malloc(sizeof(char*));
                        // length will never be > than that of val_s
                        *vs = malloc(sizeof(val_s));
                        memcpy(*vs, val_s, sizeof(val_s));
                  }
                  create_lock(&lc, pid, &addr, &val, vs, 1, false, integers);
                  if(integers)printf("address %p in proccess %i locked to %i\n", addr, pid, val);
                  else printf("address %p in proccess %i locked to \"%s\"\n", addr, pid, val_s);
            }
            pa = addr; pv = val;
      }
      if(ch == 'q'){
            puts("removing the following locks:");
            print_locks(&lc);
            printf("%i locks have been removed\n", free_locks(&lc));
      }
      else if(lc.n > 0){
            pr_pid = fork();
            unsigned int n = lc.n;
            // assuring that lc.n == lc.n_removed - which will always be 0
            // when lc.n == lc.n_removed, lock_thread will return
            lc.n = 0;
            pthread_join(lc.thread, NULL);
            lc.n = n;
            if(pr_pid == 0)lock_th(&lc);
            printf("%i locks in place\nto remove locks, enter the following:\nkill -9 %i\n", lc.n, pr_pid);
      }
      return 0;
}
