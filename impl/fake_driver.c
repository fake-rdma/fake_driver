#include "linux/device.h"
#include "linux/init.h"
#include "linux/io.h"
#include "linux/ioport.h"
#include "linux/kern_levels.h"
#include "linux/list.h"
#include "linux/pci.h"
#include "linux/printk.h"
#include "rdma/rdma_netlink.h"
#include <linux/dev_printk.h>
#include <linux/module.h>
#include <rdma/ib_verbs.h>

#include "fake_driver.h"

MODULE_AUTHOR("Chen Miao <chenmiao.ku@gmail.com>");
MODULE_LICENSE("Dual BSD/GPL");

static const struct ib_device_ops frdma_device_ops = {
	.owner = THIS_MODULE,
	.driver_id = 21,
};

static int frdma_pdev_init(struct pci_dev* pdev) {
	struct frdma_dev* dev;
	int bars, err;

	printk(KERN_WARNING "frdma pci: start to enable device\n");
	err = pci_enable_device(pdev);
	if (err) {
		dev_err(&pdev->dev, "pci_enable_device failed(%d)\n", err);
		return err;
	}

	pci_set_master(pdev);

	printk(KERN_WARNING "frdma pci: start to alloce ib device\n");
	dev = ib_alloc_device(frdma_dev, ibdev);
	if (!dev) {
		dev_err(&pdev->dev, "ib_alloce_device failed\n");
		err = -ENOMEM;
		goto err_disable_device;
	}

	pci_set_drvdata(pdev, dev);
	dev->pci_dev = pdev;
	dev->attrs->numa_node = dev_to_node(&pdev->dev);

	bars = pci_select_bars(pdev, IORESOURCE_MEM);
	err = pci_request_selected_regions(pdev, bars, FRDMA_MODULE_NAME);
	if (bars != FRDMA_BAR_MASK || err) {
		err = err ? err : -EINVAL;
		goto err_ib_device_release;
	}

	dev->pci_bar_addr = pci_resource_start(pdev, FRDMA_FUNC_BAR);
	dev->pci_bar_len = pci_resource_len(pdev, FRDMA_FUNC_BAR);

	dev->pci_bar_mem = devm_ioremap(&pdev->dev, dev->pci_bar_addr, dev->pci_bar_len);
	if (!dev->pci_bar_mem) {
		dev_err(&pdev->dev, "devm_ioremap failed\n");
		err = -EFAULT;
		goto err_release_bars;
	}


err_release_bars:
	pci_release_selected_regions(pdev, bars);

err_ib_device_release:
	ib_dealloc_device(&dev->ibdev);

err_disable_device:
	pci_disable_device(pdev);

	return err;
}

static int frdma_ib_device_add(struct pci_dev* pdev) {
	struct frdma_dev* dev = pci_get_drvdata(pdev);
	struct ib_device* ibdev = &dev->ibdev;
	int ret;

	ibdev->node_type = RDMA_NODE_RNIC;
	memcpy(ibdev->node_desc, FRDMA_NODE_DESC, sizeof(FRDMA_NODE_DESC));

	ib_set_device_ops(ibdev, &frdma_device_ops);
	INIT_LIST_HEAD(&dev->cep_list);

	ret = ib_register_device(ibdev, "frdma_%d", &dev->pci_dev->dev);
	if (ret) {
		dev_err(&dev->pci_dev->dev, "ib_register_device failed: ret = %d\n", ret);
		return ret;
	}

	return ret;
}

static int frdma_probe(struct pci_dev* pdev, const struct pci_device_id* id) {
	struct frdma_dev* dev;
	int ret;

	ret = frdma_pdev_init(pdev);
	if (ret) return ret;

	ret = frdma_ib_device_add(pdev);
	if (ret) {
		dev = pci_get_drvdata(pdev);
		pci_release_selected_regions(pdev, FRDMA_BAR_MASK);
		ib_dealloc_device(&dev->ibdev);
		pci_disable_device(pdev);
		return ret;
	}

	return ret;
}

static void frdma_remove(struct pci_dev* pdev) {
	struct frdma_dev* dev = pci_get_drvdata(pdev);
	ib_unregister_device(&dev->ibdev);
	pci_release_selected_regions(pdev, FRDMA_BAR_MASK);
	ib_dealloc_device(&dev->ibdev);
	pci_disable_device(pdev);
}

static const struct pci_device_id frdma_pci_tbl[] = {
	{ PCI_DEVICE(FRDMA_PCI_VENDOR_ID, FRDMA_PCI_DEVICE_ID) },
	{ 0 },
};

static struct pci_driver frdma_pci_driver = {
	.name = FRDMA_MODULE_NAME,
	.id_table = frdma_pci_tbl,
	.probe = frdma_probe,
	.remove = frdma_remove,
};

static __init int frdma_init_module(void) {
	int ret;

	printk(KERN_WARNING "frdma init module\n");
	ret = pci_register_driver(&frdma_pci_driver);

	return ret;
}

static void __exit frdma_exit_module(void) {
	printk(KERN_WARNING "frdma exit module\n");
	pci_unregister_driver(&frdma_pci_driver);
}

module_init(frdma_init_module);
module_exit(frdma_exit_module);
