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
	printf("len=%d, error=%d, class=%02x, msg_id=%02x\n", ubx.receive_message.message_length, ubx.last_error, ubx.receive_message.message_class, ubx.receive_message.message_id);

        switch (event)
        {
        case EUBXReceivedCfgNAV5:
		printf("CfgNAVS mask=%x %x, DynMode=%x, FixMode=%x\n", ubx.receive_message.message_buffer[0], ubx.receive_message.message_buffer[1], ubx.receive_message.message_buffer[2], ubx.receive_message.message_buffer[3]);  
            
            break;

        case EUBXReceivedCfgPRT:
		printf("CfgPRT port=%02x, inmask=%02x %02x, outmaks=%x %x\n", ubx.receive_message.message_buffer[0], ubx.receive_message.message_buffer[12], ubx.receive_message.message_buffer[13], ubx.receive_message.message_buffer[14], ubx.receive_message.message_buffer[15]);
		break;

        case EUBXReceivedACK:
		printf("ACK for class=%02x, id=%02x\n", ubx.receive_message.message_buffer[0], ubx.receive_message.message_buffer[1]);
		break;
 
        case EUBXReceivedNAK:
		printf("NACK for class=%02x, id=%02x\n", ubx.receive_message.message_buffer[0], ubx.receive_message.message_buffer[1]);
            break;
	default:
		for (int i=0; i< ubx.receive_message.message_length; i++ ) {
            printf("%02x ", ubx.receive_message.message_buffer[i]);
        }
        printf("\n");
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

void processfile(const char* fn, void (*process)(const char* ln, int lnum)) {
    FILE* f = fopen(fn, "r");
    if (!f) {
        printf("File does not exist; %s\n", fn);
        return;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int lnum = 0;
    while ((read = getline(&line, &len, f)) != -1) {
        printf("Retrieved line of length %zu :\n", read);
        printf("%s", line);
        lnum++;
        if (read == 0) continue;
        if (line[0] == ';') continue;
        process(line, lnum);
    }
    free(line);
    fclose(f);
}

unsigned char charToHexDigit(char c)
{
  if (c >= 'A')
    return c - 'A' + 10;
  else
    return c - '0';
}

unsigned char stringToByte(char c[2])
{
  return charToHexDigit(c[0]) * 16 + charToHexDigit(c[1]);
}

int16_t read_next_hexbyte(const char** pp) {
    const char* p = *pp;
    while(isspace(*p)) p++;
    char c = toupper(*p);
    if (!isdigit(c) && !(c >= 'A' && c <= 'F')) {
        printf("e1 %c|%s\n", c, p);
        *pp = p;
        return -1;
    }
    printf("c is %c\n", c);
    char buf[2];
    buf[0] = *p;
    p++;
    c = toupper(*p);
    if (!isdigit(c) && !(c >= 'A' && c <= 'F')) {
        
        printf("e2 %c|%s\n", c, p);
        *pp = p;
        return -1;
    }
    buf[1] = *p;
    *pp = ++p;
    printf("b is %c %c\n", buf[0], buf[1]);
    return stringToByte(buf);
}

void handleubxline(const char* l, int lnum) 
{
    const char* p = l + 1;
    if (l[0] == '>') { //send
        int16_t cls = read_next_hexbyte(&p);
        if (cls < 0) {
            printf("invalid msg class line %d\n", lnum);
            return;
        }
        ubx.send_message.message_class = cls;
        cls = read_next_hexbyte(&p);
        if (cls < 0) {
            printf("invalid id line %d", lnum);
            return;
        }
        ubx.send_message.message_id = cls;
        int cnt = 0;
        while((cls = read_next_hexbyte(&p)) >= 0) {
            ubx.send_message.message_buffer[cnt++] = cls;
        }
        ubx.send_message.message_length = cnt;
        printf("sending command %x %x len %x\n", ubx.send_message.message_class, ubx.send_message.message_id, cnt);
        TEasyUBXError e0 = eubx_send_message(&ubx);
        if (e0 != EUBX_ERROR_OK) {
            printf("Error sending command %d, line %d", e0, lnum);
            return;
        }
        eubx_loop(&ubx);
    }
    else if (l[0] == '<') { //expect ack
        int16_t cls = read_next_hexbyte(&p);
        if (cls < 0) {
            printf("invalid msg class line %d\n", lnum);
            return;
        }
        int16_t mid = read_next_hexbyte(&p);
        if (cls < 0) {
            printf("invalid id line %d", lnum);
            return;
        }
        printf("awaiting ack %x %x \n", cls, mid);
        TEasyUBXError e0 = eubx_waitfor_ack(&ubx, cls, mid);
        if (e0 == EUBX_ERROR_OK) {
            printf("acked ok");
            eubx_loop(&ubx);
        }
        else {
            printf("ack error %d\n", e0);
        }
    }
    else {
        //skip
    }
}

int main(int argc, const char** argv)
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

    if (argc > 1) {
        for(int i=1; i<argc; i++) {
            printf("Arg %d: %s\n", i, argv[i]);
            processfile(argv[i], handleubxline);
        }
    }

	while(1) {
		eubx_loop(&ubx);
		sleep(10);	
		
	}
	
}
