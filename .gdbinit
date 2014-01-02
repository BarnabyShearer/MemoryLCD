target remote localhost:3333
set remotetimeout 10000
monitor init
monitor arm semihosting enable
monitor reset halt
monitor flash write_image erase "/home/barnaby/po/main.elf" 0
monitor reset halt

