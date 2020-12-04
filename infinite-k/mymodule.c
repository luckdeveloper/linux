#include <linux/module.h>
#include <linux/init.h>
#include <linux/kthread.h>

struct task_struct	*g_newthread;
int g_thread_exitflag = 0;

static int infinite_thread(void *arg)
{
	int i = 0;
	while (!g_thread_exitflag)
	{
		i++;
	}	
	return 0;
}

static int __init mymodule_init(void)
{	
	/*infinite_thread*/
    g_newthread = kthread_create(infinite_thread, NULL, "infinite_thread");
    if (IS_ERR(g_newthread))
    {
        printk("kthread_create failed\n");
        return EIO;
    }
    wake_up_process(g_newthread);
	return 0;
}

static void __exit mymodule_exit(void)
{
	printk("Unloading my module.\n");
    /*先停止flush线程*/
    g_thread_exitflag = 1;
    kthread_stop(g_newthread);	
	return;
}

module_init(mymodule_init);
module_exit(mymodule_exit);

MODULE_LICENSE("GPL");
