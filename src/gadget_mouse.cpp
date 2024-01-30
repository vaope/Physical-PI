#include"gadget_mouse.h"

int mouse_fd,mouse_event_fd,gadget_fd,max_fd;
fd_set readfds;
struct timeval tv;

int init_mouse(char *mouse_name,char *mouse_event_name,char *gadget_name){
    if ((mouse_fd = open(mouse_name, O_RDONLY, 0666)) == -1) {
	    perror(mouse_name);	
	    return -1;	  
    }	 
    if ((mouse_event_fd = open(mouse_event_name, O_RDONLY, 0666)) == -1) {
	    perror(mouse_event_name);	
	    return -1;	  
    }	
    if ((gadget_fd = open(gadget_name, O_RDWR, 0666)) == -1) {
	    perror(gadget_name);	
	    return -1;	  
    }
    int max_fd = (mouse_fd>mouse_event_fd)?mouse_fd:mouse_event_fd;
    memset(&tv,0,sizeof(tv));
}
void thread_mouse_to_gadget(void *args){
	int *mouse_key = (int *)args;
	unsigned char data[8];
    struct input_event ev;
	bool PI_state = 0;
    while(1){
		FD_ZERO(&readfds);
    	FD_SET(mouse_fd, &readfds);
    	FD_SET(mouse_event_fd, &readfds);
    	int retval = select(max_fd + 1, &readfds, NULL, NULL,&tv);
    	if (retval == -1) {
    	    perror("select()");
			*mouse_key = -1;
    	    return;
    	}
    	if(retval == 0) continue;
    	memset(data,0,sizeof(data));


    	if (FD_ISSET(mouse_event_fd, &readfds)) {
    	    read(mouse_event_fd, &ev, sizeof(struct input_event));
    	    if (ev.type == EV_KEY && (ev.code == BTN_SIDE || ev.code == BTN_EXTRA )) {
    	        if(ev.code == BTN_SIDE){
					if(ev.value == 1){
						*mouse_key = ENABLE_PI;
						PI_state = 1;
					}
					else {
						*mouse_key = FREE_ENABEL_PI;
						PI_state = 0;
					}
				}
    	        if(ev.code == BTN_EXTRA && ev.value == 0)*mouse_key = GET_ACCURACY;
    	    }
    	}
		if(PI_state)continue;
    	if (FD_ISSET(mouse_fd, &readfds)) {
    	    int bytes = read(mouse_fd, data, sizeof(data));
    	    if(data[2]!=0)data[2] = 256 - data[2];
		}
    	write(gadget_fd, data,3);
	}
}
int mouse_to_gadget(){
    unsigned char data[8];
    struct input_event ev;
    FD_ZERO(&readfds);readfds);
    FD_SET(mouse_fd, &readfds);
    FD_SET(mouse_event_fd, &readfds);
    int retval = select(max_fd + 1, &readfds, NULL, NULL,&tv);
    if (retval == -1) {
        perror("select()");
        return -1;
    }
    if(retval == 0)return 0;
    memset(data,0,sizeof(data));


    if (FD_ISSET(mouse_event_fd, &readfds)) {
        read(mouse_event_fd, &ev, sizeof(struct input_event));
        if (ev.type == EV_KEY && (ev.code == BTN_SIDE || ev.code == BTN_EXTRA )) {
            if(ev.code == BTN_SIDE){
				if(ev.value == 1)return ENABLE_PI;
				else return FREE_ENABEL_PI;
            if(ev.code == BTN_EXTRA && )return GET_ACCURACY;
        }
    }
        if (FD_ISSET(mouse_fd, &readfds)) {
        int bytes = read(mouse_fd, data, sizeof(data));
        if(data[2]!=0)data[2] = 256 - data[2];
    }

    
    write(gadget_fd, data,3);
}

int send_message_mouse(struct Gadget_mouse_input input){
	char report[8];
	memset(report,0,8);
	report[1] = input.x;
	report[2] = input.y;
	if (write(gadget_fd, report, 3) != 3) {
		printf("send_message_mouse error\n");
	  	return -1;
	}
	return 1;
}

int click_mouse(){
	char report[8];
	memset(report,0,sizeof(report));
	report[0] = 1;
	if (write(gadget_fd, report, 3) != 3) {
		printf("send_message_mouse error\n");
	  	return -1;
	}
	report[0] = 0;
	if (write(gadget_fd, report, 3) != 3) {
		printf("send_message_mouse error\n");
	  	return -1;
	}
	return 1;

}
  