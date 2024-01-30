#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "linux/input.h"

#define ENABLE_PI 1
#define GET_ACCURACY 2

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
            if(ev.code == BTN_SIDE)return ENABLE_PI;
            if(ev.code == BTN_EXTRA)return GET_ACCURACY;
        }
    }
        if (FD_ISSET(mouse_fd, &readfds)) {
        int bytes = read(mouse_fd, data, sizeof(data));
        if(data[2]!=0)data[2] = 256 - data[2];
    }

    
    write(gadget_fd, data,3);
}
int main(int argc, char** argv){
    char *mouse_name = argv[1];
    char *gadget_name = argv[2];
    char *mouse_event_name = argv[3];
    int mouse_fd,mouse_event_fd,gadget_fd;
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
    unsigned char data[8];
    struct input_event ev;
    
    while(1){
        FD_ZERO(&readfds);readfds);
        FD_SET(mouse_fd, &readfds);
        FD_SET(mouse_event_fd, &readfds);
        int retval = select(max_fd + 1, &readfds, NULL, NULL,&tv);
        if (retval == -1) {
            perror("select()");
            exit(EXIT_FAILURE);
        }
        if(retval == 0)continue;
        memset(data,0,sizeof(data));
        if (FD_ISSET(mouse_fd, &readfds)) {
            int bytes = read(mouse_fd, data, sizeof(data));
            // printf("data : 0x%x 0x%x\n",data[1],data[2]);
            if(data[2]!=0)data[2] = 256 - data[2];
        }

        if (FD_ISSET(mouse_event_fd, &readfds)) {
            read(mouse_event_fd, &ev, sizeof(struct input_event));
            if (ev.type == EV_KEY && (ev.code == BTN_SIDE || ev.code == BTN_EXTRA )) {
                if(ev.code == BTN_SIDE){
                    if(ev.value)  data[0] |= 0x08;
                    else  data[0] &= (~0x08);
                }
                if(ev.code == BTN_EXTRA){
                    if(ev.value)  data[0] |= 0x10;
                    else  data[0] &= (~0x10);
                }
            }
            if (ev.type == EV_REL && ev.code == REL_WHEEL) {
                data[3] = ev.value;
                printf("Wheel Event: time %ld.%06ld, type %d, code %d, value %d\n",
                       ev.time.tv_sec, ev.time.tv_usec, ev.type, ev.code, ev.value);
            }
        }
        // int bytes = read(mouse_fd, data, sizeof(data));
        
        write(gadget_fd, data,3);

    }
	// filename = mouse_name;
}