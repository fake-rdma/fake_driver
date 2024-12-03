#include "linux/device.h"
#include "linux/init.h"
#include "linux/io.h"
#include "linux/ioport.h"
#include "linux/kern_levels.h"
#include "linux/list.h"
#include "linux/pci.h"
#include "linux/printk.h"
#include <linux/module.h>

#include <rdma/ib_verbs.h>
#include "rdma/rdma_netlink.h"

#include "fake_driver.h"

MODULE_AUTHOR("Chen Miao <chenmiao.ku@gmail.com>");
MODULE_LICENSE("Dual BSD/GPL");

const struct ib_device_ops frdma_device_ops = {
	.owner = THIS_MODULE,
	.driver_id = 21,
};

static struct frdma_dev *dev;

// static int frdma_get_netdev(struct frdma_dev *dev)
// {
// 	struct net_device *netdev;
// 	int ret = -EPROBE_DEFER;

// 	if (dev->netdev)
// 		return 0;

// 	// rtnl_lock();
// 	// for_each_netdev(&init_net, netdev) {
// 	// }
// 	// rtnl_unlock();

// 	return ret;
// }

static __init int frdma_init_module(void)
{
	int ret = 0;
	printk(KERN_INFO "frdma init module\n");

	dev = ib_alloc_device(frdma_dev, ibdev);
	if (!dev || !(&(dev->ibdev))) {
		dev_err(&dev->ibdev.dev, "ib_alloc_device failed\n");
		return -ENOMEM;
	}

	dev->ibdev.node_type = RDMA_NODE_RNIC;
	memcpy(&dev->ibdev.node_desc, FRDMA_NODE_DESC, sizeof(FRDMA_NODE_DESC));

	dev->ibdev.phys_port_cnt = 1;
	dev->ibdev.num_comp_vectors = 1;

	// ret = ib_device_set_netdev(&dev->ibdev, NULL, 1);
	// if (ret) {
	// 	dev_err(&dev->ibdev.dev, "ib_device_set_netdev failed\n");
	// 	return ret;
	// }
	// dev->netdev = NULL;

	ib_set_device_ops(&dev->ibdev, &frdma_device_ops);

	// ret = frdma_get_netdev(dev);
	ret = ib_register_device(&dev->ibdev, "frdma_%d");
	if (ret) {
		dev_err(&dev->ibdev.dev, "ib_register_device: ret = %d\n", ret);
		ib_dealloc_device(&dev->ibdev);
		return ret;
	}

	return ret;
}

static void __exit frdma_exit_module(void)
{
	printk(KERN_INFO "frdma exit module\n");
	ib_unregister_device(&dev->ibdev);
	ib_dealloc_device(&dev->ibdev);
}

module_init(frdma_init_module);
module_exit(frdma_exit_module);
