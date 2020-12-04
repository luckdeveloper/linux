#include <linux/module.h>
#include <linux/init.h>
#include <linux/netdevice.h>

#define ETH_NAMELEN 20
#define ETH_NUM 3

static int __init mymodule_init(void)
{
	int i = 0;
	local_irq_disable();
	while (1)
	{
		i++;
	}	
	preempt_enable();
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
