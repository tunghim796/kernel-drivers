#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(.gnu.linkonce.this_module) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section(__versions) = {
	{ 0x1317dc83, "module_layout" },
	{ 0x1ebd2813, "class_unregister" },
	{ 0x3645055f, "device_destroy" },
	{ 0x2e543947, "class_destroy" },
	{ 0xeccef39d, "device_create" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0xcf3a4bb0, "__class_create" },
	{ 0xc5850110, "printk" },
	{ 0xae4aee67, "__register_chrdev" },
	{ 0xb44ad4b3, "_copy_to_user" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "8549D53ECF4FFC845D893CD");
