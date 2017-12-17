/*
 * Structure to store private data for each driver instance (for each port).
 */
struct hclge_dev {
	struct rte_eth_dev *eth_dev;
	struct rte_pci_device *pdev;
	struct hnae3_ae_dev *ae_dev;
	struct hclge_hw hw;
	struct hclge_misc_vector misc_vector;
	unsigned long state;
	enum hnae3_reset_type reset_type;
	struct hclge_hw_stats hw_stats;

	u32 fw_version;
	u16 num_vmdq_vport;		/* Num vmdq vport this PF has set up */
	u16 num_tqps;			/* Num task queue pairs of this PF */
	//u16 num_vport_msix;		/* Num queue vectors per vport */
	u16 num_req_vfs;		/* Num VFs requested for this PF */
	atomic_t num_running_vfs;		/* Num of running VFs */

	u16 num_roce_msix;		/* Num of roce vectors for this PF */
	int roce_base_vector;

	/* Base task tqp physical id of this PF */
	u16 base_tqp_pid;
	u16 alloc_rss_size;		/* Allocated RSS task queue */
	u16 rss_size_max;		/* HW defined max RSS task queue */

	/* Num of guaranteed filters for this PF */
	u16 fdir_pf_filter_count;
	u16 num_alloc_vport;		/* Num vports this driver supports */
	u32 numa_node_mask;
	u16 rx_buf_len;
	u16 num_desc;
	u8 hw_tc_map;
	u8 tc_num_last_time;
	enum hclge_fc_mode fc_mode_last_time;
#define HCLGE_FLAG_TC_BASE_SCH_MODE		0
#define HCLGE_FLAG_VNET_BASE_SCH_MODE		1
	u8 tx_sch_mode;
	u8 dcbx_cap;
	u8 default_up;
	u8 tc_cap;
	u8 pfc_cap;
	u8 pg_cap;
	struct hclge_dcb_info dcb_info;
	struct hclge_dcb_info dcb_info_tmp;

	//struct hlist_head fdir_filter_list;
	//u16 fdir_pf_active_filters;
	//unsigned long fd_flush_timestamp;
	//u32 fd_flush_cnt;
	//u32 fd_add_err;
	//u32 fd_atr_cnt;
	//u32 fd_tcp_rule;
	u16 num_msi;
	u16 num_msi_left;
	u16 num_msi_used;
	u32 base_msi_vector;
	struct msix_entry *msix_entries;
	u16 *vector_status;

	u16 pending_udp_bitmap;

	u16 rx_itr_default;
	u16 tx_itr_default;

	u16 adminq_work_limit; /* Num of admin receive queue desc to process */
	unsigned long service_timer_period;
	unsigned long service_timer_previous;
	struct timer_list service_timer;
	struct work_struct service_task;

	bool cur_promisc;
	int num_alloc_vfs;	/* Actual number of VFs allocated */

	struct hclge_tqp *htqp;
	struct hclge_vport *vport;

	struct dentry *hclge_dbgfs;

	struct hnae3_client *nic_client;
	struct hnae3_client *roce_client;
	u32 flag;
#define HCLGE_FLAG_USE_MSI	0x00000001
#define HCLGE_FLAG_USE_MSIX	0x00000002
#define HCLGE_FLAG_MAIN		0x00000004
#define HCLGE_FLAG_DCB_CAPABLE	0x00000008
#define HCLGE_FLAG_DCB_ENABLE	0x00000010

	u32 pkt_buf_size; /* Total pf buf size for tx/rx */
	u32 mps; /* Max packet size */

	enum hclge_mta_dmac_sel_type mta_mac_sel_type;
	bool enable_mta; /* Mutilcast filter enable */
	bool accept_mta_mc; /* Whether accept mta filter multicast */

	struct hclge_vlan_type_cfg vlan_type_cfg;
};


struct hclge_hw {
	void  *io_base;
	struct hclge_mac mac;
	int num_vec;
	struct hclge_cmq cmq;
	struct hclge_caps caps;
	void *back;
};


