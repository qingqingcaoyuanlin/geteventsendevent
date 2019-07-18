#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <unistd.h>
//#include <linux/input.h> // this does not compile
#include <errno.h>
// from <linux/input.h>
struct input_event {
    struct timeval time;
    __u16 type;
    __u16 code;
    __s32 value;
};
#define EVIOCGVERSION       _IOR('E', 0x01, int)            /* get driver version */
#define EVIOCGID        _IOR('E', 0x02, struct input_id)    /* get device ID */
#define EVIOCGKEYCODE       _IOR('E', 0x04, int[2])         /* get keycode */
#define EVIOCSKEYCODE       _IOW('E', 0x04, int[2])         /* set keycode */
#define EVIOCGNAME(len)     _IOC(_IOC_READ, 'E', 0x06, len)     /* get device name */
#define EVIOCGPHYS(len)     _IOC(_IOC_READ, 'E', 0x07, len)     /* get physical location */
#define EVIOCGUNIQ(len)     _IOC(_IOC_READ, 'E', 0x08, len)     /* get unique identifier */
#define EVIOCGKEY(len)      _IOC(_IOC_READ, 'E', 0x18, len)     /* get global keystate */
#define EVIOCGLED(len)      _IOC(_IOC_READ, 'E', 0x19, len)     /* get all LEDs */
#define EVIOCGSND(len)      _IOC(_IOC_READ, 'E', 0x1a, len)     /* get all sounds status */
#define EVIOCGSW(len)       _IOC(_IOC_READ, 'E', 0x1b, len)     /* get all switch states */
#define EVIOCGBIT(ev,len)   _IOC(_IOC_READ, 'E', 0x20 + ev, len)    /* get event bits */
#define EVIOCGABS(abs)      _IOR('E', 0x40 + abs, struct input_absinfo)     /* get abs value/limits */
#define EVIOCSABS(abs)      _IOW('E', 0xc0 + abs, struct input_absinfo)     /* set abs value/limits */
#define EVIOCSFF        _IOC(_IOC_WRITE, 'E', 0x80, sizeof(struct ff_effect))   /* send a force effect to a force feedback device */
#define EVIOCRMFF       _IOW('E', 0x81, int)            /* Erase a force effect */
#define EVIOCGEFFECTS       _IOR('E', 0x84, int)            /* Report number of effects playable at the same time */
#define EVIOCGRAB       _IOW('E', 0x90, int)            /* Grab/Release device */
// end <linux/input.h>
int main(int argc, char *argv[])
{
    int fd;
    int struct_count =0;
    ssize_t ret;
    int version;
    struct input_event event[(argc-2)/4];

    fd = open(argv[1], O_RDWR);
    if(fd < 0) {
        fprintf(stderr, "could not open %s, %s\n", argv[1], strerror(errno));
        return 1;
    }
    if (ioctl(fd, EVIOCGVERSION, &version)) {
        fprintf(stderr, "could not get driver version for %s, %s\n", argv[1], strerror(errno));
        return 1;
    }

    int replay_speed = atoi(argv[2]);

    for(int x=3;x< argc;x+=4)
    {

        memset(&event[struct_count], 0, sizeof(event[struct_count]));
        event[struct_count].type = atoi(argv[x]);
        event[struct_count].code = atoi(argv[x+1]);
        event[struct_count].value = atoi(argv[x+2]);
        ret = write(fd, &event[struct_count], sizeof(event[struct_count]));
        if(ret < (ssize_t) sizeof(event[struct_count])) {
            fprintf(stderr, "write event failed, %s\n", strerror(errno));
            return -1;
        }
        
        struct_count++;

        double delay = strtod(argv[x+3],NULL);

        if(delay!=0.0){
            double d = delay*1000*1000;
            //printf("usleep");
            usleep(d/replay_speed);
        }


    }

    
    return 0;
}