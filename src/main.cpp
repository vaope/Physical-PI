#include "gadget_mouse.h"
#include "v4l2_camera.h"
#include "v5lite.h"
#include "pid.h"
#include <pthread.h>

#define DEBUG 0

char *temp_image_path = "temp_image.jpeg";

#if DEBUG

int main(){
    if(init_mouse("/dev/hidg0")<0){
        printf("init mouse error\n");
        return 0;
    }
    struct Gadget_mouse_input input;
    input.x = 1;
    input.y = 1;
    while(1){
        send_message_mouse(input);
        if(click_mouse()<0){
            perror("send message to mouse error");
            return 0;
        }
    }
}
#else
int main(int argc, char** argv){
    if(argc != 3){
        printf("Usage: %s [mouse_devname] [mouse_event_name] [gadget_name] [camera_devname]\n",argv[0]);
        return 0;
    }
    char *mouse_name = argv[1];
    char *mouse_event_name = argv[2];
    char *gadget_name = argv[3];
    char *camera_name = argv[4];

    if(init_mouse(mouse_name,mouse_event_name,gadget_name)<0){
        printf("init mouse error\n");
        return 0;
    }

    if(init_camera(camera_name)<0){
        printf("init camera error\n");
        return 0;
    }

#if USE_INT8
    char *param_name ="v5lite-i8e.param";
    char *bin_name ="yolov5-i8e.bin";
    bool is_int8 = 1;
#else 
    char *param_name ="PI-opt.param";
    char *bin_name ="PI-opt.bin";
    bool is_int8 = 0;
#endif
    init_yolov5(param_name,bin_name,is_int8);


    pthread_mutex_t Mutex;
    if (pthread_mutex_init(&Mutex, NULL) != 0) {
        // Initialization failed
        perror("Mutex initialization failed");
        return 1;
    }

    pthread_t Video_thread,Mouse_thread;;
    ThreadArgs_camera* args = new ThreadArgs_camera;
    int mouse_key;

    args->save_image_name = temp_image_path;
    args->Mutex = &Mutex;
    int result = pthread_create(&Video_thread, NULL, Thread_get_image_from_camera,static_cast<void*>(args));
    if (result != 0) {
        perror("Unable to create thread");
        return 0;
    }
    result = pthread_create()
    
    std::vector<struct Detect_output> detect_result;
    struct Gadget_mouse_input gadget_mouse_control;
    int ret,mouse_key;
    while(1){
        
        ret = detect_image(temp_image_path,Mutex,detect_result);
        if(ret<0){
            perror("detect error");
            return 0;
        }
        if(ret == 1){
            pid_control_mouse(detect_result,gadget_mouse_control);
            if(send_message_mouse(gadget_mouse_control)<0){
                perror("send message to mouse error");
                return 0;
            }
        }
        if(click_mouse()<0){
            perror("send message to mouse error");
            return 0;
        }
    }
}

#endif