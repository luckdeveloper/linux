#include <linux/module.h>
#include <linux/init.h>
#include <linux/netdevice.h>

#define ETH_NAMELEN 20
#define ETH_NUM 3

static int __init mymodule_init(void)
{
	int refcnt = 0;
	struct net_device *dev;
    int i = 0;
    char dev_name[ETH_NAMELEN];

	printk("PALMNAS: My module worked!\n");

    memset(dev_name, 0, sizeof(dev_name));
    for (i = 0; i < ETH_NUM; i++)
    {
        sprintf(dev_name, "eth%d", i);
        dev = dev_get_by_name(&init_net, dev_name);
        if (!dev) 
        {
            printk("PALMNAS: device %s not found\n", dev_name);
            continue;
        } 
        
        refcnt = netdev_refcnt_read(dev);
        printk("PALMNAS: device %s ref count %d\n", dev_name, refcnt);

        // don't forget call dev_put
        dev_put(dev);   
    }
	
	return 0;
}

static void __exit mymodule_exit(void)
{
	printk("PALMNAS: Unloading my module.\n");
	return;
}

module_init(mymodule_init);
module_exit(mymodule_exit);

MODULE_LICENSE("GPL");
