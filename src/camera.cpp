#include "camera.hpp"

static char* videobuf;
int width = 720;
int height = 404;
cv::Mat frame_read;

libvlc_instance_t *instance;
libvlc_media_t  *media;
libvlc_media_player_t *media_player;

/******************   Open Camera   ********************/

void *lock(void *data, void**p_pixels)
{
	*p_pixels = videobuf;
	return NULL;
}

void unlock(void *data, void *id, void *const *p_pixels)
{
	(void)data;
	assert(id == NULL);
}

void display(void*data, void *id)
{
	IplImage *img = cvCreateImage(CvSize(width, height), IPL_DEPTH_8U, 4);
	img->imageData = videobuf;
	frame_read = cv::cvarrToMat(img);
	cvReleaseImage(&img);
}

void libvlc_camera_open()
{
	videobuf = (char*)malloc((width*height) << 2);

	memset(videobuf, 0, (width*height) << 2);

	instance = libvlc_new(0, NULL);

	const char* option = ":network-caching=30";

	media = libvlc_media_new_location(instance, "rtsp://192.168.2.10");
	libvlc_media_add_option(media, option);
	media_player = libvlc_media_player_new_from_media(media);
	libvlc_media_release(media);

	libvlc_video_set_callbacks(media_player, lock, unlock, display, NULL);
	libvlc_video_set_format(media_player, "RV32", width, height, width << 2);
	libvlc_media_player_play(media_player);
}

void libvlc_camera_close()
{
	libvlc_media_player_stop(media_player);
	libvlc_media_player_release(media_player);
	libvlc_release(instance);
	free(videobuf);
	frame_read.release();
}

/******************   Open Camera   ********************/