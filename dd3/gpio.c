#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#define BUFSIZE 	50

void signal_handler(int signum)
{
	static int count=0;
	printf("user app : signal is catched\n");
	if(signum==SIGIO)
	{
		printf("SIGIO\n");
	}
	count++;
	if(count==5)
		exit(1);
}

int main(int argc, char** argv)
{
	char buf[BUFSIZE];
	char i=0;
	int fd;
	int count;
	
	memset(buf, 0, BUFSIZE);
	signal(SIGIO, signal_handler);
	
	printf("GPIO Set : %s\n", argv[1]);

	// 1. open() 테스트 -> gpio_open()
	fd=open("/dev/gpioled", O_RDWR);
	if(fd<0)
	{
		printf("Error : open()\n");
		return -1;
	}
	
	sprintf(buf,"%s:%d", argv[1], getpid());
	
	// 2. write() 테스 트-> gpio_write()
	count = write(fd, buf, strlen(buf));
	if(count<0)
			printf("Error : write()\n");
			
	// 3. read() 테스트 -> gpio_read()
	count = read(fd, buf, strlen(argv[1]));
	printf("Read data : %s\n", buf);

	while(1);
	
	// 4. close() 테스트 -> gpio_close()
	close(fd);		
	
	return 0;
}
