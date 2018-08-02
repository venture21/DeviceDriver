#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/module.h>
#include <linux/io.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("HJ PARK");
MODULE_DESCRIPTION("RASPBERRY PI GPIO LED DRIVER");


// Raspi 3 PHYSICAL I/O PERI BASE ADDR
#define BCM_IO_BASE 0x3F000000

// GPIO_ADDR(BASE+0x200000)
#define GPIO_BASE  (BCM_IO_BASE + 0x200000)

// GPIO Function Select 0~5 [0x3F200000 ~ 0x3F200014]
//                          [해당핀의 설정 레지스터 번지] [해당 핀의 설정 비트 위치]
#define GPIO_IN(g)		(*(gpio+((g)/10)) &= ~(7<<(((g)%10)*3)))
#define GPIO_OUT(g)     (*(gpio+((g)/10)) |= (1<<(((g)%10)*3)))

//GPIO Pin Output Set 0 / Clr 0
#define GPIO_SET(g) (*(gpio+7) = (1<<g))
#define GPIO_CLR(g) (*(gpio+10) = 1<<g)

#define GPIO_GET(g) (*(gpio+13)&(1<<g))

#define GPIO_SIZE 0xB4

#define GPIO_MAJOR 200
#define GPIO_MINOR 0
#define GPIO_DEVICE "gpioled"
#define GPIO_LED		17	// Raspi GPIO17 -> LED

struct cdev gpio_cdev;

static struct file_operations gpio_fops = {
		.owner = THIS_MODULE,
	//	.read   = gpio_read,
	//	.write = gpio_write,
	//	.open   = gpio_open,
	//	.release = gpio_close,
};

volatile unsigned int *gpio;

int initModule(void)
{
	dev_t devno;
	unsigned int count;
	static void *map;
	
	int err;
	
	printk(KERN_INFO "initModule : gpio module init\n");
	
	// 1. 문자 디바이스를 등록한다.
	devno = MKDEV(GPIO_MAJOR, GPIO_MINOR);
	printk(KERN_INFO "devno=%x\n",devno); 
	register_chrdev_region(devno, 1, GPIO_DEVICE);
	
	// 2. 문자 디바이스를 위한 구조체를 초기화 한다.	
	cdev_init(&gpio_cdev, &gpio_fops);
	gpio_cdev.owner = THIS_MODULE;
	count =1;
	
	// 3. 문자디바이스 추가
	err =cdev_add(&gpio_cdev, devno, count);
	if(err<0)
	{
		printk(KERN_INFO "Error : cdev_add()\n");
		return -1;
	}
	
	printk(KERN_INFO "'mknod /dev/%s c %d 0'\n", GPIO_DEVICE, GPIO_MAJOR);
	printk(KERN_INFO "'chmod  666 /dev/%s'\n", GPIO_DEVICE);
	
	// 4. 물리 메모리 번지를 인자로 전달하면 가상 메모리 번지를 리턴한다.
	map = ioremap(GPIO_BASE, GPIO_SIZE);
	if(!map)
	{
		printk(KERN_INFO "Error : mapping GPIO memory\n");
		iounmap(map);
		return -EBUSY;
	}
	
	gpio = (volatile unsigned int*)map; 
	GPIO_OUT(GPIO_LED);     // 위에서 #define으로 17번으로 설정
	
 	return 0;
}

void cleanupModule(void)
{
	printk(KERN_INFO "called : cleanupModule()\n");
}


module_init(initModule);
module_exit(cleanupModule);


