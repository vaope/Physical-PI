#ifndef GADGET_MOUSE_H
#define GADGET_MOUSE_H

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
#define FREE_ENABEL_PI 3
struct Gadget_mouse_input
{
    char click;
    char x;
    char y;
};

int init_mouse(char *mouse_name,char *mouse_event_name,char *gadget_name);
int send_message_mouse(struct Gadget_mouse_input input);
int mouse_to_gadget();
int click_mouse();
#endif //!GADGET_MOUSE_H