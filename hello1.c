#include <linux/init.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>
#include <linux/printk.h>
#include "hello1.h"

MODULE_AUTHOR("Vovchenko Kostya <vovchenko.kostiantyn@lll.kpi.ua>");
MODULE_DESCRIPTION("Hello1 module for Linux");
MODULE_LICENSE("Dual BSD/GPL");

// Linked list structure
struct time_list {
    struct list_head list;
    ktime_t time_before;
    ktime_t time_after;
};

// Static head of the list
static LIST_HEAD(time_head);

// Exported function
void print_hello(void)
{
    struct time_list *entry;

    entry = kmalloc(sizeof(*entry), GFP_KERNEL);
    if (!entry) {
        pr_err("Memory allocation failed in print_hello.\n");
        return;
    }

    entry->time_before = ktime_get();
    pr_info("Hello, world!\n");
    entry->time_after = ktime_get();

    list_add_tail(&entry->list, &time_head);
}
EXPORT_SYMBOL(print_hello);

// Cleanup function
static void __exit hello1_exit(void)
{
    struct time_list *entry, *tmp;

    pr_info("Exiting hello1 and printing recorded times.\n");
    list_for_each_entry_safe(entry, tmp, &time_head, list) {
        pr_info("Time before: %lld ns, Time after: %lld ns, Duration: %lld ns\n",
                ktime_to_ns(entry->time_before),
                ktime_to_ns(entry->time_after),
                ktime_to_ns(entry->time_after) - ktime_to_ns(entry->time_before));
        list_del(&entry->list);
        kfree(entry);
    }
}

module_exit(hello1_exit);

