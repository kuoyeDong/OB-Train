#include <opencv.hpp>
#include <vlc/vlc.h>

void *lock(void *data, void**p_pixels);
void unlock(void *data, void *id, void *const *p_pixels);
void display(void*data, void *id);
void libvlc_camera_open();
void libvlc_camera_close();