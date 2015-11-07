#include "monitor/watchpoint.h"
#include "monitor/expr.h"
#include "monitor/monitor.h"
#include "nemu.h"
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
	if(free_ == NULL ){
		printf("MZYDEBG free_ == NULL\n"); 
		return NULL; 
	}
	WP *new_free_ = free_->next;
	if(new_free_!=NULL)
		new_free_->last = NULL;//TODO 
	free_->next  = head->next;
	if(head->next!=NULL)
		head->next->last = free_;
	head->next = free_;
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
	WP* new = new_wp(); 
	if(new==NULL){
		printf("new_wp return NULL\n");//TODO 
		return NULL;
		
	} 
	new->NO = ++top_watchpoint_NO; 
	bool success=true;
	//TODO
	uint32_t temp= expr_cmd_x(expr , &success);  
	if(!success)panic("Exception: invalid expression for get_new_wp()\n"); 
	new->old_value = temp; 
	strcpy(new->expr , expr);
#ifdef MZYDEBUG
	printf("MZYDEBUG expr recorded is %s from %s\n" , new->expr , expr); 
#endif
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

void print_watchpoint_list(){
	if(head->next==NULL){
		printf("Empty watchpoint list\n"); 
		return; 
	}
	WP* temp; 
	for(temp=head->next; temp!=NULL; temp=temp->next){
		printf("%d    watchpoint\t%s\n\tvalue %d  0x%x\n" , temp->NO , temp->expr , temp->old_value , temp->old_value); 	
	}
	return; 
}
/* TODO: Implement the functionality of watchpoint */
//unsigned int monitor_get_eip32(); 
void check_watchpoints(){
	if(head->next==NULL)
		return; 
	WP *temp;
	int found=0;
	bool success=true; 
	for(temp=head->next; temp!=NULL; temp=temp->next){
		uint32_t new_value = expr_cmd_x(temp->expr , &success);
		//printf("expr is %s\n" , temp->expr); 
		if(!success){
			printf("Watchpoint %d in an invalid state\n" , temp->NO); 
			continue; 
		}
		if(new_value!=temp->old_value){
			printf("Watchpoint %d: %s\n\tOld value = %u\t0x%x\n\tNew value = %u\t0x%x\n" , temp->NO , temp->expr ,  temp->old_value  , temp->old_value, new_value , new_value); 
			temp->old_value = new_value;
			found=1; 
		}
	}
	if(found){
		printf("0x%x in nemu\n" , cpu.eip); 
	}
	printf("MZYDEBUG free_ is %p" , free_); 
	return;
}
