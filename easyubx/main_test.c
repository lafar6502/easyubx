#include <errno.h>
#include <fcntl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "easyubx_drv.h"


struct eubx_handle ubx;

uint16_t ser_receive_buffer(void *usr_ptr, uint8_t *buffer, uint16_t max_len) 
{
	int fd = (int) usr_ptr;
	uint16_t len = read(fd, buffer, max_len);
	return len;
};

void ser_send_byte(void* user_ptr, uint8_t b) {
	int fd = (int) user_ptr;
	write(fd, &b, 1);
}

void ser_send_buffer(void* ptr, const uint8_t *buf, uint16_t len) {
	int fd = (int) ptr;
	write(fd, buf, len);
}

void test_notify_event(void* ptr, TEasyUBXEvent event) {
	printf("\n--Notify event %d\n", event);
	printf("len=%d, error=%d, class=%x, msg_id=%x\n", ubx.receive_message.message_length, ubx.last_error, ubx.receive_message.message_class, ubx.receive_message.message_id);

        switch (event)
        {
        case EUBXReceivedCfgNAV5:
		printf("CfgNAVS mask=%x %x, DynMode=%x, FixMode=%x\n", ubx.receive_message.message_buffer[0], ubx.receive_message.message_buffer[1], ubx.receive_message.message_buffer[2], ubx.receive_message.message_buffer[3]);  
            
            break;

        case EUBXReceivedCfgPRT:
		printf("CfgPRT port=%x, inmask=%x %x, outmaks=%x %x\n", ubx.receive_message.message_buffer[0], ubx.receive_message.message_buffer[12], ubx.receive_message.message_buffer[13], ubx.receive_message.message_buffer[14], ubx.receive_message.message_buffer[15]);
		break;

        case EUBXReceivedACK:
		printf("ACK for class=%x, id=%x\n", ubx.receive_message.message_buffer[0], ubx.receive_message.message_buffer[1]);
		break;
 
        case EUBXReceivedNAK:
		printf("NACK for class=%x, id=%x\n", ubx.receive_message.message_buffer[0], ubx.receive_message.message_buffer[1]);
            break;
	default:
		printf("unknown\n");
		break;
        }
    
};

int set_interface_attribs(int fd, int speed)
{
    struct termios tty;

    if (tcgetattr(fd, &tty) < 0) {
        printf("Error from tcgetattr: %s\n", strerror(errno));
        return -1;
    }

    cfsetospeed(&tty, (speed_t)speed);
    cfsetispeed(&tty, (speed_t)speed);

    tty.c_cflag |= (CLOCAL | CREAD);    /* ignore modem controls */
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;         /* 8-bit characters */
    tty.c_cflag &= ~PARENB;     /* no parity bit */
    tty.c_cflag &= ~CSTOPB;     /* only need 1 stop bit */
    tty.c_cflag &= ~CRTSCTS;    /* no hardware flowcontrol */

    /* setup for non-canonical mode */
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    tty.c_oflag &= ~OPOST;

    /* fetch bytes as they become available */
    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 1;

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        printf("Error from tcsetattr: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

void set_mincount(int fd, int mcount)
{
    struct termios tty;

    if (tcgetattr(fd, &tty) < 0) {
        printf("Error tcgetattr: %s\n", strerror(errno));
        return;
    }

    tty.c_cc[VMIN] = mcount ? 1 : 0;
    tty.c_cc[VTIME] = 5;        /* half second timer */

    if (tcsetattr(fd, TCSANOW, &tty) < 0)
        printf("Error tcsetattr: %s\n", strerror(errno));
}


int main()
{
    char *portname = "/dev/ttyUSB0";
    int fd;
    int wlen;

    fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        printf("Error opening %s: %s\n", portname, strerror(errno));
        return -1;
    }
    /*baudrate 115200, 8 bits, no parity, 1 stop bit */
    set_interface_attribs(fd, B9600);
    //set_mincount(fd, 0);                /* set to pure timed read */
	

    TEasyUBXError e0 = eubx_init(&ubx, ser_receive_buffer, ser_send_byte, ser_send_buffer, test_notify_event, (void *) fd);
	if (e0 != EUBX_ERROR_OK) {
		printf("Error initializing ubx: %d\n", e0);
		return e0;	
	}

	printf("ubx inited\n");
	printf("chipset %x, software %s\n", ubx.receiver_info.chipset_version, ubx.receiver_info.software_version);


	eubx_loop(&ubx);
}
