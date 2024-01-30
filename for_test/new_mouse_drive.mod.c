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
	{ 0x3e549f1d, "module_layout" },
	{ 0x4df55e5b, "usb_deregister" },
	{ 0x8e48f69b, "usb_register_driver" },
	{ 0x4e2531ba, "input_register_device" },
	{ 0xc2f800d5, "devm_input_allocate_device" },
	{ 0xd2c63d3, "usb_alloc_coherent" },
	{ 0x7a4fec84, "kmem_cache_alloc_trace" },
	{ 0xf23ce8b, "kmalloc_caches" },
	{ 0x76454a84, "input_event" },
	{ 0xf6d62746, "usb_submit_urb" },
	{ 0x6d65cce6, "usb_alloc_urb" },
	{ 0xc5850110, "printk" },
	{ 0x6fa1627, "usb_free_urb" },
	{ 0x63da51a6, "usb_kill_urb" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x2a57243, "input_unregister_device" },
	{ 0x37a0cba, "kfree" },
	{ 0x11131f1b, "usb_free_coherent" },
};

MODULE_INFO(depends, "");

