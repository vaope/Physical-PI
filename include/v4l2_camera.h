#ifndef V4L2_CAMERA_H
#define V4L2_CAMERA_H

#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <poll.h>

struct ThreadArgs_camera{
    char* save_image_name;
    pthread_mutex_t* Mutex;
};

int init_camera(char *cemera_name);
void *Thread_get_image_from_camera(void *args);
int get_image_from_camera(char *imagepath);

#endif //!V4L2_CAMERA_H