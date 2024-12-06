#ifndef __FAKE_DRIVER_H__
#define __FAKE_DRIVER_H__

#include <linux/netdevice.h>
#include <linux/pci.h>
#include <linux/types.h>
#include <rdma/ib_verbs.h>

#define FRDMA_MODULE_NAME   "frdma"
#define FRDMA_NODE_DESC     "Fake RDMA stack"
#define FRDMA_PCI_VENDOR_ID 0x6672// 0x66 0x72
#define FRDMA_PCI_DEVICE_ID 0x646d// 0x64 0x6d 0x61

#define FRDMA_FUNC_BAR 0
#define FRDMA_MISX_BAR 2
#define FRDMA_BAR_MASK ( BIT( FRDMA_FUNC_BAR ) | BIT( FRDMA_MISX_BAR ) )

struct frdma_devattr {
  unsigned char peer_addr[ ETH_ALEN ];
  int           numa_node;
};

struct frdma_dev {
  struct ib_device   ibdev;
  struct net_device* netdev;

  struct frdma_devattr* attrs;

  struct list_head cep_list;
};

#endif//! __FAKE_DRIVER_H__
