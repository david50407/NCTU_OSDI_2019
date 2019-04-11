#include <inc/stdio.h>
#include <inc/string.h>
#include <inc/shell.h>
#include <inc/timer.h>

struct Command {
	const char *name;
	const char *desc;
	// return -1 to force monitor to exit
	int (*func)(int argc, char** argv);
};

static struct Command commands[] = {
	{ "help", "Display this list of commands", mon_help },
	{ "kerninfo", "Display information about the kernel", mon_kerninfo },
	{ "chgcolor", "Change output text color", mon_chgcolor },
	{ "print_tick", "Display system tick", print_tick },
	{ "page_fault", "Danger!! Page fault now!", page_fault }
};
#define NCOMMANDS (sizeof(commands)/sizeof(commands[0]))


int mon_help(int argc, char **argv)
{
	int i;

	for (i = 0; i < NCOMMANDS; i++)
		cprintf("%s - %s\n", commands[i].name, commands[i].desc);
	return 0;
}

extern char __TEXT_BEGIN__, __TEXT_END__, __DATA_BEGIN__, __DATA_END__;

int mon_kerninfo(int argc, char **argv)
{
	cprintf("Kernel Code Base: %p Size: %6d bytes.\n", &__TEXT_BEGIN__, &__TEXT_END__ - &__TEXT_BEGIN__);
	cprintf("       Data Base: %p Size: %6d bytes.\n", &__DATA_BEGIN__, &__DATA_END__ - &__DATA_BEGIN__);
	cprintf("Memory footprint size: %6d bytes.\n", &__DATA_END__ - &__TEXT_BEGIN__);

	return 0;
}

int mon_chgcolor(int argc, char **argv)
{
	unsigned char fg = 0, bg = 0;

	if (argc == 1) {
		cprintf("No input text color!\n");
		return 1;
	}

	fg = *argv[1] - '0';
	settextcolor(fg, bg);
	cprintf("Change color %d!\n", fg);

	return 0;
}

int print_tick(int argc, char **argv)
{
	cprintf("Now tick = %d\n", get_tick());
}

int page_fault(int argc, char **argv)
{
	int *ptr = (int *) 0x12345678;
	*ptr = 1;
}

#define WHITESPACE "\t\r\n "
#define MAXARGS 16

static int runcmd(char *buf)
{
	int argc;
	char *argv[MAXARGS];
	int i;

	// Parse the command buffer into whitespace-separated arguments
	argc = 0;
	argv[argc] = 0;
	while (1) {
		// gobble whitespace
		while (*buf && strchr(WHITESPACE, *buf))
			*buf++ = 0;
		if (*buf == 0)
			break;

		// save and scan past next arg
		if (argc == MAXARGS-1) {
			cprintf("Too many arguments (max %d)\n", MAXARGS);
			return 0;
		}
		argv[argc++] = buf;
		while (*buf && !strchr(WHITESPACE, *buf))
			buf++;
	}
	argv[argc] = 0;

	// Lookup and invoke the command
	if (argc == 0)
		return 0;
	for (i = 0; i < NCOMMANDS; i++) {
		if (strcmp(argv[0], commands[i].name) == 0)
			return commands[i].func(argc, argv);
	}
	cprintf("Unknown command '%s'\n", argv[0]);
	return 0;
}
void shell()
{
	char *buf;
	cprintf("Welcome to the OSDI course!\n");
	cprintf("Type 'help' for a list of commands.\n");

	while(1)
	{
		buf = readline("OSDI> ");
		if (buf != NULL)
		{
			if (runcmd(buf) < 0)
				break;
		}
	}
}
