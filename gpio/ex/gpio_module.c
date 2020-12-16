#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/module.h>
#include <linux/io.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("KOOK");
MODULE_DESCRIPTION("Raspberry Pi GPIO LED Device Module");

#define BCM_IO_BASE 0x3F000000
#define GPIO_BASE (BCM_IO_BASE + 0x200000)
#define GPIO_SIZE (256)
#define GPIO_IN(g) (*(gpio+((g)/10)) &= ~(7<<(((g)%10)*3)))
#define GPIO_OUT(g) (*(gpio+((g)/10)) |= (1<<(((g)%10)*3)))
#define GPIO_SET(g) (*(gpio+7) = 1<<g)
#define GPIO_CLR(g) (*(gpio+10) = 1<<g)
#define GPIO_GET(g) (*(gpio+13) & (1<<g))
#define GPIO_MAJOR 200
#define GPIO_DEVICE "GPIO_IRQ"
#define GPIO_LED 18
#define GPIO_SW 12
#define INT_BUFF_MAX 64

volatile unsigned *gpio;

static int gpio_open(struct inode*, struct file*);
static ssize_t gpio_read(struct file*, char*, size_t, loff_t*);
static ssize_t gpio_write(struct file*, const char*, size_t, loff_t*);
static int gpio_close(struct inode*, struct file*);

static struct file_operations gpio_fops = {
.owner = THIS_MODULE,
.read = gpio_read,
.write = gpio_write,
.open = gpio_open,
.release = gpio_close,
};

typedef struct{
unsigned long time;
} __attribute__ ((packed)) R_INT_INFO;

R_INT_INFO intbuffer[INT_BUFF_MAX];
int intcount;
static int switch_irq;

void int_clear(void)
{
    int lp;
    for(lp = 0; lp < INT_BUFF_MAX; lp++)
    {
        intbuffer[lp].time = 0;
    }
    
    intcount = 0;
}

static irqreturn_t isr_func(int irq, void *data)
{
    if(intcount < INT_BUFF_MAX)
    {
        intbuffer[intcount].time = get_jiffies_64();
        intcount++;
    }
    
    return IRQ_HANDLED;
}

int gpio_init(void)
{
    static void *map;
    int result =-1;
	
    printk(KERN_INFO "Hello module!\n");
	
    result = register_chrdev(GPIO_MAJOR, GPIO_DEVICE, &gpio_fops);
    
    if(result < 0)
    {
        return result;
    }
	
    map = ioremap(GPIO_BASE, GPIO_SIZE);
	
    if(!map)
    {
        printk("Error : mapping GPIO Memory\n");
        iounmap(map);
        return -EBUSY;
    }
    
    gpio = (volatile unsigned int*)map;
    GPIO_IN(GPIO_LED);
    GPIO_OUT(GPIO_LED);
    gpio_request(GPIO_SW, "SWITCH");
    switch_irq = gpio_to_irq(GPIO_SW);
	
    return 0;
}

void gpio_exit(void)
{
    unregister_chrdev(GPIO_MAJOR, GPIO_DEVICE);
    gpio_free(GPIO_SW);
	
    if(gpio)
    {
        iounmap(gpio);
    }

    printk(KERN_INFO "Good-bye module!\n");
}

static int gpio_open(struct inode *inod, struct file* fil)
{
    printk("GPIO Device Opened(%d/%d)\n", imajor(inod), iminor(inod));
    try_module_get(THIS_MODULE);

    if(request_irq(switch_irq, isr_func, IRQF_TRIGGER_RISING, "switch", NULL) < 0)
    {
        return -EBUSY;
    }

    int_clear();
	
    return 0;
}

static int gpio_close(struct inode* inod, struct file* fil)
{
    printk("GPIO Device closed(%d)\n", MAJOR(fil->f_inode->i_rdev));
    module_put(THIS_MODULE);
    free_irq(switch_irq, NULL);
    
    return 0;
}

static ssize_t gpio_read(struct file* inode, char* buff, size_t len, loff_t* off)
{
    int readcount;
    char* ptrdata;
    int loop;
    readcount = len / sizeof(R_INT_INFO);

    if(readcount > intcount)
    {
        readcount = intcount;
    }

    ptrdata = (char*) &intbuffer[0];

    for(loop = 0; loop < readcount * sizeof(R_INT_INFO); loop++)
    {
        copy_to_user(&buff[loop], &ptrdata[loop], sizeof(R_INT_INFO));
    }

    return readcount * sizeof(R_INT_INFO);
}

static ssize_t gpio_write(struct file* inode, const char* buff, size_t len, loff_t* off)
{
    static char status[5] ={0};
    int loop;
    short count=0;
    //const char* cmpvalue = "0";
    int_clear();
    
    for(loop = 0; loop < len; loop++)
    {
        count = copy_from_user(status, buff, len);
        (!strcmp(status, "0"))?GPIO_CLR(GPIO_LED):GPIO_SET(GPIO_LED);
    }
    
    return count;
}

module_init(gpio_init);
module_exit(gpio_exit);