#include "linux/init.h"
#include "linux/kern_levels.h"
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

static int frdma_get_port_immutable( struct ib_device*         dev,
                                     u32                       port,
                                     struct ib_port_immutable* port_immutable ) {
  // port_immutable->gid_tbl_len    = 1;
  // port_immutable->core_cap_flags = RDMA_CORE_PORT_IWARP;
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
  memset( attr, 0, sizeof( *attr ) );
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

static int frdma_alloc_pd( struct ib_pd* ibpd, struct ib_udata* udata ) {
  return 0;
}

static int frdma_dealloc_pd( struct ib_pd* ibpd, struct ib_udata* udata ) {
  return 0;
}

static int frdma_create_qp( struct ib_qp*           ibqp,
                            struct ib_qp_init_attr* attrs,
                            struct ib_udata*        udata ) {
  return 0;
}

static int frdma_destroy_cq( struct ib_cq* ibcq, struct ib_udata* udata ) {
  return 0;
}

static int frdma_destroy_qp( struct ib_qp* ibqp, struct ib_udata* udata ) {
  return 0;
}

static int frdma_post_send( struct ib_qp*             ibqp,
                            const struct ib_send_wr*  send_wr,
                            const struct ib_send_wr** bad_send_wr ) {
  return 0;
}

static int frdma_post_recv( struct ib_qp*             ibqp,
                            const struct ib_recv_wr*  recv_wr,
                            const struct ib_recv_wr** bad_recv_wr ) {
  return 0;
}

static int frdma_create_cq( struct ib_cq*                 ibcq,
                            const struct ib_cq_init_attr* attr,
                            struct uverbs_attr_bundle*    attrs ) {
  return 0;
}

static int frdma_poll_cq( struct ib_cq* ibcq,
                          int           num_entries,
                          struct ib_wc* wc ) {
  return 0;
}


static struct ib_mr* frdma_get_dma_mr( struct ib_pd* ibpd, int acc ) {
  return NULL;
}

static int frdma_dereg_mr( struct ib_mr* ibmr, struct ib_udata* udata ) {
  return 0;
}

const struct ib_device_ops frdma_device_ops = {
  .owner          = THIS_MODULE,
  .driver_id      = 21,
  .uverbs_abi_ver = 1,

  .query_device       = frdma_query_device,
  .query_port         = frdma_query_port,
  .alloc_pd           = frdma_alloc_pd,
  .dealloc_pd         = frdma_dealloc_pd,
  .create_qp          = frdma_create_qp,
  .modify_qp          = frdma_modify_qp,
  .destroy_qp         = frdma_destroy_qp,
  .post_send          = frdma_post_send,
  .post_recv          = frdma_post_recv,
  .create_cq          = frdma_create_cq,
  .destroy_cq         = frdma_destroy_cq,
  .poll_cq            = frdma_poll_cq,
  .req_notify_cq      = frdma_req_notify_cq,
  .get_dma_mr         = frdma_get_dma_mr,
  .reg_user_mr        = frdma_reg_user_mr,
  .dereg_mr           = frdma_dereg_mr,
  .get_port_immutable = frdma_get_port_immutable,
};

static struct frdma_dev* dev;

static __init int frdma_init_module( void ) {
  printk( KERN_WARNING "frdma init module\n" );
  int ret = 0;

  dev = ib_alloc_device( frdma_dev, ibdev );
  if ( !dev ) {
    dev_err( &dev->ibdev.dev, "ib_alloc_device failed\n" );
    return -ENOMEM;
  }

  dev->ibdev.node_type = RDMA_NODE_RNIC;
  memcpy( &dev->ibdev.node_desc, FRDMA_NODE_DESC, sizeof( FRDMA_NODE_DESC ) );

  dev->ibdev.phys_port_cnt    = 1;
  dev->ibdev.num_comp_vectors = 1;
  // dev->ibdev.local_dma_lkey   = 0;

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
