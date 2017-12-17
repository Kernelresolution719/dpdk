#include <rte_dev.h>

static int  hns3pf_dev_configure(struct rte_eth_dev *dev);

/*Vendor ID */
#define HNAE3_HUAWEI_VENDOR_ID 0x19e5


/*Device IDs*/
#define HNAE3_DEV_ID_GE				0xA220
#define HNAE3_DEV_ID_25GE			0xA221
#define HNAE3_DEV_ID_25GE_RDMA			0xA222
#define HNAE3_DEV_ID_25GE_RDMA_MACSEC		0xA223
#define HNAE3_DEV_ID_50GE_RDMA			0xA224
#define HNAE3_DEV_ID_50GE_RDMA_MACSEC		0xA225
#define HNAE3_DEV_ID_100G_RDMA_MACSEC		0xA226
#define HNAE3_DEV_ID_100G_VF			0xA22E
#define HNAE3_DEV_ID_100G_RDMA_DCB_PFC_VF	0xA22F
#define HNAE3_DEV_ID_SDI 0xA125

static struct rte_pci_id pci_id_hns3pf_map[] = {
	{ RTE_PCI_DEVICE(HNAE3_HUAWEI_VENDOR_ID, HNAE3_DEV_ID_GE) },
	{ RTE_PCI_DEVICE(HNAE3_HUAWEI_VENDOR_ID, HNAE3_DEV_ID_SDI) },
	{ .vendor_id = 0, }
};

static struct rte_pci_id pci_id_hns3vf_map[] = {
	{ RTE_PCI_DEVICE(HNAE3_HUAWEI_VENDOR_ID, HNAE3_DEV_ID_GE) },
	{ RTE_PCI_DEVICE(HNAE3_HUAWEI_VENDOR_ID, HNAE3_DEV_ID_SDI) },
	{ .vendor_id = 0, }
};

static const struct eth_dev_ops hns3pf_eth_dev_ops = {
       .dev_configure        = hns3pf_dev_configure,
       .dev_start            = hns3pf_dev_start,
       .dev_stop             = hns3pf_dev_stop,
};

static const struct eth_dev_ops hns3vf_eth_dev_ops = {
       .dev_configure        = hns3vf_dev_configure,
       .dev_start            = hns3vf_dev_start,
       .dev_stop             = hns3vf_dev_stop,
};

static int hclge_pci_init(struct rte_eth_dev *eth_dev)
{
	struct rte_pci_device *pci_dev = RTE_ETH_DEV_TO_PCI(eth_dev);
	struct hclge_dev *hdev = eth_dev->data->dev_private;
	struct hclge_hw *hw;
	int ret;

	if (!pci_dev->mem_resource[0].addr) {
		RTE_LOG(ERR, PMD,
			"Cannot find PCI device base address, aborting\n");
		ret = -ENODEV;
		goto err_no_drvdata;//这个err命名不太科学
	}

	hw = &hdev->hw;
	hw->back = hdev;
	hw->io_base = (void *)pci_dev->mem_resource[0].addr;//bar2 but void * mem_resource.addr

	if (!hw->io_base) {
		RTE_LOG(ERR, PMD, "Can't map configuration register space\n");
		ret = -ENOMEM;
		goto err_clr_master;
	}

	//hdev->num_req_vfs = pci_sriov_get_totalvfs(pdev);// how to translate

	return 0;

err_clr_master:
	if (hw->io_base)
		hw->io_base = NULL;
err_no_drvdata:
	
	return ret;
}


static int
eth_hns3pf_dev_init(struct rte_eth_dev *eth_dev)
{
	struct rte_pci_device *pci_dev = RTE_ETH_DEV_TO_PCI(eth_dev);
	struct hclge_dev *hdev = eth_dev->data->dev_private;
	int ret;

	eth_dev->dev_ops = &hns3pf_eth_dev_ops;
	eth_dev->rx_pkt_burst = &hns3_recv_pkts;
	eth_dev->tx_pkt_burst = &hns3_xmit_pkts;

	rte_eth_copy_pci_info(eth_dev, pci_dev);
	eth_dev->data->dev_flags |= RTE_ETH_DEV_DETACHABLE;//really support hotplug? not sure
	
	hdev->eth_dev = eth_dev;
	hdev->pdev = pci_dev;

	hclge_pci_init(eth_dev);
	
	if (ret) {
		RTE_LOG(ERR, PMD,
			"PCI init failed ret: %x\n", ret);
		goto error;
	}
	
	/* Command queue initialize */
	ret = hclge_cmd_init(eth_dev);
	if (ret) {
		RTE_LOG(ERR, PMD,
			"cmd init failed ret: %x\n", ret);
		goto error;
	}

	

error:
	return ret;
}

static int
eth_hns3pf_dev_uninit(struct rte_eth_dev *eth_dev)
{



}

static int
eth_hns3vf_dev_init(struct rte_eth_dev *eth_dev)
{


}

static int
eth_hns3vf_dev_uninit(struct rte_eth_dev *eth_dev)
{


}



static int eth_hns3pf_pci_probe(struct rte_pci_driver *pci_drv __rte_unused,
	struct rte_pci_device *pci_dev)
{
	return rte_eth_dev_pci_generic_probe(pci_dev,
		sizeof(struct hclge_dev), eth_hns3pf_dev_init);//hns3_struct?
}

static int eth_hns3pf_pci_remove(struct rte_pci_device *pci_dev)
{
	return rte_eth_dev_pci_generic_remove(pci_dev, eth_hns3pf_dev_uninit);
}


static struct rte_pci_driver rte_hns3pf_pmd = {
	.id_table = pci_id_hns3pf_map,
	.drv_flags = RTE_PCI_DRV_NEED_MAPPING | RTE_PCI_DRV_INTR_LSC,
	.probe = eth_hns3pf_pci_probe,
	.remove = eth_hns3pf_pci_remove,
};


static int eth_hns3vf_pci_probe(struct rte_pci_driver *pci_drv __rte_unused,
	struct rte_pci_device *pci_dev)
{

}

static int eth_hns3vf_pci_remove(struct rte_pci_device *pci_dev)
{
	
}


static struct rte_pci_driver rte_hns3vf_pmd = {
	.id_table = pci_id_hns3vf_map,
	.drv_flags = RTE_PCI_DRV_NEED_MAPPING,
	.probe = eth_hns3vf_pci_probe,
	.remove = eth_hns3vf_pci_remove,

};


static int
hns3pf_dev_configure(struct rte_eth_dev *dev)
{

}

static int
hns3pf_dev_start(struct rte_eth_dev *dev)
{


}

static void
hns3pf_dev_stop(struct rte_eth_dev *dev)
{


}




RTE_PMD_REGISTER_PCI(net_hns3pf, rte_hns3pf_pmd.pci_drv);
RTE_PMD_REGISTER_PCI_TABLE(net_hns3pf, pci_id_hns3pf_map);
RTE_PMD_REGISTER_KMOD_DEP(net_hns3pf, "* igb_uio | uio_pci_generic | vfio-pci");
RTE_PMD_REGISTER_PCI(net_hns3vf, rte_hns3vf_pmd.pci_drv);
RTE_PMD_REGISTER_PCI_TABLE(net_hns3vf, pci_id_hns3vf_map);
RTE_PMD_REGISTER_KMOD_DEP(net_hns3vf, "* igb_uio | vfio-pci");
