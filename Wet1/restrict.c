#include <linux/sched.h>
#define LOG_SIZE 100


void shift_left(struct task_struct *p) {
	int i = 1 ;
	for( ; i < p->violations ; i++ ){
		p->log_forbidden_activity[ i-1 ].syscall_num = p->log_forbidden_activity[ i ].syscall_num;
		p->log_forbidden_activity[ i-1 ].syscall_restriction_threshold = p->log_forbidden_activity[ i ].syscall_restriction_threshold;
		p->log_forbidden_activity[ i-1 ].proc_restriction_level = p->log_forbidden_activity[ i ].proc_restriction_level;
		p->log_forbidden_activity[ i-1 ].time = p->log_forbidden_activity[ i ].time;
	}
	p->violations--;
}

// recording forbidden activity - when current task privilege level is lower then the privilege level threshold of the involved system call syscall_number
void recording_forbidden_activity (struct task_struct *p, int syscall_num, int threshold) {
	if(violations = LOG_SIZE ){
		shift_left(p);
	}
	p->log_forbidden_activity[ p->violations ].syscall_num = syscall_num;
	p->log_forbidden_activity[ p->violations ].syscall_restriction_threshold = threshold;
	p->log_forbidden_activity[ p->violations ].proc_restriction_level = p->restriction_level;
	p->log_forbidden_activity[ p->violations ].time = jiffies;
	p->violations++;  
}

// Checks whether the requested system call is a valid call 
int is_valid(int syscall_num){
	if ( current->feature == OFF ){
		return 1;
	}
	int i = 0;
	for( ; i < current->restricions_list_size ; i++ ){
		if( current->restricions_list[i].syscall_num == syscall_num){
			if( current->restriction_level < current->restricions_list[i].restriction_threshold){
				recording_forbidden_activity(current, syscall_num, restricions_list[i].restriction_threshold);
				return 0;
			}
			else {
				return 1;
			}
	}
	return 1;	
}