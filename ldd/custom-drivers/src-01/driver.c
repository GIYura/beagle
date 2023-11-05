/* header section */
#include <linux/module.h>
#include <linux/init.h>

/* init entry point */
static int __init moduleInit(void)
{
   pr_info("Hello world\n");
   return 0;
}

/* cleanup entry point */
static void __exit moduleExit(void)
{
  pr_info("Good bye world\n");
}

/* registration code */
module_init(moduleInit);
module_exit(moduleExit);

/* description section */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jura");
MODULE_DESCRIPTION("A simple hello world kernel module");
MODULE_INFO(board,"Beaglebone black REV C");

