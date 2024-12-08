#ifndef __FAKE_DRIVER_VERBS_OPERATIONS_H__
#define __FAKE_DRIVER_VERBS_OPERATIONS_H__

#include "rdma/ib_verbs.h"

struct rdma_hw_stats* frdma_alloc_hw_port_stats( struct ib_device* device,
                                                 u32               port_num );

int frdma_get_hw_stats( struct ib_device*     ibdev,
                        struct rdma_hw_stats* stats,
                        u32                   port,
                        int                   index );

int frdma_get_port_immutable( struct ib_device*         dev,
                              u32                       port,
                              struct ib_port_immutable* ib_port_immutable );

int frdma_map_mr_sg( struct ib_mr*       ibmr,
                     struct scatterlist* sg,
                     int                 sg_nents,
                     unsigned int*       sg_offset );

int frdma_mmap( struct ib_ucontext*    ctx,
                struct vm_area_struct* vma );

void frdma_mmap_free( struct rdma_user_mmap_entry* rdma_entry );

int frdma_modify_qp( struct ib_qp*      ibqp,
                     struct ib_qp_attr* attr,
                     int                mask,
                     struct ib_udata*   data );

int frdma_query_device( struct ib_device*      dev,
                        struct ib_device_attr* attr,
                        struct ib_udata*       data );

int frdma_query_gid( struct ib_device* dev,
                     u32               port,
                     int               idx,
                     union ib_gid*     gid );

int frdma_query_port( struct ib_device*    dev,
                      u32                  port,
                      struct ib_port_attr* attr );

int frdma_query_qp( struct ib_qp*           ibqp,
                    struct ib_qp_attr*      attr,
                    int                     mask,
                    struct ib_qp_init_attr* init_attr );

int frdma_req_notify_cq( struct ib_cq*           ibcq,
                         enum ib_cq_notify_flags flags );

struct ib_mr* frdma_reg_user_mr( struct ib_pd*    ibpd,
                                 u64              start,
                                 u64              len,
                                 u64              virt,
                                 int              access,
                                 struct ib_udata* udata );

#endif//! __FAKE_DRIVER_VERBS_OPERATIONS_H__
