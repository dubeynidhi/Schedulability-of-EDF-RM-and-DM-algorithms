ifeq ($(TARGET), Galileo)
	CC=/opt/iot-devkit/1.7.2/sysroots/x86_64-pokysdk-linux/usr/bin/i586-poky-linux/i586-poky-linux-gcc
	
else
	CC=gcc

endif


APP = main
APP1 = main1
all:
	$(CC) -Wall -o $(APP) main.c -lm
	$(CC) -Wall -o $(APP1) mainpart2.c -lm  
clean:
	
	rm -f *.o
	rm -f $(APP)
	rm -f *.o 
	rm -f $(APP1)
