#include "dummy_driver.h"

// Global variable to hold the statistics
static struct dummy_rc_stats tungnh_stats;
struct tungnh_base *tnhb = NULL;

static char example_data[100] = "default_data";

static ssize_t example_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
    return sprintf(buf, "%s\n", example_data);
}

static ssize_t example_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count) {
    size_t len = min(count, sizeof(example_data) - 1);
    if (copy_from_user(example_data, buf, len))
        return -EFAULT;
    example_data[len] = '\0';
    return count;
}


static long tungnh_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
    switch (cmd) {
        case IOCTL_GET_STATS:
            if (copy_to_user((void __user *)arg, &tungnh_stats, sizeof(tungnh_stats))) {
                return -EFAULT;
            }
            break;
        default:
            return -ENOTTY;
    }
    return 0;
}

static int tungnh_open(struct inode *inode, struct file *file) {
    return 0;
}

static int tungnh_release(struct inode *inode, struct file *file) {
    return 0;
}

static struct file_operations tungnh_fops = {
    .owner = THIS_MODULE,
    .open = tungnh_open,
    .release = tungnh_release,
    .unlocked_ioctl = tungnh_ioctl,
};

static int __init tungnh_init(void) {
    uint8_t mac_addr[6] = {0x1, 0x2, 0x3, 0x4, 0x6, 0x5};
    uint8_t mac_addr2[6] = {0x1, 0x2, 0x3, 0x4, 0x5, 0x6};
    int error;

    tnhb = kzalloc(sizeof(*tnhb), GFP_KERNEL);
    if (!tnhb) {
		pr_err("Unable to allocate memory for base struct...\n");
		return -ENOMEM;
	}

    // Register character device and get major number
    tnhb->dev_num = register_chrdev(0, DEVICE_NAME, &tungnh_fops);
    if (tnhb->dev_num < 0) {
        pr_err("Failed to register character device\n");
        goto fail_to_register_chardev;
    }

    // Register the device class
    tnhb->dev_class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(tnhb->dev_class)) {
        pr_err("Failed to register device class\n");
        goto fail_to_create_class;
    }

    // Register the device driver
    tnhb->dev = device_create(tnhb->dev_class, NULL, MKDEV(tnhb->dev_num, 0), NULL, DEVICE_NAME);
    if (IS_ERR(tnhb->dev)) {
        pr_err("Failed to create the device\n");
        goto fail_to_create_device;
    }

    KOBJ_ATTR_SET(tnhb->tnhb_kobj.example_attr, example, 0664, example_show, example_store);
    tnhb->tnhb_kobj.example_kobj = kobject_create_and_add("example", kernel_kobj);
    if (!tnhb->tnhb_kobj.example_kobj)
        return -ENOMEM;
       

    error = sysfs_create_file(tnhb->tnhb_kobj.example_kobj, &tnhb->tnhb_kobj.example_attr.attr);
    if (error) {
        pr_debug("failed to create the example file in /sys/kernel/example/\n");
        goto fail_to_create_sysfs;
    }

    tungnh_stats.nr_of_stas = 2;
    memcpy(&tungnh_stats.sta_rc_stats[0].mac_addr, mac_addr,6);
    memcpy(&tungnh_stats.sta_rc_stats[1].mac_addr, mac_addr2,6);


    pr_info("dummy device created successfully\n");
    return 0;
fail_to_create_sysfs:
    kobject_put(tnhb->tnhb_kobj.example_kobj);
fail_to_create_device:
    class_unregister(tnhb->dev_class);
    class_destroy(tnhb->dev_class);
fail_to_create_class:
    unregister_chrdev(tnhb->dev_num, DEVICE_NAME);
fail_to_register_chardev:
    kfree(tnhb);
    return -1;
}

static void __exit tungnh_exit(void) {
    kobject_put(tnhb->tnhb_kobj.example_kobj);
    device_destroy(tnhb->dev_class, MKDEV(tnhb->dev_num, 0));
    class_unregister(tnhb->dev_class);
    class_destroy(tnhb->dev_class);
    unregister_chrdev(tnhb->dev_num, DEVICE_NAME);
    pr_info("dummy device removed\n");
}

module_init(tungnh_init);
module_exit(tungnh_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("FPGA WiFi Stats Module");
