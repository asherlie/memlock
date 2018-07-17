# memlock
memlock is a simple memory address integer value locking utility written using [vmem_access](http://github.com/asherlie/varedit#memory-locking)

this program can be used in two different modes and exited with two different options
#### modes
when supplied with a pid as an argument, memlock will assume that all locks to be applied 

otherwise, memlock makes no assumption. pid must be entered with every lock creation

#### options
memlock can be exited with 'q' or 'Q'

when 'q' is used, all locks in place will be removed

otherwise, locks will be kept in place after memlock exits and a kill command will be printed to kill all locks in place
