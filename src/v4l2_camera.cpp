#include "v4l2_camera.h"

#define NB_BUFFER 300
int camera_fd;
unsigned char *addr[NB_BUFFER];


void save_as_jpeg(unsigned char *frame_buffer, size_t length, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Error opening file for writing");
        exit(EXIT_FAILURE);
    }

    fwrite(frame_buffer, 1, length, file);

    fclose(file);

    printf("%s saved successfully. wriite data %d \n",filename,length);
}

int init_camera(char *name){
	
	camera_fd = open(name, O_RDWR);
	if (camera_fd == -1)
	{
  	  // couldn't find capture device
  		printf("ERR(%s):failed to open %s\n", __func__, name);
    	return -1;
	}

	struct v4l2_capability cap;
	int iError ;
	// iError = ioctl(fd, VIDIOC_QUERYCAP, &cap);
    memset(&cap, 0, sizeof(struct v4l2_capability));
	if (ioctl(camera_fd, VIDIOC_QUERYCAP, &cap) < 0)
	{
    	printf("ERR(%s):VIDIOC_QUERYCAP failed\n", __func__);
		return -1;
	}
	if(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)		printf("video support V4L2_CAP_VIDEO_CAPTURE\n");
	if(cap.capabilities & V4L2_CAP_VIDEO_OUTPUT)		printf("video support V4L2_CAP_VIDEO_OUTPUT\n");
	if(cap.capabilities & V4L2_CAP_VIDEO_OVERLAY)		printf("video support V4L2_CAP_VIDEO_OVERLAY\n");
	if(cap.capabilities & V4L2_CAP_STREAMING)			printf("video support V4L2_CAP_STREAMING\n");
	if(cap.capabilities & V4L2_CAP_READWRITE)			printf("video support V4L2_CAP_READWRITE\n");


	struct v4l2_streamparm Stream_Parm;
	
	Stream_Parm.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
	
	if(ioctl(camera_fd,VIDIOC_G_PARM,&Stream_Parm)==-1)
	
	{
	
	perror("ioctl");
	
	exit(EXIT_FAILURE);
	
	}
	
	printf(">>: Frame rate: %u/%u\n",Stream_Parm.parm.capture.timeperframe.numerator,Stream_Parm.parm.capture.timeperframe.denominator);
	
	printf("\n");


	struct v4l2_input input;

	input.index = 0;
	while (!ioctl(camera_fd, VIDIOC_ENUMINPUT, &input))
	{
    	printf("input:%s\n", input.name);
    	++input.index;
	}

	input.index = 0; //指定输入设备

	if (ioctl(camera_fd, VIDIOC_S_INPUT, &input) < 0)
	{
		printf("ERR(%s):VIDIOC_S_INPUT failed\n", __func__);
		return -1;
	}

	struct v4l2_fmtdesc fmtdesc;

	fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmtdesc.index = 0;

	while (!ioctl(camera_fd, VIDIOC_ENUM_FMT, &fmtdesc))
	{
		printf("fmt:%s\n", fmtdesc.description);

		fmtdesc.index++;
	}

	struct v4l2_format v4l2_fmt;

	memset(&v4l2_fmt, 0, sizeof(struct v4l2_format));
	v4l2_fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE; 
	v4l2_fmt.fmt.pix.width = 320; //宽度
	v4l2_fmt.fmt.pix.height = 320; //高度
	v4l2_fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG; //像素格式
	v4l2_fmt.fmt.pix.field = V4L2_FIELD_ANY;

	if (ioctl(camera_fd, VIDIOC_S_FMT, &v4l2_fmt) < 0)
	{
		printf("ERR(%s):VIDIOC_S_FMT failed\n", __func__);
		return -1;
	}

	if (v4l2_fmt.fmt.pix.pixelformat == V4L2_PIX_FMT_MJPEG)
    {
        printf("Set VIDIOC_S_FMT sucessful\n");
	}
	struct v4l2_requestbuffers req;

	req.count = NB_BUFFER; //缓存数量
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_MMAP;

	if (ioctl(camera_fd, VIDIOC_REQBUFS, &req) < 0)
	{
		printf("ERR(%s):VIDIOC_REQBUFS failed\n", __func__);
		return -1;
	}

	struct v4l2_buffer v4l2_buffer;
	

	for (int i = 0; i < req.count; i++) 
    {
    	memset(&v4l2_buffer, 0, sizeof(struct v4l2_buffer));
    	v4l2_buffer.index = i;
    	v4l2_buffer.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    	v4l2_buffer.memory = V4L2_MEMORY_MMAP;
		
    	iError = ioctl(camera_fd, VIDIOC_QUERYBUF, &v4l2_buffer);
    	if (iError) 
        {
    	    printf("Unable to query buffer.\n");
    	    return -1;
    	}
		addr[i] = static_cast<unsigned char*> (mmap(0 /* start anywhere */ ,v4l2_buffer.length, PROT_READ, MAP_SHARED, camera_fd,v4l2_buffer.m.offset));
        if (addr[i] == MAP_FAILED) 
        {
            printf("Unable to map buffer\n");
            return -1;;
        }
        
    }        

    /* Queue the buffers. */
    for (int i = 0; i < req.count; i++) 
    {
    	memset(&v4l2_buffer, 0, sizeof(struct v4l2_buffer));
    	v4l2_buffer.index = i;
    	v4l2_buffer.type  = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    	v4l2_buffer.memory = V4L2_MEMORY_MMAP;
    	iError = ioctl(camera_fd, VIDIOC_QBUF, &v4l2_buffer);
    	if (iError)
        {
    	    printf("Unable to queue buffer.\n");
    	    return -1;
    	}
    }

}

void *Thread_get_image_from_camera(void *args){

	struct ThreadArgs_camera* new_args = (struct ThreadArgs_camera*)args;
	
	char* imagepath = new_args->save_image_name;
    pthread_mutex_t* Mutex = new_args->Mutex;

	enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if (ioctl(camera_fd, VIDIOC_STREAMON, &type) < 0)
	{
		printf("ERR(%s):VIDIOC_STREAMON failed\n", __func__);
		return;
	}
	int i = 0;
	
	while(1){
		struct pollfd poll_fds[1];

		poll_fds[0].fd = camera_fd;
		poll_fds[0].events = POLLIN; //等待可读

		poll(poll_fds, 1, -1);

		struct v4l2_buffer buffer;
		memset(&buffer, 0, sizeof(struct v4l2_buffer));
		buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buffer.memory = V4L2_MEMORY_MMAP;

		if (ioctl(camera_fd, VIDIOC_DQBUF, &buffer) < 0)
		{
			printf("ERR(%s):VIDIOC_DQBUF failed, dropped frame\n", __func__);
			return;
		}

		int index = buffer.index;
		pthread_mutex_lock(Mutex);
		int jpg_fd = open(imagepath,O_RDWR|O_CREAT,00666);
		if(jpg_fd == -1)
		{
			printf("open jpg Failed!\n ");
			return;     
		}
		int writesize = write(jpg_fd, addr[index], buffer.bytesused);

		pthread_mutex_unlock(Mutex);
		struct v4l2_buffer v4l2_buf;
		memset(&v4l2_buf, 0, sizeof(struct v4l2_buffer));
		v4l2_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		v4l2_buf.memory = V4L2_MEMORY_MMAP;
		v4l2_buf.index = buffer.index; //指定buf

		if (ioctl(camera_fd, VIDIOC_QBUF, &v4l2_buf) < 0)
		{
			printf("ERR(%s):VIDIOC_QBUF failed\n", __func__);
			return;
		}
		}
	// printf("detect_pic %d\n",i++);
	return;
}



int get_image_from_camera(char *imagepath){

	enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if (ioctl(camera_fd, VIDIOC_STREAMON, &type) < 0)
	{
		printf("ERR(%s):VIDIOC_STREAMON failed\n", __func__);
		return -1;
	}
	int i = 0;
	
	struct pollfd poll_fds[1];

	poll_fds[0].fd = camera_fd;
	poll_fds[0].events = POLLIN; //等待可读

	poll(poll_fds, 1, -1);

	struct v4l2_buffer buffer;
	memset(&buffer, 0, sizeof(struct v4l2_buffer));
	buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buffer.memory = V4L2_MEMORY_MMAP;

	if (ioctl(camera_fd, VIDIOC_DQBUF, &buffer) < 0)
	{
		printf("ERR(%s):VIDIOC_DQBUF failed, dropped frame\n", __func__);
		return -1;
	}

	int index = buffer.index;

	int jpg_fd = open(imagepath,O_RDWR|O_CREAT,00666);
	if(jpg_fd == -1)
	{
		printf("open jpg Failed!\n ");
		return -1;     
	}
	int writesize = write(jpg_fd,addr[index], buffer.bytesused);
	// printf("Write successfully size : %d\n",writesize);

	// save_as_jpeg(addr[index],buffer.length,name);

	struct v4l2_buffer v4l2_buf;
	memset(&v4l2_buf, 0, sizeof(struct v4l2_buffer));
	v4l2_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	v4l2_buf.memory = V4L2_MEMORY_MMAP;
	v4l2_buf.index = buffer.index; //指定buf

	if (ioctl(camera_fd, VIDIOC_QBUF, &v4l2_buf) < 0)
	{
		printf("ERR(%s):VIDIOC_QBUF failed\n", __func__);
		return -1;
	}
	// printf("detect_pic %d\n",i++);
	return 1;
}

