#ifndef __FAKE_DRIVER_H__
#define __FAKE_DRIVER_H__

#include "rdma/ib_mad.h"
#include <linux/netdevice.h>
#include <linux/pci.h>
#include <linux/types.h>
#include <rdma/ib_verbs.h>

#define FRDMA_MODULE_NAME   "frdma"
#define FRDMA_NODE_DESC     "Fake RDMA stack"
#define FRDMA_PCI_VENDOR_ID 0x6672// 0x66 0x72
#define FRDMA_PCI_DEVICE_ID 0x646d// 0x64 0x6d 0x61

#define FRDMA_FUNC_BAR    0
#define FRDMA_MISX_BAR    2
#define FRDMA_BAR_MASK    ( BIT( FRDMA_FUNC_BAR ) | BIT( FRDMA_MISX_BAR ) )
#define DEFAULT_MAX_VALUE ( 1 << 20 )

enum rxe_device_param {
  FRDMA_MAX_MR_SIZE      = -1ull,
  FRDMA_PAGE_SIZE_CAP    = 0xfffff000,
  FRDMA_MAX_QP_WR        = DEFAULT_MAX_VALUE,
  FRDMA_DEVICE_CAP_FLAGS = IB_DEVICE_BAD_PKEY_CNTR |
                           IB_DEVICE_BAD_QKEY_CNTR |
                           IB_DEVICE_AUTO_PATH_MIG |
                           IB_DEVICE_CHANGE_PHY_PORT |
                           IB_DEVICE_UD_AV_PORT_ENFORCE |
                           IB_DEVICE_PORT_ACTIVE_EVENT |
                           IB_DEVICE_SYS_IMAGE_GUID |
                           IB_DEVICE_RC_RNR_NAK_GEN |
                           IB_DEVICE_SRQ_RESIZE |
                           IB_DEVICE_MEM_MGT_EXTENSIONS |
                           IB_DEVICE_MEM_WINDOW |
                           IB_DEVICE_FLUSH_GLOBAL |
                           IB_DEVICE_FLUSH_PERSISTENT
#ifdef CONFIG_64BIT
                           | IB_DEVICE_MEM_WINDOW_TYPE_2B | IB_DEVICE_ATOMIC_WRITE,
#else
                           | IB_DEVICE_MEM_WINDOW_TYPE_2B,
#endif /* CONFIG_64BIT */
  FRDMA_MAX_SGE                 = 32,
  FRDMA_MAX_SGE_RD              = 32,
  FRDMA_MAX_CQ                  = DEFAULT_MAX_VALUE,
  FRDMA_MAX_LOG_CQE             = 15,
  FRDMA_MAX_PD                  = DEFAULT_MAX_VALUE,
  FRDMA_MAX_QP_RD_ATOM          = 128,
  FRDMA_MAX_RES_RD_ATOM         = 0x3f000,
  FRDMA_MAX_QP_INIT_RD_ATOM     = 128,
  FRDMA_MAX_MCAST_GRP           = 8192,
  FRDMA_MAX_MCAST_QP_ATTACH     = 56,
  FRDMA_MAX_TOT_MCAST_QP_ATTACH = 0x70000,
  FRDMA_MAX_AH                  = ( 1 << 15 ) - 1, /* 32Ki - 1 */
  FRDMA_MIN_AH_INDEX            = 1,
  FRDMA_MAX_AH_INDEX            = FRDMA_MAX_AH,
  FRDMA_MAX_SRQ_WR              = DEFAULT_MAX_VALUE,
  FRDMA_MIN_SRQ_WR              = 1,
  FRDMA_MAX_SRQ_SGE             = 27,
  FRDMA_MIN_SRQ_SGE             = 1,
  FRDMA_MAX_FMR_PAGE_LIST_LEN   = 512,
  FRDMA_MAX_PKEYS               = 64,
  FRDMA_LOCAL_CA_ACK_DELAY      = 15,

  FRDMA_MAX_UCONTEXT = DEFAULT_MAX_VALUE,

  FRDMA_NUM_PORT = 1,

  FRDMA_MIN_QP_INDEX = 16,
  FRDMA_MAX_QP_INDEX = DEFAULT_MAX_VALUE,
  FRDMA_MAX_QP       = DEFAULT_MAX_VALUE - FRDMA_MIN_QP_INDEX,

  FRDMA_MIN_SRQ_INDEX = 0x00020001,
  FRDMA_MAX_SRQ_INDEX = DEFAULT_MAX_VALUE,
  FRDMA_MAX_SRQ       = DEFAULT_MAX_VALUE - FRDMA_MIN_SRQ_INDEX,

  FRDMA_MIN_MR_INDEX = 0x00000001,
  FRDMA_MAX_MR_INDEX = DEFAULT_MAX_VALUE >> 1,
  FRDMA_MAX_MR       = FRDMA_MAX_MR_INDEX - FRDMA_MIN_MR_INDEX,
  FRDMA_MIN_MW_INDEX = FRDMA_MAX_MR_INDEX + 1,
  FRDMA_MAX_MW_INDEX = DEFAULT_MAX_VALUE,
  FRDMA_MAX_MW       = FRDMA_MAX_MW_INDEX - FRDMA_MIN_MW_INDEX,

  FRDMA_MAX_PKT_PER_ACK = 64,

  FRDMA_MAX_UNACKED_PSNS = 128,

  /* Max inflight SKBs per queue pair */
  FRDMA_INFLIGHT_SKBS_PER_QP_HIGH = 64,
  FRDMA_INFLIGHT_SKBS_PER_QP_LOW  = 16,

  /* Max number of interations of each work item
	 * before yielding the cpu to let other
	 * work make progress
	 */
  FRDMA_MAX_ITERATIONS = 1024,

  /* Delay before calling arbiter timer */
  FRDMA_NSEC_ARB_TIMER_DELAY = 200,

  /* IBTA v1.4 A3.3.1 VENDOR INFORMATION section */
  FRDMA_VENDOR_ID = 0XFFFFFF,
};


enum frdma_port_param {
  FRDMA_PORT_GID_TBL_LEN     = 1024,
  FRDMA_PORT_PORT_CAP_FLAGS  = IB_PORT_CM_SUP,
  FRDMA_PORT_MAX_MSG_SZ      = 0x800000,
  FRDMA_PORT_BAD_PKEY_CNTR   = 0,
  FRDMA_PORT_QKEY_VIOL_CNTR  = 0,
  FRDMA_PORT_LID             = 0,
  FRDMA_PORT_SM_LID          = 0,
  FRDMA_PORT_SM_SL           = 0,
  FRDMA_PORT_LMC             = 0,
  FRDMA_PORT_MAX_VL_NUM      = 1,
  FRDMA_PORT_SUBNET_TIMEOUT  = 0,
  FRDMA_PORT_INIT_TYPE_REPLY = 0,
  FRDMA_PORT_ACTIVE_WIDTH    = IB_WIDTH_1X,
  FRDMA_PORT_ACTIVE_SPEED    = 1,
  FRDMA_PORT_PKEY_TBL_LEN    = 1,
  FRDMA_PORT_PHYS_STATE      = IB_PORT_PHYS_STATE_POLLING,
  FRDMA_PORT_SUBNET_PREFIX   = 0xfe80000000000000ULL,
};

struct frdma_port {
  struct ib_port_attr attr;
};

struct frdma_devattr {
  unsigned char peer_addr[ ETH_ALEN ];
  int           numa_node;

  u64                fw_ver;
  __be64             sys_image_guid;
  u64                max_mr_size;
  u64                page_size_cap;
  u32                vendor_id;
  u32                vendor_part_id;
  u32                hw_ver;
  int                max_qp;
  int                max_qp_wr;
  u64                device_cap_flags;
  u64                kernel_cap_flags;
  int                max_send_sge;
  int                max_recv_sge;
  int                max_sge_rd;
  int                max_cq;
  int                max_cqe;
  int                max_mr;
  int                max_pd;
  int                max_qp_rd_atom;
  int                max_ee_rd_atom;
  int                max_res_rd_atom;
  int                max_qp_init_rd_atom;
  int                max_ee_init_rd_atom;
  enum ib_atomic_cap atomic_cap;
  enum ib_atomic_cap masked_atomic_cap;
  int                max_ee;
  int                max_rdd;
  int                max_mw;
  int                max_raw_ipv6_qp;
  int                max_raw_ethy_qp;
  int                max_mcast_grp;
  int                max_mcast_qp_attach;
  int                max_total_mcast_qp_attach;
  int                max_ah;
  int                max_srq;
  int                max_srq_wr;
  int                max_srq_sge;
  unsigned int       max_fast_reg_page_list_len;
  unsigned int       max_pi_fast_reg_page_list_len;
  u16                max_pkeys;
  u8                 local_ca_ack_delay;
  int                sig_prot_cap;
  int                sig_guard_cap;
  struct ib_odp_caps odp_caps;
  uint64_t           timestamp_mask;
  uint64_t           hca_core_clock; /* in KHZ */
  struct ib_rss_caps rss_caps;
  u32                max_wq_type_rq;
  u32                raw_packet_caps; /* Use ib_raw_packet_caps enum */
  struct ib_tm_caps  tm_caps;
  struct ib_cq_caps  cq_caps;
  u64                max_dm_size;
  /* Max entries for sgl for optimized performance per READ */
  u32 max_sgl_rd;
};

struct frdma_dev {
  struct ib_device   ibdev;
  struct net_device* netdev;

  struct frdma_devattr attrs;
  struct frdma_port    port;

  struct list_head cep_list;
};

#endif//! __FAKE_DRIVER_H__
