#include <termios.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "sensor_data.h"

#define BUFFERSIZE 255

int main(int argc, char const* argv[]) {
  struct termios tio;
  int tty_fd;
  fd_set rdset;
  unsigned char c;
  unsigned char k[BUFFERSIZE];
  int i = 0;
  float value[3], val;
  unsigned char data[18];

  // struct sensor_data data;

  const char *device = "/dev/tty.RNBT-DBFD-RNI-SPP";

  memset(&tio, 0, sizeof(tio));
  tio.c_iflag = 0;
  tio.c_oflag = 0;
  tio.c_cflag = CS8|CREAD|CLOCAL;
  tio.c_lflag = 0;
  tio.c_cc[VMIN] = 1;
  tio.c_cc[VTIME] = 1;


  tty_fd = open(device, O_RDWR | O_NDELAY);
  if (tty_fd == -1) {
    printf("Failed to open port %s\n", device);
    close(tty_fd);
    return 1;
  }

  cfsetospeed(&tio, B115200);
  cfsetispeed(&tio, B115200);

  tcsetattr(tty_fd, TCSANOW, &tio);

  while(i < BUFFERSIZE) {
    if(read(tty_fd, &c, 1) > 0) {
      k[i] = c;
      i++;
    }
  }

  for (i = 0; i < BUFFERSIZE; i++) {
    printf("%02X ", k[i]);
    if ((i < BUFFERSIZE - 1) && k[i + 1] == 0x55 && k[i + 2] == 0x55)
      printf("\n");
  }

  printf("\n\n");

  for (i = 0; i < 18; i++)
    data[i] = k[i+2];

  printf("Gyroscope:\n");
  for (i = 0; i < 3; i++) {
    value[i] = (float) ((int)(((unsigned int) data[2*i+1] << 8) + data[2*i])) / L3G_Sensitivity_500dps;
    printf("%f dps\n", value[i]);
  }

  printf("Compass:\n");
  for (i = 3; i < 5; i++) {
    val = (float)((int)(((unsigned int)data[2*i] << 8) + data[2*i+1])*1000)/ LSM303DLHC_M_SENSITIVITY_XY_8_1Ga;
    printf("%f Ga\n", val);
  }
  val = (float)((int)(((unsigned int)data[10] << 8) + data[11])*1000)/ LSM303DLHC_M_SENSITIVITY_Z_8_1Ga;
  printf("%f Ga\n", val);

  printf("Accelerometer:\n");
  for (i = 6; i < 9; i++) {
    int raw = (int)((((unsigned int)data[2*i + 1] << 8) + data[2*i]) >> 4);
    val = (float)raw/LSM_Acc_Sensitivity_2g;
    printf("%f mg\n", val);
  }

  printf("\n");

  printf("Closing connection to %s\n", device);
  close(tty_fd);
  return 0;
}
