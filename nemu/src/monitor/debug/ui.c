#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint32_t);

static int int_cpu_exec(char *args){
	//convert char arg into int arg
	//return int s.t. cmd_table can take
	uint32_t ans=0;
	int len=strlen(args);
	if(len>10) { 
		printf("invalid argument for cpu execution.\n");
		return -1;
	}
	int i;
	for(i=0;i<len;i++){
		Assert('0'<=args[i]&&args[i]<='9',"invalid argument for cpu execution.");
	//ToCheck maybe not assert?
		ans=ans*10+args[i]-'0';
	}
	cpu_exec(ans);
	return 1;
}



/* We use the ``readline'' library to provide more flexibility to read from stdin. */
char* rl_gets() {
	static char *line_read = NULL;

	if (line_read ) {
		free(line_read);
		line_read = NULL;
	} 

	line_read = readline("(nemu) ");

	if (line_read  && *line_read) {
		add_history(line_read);
	}

	return line_read;
}

static int cmd_c(char *args) {
	cpu_exec(-1);
	return 0;
}

static int cmd_q(char *args) {
	return -1;
}

static int cmd_help(char *args);
static int info(char *args){
	//cmd: info r
	if(!strcmp(args,"r")){
		//eax, ecx, edx, ebx, esp, ebp, esi, edi
		printf("eax\t\t%p\t%-8x ( | |AH|AL)\n",&cpu.eax,cpu.eax);
		printf("ebx\t\t%p\t%-8x ( | |BH|BL)\n",&cpu.ebx,cpu.ebx);
		printf("ecx\t\t%p\t%-8x ( | |CH|CL)\n",&cpu.ecx,cpu.ecx);
		printf("edx\t\t%p\t%-8x ( | |DH|DL)\n",&cpu.edx,cpu.edx);
		printf("esp\t\t%p\t%-8x ( | |SP   )\n",&cpu.esp,cpu.esp);
		printf("ebp\t\t%p\t%-8x ( | |BP   )\n",&cpu.ebp,cpu.ebp);
		printf("esi\t\t%p\t%-8x ( | |SI   )\n",&cpu.esi,cpu.esi);
		printf("edi\t\t%p\t%-8x ( | |DI   )\n",&cpu.edi,cpu.edi);
		printf("eip\t\t%p\t%-8x ( | |IP   )\n\n",&cpu.eip,cpu.eip);
	}
	return 0;
}

static struct {
	char *name;
	char *description;
	int (*handler) (char *);//hadler is a name of the pointers to func//it takes cahr* arg and return int
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit NEMU", cmd_q },
	{"info", "print information(e.g. info r)", info},
	//????????????????????????????????????????????????????????????????????????????????????????
	{ "si", "Implement N single instructions and then pause, N=1 as default", int_cpu_exec },
	//{ "si", "Implement single instruction n times", cpu_exec },
	/* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

	if(arg == NULL) {
		/* no argument given */
		for(i = 0; i < NR_CMD; i ++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
 		}
 	}
	else {
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(arg, cmd_table[i].name) == 0) {
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
 			}
 		}
		printf("Unknown command '%s'\n", arg);
 	}
	return 0;
}

void ui_mainloop() {
 	while(1) {
		char *str = rl_gets();
		char *str_end = str + strlen(str);

		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if(cmd == NULL) { continue; }

		/* treat the remaining string as the arguments,
		 * which may need further parsing
 		 */
		char *args = cmd + strlen(cmd) + 1;
		if(args >= str_end) {
			args = NULL;
 		}

#ifdef HAS_DEVICE
		extern void sdl_clear_event_queue(void);
		sdl_clear_event_queue();
#endif

		int i;
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(cmd, cmd_table[i].name) == 0) {
				if(cmd_table[i].handler(args) < 0) { return; }
				break;
 			}
 		}

		if(i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
	}
}
