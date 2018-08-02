#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define BUFSIZE 	50

int main(int argc, char** argv)
{
	char buf[BUFSIZE];
	char i=0;
	int fd;
	int count;
	
	memset(buf, 0, BUFSIZE);
	
	printf("GPIO Set : %s\n", argv[1]);

	// 1. open() 테스트 -> gpio_open()
	fd=open("/dev/gpioled", O_RDWR);
	if(fd<0)
	{
		printf("Error : open()\n");
		return -1;
	}
	
	// 2. write() 테스 트-> gpio_write()
	count = write(fd, argv[1], strlen(argv[1]));
	if(count<0)
			printf("Error : write()\n");
			
	// 3. read() 테스트 -> gpio_read()
	count = read(fd, buf, strlen(argv[1]));
	printf("Read data : %s\n", buf);

	// 4. close() 테스트 -> gpio_close()
	close(fd);		
	
	return 0;
}
