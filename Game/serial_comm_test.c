// #include <stdio.h>   /* Standard input/output definitions */
// #include <string.h>  /* String function definitions */
// #include <unistd.h>  /* UNIX standard function definitions */
// #include <fcntl.h>   /* File control definitions */
// #include <errno.h>   /* Error number definitions */
// #include <termios.h> /* POSIX terminal control definitions */
// #include <sys/types.h>
//
//
// #define USB_SERIAL_PORT "/dev/cu.usbmodem1431" // (OSX MacbookPro)
//
// int open_port(void)
// {
//   int fd; /* File descriptor for the port */
//
//   fd = open("/dev/tty.usbmodemfd121", O_RDWR | O_NOCTTY | O_NDELAY);
//   if (fd == -1)
//   {
//     perror("open_port: Unable to open /dev/tty");
//   }
//   else
//     fcntl(fd, F_SETFL, 0);
//
//   struct termios options;
//   tcgetattr(fd,&options);
//   cfsetospeed(&options,B9600);
//   options.c_cflag |=(CLOCAL | CREAD);
//   tcsetattr(fd, TCSANOW, &options);
//
//   return (fd);
// }
//
//
// int main (){
//
//     fd = open_port();
//
//     if (fd < 0) {
//       /* handle error condition */
//     }
//
//     rc = fcntl(fd, F_SETFL, FNDELAY);
//
//     if (rc < 0) {
//       /* handle error condition */
//     }
//
//     for (i=0; i<50; i++) {
//
//      bytes_read = read(fd, buf, nbytes);
//      if (bytes_read < 0) {
//           /* handle error condition */
//       }
//
//     }
//     close(fd);
//
// }

#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <sys/types.h>


int open_port(void)
{
  int fd; /* File descriptor for the port */

  fd = open("/dev/tty.usbmodem1431", O_RDWR | O_NOCTTY | O_NDELAY);
  if (fd == -1)
  {
    perror("open_port: Unable to open /dev/tty");
  }
  else
    fcntl(fd, F_SETFL, 0);

  struct termios options;
  tcgetattr(fd,&options);
  cfsetospeed(&options,B9600);
  options.c_cflag |=(CLOCAL | CREAD);
  tcsetattr(fd, TCSANOW, &options);

  return (fd);
}


int main (){

    int i;
    for(i=0; i<50; i++){

    fcntl(open_port(), F_SETFL, FNDELAY);
    char buf[5];
    size_t nbytes;
    ssize_t bytes_read;

    nbytes = sizeof(buf);
    bytes_read = read(open_port(), buf, nbytes);
    printf("%s ", buf);
    buf[0]=0;
    }

    return 0;

}
