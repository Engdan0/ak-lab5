#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include "hello1.h"

MODULE_AUTHOR("Vovchenko Kostya <vovchenko.kostiantyn@lll.kpi.ua>");
MODULE_DESCRIPTION("Hello2 module for Linux");
MODULE_LICENSE("Dual BSD/GPL");

// Parameter definition
static uint count = 1;
module_param(count, uint, 0444);
MODULE_PARM_DESC(count, "Number of times 'Hello, world!' should be printed.");

// Initialization function
static int __init hello2_init(void)
{
    int i;

    if (count == 0 || (count >= 5 && count <= 10)) {
        pr_warn("Parameter 'count'==0 or 5<count<10. Proceeding with caution.\n");
    } else if (count > 10) {
        pr_err("Parameter 'count'>10. Exiting with -EINVAL.\n");
        return -EINVAL;
    }

    for (i = 0; i < count; i++) {
        print_hello();
    }

    return 0;
}

// Cleanup function
static void __exit hello2_exit(void)
{
    pr_info("Exiting hello2 module.\n");
}

module_init(hello2_init);
module_exit(hello2_exit);

