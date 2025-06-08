#include <linux/module.h>
#include <linux/printk.h> 
#include <linux/kobject.h>
#include <linux/sysfs.h> 
#include <linux/fs.h> 
#include <linux/string.h>
#include <linux/init.h>
#include <linux/tty.h>          // fg_console, MAX_NR_CONSOLES
#include <linux/kd.h>           // KDSETLED
#include <linux/vt.h>
#include <linux/console_struct.h>
#include <linux/vt_kern.h>
#include <linux/timer.h>

#define BLINK_DELAY   (HZ / 5)
#define ALL_LEDS_ON   0x07
#define RESTORE_LEDS  0xFF

MODULE_AUTHOR("Silantev Dmitriy");
MODULE_DESCRIPTION("Keyboard blinking LEDs.");
MODULE_LICENSE("GPL");

static struct timer_list my_timer;
static struct tty_driver *my_driver;
static int _kbledstatus = 0;
static int test = 0;
static struct kobject *example_kobject;

// === SYSFS ===

static ssize_t foo_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "%d\n", test);
}

static ssize_t foo_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    sscanf(buf, "%d", &test);  // исправлено: убрано 'u'
    return count;
}

static struct kobj_attribute foo_attribute = __ATTR(test, 0660, foo_show, foo_store);

static int __init sys_init(void)
{
    int error = 0;

    pr_debug("sys_init: creating sysfs interface\n");

    example_kobject = kobject_create_and_add("systest", kernel_kobj);
    if (!example_kobject)
        return -ENOMEM;

    error = sysfs_create_file(example_kobject, &foo_attribute.attr);
    if (error) {
        pr_debug("sys_init: failed to create sysfs file\n");
        kobject_put(example_kobject);
    }

    return error;
}

static void __exit sys_exit(void)
{
    pr_debug("sys_exit: removing sysfs interface\n");
    kobject_put(example_kobject);
}

// === TIMER FUNCTION ===

static void my_timer_func(struct timer_list *ptr)
{
    int *pstatus = &_kbledstatus;

    if (*pstatus == test)
        *pstatus = RESTORE_LEDS;
    else
        *pstatus = test;

    if (vc_cons[fg_console].d && vc_cons[fg_console].d->port.tty) {
        my_driver->ops->ioctl(vc_cons[fg_console].d->port.tty, KDSETLED, *pstatus);
    }

    my_timer.expires = jiffies + BLINK_DELAY;
    add_timer(&my_timer);
}

// === MODULE INIT/EXIT ===

static int __init kbleds_init(void)
{
    printk(KERN_INFO "kbleds: loading module\n");

    if (!vc_cons[fg_console].d || !vc_cons[fg_console].d->port.tty) {
        printk(KERN_ERR "kbleds: no tty for fg_console\n");
        return -ENODEV;
    }

    my_driver = vc_cons[fg_console].d->port.tty->driver;

    timer_setup(&my_timer, my_timer_func, 0);
    my_timer.expires = jiffies + BLINK_DELAY;
    add_timer(&my_timer);

    if (sys_init() != 0) {
        printk(KERN_ERR "kbleds: failed to initialize sysfs\n");
        return -1;
    }

    return 0;
}

static void __exit kbleds_cleanup(void)
{
    printk(KERN_INFO "kbleds: unloading module\n");
    del_timer(&my_timer);

    if (vc_cons[fg_console].d && vc_cons[fg_console].d->port.tty) {
        my_driver->ops->ioctl(vc_cons[fg_console].d->port.tty, KDSETLED, RESTORE_LEDS);
    }

    sys_exit();
}

module_init(kbleds_init);
module_exit(kbleds_cleanup);
