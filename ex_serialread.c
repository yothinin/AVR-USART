#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

int main(void){
  int serial_port = open("/dev/ttyUSB0", O_RDWR);
  if (serial_port < 0){
    printf("Error %i from open: %s\n", errno, strerror(errno));
    return 1;
  }

  struct termios tty;
  if (tcgetattr(serial_port, &tty) != 0)
    printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
  else
    printf("Connected...\n");

  tty.c_cflag &= ~PARENB;
  tty.c_cflag &= ~CSTOPB;
  tty.c_cflag |= CS8;
  tty.c_cflag &= ~CRTSCTS;
  
  //cfsetispeed(&tty, B1200);
  //cfsetospeed(&tty, B1200);
  cfsetspeed(&tty, B1200);
  
  // Save tty settings, also checking for error
  if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
    printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
  }
  
  char read_buf[256];
  int i = 0, stop = 0;
  
  while (stop == 0){
    int n = read(serial_port, &read_buf, sizeof(read_buf));
    if (read_buf[0] == '!' && 
        read_buf[1] == 'q' && 
        read_buf[2] == '!')
      stop = 1;
    else
      if (n != 0)
          printf("%s", read_buf);
    
    ++i;
  }
  printf("Quit by user!!!\nWait to close serial port.\n");
  close(serial_port);

  return 0;
}
