#include <linux/fs.h>				//open(), read(), write(), close()
#include <linux/cdev.h>			//register_chrdev_region(), cdev_init()
#include <linux/module.h>		// device Driver
#include <linux/io.h>				// ioremap(), iounmap() 삭제 해도 됨
#include <linux/uaccess.h>		// copy_from_user(), copy_to_user()
#include <linux/gpio.h>			// request_gpio(), gpio_set_value()
#include <linux/interrupt.h>	// gpio_to_irq(), request_irq()
#include <linux/timer.h>			// init_timer(), add_timer(), del_timer()

MODULE_LICENSE("GPL");
MODULE_AUTHOR("HJ PARK");
MODULE_DESCRIPTION("RASPBERRY PI GPIO LED DRIVER");

#define GPIO_MAJOR 200
#define GPIO_MINOR 0
#define GPIO_DEVICE "gpioled"
#define GPIO_LED		17	// Raspi GPIO17 -> LED
#define GPIO_SW		18	// Raspi GPIO18 -> SW

#define STR_SIZE 100

static char msg[STR_SIZE] = {0};

struct cdev gpio_cdev;
static int switch_irq;
static struct timer_list timer;	// 타이머 처리를 위한 구조체

// 함수원형 선언 
static int gpio_open(struct inode *, struct file *);
static int gpio_close(struct inode *, struct file *);
static ssize_t gpio_read(struct file*, char *, size_t, loff_t *);
static ssize_t gpio_write(struct file*, const char *, size_t, loff_t *);

static struct file_operations gpio_fops = {
		.owner = THIS_MODULE,
		.read   = gpio_read,
		.write = gpio_write,
		.open   = gpio_open,
		.release = gpio_close,
};

volatile unsigned int *gpio;


static void timer_func(unsigned long data)
{
	gpio_set_value(GPIO_LED, data);
	if(data)
		timer.data = 0;
	else
		timer.data = 1;
	
	timer.expires = jiffies + (1*HZ);	
	add_timer(&timer);
}
 
static irqreturn_t isr_func(int irq, void *data)
{
	// GPIO18번 스위치에서 IRQ Rising Edge발생 && LED가 OFF일때
	static int count;
	if(irq==switch_irq && !gpio_get_value(GPIO_LED))
		gpio_set_value(GPIO_LED, 1);
	else
		gpio_set_value(GPIO_LED, 0);
	
	printk(KERN_INFO "Called isr_func():%d\n", ++count);
	return IRQ_HANDLED;
}

static int gpio_open(struct inode *inod, struct file *fil)
{
	// app에서 open()가 호출될 때마다 모듈의 사용 카운터를 증가 시킨다.
	try_module_get(THIS_MODULE);
	printk(KERN_INFO "GPIO Device opened\n");
	return 0;
	}

static int gpio_close(struct inode *inod, struct file *fil)
{
	// app에서 close()함수가 호출될 때마다 모듈의 사용 카운터를 감소 시킨다.
	module_put(THIS_MODULE);
	printk(KERN_INFO "GPIO Device closed\n");
	return 0;
}

static ssize_t gpio_read(struct file *inode, char *buff, size_t len, loff_t *off)
{
	// app에서 read()함수가 호출될 때마다 gpio_read()함수가 호출된다.
	int count;
	
	if(gpio_get_value(GPIO_LED))
		msg[0]='1';
	else
		msg[0]='0';

	strcat(msg, "from kernel");
	count = copy_to_user(buff, msg, strlen(msg)+1);
	printk(KERN_INFO "GPIO Device read:%s\n", msg);
	return (ssize_t)count;
}

static ssize_t gpio_write(struct file *inode, const char *buff, size_t len, loff_t *off)
{
	int count;
	memset(msg, 0, STR_SIZE);
	count = copy_from_user(msg,buff,len);

	if(!strcmp(msg,"0"))
	{
		init_timer(&timer);
		timer.function = timer_func;
		timer.data = 1L;
		timer.expires = jiffies + (1*HZ);
		add_timer(&timer);
		
	}


	//gpio_set_value(GPIO_LED, (!strcmp(msg,"0"))?0:1);

	printk(KERN_INFO "GPIO Device write : $s\n", msg);
	
	return (ssize_t) count;
}

int initModule(void)
{
	dev_t devno;
	unsigned int count;
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
	
	// gpio.h에 정의된 gpio_request함수를 사용
	// LED pin에 대한 요청
	err = gpio_request(GPIO_LED, "LED");
	if(err==-EBUSY)
	{
		printk(KERN_INFO "Error gpio_request : LED\n");
		return -1;
	}
	
	// SW pin에 대한 요청
	err = gpio_request(GPIO_SW, "SWITCH");
	if(err==-EBUSY)
	{
		printk(KERN_INFO "Error gpio_request : SW\n");
		return -1;
	}
	
	// GPIO 인터럽트 번호를 할당
	switch_irq =gpio_to_irq(GPIO_SW);
	err = request_irq(switch_irq, isr_func, IRQF_TRIGGER_RISING, "switch", NULL);
	if(err)
	{
		printk(KERN_INFO "Error request_irq\n");
		return -1;
	}
	
	
	gpio_direction_output(GPIO_LED, 0);
	
 	return 0;
}

void cleanupModule(void)
{
	dev_t devno;
	devno = MKDEV(GPIO_MAJOR, GPIO_MINOR);
	del_timer_sync(&timer);
	
	// 1. 문자 디바이스의 등록을 해제한다.
	unregister_chrdev_region(devno, 1);
	
	// 2. 문자 디바이스의 구조체를 삭제한다.
	cdev_del(&gpio_cdev);
	
	// 3. 등록된 switch_irq의 인터럽트 해제 
	free_irq(switch_irq, NULL);
	
	gpio_direction_output(GPIO_LED, 0);
	gpio_free(GPIO_LED);
	gpio_free(GPIO_SW);
			
	printk(KERN_INFO "operation is done! : cleanupModule()\n");		
}


module_init(initModule);
module_exit(cleanupModule);


