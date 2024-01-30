#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>

int main(int agv,char **args) {
    const char *device = args[1];  // 鼠标设备文件路径
    int fd;
    ssize_t bytes;
    unsigned char data[8];  // 鼠标事件通常包含3个字节的数据

    // 打开鼠标设备文件
    fd = open(device, O_RDONLY);
    if (fd == -1) {
        perror("Error opening mouse device");
        return EXIT_FAILURE;
    }

    printf("Reading from %s (mouse device)\n", device);

    // 读取并打印鼠标事件数据
    while (1) {
        bytes = read(fd, data, sizeof(data));
        if (bytes > 0) {
            printf("Bytes read: %ld\n", bytes);
            printf("Data: %02x %02x %02x 02x %02x %02x 02x %02x\n", data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);
        }
    }

    close(fd);
    return EXIT_SUCCESS;
}