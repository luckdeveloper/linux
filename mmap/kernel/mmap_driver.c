#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <asm/io.h>
#include <linux/mman.h>
#include <linux/version.h>

#define MAP_PAGE_COUNT 10
#define MAPLEN (PAGE_SIZE * MAP_PAGE_COUNT)
#define MAP_DEV_MAJOR 240
#define MAP_DEV_NAME "mmaptest"

extern struct mm_struct init_mm;
void map_vopen(struct vm_area_struct *vma);
void map_vclose(struct vm_area_struct *vma);
/*device mmap */
static int mapdrv_mmap(struct file *file, struct vm_area_struct *vma);
static int mapdrv_open(struct inode *inode, struct file *file);

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4,11,0))   
/* vm area nopage */
int map_fault(struct vm_fault *vmf);
#else
int map_fault(struct vm_area_struct *vma, struct vm_fault *vmf);
#endif


static struct file_operations mapdrvo_fops = {
    .owner = THIS_MODULE,
    .mmap = mapdrv_mmap,
    .open = mapdrv_open,
};

static struct vm_operations_struct map_vm_ops = {
    .open = map_vopen,
    .close = map_vclose,
    .fault = map_fault,
};

static char *vmalloc_area = NULL;

MODULE_LICENSE("GPL");

static int __init mapdrv_init(void)
{
    int result;
    unsigned long virt_addr;
    int i = 1;
    result = register_chrdev(MAP_DEV_MAJOR, MAP_DEV_NAME, &mapdrvo_fops);
    if (result < 0)
    {
        printk("MMAPTEST: register_chrdev failed, return code is %d\n", result);
        return result;
    }
    vmalloc_area = vmalloc(MAPLEN);
    virt_addr = (unsigned long)vmalloc_area;
    for (virt_addr = (unsigned long)vmalloc_area; virt_addr < (unsigned long)vmalloc_area + MAPLEN; virt_addr += PAGE_SIZE)
    {
        SetPageReserved(vmalloc_to_page((void *)virt_addr));
        sprintf((char *)virt_addr, "test %d", i++);
    }
    /* printk("vmalloc_area at 0x%lx (phys 0x%lx)\n",(unsigned long)vmalloc_area,(unsigned long)vmalloc_to_pfn((void *)vmalloc_area) << PAGE_SHIFT);  */
    printk("MMAPTEST: vmalloc area apply complete!");
    return 0;
}

static void __exit mapdrv_exit(void)
{
    unsigned long virt_addr;
    /* unreserve all pages */
    for (virt_addr = (unsigned long)vmalloc_area; virt_addr < (unsigned long)vmalloc_area + MAPLEN; virt_addr += PAGE_SIZE)
    {
        ClearPageReserved(vmalloc_to_page((void *)virt_addr));
    }
    /* and free the two areas */
    if (vmalloc_area)
        vfree(vmalloc_area);
    unregister_chrdev(MAP_DEV_MAJOR, MAP_DEV_NAME);
}

static int mapdrv_mmap(struct file *file, struct vm_area_struct *vma)
{
    unsigned long offset = vma->vm_pgoff << PAGE_SHIFT;
    unsigned long size = vma->vm_end - vma->vm_start;

    if (size > MAPLEN)
    {
        printk("MMAPTEST: size too big\n");
        return -ENXIO;
    }
    /*  only support shared mappings. */
    if ((vma->vm_flags & VM_WRITE) && !(vma->vm_flags & VM_SHARED))
    {
        printk("MMAPTEST: writeable mappings must be shared, rejecting\n");
        return -EINVAL;
    }
    /* do not want to have this area swapped out, lock it */
    vma->vm_flags |= VM_LOCKONFAULT;
    if (offset == 0)
    {
        vma->vm_ops = &map_vm_ops;
    }
    else
    {
        printk("MMAPTEST: offset out of range\n");
        return -ENXIO;
    }
    return 0;
}
static int mapdrv_open(struct inode *inoe, struct file *file)
{

    printk("MMAPTEST: process: %s (%d)\n", current->comm, current->pid);
    return 0;
}

/* open handler for vm area */
void map_vopen(struct vm_area_struct *vma)
{
    printk("MMAPTEST: mapping vma is opened\n");
}

/* close handler form vm area */
void map_vclose(struct vm_area_struct *vma)
{
    printk("MMAPTEST: mapping vma is closed\n");
}

/* page fault handler */

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4,11,0))   
int map_fault(struct vm_fault *vmf)
{
    struct page *page;
    void *page_ptr;
    unsigned long offset, virt_start, pfn_start;
    offset = vmf->address - vmf->vma->vm_start;
    virt_start = (unsigned long)vmalloc_area + (unsigned long)(vmf->pgoff << PAGE_SHIFT);
    pfn_start = (unsigned long)vmalloc_to_pfn((void *)virt_start);

    printk("\n");
    /*printk("%-25s %d\n","7)PAGE_SHIFT",PAGE_SHIFT);*/
    page_ptr = NULL;

    if ((vmf->vma == NULL) || (vmalloc_area == NULL))
    {
        printk("return VM_FAULT_SIGBUS!\n");
        return VM_FAULT_SIGBUS;
    }
    if (offset >= MAPLEN)
    {
        printk("return VM_FAULT_SIGBUS!");
        return VM_FAULT_SIGBUS;
    }
    page_ptr = vmalloc_area + offset;
    page = vmalloc_to_page(page_ptr);
    get_page(page);
    vmf->page = page;
    printk("%s: map 0x%lx (0x%016lx) to 0x%lx , size: 0x%lx, page:%ld \n", __func__, virt_start, pfn_start << PAGE_SHIFT, vmf->address, PAGE_SIZE, vmf->pgoff);
   return 0;
}
#else
int map_fault(struct vm_area_struct *vma, struct vm_fault *vmf)
{

    struct page *page = NULL;
    void *page_ptr = NULL;
    unsigned long offset, virt_start, pfn_start;

    if ((vmf->vma == NULL) || (vmalloc_area == NULL))
    {
        printk("MMAPTEST: return VM_FAULT_SIGBUS!\n");
        return VM_FAULT_SIGBUS;
    }

    offset = (unsigned long)(vmf->virtual_address - vma->vm_start);
    if (offset >= MAPLEN)
    {
        printk("MMAPTEST: return VM_FAULT_SIGBUS!");
        return VM_FAULT_SIGBUS;
    }

    page_ptr = vmalloc_area + offset;
    page = vmalloc_to_page(page_ptr);
    get_page(page);
    vmf->page = page;

    virt_start = (unsigned long)vmalloc_area + (unsigned long)(vmf->pgoff << PAGE_SHIFT);
    pfn_start = (unsigned long)vmalloc_to_pfn((void *)virt_start);

    printk("MMAPTEST: %s: map 0x%lx (0x%016lx) to 0x%lx , size: 0x%lx, page:%ld \n", __func__, virt_start, pfn_start << PAGE_SHIFT, vmf->virtual_address, PAGE_SIZE, vmf->pgoff);
    return 0;
}
#endif

module_init(mapdrv_init);
module_exit(mapdrv_exit);
