#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include "MRF89XA.h"

static const char* config = "/dev/spidev0.0";
static const char* data = "/dev/spidev0.1";
uint8_t bits = 8;
uint32_t speed = 5e5;
uint16_t delay = 5;

uint8_t rf_mode;

int fd_config;
int fd_data;

int open_device(const char* device) {
  uint32_t mode = 0;
  int ret = 0;
  int fd = open(device, O_RDWR);
  if (fd < 0) 
    fprintf(stderr, "Error: cannot open %s\n", device);

  // SPI mode
  ret = ioctl(fd, SPI_IOC_WR_MODE32, &mode);
  if (ret == -1)
    fprintf(stderr, "Error: cannot set spi mode\n");
  ret = ioctl(fd, SPI_IOC_RD_MODE32, &mode);
  if (ret == -1)
    fprintf(stderr, "Error: cannot set spi mode\n");
  // Bits per word
  ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
  if (ret == -1)
    fprintf(stderr, "Error: cannot set spi bits\n");
  ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
  if (ret == -1)
    fprintf(stderr, "Error: cannot set spi bits\n");
  // Speed
  ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
  if (ret == -1)
    fprintf(stderr, "Error: cannot set spi speed\n");
  ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
  if (ret == -1)
    fprintf(stderr, "Error: cannot set spi speed\n");

  return fd;
}


static void transfer(int fd, uint8_t const *tx, uint8_t const *rx, size_t len) {
  int ret;
  struct spi_ioc_transfer tr = {
    .tx_buf = (unsigned long)tx,
    .rx_buf = (unsigned long)rx,
    .len = len,
    .delay_usecs = delay,
    .speed_hz = speed,
    .bits_per_word = bits,
  };

  ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
  if (ret < 1)
    fprintf(stderr, "Error: cannot send spi message\n");
}

void remote_config(uint8_t address, uint8_t data) {
  char tx_buffer[2] = {0};
  char rx_buffer[2] = {0};
  tx_buffer[0] = (address << 1);
  tx_buffer[1] = data;
  transfer(fd_config, tx_buffer, rx_buffer, 2);
  /* printf("Writing: %02X %02X : %02X\n", tx_buffer[0], tx_buffer[1], rx_buffer[1]); */
}

void remote_read(uint8_t address) {
  char tx_buffer[2] = {0};
  char rx_buffer[2] = {0};
  int i = 0;
  tx_buffer[0] = (address << 1) | 0x40;

  transfer(fd_config, tx_buffer, rx_buffer, 2);
  printf("Reading: %02X : %02X\n", tx_buffer[0], rx_buffer[1]);
}

int main(int argc, char** argv) {
  // Configure SPIDEV 0 and 1
  // 0: Configuration
  // 1: Data
  int i = 0;
  fd_config = open_device(config);
  fd_data = open_device(data);

  printf("Configuring device with default settings...\n");
  remote_setup();

  return 0;
}
