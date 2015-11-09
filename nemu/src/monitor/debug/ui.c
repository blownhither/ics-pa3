#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <string.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint32_t);

static int cmd_si(char *args){//int_cpu_exec
	//convert char arg into int arg
	//return int s.t. cmd_table can take
	uint32_t ans=0;
	if(args==NULL){
		cpu_exec(1);
		return 0; 
	} 
	int len=strlen(args);
	if(!len)ans=1;
	else{
		if(len>10) { 
		printf("invalid argument for cpu execution.\n");
		return -1;
		}  
		int i;
		for (i=0;i<len;i++){
		Assert('0'<=args[i]&&args[i]<='9',"invalid argument for cpu execution.");
		//ToCheck maybe not assert?
		ans=ans*10+args[i]-'0';
		} 
	}
	cpu_exec(ans);
	return 0;
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

bool info_register_overflow_flag = false; 
uint32_t swaddr_read(swaddr_t addr, size_t len);
uint32_t swaddr_read_safe(swaddr_t addr , size_t len){
	if(addr >= 1<<27){
		info_register_overflow_flag = true; 
		return -1; 
	} 
	else return swaddr_read(addr , len); 
}
extern void print_watchpoint_list();
static int cmd_info(char *args){
	//cmd: info r
	if(!strcmp(args,"r")){
		//eax, ecx, edx, ebx, esp, ebp, esi, edi
		info_register_overflow_flag = false; 
		printf("eax\t0x%-8x\t0x%-8x ( | |AH|AL)\n",cpu.eax,swaddr_read_safe(cpu.eax , 4));
		printf("ebx\t0x%-8x\t0x%-8x ( | |BH|BL)\n",cpu.ebx,swaddr_read_safe(cpu.ebx , 4));
		printf("ecx\t0x%-8x\t0x%-8x ( | |CH|CL)\n",cpu.ecx,swaddr_read_safe(cpu.ecx , 4));
		printf("edx\t0x%-8x\t0x%-8x ( | |DH|DL)\n",cpu.edx,swaddr_read_safe(cpu.edx , 4));
		printf("esp\t0x%-8x\t0x%-8x ( | |SP   )\n",cpu.esp,swaddr_read_safe(cpu.esp , 4));
		printf("ebp\t0x%-8x\t0x%-8x ( | |BP   )\n",cpu.ebp,swaddr_read_safe(cpu.ebp , 4));
		printf("esi\t0x%-8x\t0x%-8x ( | |SI   )\n",cpu.esi,swaddr_read_safe(cpu.esi, 4));
		printf("edi\t0x%-8x\t0x%-8x ( | |DI   )\n",cpu.edi,swaddr_read_safe(cpu.edi , 4));
		printf("eip\t0x%-8x\t0x%-8x ( | |IP   )\n",cpu.eip,swaddr_read_safe(cpu.eip , 4));
		if(info_register_overflow_flag)printf("Warning: address out of physical memory were treated as -1.\n\tUse x cmd for details.\n\n"); 	
	}
	else if(!strcmp(args , "b" )|| !strcmp(args , "w")){
		print_watchpoint_list(); 
	}
	return 0;
}

//size_t equals long unsigned int in 64x and unsigned in others
uint32_t expr_cmd_x(char *e , bool *success); 
static int cmd_x(char *args){
	if(args==NULL)return 0; 
	char *args1 =strtok(args," ");///safety
	int n;
	sscanf(args1,"%d",&n);///TODO confirm: try to use args here!
	
	char *args2=strtok(NULL," ");
	if(args2==NULL){
		args2=args1; 
		n=1; 
	}
	bool success=true;  
	int addr = expr_cmd_x(args2 , &success); 
	if(!success){
		printf("Invalid expression.\n"); 
		return 0; 
	}
	//output
	int i=0;

	if(addr+n-1 >= (1<<(10+10+3+(27-10-10-3)))){
		printf("Physical address 0x%x is outside of the physical memory!\n" , addr); 
		//success=false; 
		return 0; 
	}
	printf("0x%x <addr>:\t0x%x\t",addr,swaddr_read(addr,1));
	for(i=1;i<n;i++){ 
		if(i%8==0&&i!=0){
			printf("\n0x%x <addr+%d>:\t",addr+i,i);
	 	} 
		//currently using "addr" sign
		printf("0x%x\t",swaddr_read(addr+i,1));
	} 
	printf("\n");
	return 0;
}

uint32_t expr(char *e , bool *success); 
static int cmd_p(char *args){
	bool success=true; 
	if(args==NULL)return 0; 
	expr(args , &success); 
	if(!success){
		printf("\035\tinvalid expression\n"); 
	}  
	return 0; 
}
bool flag_const_watchpoint = false;  
extern WP *get_new_wp(char *expr); 
static int cmd_w(char *args){
	if(strlen(args)>254){
		printf("Expression too long.\n"); 
		return 0; 
	}
	bool success=true; 
	expr_cmd_x(args , &success);
	if(!success){
		printf("Unsuccessful expression parsing.\n"); 	
		return 0;
	} 
	if(flag_const_watchpoint){
		printf("Cannot watch constant value %s\n" ,args); 
		return 0; 
	}
#ifdef MZYDEBUG
	printf("Args is %s\n" , args); 
#endif
	WP *temp; temp = get_new_wp(args); 
	if(temp==NULL){
		panic("Watchpoint pool depleted.\n"); 
		return 0; 
	}
	//if(get_new_wp(args) == NULL)
	//	panic("Watchpoint pool depleted.\n"); 
	//considering assert action
	else return 0; 
}
extern void delete_wp(int num_2_delete); 
static int cmd_d(char *args){
	int num_2_delete; 
	sscanf(args , "%d" , &num_2_delete); 
	delete_wp(num_2_delete); 
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
	{"info", "Generic command for showing things about the program being debugged", cmd_info},
	{ "si", "Step N instruction exactly. Usage si [N]. (default N=1)", cmd_si },
	{"x","Examine memory: x [N] ADDRESS",cmd_x} , 
	{"p" , "Print value of expression EXP." , cmd_p} , 
	{"w" , "Stop execution whenever the value of an expression changes." , cmd_w} ,   
	{"d" , "Delete breakpoints or auto-display expressions." , cmd_d} , 
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

extern void init_wp_list();
extern int top_watchpoint_NO; 
void ui_mainloop() {
	init_wp_list();//initialize watchpoint list 
	top_watchpoint_NO=0; 
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
