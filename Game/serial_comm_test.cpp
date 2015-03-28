#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */


#define USB_SERIAL_PORT "/dev/cu.usbmodem1431" // (OSX MacbookPro)
//#define USB_SERIAL_PORT  "/dev/cu.usbserial-3B1" // (OSX Mac Book)
//#define USB_SERIAL_PORT "/dev/tty.usbserial-0000103D" // (Linux Ubuntu Mac Book)
//#define USB_SERIAL_PORT "/dev/tty.usbserial-0000103D"

/*
 * 'open_port()' - Open serial port 1.
 *
 * Returns the file descriptor on success or -1 on error.
 */

int
open_port(void)
{
  int fd; /* File descriptor for the port */

  fd = open("/dev/ttyf1", O_RDWR | O_NOCTTY | O_NDELAY);
  if (fd == -1)
  {
   /*
    * Could not open the port.
    */

    perror("open_port: Unable to open /dev/ttyf1 - ");
  }
  else
    fcntl(fd, F_SETFL, 0);

  return (fd);
}
