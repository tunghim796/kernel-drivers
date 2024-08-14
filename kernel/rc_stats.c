#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/ioctl.h>

#define DEVICE_NAME "fpgawifi"
#define CLASS_NAME  "fpgaclass"
#define IOCTL_GET_STATS _IOR('f', 1, struct fpgawifi_rc_stats*)

static int major_number;
static struct class*  fpgawifi_class  = NULL;
static struct device* fpgawifi_device = NULL;

struct fpgawifi_rate_stats {
    uint8_t mcs;
    uint8_t nss;
    unsigned long nr_transmits;
    unsigned long nr_pkts;
    unsigned long nr_success;
};

struct fpgawifi_rc_stats_per_sta {
    uint8_t mac_addr[6];
    uint8_t last_rate_selected;
    struct fpgawifi_rate_stats rate_stats[40];
};

struct fpgawifi_rc_stats {
    uint8_t nr_of_stas;
    struct fpgawifi_rc_stats_per_sta sta_rc_stats[32];
};

// Global variable to hold the statistics
static struct fpgawifi_rc_stats fpgawifi_stats;

static long fpgawifi_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
    switch (cmd) {
        case IOCTL_GET_STATS:
            if (copy_to_user((void __user *)arg, &fpgawifi_stats, sizeof(fpgawifi_stats))) {
                return -EFAULT;
            }
            break;
        default:
            return -ENOTTY;
    }
    return 0;
}

static int fpgawifi_open(struct inode *inode, struct file *file) {
    return 0;
}

static int fpgawifi_release(struct inode *inode, struct file *file) {
    return 0;
}

static struct file_operations fpgawifi_fops = {
    .owner = THIS_MODULE,
    .open = fpgawifi_open,
    .release = fpgawifi_release,
    .unlocked_ioctl = fpgawifi_ioctl,
};

static int __init fpgawifi_init(void) {
    uint8_t mac_addr[6] = {0x1, 0x2, 0x3, 0x4, 0x6, 0x5};
    uint8_t mac_addr2[6] = {0x1, 0x2, 0x3, 0x4, 0x5, 0x6};

    // Register character device and get major number
    major_number = register_chrdev(0, DEVICE_NAME, &fpgawifi_fops);
    if (major_number < 0) {
        pr_err("Failed to register character device\n");
        return major_number;
    }

    // Register the device class
    fpgawifi_class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(fpgawifi_class)) {
        unregister_chrdev(major_number, DEVICE_NAME);
        pr_err("Failed to register device class\n");
        return PTR_ERR(fpgawifi_class);
    }

    // Register the device driver
    fpgawifi_device = device_create(fpgawifi_class, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);
    if (IS_ERR(fpgawifi_device)) {
        class_destroy(fpgawifi_class);
        unregister_chrdev(major_number, DEVICE_NAME);
        pr_err("Failed to create the device\n");
        return PTR_ERR(fpgawifi_device);
    }

    fpgawifi_stats.nr_of_stas = 2;
    memcpy(&fpgawifi_stats.sta_rc_stats[0].mac_addr, mac_addr,6);
    memcpy(&fpgawifi_stats.sta_rc_stats[1].mac_addr, mac_addr2,6);


    pr_info("fpgawifi device created successfully\n");
    return 0;
}

static void __exit fpgawifi_exit(void) {
    device_destroy(fpgawifi_class, MKDEV(major_number, 0));
    class_unregister(fpgawifi_class);
    class_destroy(fpgawifi_class);
    unregister_chrdev(major_number, DEVICE_NAME);
    pr_info("fpgawifi device removed\n");
}

module_init(fpgawifi_init);
module_exit(fpgawifi_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("FPGA WiFi Stats Module");
