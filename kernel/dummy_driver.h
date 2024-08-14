#ifndef __DUMMY_DRIVER_H__
#define __DUMMY_DRIVER_H__

#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/of.h>	//of_device_table ,....
#include <linux/module.h> /* macro definition: module_init va module_exit */
#include <linux/fs.h> /* function to allocate/free device number */
#include <linux/device.h> /* create device file */
#include <linux/slab.h>     // kmalloc va kfree
#include <linux/cdev.h> /* cdev */
#include <linux/uaccess.h> //copy_to_user, copy_from_user
#include <linux/interrupt.h> 
#include <linux/err.h>
#include <linux/kernel.h> /* sprintf*/

#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/of_platform.h>

#define DEVICE_NAME "dummy_device"
#define CLASS_NAME  "dummy_class"
#define IOCTL_GET_STATS _IOR('f', 1, struct dummy_rc_stats*)


struct dummy_rate_stats {
    uint8_t mcs;
    uint8_t nss;
    unsigned long nr_transmits;
    unsigned long nr_pkts;
    unsigned long nr_success;
};

struct dummy_rc_stats_per_sta {
    uint8_t mac_addr[6];
    uint8_t last_rate_selected;
    struct dummy_rate_stats rate_stats[40];
};

struct dummy_rc_stats {
    uint8_t nr_of_stas;
    struct dummy_rc_stats_per_sta sta_rc_stats[32];
};

struct tungnh_base {
    dev_t dev_num;
    struct class *dev_class;
    struct device *dev;
};


#endif