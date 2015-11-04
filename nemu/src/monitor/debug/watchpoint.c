#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 48

static WP wp_list[NR_WP];
static WP aux_wp_head; 
static WP *head, *free_;
int top_watchpoint_NO=1; 
//initilazation called in ui_mainloop(); 
void init_wp_list() {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_list[i].NO = i;
		wp_list[i].next = &wp_list[i + 1];
	}
	wp_list[NR_WP - 1].next = NULL;

	head = &aux_wp_head;
	head->next=NULL; 
	free_ = wp_list;
}

WP *new_wp(){
	if(free_ == NULL){
		return NULL; 
	}
	WP *new_free_ = free_->next; 
	free_->next  = head->next; 
	head->next->last = free_; 
	head->next = free_;
	free_->last = head; 
	free_ = new_free_; 
	return head->next; 
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
	WP *new = new_wp(); 
	if(new==NULL)return NULL; 
	//?	free_->NO = top_watchpoint_NO++; 
	new->NO = top_watchpoint_NO++; 
	bool success;
	printf("at line 53 in watchpoint\n"); 
	uint32_t temp= expr_cmd_x(expr , &success);  
	if(!success)panic("Exception: invalid expression for get_new_wp()\n"); 
	new->old_value = temp; 
	strcpy(free_->expr , expr);
	printf("Watchpoint %d: %s" , top_watchpoint_NO , expr); 
	return new; 
}


/* TODO: Implement the functionality of watchpoint */


