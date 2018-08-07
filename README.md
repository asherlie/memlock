# memlock
memlock is a simple memory address value locking utility that depends on [memcarve](http://github.com/asherlie/memcarve) library

memlock can be used to lock either integer values or string values

to lock an address to an integer value, enter an integer

to lock an address to a string value, enter a string


this program can be used in two different modes and exited with two different options
#### modes
when supplied with a pid as an argument, memlock will assume that all locks to be applied will be to addresses in the specified pid

otherwise, memlock makes no such assumption - a pid must be entered with every lock creation

#### exit options
memlock can be exited with 'q' or 'Q'

when 'q' is used, all locks in place will be removed

otherwise, locks will be kept in place after memlock exits and a kill command will be printed to kill all locks in place
