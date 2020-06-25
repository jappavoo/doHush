#include <common.h>
#include <command.h>

static int do_hello(struct cmd_tbl *cmdtp, int flag, int argc,
		   char *const argv[])
{
  printf("hello world\n");
  return 0;
}

U_BOOT_CMD(
	echo,	CONFIG_SYS_MAXARGS,	0,	do_hello,
	"echo hello to console",
	"No arguments\n"
	"    - echo hello to console"
);
