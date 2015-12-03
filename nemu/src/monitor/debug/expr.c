#include "nemu.h"
/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
#include<string.h>
enum {
	NOTYPE = 256, EQ , DEC  , HEX , REG , NEG , LE , GE=263 , 
	DREF = 264 , SL , SR , NEQ , AND , OR
	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */
	//rules and type(in the enum above)
	{"0x[0-9a-fA-F]+" , HEX} ,		//hexadecimal number
	{"[0-9]+" , DEC} ,				//decimal number
	{" +",	NOTYPE},				// spaces
	{"\\+", '+'},					// plus
	{"<=" , LE} , 
	{">=" , GE} , 
	{"<<" , SL} , 
	{">>" , SR} , 
	{">" , '>'} ,
	{"<" , '<'} ,
	{"!=" , NEQ} ,   
	{"==", EQ} , 					// equal
	//leaving NEG for future parsing
	{"-" , '-'} ,					//minus
	{"\\*" , '*'} ,				    //multiply
	{"/" , '/'} ,			    	//devide
	{"\\$[a-zA-Z]{2,3}" , REG} ,    //register
	{"\\\(" , '('} , {"\\)" , ')'} ,//parenthesis
	{"%" , '%'} , 						//mod
	{"&&" , AND} , 
	{"\\|\\|" , OR} , 
	{"&" , '&'} , 
	{"\\|" , '|'} , 
	{"\\^" , '^'}
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )
regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			Assert(ret == 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;

	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				//Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array ``tokens''. For certain 
				 * types of tokens, some extra actions should be performed.
				 */

				//expression overflow
				if(nr_token>=31||substr_len>31){
					printf("expression too long\n"); 
					return false; 
				}
				switch(rules[i].token_type) {
					case NOTYPE:
						break;   
					default:
						//plain recording
						//nr_token is the number of tokens (1-32)
						tokens[nr_token].type = rules[i].token_type;
						strncpy(tokens[nr_token].str , substr_start , substr_len); 
						(tokens[nr_token].str)[substr_len]='\0';
						nr_token++;
#ifdef MZYDEBUG
						//printf("nr_token = %d" , nr_token);
#endif
				}

				break;
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true; 
}


bool check_parentheses(int p , int q){
	//pretend there is a stack always full of '('
	int end=-1; 
	if(tokens[p].type!='('||tokens[q].type!=')')
		return false; 
	int i;
	for(i=p; i<=q; i++){
		if(tokens[i].type=='('){
			++end;
		}
		else if(tokens[i].type==')'){
			if(end==-1)return false; 
			if(end==0 && i!=q)return false; 
			end--; 
		}
	}
	if(end!=-1)return false;
	return true;
}


int get_operator_priority(int operator){
	switch(operator){
		case NEG:case DREF:case '!':case '~':
			return 18; 
		case '/':case '*':case '%':
			return 17; 
		case '+':case '-':
			return 16; 
		case SL:case SR:
			return 15; 
		case '>':case GE:case '<':case LE:
			return 14; 
		case EQ:case NEQ:
			return 13; 
		case '&':return 12; 
		case '^':return 11; 
		case '|':return 10; 
		case AND:return 9; 
		case OR:return 8; 
		default: return -1;
				 //not a defined operator. probably a number or '(' ,  ')' 
	}
}
uint32_t string_to_int(char *s , int base){
	int i , n=strlen(s);
	if(!s)return 0; 
	if(n>32)n=32; 
	uint32_t ans=0;
	if(base == 16)i=2;				//omitting (0x)0000000 
	else i=0; 
	for(; i<n; i++)
		if('a'<=s[i]&&s[i]<='f'){
			ans = ans*base + s[i] - 'a' + 10; 
		}
		else if('A'<=s[i]&&s[i]<='F'){
			ans = ans*base + s[i] - 'A' + 10; 
		}
		else
			ans = ans*base + s[i] - '0'; 
	return ans; 
}


#define MZYDEBUG
#undef MZYDEBUG
int invalid_flag=0; 
const char* my_register_num_32[]={"EAX" , "ECX" , "EDX" , "EBX" , "ESP" , "EBP" , "ESI" , "EDI"}; 
const char* my_register_num_16[]={"AX" , "CX" , "DX" , "BX" , "SP" , "BP" , "SI" , "DI"};
const char* my_register_num_8h[]={"AH" , "CH" , "DH" , "BH"}; 
const char* my_register_num_8l[]={"AL" , "CL" , "DL" , "BL"}; 
uint32_t get_register_value(char *reg){
	int len=strlen(reg); 

	int i; 
	if(len==3){
		for(i=0; i<8; i++){
			if(!strcmp(reg , my_register_num_32[i]))
				return cpu.gpr[i]._32;
		}
		if(!strcmp(reg , "EIP")){
			//printf("MZYDEBUG $eip recognized as %u.\n" , cpu.eip); 
			return (uint32_t)cpu.eip; 
		}
	}
	else {
		for(i=0; i<8; i++)
			if(!strcmp(reg , my_register_num_16[i]))
				return cpu.gpr[i]._16; 
		for(i=0; i<4; i++){
			if(!strcmp(reg , my_register_num_8h[i]))
				return cpu.gpr[i]._8[1]; 
		}
		for(i=0; i<4; i++){
			if(!strcmp(reg , my_register_num_8l[i])){
				return cpu.gpr[i]._8[0]; 
			}
		}

	}	

	invalid_flag=1; 
	return 0; 
}

void tool_to_upper_case(char *s){
	int len=strlen(s); 
	int i; 
	for(i=0; i<len; i++){
		if(s[i]>='a' && s[i]<='z')
			s[i]+='A'-'a'; 
	}
	return; 
}

uint32_t eval(int p , int q){
	if(invalid_flag)return 0; 	
	//p , q is the beginning and ending of a subexpression
	if(p>q){
		/*bad expression*/
		//invalid_flag=1; 
		return 0; 
	}
	else if(p == q) { 
		/* Single token.
		 *		 * For now this token should be a number. 
		 *				 * Return the value of the number.
		 *						 */ 
		if(tokens[p].type==DEC)
			return string_to_int(tokens[p].str , 10);  
		else if(tokens[p].type==HEX)
			return string_to_int(tokens[p].str , 16);
		else if(tokens[p].type==REG){
			if(strlen(tokens[p].str)>4){
				printf("register not found.\n"); 
				invalid_flag=1; 
				return 0; 
			}
			char reg[10]; 
			strcpy(reg , &tokens[p].str[1]) ;		//first ch is '$'
			tool_to_upper_case(reg);  
			return get_register_value(reg); 
		}
		else {
			invalid_flag=1; 
			return 0; 
		} 
	}
	else if(check_parentheses(p ,  q) == true) {
		/*  The expression is surrounded by a matched pair of parentheses. 
		 *		 * If that is the case ,  just throw away the parentheses.
		 *				 */
		return eval(p + 1 ,  q - 1);  
	}
	else { 
		//dominant operator
		int i , op=p , op_priority = 1000; 
		//op is the position of current choice of dominant operator
		for (i=p; i<=q; i++){
			if(tokens[i].type=='('){
				int count=1; 
				while(count && i<q){
					i++; 
					if(tokens[i].type==')')
						count--; 
					else if(tokens[i].type=='(')
						count++;
				}
				if(!count)continue;
				else {
					//printf("invalid expression (line 223)\n"); 
					invalid_flag=1; 
					return 0; 
				} 
			}
			//now i is not in a pair of paren
			else if(get_operator_priority(tokens[i].type)==-1)continue; 
			else if(get_operator_priority(tokens[i].type) <= op_priority ){
				op_priority = get_operator_priority(tokens[i].type); 
				op = i; 
			}
		}
		//now op is the dominant operator
		if(tokens[op].type==NEG)return -eval(op+1 , q);
		if(tokens[op].type==DREF){
			swaddr_t temp = eval(op+1 , q); 
			if(temp >= 0x8000000){
				printf("physical address %x is outside of the physical memory!\n" , temp);
				invalid_flag=1; 
				return 0; 
			}
			printf("derf src is %x" , temp); 
			return swaddr_read(eval(op+1 , q) , 4);
		}
		uint32_t val1 = eval(p , op-1); 
		uint32_t val2 = eval(op+1 , q);
#ifdef MZYDEBUG
		printf("p=%d , q=%d , val1=%d, op=%d , val2=%d\n " ,p , q ,  val1 , op , val2); 
#endif
		switch(tokens[op].type){
			case '+':return val1+val2; 
			case '-':return val1-val2; 
			case '*':return val1*val2; 
			case '/':if(!val2)printf("warning: devided by 0\n"); 
					 return (double)val1/val2;
			case '%':if(!val2){invalid_flag=1; return 0; }
					 return val1%val2; 
			case '>':return val1>val2; 
			case '<':return val1<val2; 
			case GE:return val1>=val2; 
			case LE: return val1<=val2; 
			case '&':return val1&val2; 
			case '|':return val1|val2; 
			case '^':return val1^val2; 
			case AND:return val1&&val2; 
			case OR:return val1||val2; 
			case EQ:return val1==val2;
			case NEQ:return val1!=val2;
			case SL:return val1<<val2; 
			case SR:return val1>>val2; 
			default:invalid_flag=1; 
					printf( "operator %c not defined.\n" , tokens[op].type); 
			return 0; 
		}
	}
	return 0; 

}
#include<elf.h>
extern Elf32_Sym symtab[]; 
uint32_t expr(char *e, bool *success) {
	//if(!strlen(e))return 0; 
	//Elf32_Sym entry = symtab[0]; 
	printf("MZYDEBUG:\n"); 
	int i; 
	for(i=0; i<10; i++)printf("%x" , symtab[i].st_name); 


	if (!make_token(e)) {
		*success = false;
		return 0;
	}
#ifdef MZYDEBUG
	//int i; for(i=0; i<nr_token; i++)printf("%c " , tokens[i].type); 
	int paren=check_parentheses(0 , nr_token-1)?1:0;  
	printf("paren %d\n" , paren); 
	//printf("-----end of tokening-----\n" );
#endif
	//subdevide operators
	//int i; 
	for (i=0; i<nr_token; i++){
		//if tokens[i-1] is operator
		if(tokens[i].type == '*' && (i==0 || get_operator_priority(tokens[i-1].type) !=-1))
			tokens[i].type= DREF; 

		if(tokens[i].type == '-' && (i==0 || get_operator_priority(tokens[i-1].type) !=-1))
			tokens[i].type= NEG; 
	}

	static int gdb_expr_count=0; 
	uint32_t ans = eval(0 , nr_token-1);
	if (invalid_flag){
		printf("invalid expression\n"); 
		invalid_flag=0; 
		return 0; 
	} 
	else printf(" $%d = 0x%x\t%d\n" , gdb_expr_count++ , ans , ans); 
	return 0;
}

//different from merely expr()
//neat , without "$" index and return result as number without inner printer
//also used in cmd_w (featured with flag_const_watchpoint)
extern bool flag_const_watchpoint; 
uint32_t expr_cmd_x(char *e , bool *success){
	if(!make_token(e)){
		*success=false;
		return 0; 
	} 
	flag_const_watchpoint =  true; 
	int i; 
	for(i=0; i<nr_token; i++){
		//if tokens[i-1] is operator
		if(tokens[i].type == '*' && (i==0 || get_operator_priority(tokens[i-1].type) !=-1))
			tokens[i].type= DREF; 

		if(tokens[i].type == '-' && (i==0 || get_operator_priority(tokens[i-1].type) !=-1))
			tokens[i].type= NEG;
		if(tokens[i].type == DREF || tokens[i].type == REG)
			flag_const_watchpoint = false; 
	}
	uint32_t ans = eval(0 , nr_token-1);
	if(invalid_flag){
		printf("Invalid expression \n"); 
		invalid_flag=0;
		*success=false; 
		return 0; 
	}
	*success=true; 
	return ans; 
}
