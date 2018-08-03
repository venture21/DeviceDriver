#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x13e75575, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xfe990052, __VMLINUX_SYMBOL_STR(gpio_free) },
	{ 0xc1514a3b, __VMLINUX_SYMBOL_STR(free_irq) },
	{ 0x3def299e, __VMLINUX_SYMBOL_STR(cdev_del) },
	{ 0x7485e15e, __VMLINUX_SYMBOL_STR(unregister_chrdev_region) },
	{ 0xfc982daa, __VMLINUX_SYMBOL_STR(del_timer_sync) },
	{ 0x5be5f04e, __VMLINUX_SYMBOL_STR(gpiod_direction_output_raw) },
	{ 0xd6b8e852, __VMLINUX_SYMBOL_STR(request_threaded_irq) },
	{ 0xb6c1e257, __VMLINUX_SYMBOL_STR(gpiod_to_irq) },
	{ 0x47229b5c, __VMLINUX_SYMBOL_STR(gpio_request) },
	{ 0xe8c32752, __VMLINUX_SYMBOL_STR(cdev_add) },
	{ 0xa27b9cb6, __VMLINUX_SYMBOL_STR(cdev_init) },
	{ 0xd8e484f0, __VMLINUX_SYMBOL_STR(register_chrdev_region) },
	{ 0xf4fa543b, __VMLINUX_SYMBOL_STR(arm_copy_to_user) },
	{ 0x97255bdf, __VMLINUX_SYMBOL_STR(strlen) },
	{ 0x1e047854, __VMLINUX_SYMBOL_STR(warn_slowpath_fmt) },
	{ 0xe2d5255a, __VMLINUX_SYMBOL_STR(strcmp) },
	{ 0xb742fd7, __VMLINUX_SYMBOL_STR(simple_strtol) },
	{ 0x85df9b6c, __VMLINUX_SYMBOL_STR(strsep) },
	{ 0xc499ae1e, __VMLINUX_SYMBOL_STR(kstrdup) },
	{ 0x28cc25db, __VMLINUX_SYMBOL_STR(arm_copy_from_user) },
	{ 0xa5d02a55, __VMLINUX_SYMBOL_STR(try_module_get) },
	{ 0x997d395a, __VMLINUX_SYMBOL_STR(module_put) },
	{ 0x2e5810c6, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr1) },
	{ 0xf8791c99, __VMLINUX_SYMBOL_STR(send_sig_info) },
	{ 0x2b44db01, __VMLINUX_SYMBOL_STR(pid_task) },
	{ 0xbd7683a8, __VMLINUX_SYMBOL_STR(find_vpid) },
	{ 0xfa2a45e, __VMLINUX_SYMBOL_STR(__memzero) },
	{ 0xf64a8ce, __VMLINUX_SYMBOL_STR(gpiod_get_raw_value) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x9ca0b1e0, __VMLINUX_SYMBOL_STR(gpiod_set_raw_value) },
	{ 0xbaeef7b0, __VMLINUX_SYMBOL_STR(gpio_to_desc) },
	{ 0xb1ad28e0, __VMLINUX_SYMBOL_STR(__gnu_mcount_nc) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "57A37982D3C7E78E717A9F8");
