#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <string.h>

int main(int argc, char **argv[]) {
    const char *device = argv[1]; // 替换 X 为你的设备号
    int fd;
    struct input_event ev;
    int errno=0;

    fd = open(device, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Cannot open %s: %s.\n", device, strerror(errno));
        return EXIT_FAILURE;
    }

    printf("Reading from: %s\n", device);
    

    fd_set readfds;
    struct timeval tv;


    while (1) {
        FD_ZERO(&readfds);
        FD_SET(fd, &readfds);

        // 设置超时为5秒
        tv.tv_sec = 0;
        tv.tv_usec = 0;

        int retval = select(fd + 1, &readfds, NULL, NULL, &tv);

        if (retval == -1) {
            perror("select()");
            return EXIT_FAILURE;
        } else if (retval) {
            // 数据可读
            read(fd, &ev, sizeof(struct input_event));
            if (ev.type == EV_KEY && (ev.code == BTN_SIDE || ev.code == BTN_EXTRA ||
                ev.code == BTN_FORWARD || ev.code == BTN_BACK || ev.code == BTN_TASK)) {
                printf("Event: time %ld.%06ld, type %d, code %d, value %d\n",
                   ev.time.tv_sec, ev.time.tv_usec, ev.type, ev.code, ev.value);
            }
            if (ev.type == EV_REL && ev.code == REL_WHEEL) {
                printf("Wheel Event: time %ld.%06ld, type %d, code %d, value %d\n",
                       ev.time.tv_sec, ev.time.tv_usec, ev.type, ev.code, ev.value);
            }
        }
    }


    close(fd);
    return 0;
}
