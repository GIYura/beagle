#ifndef IOCTL_H
#define IOCTL_H

#define MAGIC_VALUE 'A'
#define WR          0x01
#define RD          0x02
#define GRT         0x03

struct test
{
    int value;
    char name[64];
};

#define WR_VALUE _IOW(MAGIC_VALUE, WR, int32_t *)
#define RD_VALUE _IOR(MAGIC_VALUE, RD, int32_t *)
#define GREETER  _IOW(MAGIC_VALUE, GRT, struct test *)

#endif /* IOCTL_H */


