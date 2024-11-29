#include <linux/module.h>

MODULE_AUTHOR("Chen Miao <chenmiao.ku@gmail.com>");
MODULE_LICENSE("Dual BSD/GPL");

static __init int frdma_init_module(void) {
	return 0;
}

static void __exit frdma_exit_module(void) {}

module_init(frdma_init_module);
module_exit(frdma_exit_module);
