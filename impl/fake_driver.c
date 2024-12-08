#include "linux/device.h"
#include "linux/init.h"
#include "linux/io.h"
#include "linux/ioport.h"
#include "linux/kern_levels.h"
#include "linux/list.h"
#include "linux/pci.h"
#include "linux/printk.h"
#include <linux/module.h>

#include "rdma/rdma_netlink.h"
#include <rdma/ib_verbs.h>

#include "fake_driver.h"
// #include "frdma_verb_ops.h"

MODULE_AUTHOR( "Chen Miao <chenmiao.ku@gmail.com>" );
MODULE_LICENSE( "Dual BSD/GPL" );

static struct rdma_hw_stats* frdma_alloc_hw_port_stats( struct ib_device* device,
                                                        u32               port_num ) {
  return NULL;
}

static int frdma_get_hw_stats( struct ib_device*     ibdev,
                               struct rdma_hw_stats* stats,
                               u32                   port,
                               int                   index ) {
  return 0;
}

static int frdma_get_port_immutable( struct ib_device* dev, u32 port,
                                     struct ib_port_immutable* ib_port_immutable ) {
  return 0;
}

static int frdma_map_mr_sg( struct ib_mr*       ibmr,
                            struct scatterlist* sg,
                            int                 sg_nents,
                            unsigned int*       sg_offset ) {
  return 0;
}

static int frdma_mmap( struct ib_ucontext* ctx, struct vm_area_struct* vma ) {
  return 0;
}

static void frdma_mmap_free( struct rdma_user_mmap_entry* rdma_entry ) {}

static int frdma_modify_qp( struct ib_qp*      ibqp,
                            struct ib_qp_attr* attr,
                            int                mask,
                            struct ib_udata*   data ) {
  return 0;
}

static int frdma_query_device( struct ib_device*      dev,
                               struct ib_device_attr* attr,
                               struct ib_udata*       data ) {
  return 0;
}

static int frdma_query_gid( struct ib_device* dev,
                            u32               port,
                            int               idx,
                            union ib_gid*     gid ) {
  return 0;
}

static int frdma_query_port( struct ib_device*    dev,
                             u32                  port,
                             struct ib_port_attr* attr ) {
  return 0;
}

static int frdma_query_qp( struct ib_qp*           ibqp,
                           struct ib_qp_attr*      attr,
                           int                     mask,
                           struct ib_qp_init_attr* init_attr ) {
  return 0;
}

static int frdma_req_notify_cq( struct ib_cq*           ibcq,
                                enum ib_cq_notify_flags flags ) {
  return 0;
}

static struct ib_mr* frdma_reg_user_mr( struct ib_pd*    ibpd,
                                        u64              start,
                                        u64              len,
                                        u64              virt,
                                        int              access,
                                        struct ib_udata* udata ) {
  return NULL;
}

const struct ib_device_ops frdma_device_ops = {
  .owner          = THIS_MODULE,
  .driver_id      = 21,
  .uverbs_abi_ver = 1,

  .alloc_hw_port_stats = frdma_alloc_hw_port_stats,// must
  .get_hw_stats        = frdma_get_hw_stats,       // must
  .get_port_immutable  = frdma_get_port_immutable, // must
  .map_mr_sg           = frdma_map_mr_sg,
  .mmap                = frdma_mmap,
  .mmap_free           = frdma_mmap_free,
  .modify_qp           = frdma_modify_qp,
  .query_device        = frdma_query_device,
  .query_gid           = frdma_query_gid,
  .query_port          = frdma_query_port,// must
  .query_qp            = frdma_query_qp,
  .req_notify_cq       = frdma_req_notify_cq,
  .reg_user_mr         = frdma_reg_user_mr,
};

static struct frdma_dev* dev;

static __init int frdma_init_module( void ) {
  printk( KERN_WARNING "frdma init module\n" );
  int ret = 0;

  dev = ib_alloc_device( frdma_dev, ibdev );
  if ( !dev || !( &( dev->ibdev ) ) ) {
    dev_err( &dev->ibdev.dev, "ib_alloc_device failed\n" );
    return -ENOMEM;
  }

  dev->ibdev.node_type = RDMA_NODE_RNIC;
  memcpy( &dev->ibdev.node_desc, FRDMA_NODE_DESC, sizeof( FRDMA_NODE_DESC ) );

  dev->ibdev.phys_port_cnt    = 1;
  dev->ibdev.num_comp_vectors = 1;
  dev->ibdev.local_dma_lkey   = 0;

  // ret = ib_device_set_netdev(&dev->ibdev, NULL, 1);
  // if (ret) {
  // 	dev_err(&dev->ibdev.dev, "ib_device_set_netdev failed\n");
  // 	return ret;
  // }
  // dev->netdev = NULL;

  ib_set_device_ops( &dev->ibdev, &frdma_device_ops );

  // ret = frdma_get_netdev(dev);
  ret = ib_register_device( &dev->ibdev, "frdma_%d", NULL );
  if ( ret ) {
    dev_err( &dev->ibdev.dev, "ib_register_device: ret = %d\n", ret );
    ib_dealloc_device( &dev->ibdev );
    return ret;
  }

  return ret;
}

static void __exit frdma_exit_module( void ) {
  printk( KERN_WARNING "frdma exit module\n" );
  ib_unregister_device( &dev->ibdev );
  ib_dealloc_device( &dev->ibdev );
}

module_init( frdma_init_module );
module_exit( frdma_exit_module );
