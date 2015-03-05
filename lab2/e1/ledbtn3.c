#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("maxime.bittan@gmail.com and redha.gouicem@gmail.com, 2015");
MODULE_DESCRIPTION("LED and button module for cross compiling");

#define GPIO_LED0   4
#define GPIO_LED1  17
#define GPIO_LED2  22
#define GPIO_LED3  27
#define GPIO_BTN0  18
#define GPIO_BTN1  23

static uint32_t* gpio_base_p;

static int ledparam, btnparam;

static int my_open_function (struct inode * inode, struct file * file);
static ssize_t my_read_function(struct file * file, char * buf, size_t count, loff_t * ppos);
static ssize_t my_write_function(struct file * file, const char * buf, size_t count, loff_t * ppos);
static int my_release_function(struct inode * inode, struct file * file);

struct file_operations fops = {.open = my_open_function,
			       .read = my_read_function,
			       .write = my_write_function,
			       .release = my_release_function };

static int __init mon_module_init(void)
{
  printk(KERN_DEBUG "Hello World !\n");
  if(register_chrdev(0, "ledbtn", &fops) < 0) {
    printk(KERN_ERR "Module registering failed.\n");
    return -1;
  }
  gpio_base_p = 0x20200000;
  // configuring pins
  (*(gpio_base_p + (1*(GPIO_LED0/10))))=((*(gpio_base_p + (1*(GPIO_LED0/10)))) & ~(0x7 << ((GPIO_LED0%10)*3)))
    | (2 << ((GPIO_LED0%10)*3)) ;
  (*(gpio_base_p + (1*(GPIO_LED1/10))))=((*(gpio_base_p + (1*(GPIO_LED1/10)))) & ~(0x7 << ((GPIO_LED1%10)*3)))
    | (2 << ((GPIO_LED1%10)*3)) ;
  (*(gpio_base_p + (1*(GPIO_LED2/10))))=((*(gpio_base_p + (1*(GPIO_LED2/10)))) & ~(0x7 << ((GPIO_LED2%10)*3)))
    | (2 << ((GPIO_LED2%10)*3)) ;
  (*(gpio_base_p + (1*(GPIO_LED3/10))))=((*(gpio_base_p + (1*(GPIO_LED3/10)))) & ~(0x7 << ((GPIO_LED3%10)*3)))
    | (2 << ((GPIO_LED3%10)*3)) ;
  (*(gpio_base_p + (1*(GPIO_BTN0/10))))=((*(gpio_base_p + (1*(GPIO_BTN0/10)))) & ~(0x7 << ((GPIO_BTN0%10)*3)))
    | (1 << ((GPIO_BTN0%10)*3)) ;
  (*(gpio_base_p + (1*(GPIO_BTN1/10))))=((*(gpio_base_p + (1*(GPIO_BTN1/10)))) & ~(0x7 << ((GPIO_BTN1%10)*3)))
    | (1 << ((GPIO_BTN1%10)*3)) ;
  return 0;
}

static void __exit mon_module_cleanup(void)
{
  unregister_chrdev(0, "ledbtn");
  printk(KERN_DEBUG "Goodbye World!\n");
}

static int my_open_function (struct inode * inode, struct file * file){
  printk(KERN_DEBUG "open()\n");
  return 0;
}

static ssize_t my_read_function(struct file * file, char * buf, size_t count, loff_t * ppos){
  printk(KERN_DEBUG "read()\n");
  buf[0] = ((*(gpio_base_p + ((0x34/4)+GPIO_BTN0/32)))>>(GPIO_BTN0%32)) & 0x1 ? '1' : '0';
  buf[1] = ((*(gpio_base_p + ((0x34/4)+GPIO_BTN1/32)))>>(GPIO_BTN1%32)) & 0x1 ? '1' : '0';
  return 2;
}

static ssize_t my_write_function(struct file * file, const char * buf, size_t count, loff_t * ppos){
  uint32_t *ad;
  printk(KERN_DEBUG "write()\n");
  
  ad = gpio_base_p + (0x1C/4)+ (GPIO_LED0/32);
  *ad= buf[0]=='1' ? (*ad | (0x1<<(GPIO_LED0%32))) : (*ad & ~(0x1<<(GPIO_LED0%32)));
  ad = gpio_base_p + (0x1C/4)+ (GPIO_LED0/32);
  *ad= buf[1]=='1' ? (*ad | (0x1<<(GPIO_LED1%32))) : (*ad & ~(0x1<<(GPIO_LED1%32)));
  ad = gpio_base_p + (0x1C/4)+ (GPIO_LED0/32);
  *ad= buf[2]=='1' ? (*ad | (0x1<<(GPIO_LED2%32))) : (*ad & ~(0x1<<(GPIO_LED2%32)));
  ad = gpio_base_p + (0x1C/4)+ (GPIO_LED0/32);
  *ad= buf[3]=='1' ? (*ad | (0x1<<(GPIO_LED3%32))) : (*ad & ~(0x1<<(GPIO_LED3%32)));

  return 4;
}

static int my_release_function(struct inode * inode, struct file * file){
  printk(KERN_DEBUG "close()\n");
  return 0;
}


module_param(ledparam, int, 0);
module_param(btnparam, int, 0);
module_init(mon_module_init);
module_exit(mon_module_cleanup);
