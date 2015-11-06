#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_list[NR_WP];
static WP aux_wp_head; 
static WP *head, *free_;
int top_watchpoint_NO=1; 
//initilazation called in ui_mainloop(); 
void init_wp_list() {
	int i;
	for(i = 0; i < NR_WP - 1; i ++) {
		wp_list[i].NO = i;
		wp_list[i].next = &wp_list[i + 1];
		wp_list[i+1].last = &wp_list[i]; 
	}
	wp_list[NR_WP - 1].next = NULL;
	wp_list[NR_WP - 1].last = &wp_list[NR_WP - 2];
	head = &aux_wp_head;
	head->next=NULL; 
	head->last=NULL; 
	free_ = wp_list;
	return; 
}

WP* new_wp(void){
	printf("new_wp() started\n"); 
	printf("free_ = %p\n" , free_); 
	//if(free_ == NULL){
	//	return NULL; 
	//}
	printf("0"); 
	WP *new_free_ = free_->next;
	printf("1"); 
	free_->next  = head->next;
	printf("31"); 
	head->next->last = free_;
	printf("33"); 
	head->next = free_;
	printf("at line 32 in watchpoint.c\n"); 
	free_->last = head; 
	free_ = new_free_; 
	return head->next; 

	return NULL; 
}

void free_wp(WP *wp){
	wp->last->next = wp->next;
	wp->next->last = wp->last; 
	wp->next = free_; 
	free_->last = wp; 
	free_ = wp; 
	return; 
}

extern uint32_t expr_cmd_x(char* expr ,bool *success); 
WP *get_new_wp(char *expr){
	printf("get_new_wp() started\n"); 
	new_wp(); 
/*	if(new==NULL)return NULL; 
	//?	free_->NO = top_watchpoint_NO++; 
	new->NO = top_watchpoint_NO++; 
	bool success;
	//TODO
	printf("at line 53 in watchpoint\n");
	uint32_t temp= expr_cmd_x(expr , &success);  
	if(!success)panic("Exception: invalid expression for get_new_wp()\n"); 
	new->old_value = temp; 
	strcpy(free_->expr , expr);
	printf("Watchpoint %d: %s" , top_watchpoint_NO , expr); 
	return new;
*/
	return NULL; 
}


/* TODO: Implement the functionality of watchpoint */


