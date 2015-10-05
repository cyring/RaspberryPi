INCLUDE = -I/opt/vc/include/ -I/opt/vc/include/interface/vcos/pthreads -I/opt/vc/include/interface/vmcs_host/linux/
LIBDIR  = -L/opt/vc/lib/

COMPILERFLAGS = -Wall -g
CC = cc
CFLAGS = $(COMPILERFLAGS) $(INCLUDE)
LIBRARIES = -lm -lGLESv2 -lEGL -lbcm_host -lvcos

conics4rpi : conics4rpi.o
	$(CC) $(CFLAGS) -o $@ $(LIBDIR) $< $(LIBRARIES)  

clean:
	rm conics4rpi.o conics4rpi
