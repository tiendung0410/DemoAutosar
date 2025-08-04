#ifndef SPI_CFG_H
#define SPI_CFG_H

/* MCP2515 SPI Commands */
#define MCP2515_CMD_READ_RX_BUFFER  0x90
#define MCP2515_CMD_LOAD_TX_BUFFER  0x40
#define MCP2515_CMD_RTS             0x80
#define MCP2515_CMD_READ_STATUS     0xA0

/* Channels */
#define SPI_CH_MCP2515_CMD   0
#define SPI_CH_MCP2515_DATA  1

/* Jobs */
#define SPI_JOB_READ_MCP2515  0
#define SPI_JOB_WRITE_MCP2515 1

/* Sequences */
#define SPI_SEQ_READ_MCP2515  0
#define SPI_SEQ_WRITE_MCP2515 1

/* SPI device path & speed */
#define SPI_DEVICE_PATH   "/dev/spidev0.0"
#define SPI_SPEED_HZ      8000000

#endif /* SPI_CFG_H */
