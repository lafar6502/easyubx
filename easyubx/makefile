

all: easyubxlib main_test test


easyubxlib: libeasyubx.so

OBJS = easyubx_drv.o  easyubx_drv_cfg.o  easyubx_drv_mon.o  easyubx_drv_nav.o

libeasyubx.so: $(OBJS)
	gcc -shared -o $@ $^

%.o: %.c
	gcc -fpic -o $@ -c $<


main_test: main_test.o libeasyubx.so
	gcc -L./ -o main_test  main_test.o -leasyubx 

test: main_test.o $(OBJS)
	gcc -o test $^

