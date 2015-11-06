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
	for(i = 0; i < NR_WP - 1; i++) {
		wp_list[i].NO = i;
		wp_list[i].next = &wp_list[i + 1];
		wp_list[i+1].last = &wp_list[i]; 
	}
	wp_list[NR_WP - 1].next = NULL;
	head = &aux_wp_head;
	head->next=NULL; 
	head->last=NULL; 
	free_ = &wp_list[0];
	return; 
}

WP* new_wp(void){
	printf("new_wp() started\n"); 
	printf("free_ = %p\n" , free_); 
	//if(free_ == NULL){
	//	return NULL; 
	//}
	WP *new_free_ = free_->next;
	new_free_->last = NULL;//TODO 
	free_->next  = head->next;
	if(head->next!=NULL)
		head->next->last = free_;
	head->next = free_;
	printf("at line 32 in watchpoint.c\n"); 
	free_->last = head; 
	free_ = new_free_; 
	return head->next; 

}

void free_wp(WP *wp){
	if(wp->last!=NULL)
		wp->last->next = wp->next;
	if(wp->next!=NULL)
		wp->next->last = wp->last; 
	wp->next = free_; 
	free_->last = wp; 
	free_ = wp; 
	return; 
}

extern uint32_t expr_cmd_x(char* expr ,bool *success); 
WP *get_new_wp(char *expr){
	printf("get_new_wp() started\n"); 
	WP* new = new_wp(); 
	if(new==NULL)return NULL; 
	//?	free_->NO = top_watchpoint_NO++; 
	new->NO = top_watchpoint_NO++; 
	bool success;
	//TODO
	printf("at line 53 in watchpoint\n");
	uint32_t temp= expr_cmd_x(expr , &success);  
	if(!success)panic("Exception: invalid expression for get_new_wp()\n"); 
	new->old_value = temp; 
	strcpy(free_->expr , expr);
	printf("Watchpoint %d: %s\n" , top_watchpoint_NO , expr); 
	return new;
}

void delete_wp(int num_2_delete){
	int found=0; 
	WP* temp; 
	if(head->next==NULL){
		printf("Empty watchpoint list\n"); 
		return; 
	}
	for(temp=head->next; temp!=NULL; temp=temp->next){
		if(temp->NO==num_2_delete){
			free_wp(temp);
			found=1;
			break; 
		}
	}
	if(!found){
		printf("No watchpoint number %d\n" , num_2_delete); 
	}
	return; 
}

/* TODO: Implement the functionality of watchpoint */


