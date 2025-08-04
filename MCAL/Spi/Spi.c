#include "Spi.h"
#include "Spi_Cfg.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <string.h>
#include <stdio.h>

/* ==== Static State ==== */
static Spi_StatusType Spi_GlobalStatus = SPI_UNINIT;
static Spi_AsyncModeType Spi_CurrentMode = SPI_POLLING_MODE;

static uint8 IBuffers[2][64]; /* Internal buffers cho channels */
static uint8 IBuffLen[2];

static Spi_JobResultType Spi_JobResults[2];
static Spi_SeqResultType Spi_SeqResults[2];

static int spi_fd = -1;

/* ==== Low-level SPI Transfer ==== */
static int Spi_LowLevelTransfer(uint8* tx, uint8* rx, size_t len) {
    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)tx,
        .rx_buf = (unsigned long)rx,
        .len = len,
        .speed_hz = SPI_SPEED_HZ,
        .bits_per_word = 8,
        .delay_usecs = 0,
    };
    return ioctl(spi_fd, SPI_IOC_MESSAGE(1), &tr);
}

/* ==== MCP2515 Helpers ==== */
static void mcp2515_read_rx_buffer(uint8* rxBuf, size_t len) {
    uint8 tx[65] = { MCP2515_CMD_READ_RX_BUFFER };
    uint8 rx[65] = {0};
    Spi_LowLevelTransfer(tx, rx, len + 1);
    memcpy(rxBuf, &rx[1], len);
}

static void mcp2515_load_tx_buffer(const uint8* data, size_t len) {
    uint8 tx[65] = { MCP2515_CMD_LOAD_TX_BUFFER };
    memcpy(&tx[1], data, len);
    uint8 rx[65] = {0};
    Spi_LowLevelTransfer(tx, rx, len + 1);
}

static void mcp2515_rts(void) {
    uint8 tx = MCP2515_CMD_RTS;
    uint8 rx = 0;
    Spi_LowLevelTransfer(&tx, &rx, 1);
}

/* ==== API Implementations ==== */
void Spi_Init(const void* ConfigPtr) {
    (void)ConfigPtr;
    spi_fd = open(SPI_DEVICE_PATH, O_RDWR);
    if (spi_fd < 0) {
        perror("Spi_Init: Cannot open device");
        return;
    }
    uint8 mode = SPI_MODE_0;
    ioctl(spi_fd, SPI_IOC_WR_MODE, &mode);
    uint32 speed = SPI_SPEED_HZ;
    ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    Spi_GlobalStatus = SPI_IDLE;
    Spi_JobResults[0] = Spi_JobResults[1] = SPI_JOB_OK;
    Spi_SeqResults[0] = Spi_SeqResults[1] = SPI_SEQ_OK;
}

void Spi_DeInit(void) {
    if (spi_fd >= 0) close(spi_fd);
    spi_fd = -1;
    Spi_GlobalStatus = SPI_UNINIT;
}

Std_ReturnType Spi_WriteIB(Spi_ChannelType Channel, const uint8* DataBufferPtr) {
    memcpy(IBuffers[Channel], DataBufferPtr, 64);
    IBuffLen[Channel] = strlen((const char*)DataBufferPtr);
    return E_OK;
}

Std_ReturnType Spi_ReadIB(Spi_ChannelType Channel, uint8* DataBufferPtr) {
    memcpy(DataBufferPtr, IBuffers[Channel], IBuffLen[Channel]);
    return E_OK;
}

Std_ReturnType Spi_SetupEB(Spi_ChannelType Channel,
                           const uint8* SrcDataBufferPtr,
                           uint8* DesDataBufferPtr,
                           uint16 Length) {
    memcpy(IBuffers[Channel], SrcDataBufferPtr, Length);
    IBuffLen[Channel] = Length;
    return E_OK;
}

Std_ReturnType Spi_SyncTransmit(Spi_SequenceType Sequence) {
    if (Spi_GlobalStatus == SPI_BUSY) return E_NOT_OK;
    Spi_GlobalStatus = SPI_BUSY;
    Spi_SeqResults[Sequence] = SPI_SEQ_PENDING;
    /* Thực thi ngay tại chỗ */
    if (Sequence == SPI_SEQ_READ_MCP2515) {
        uint8 rxdata[14] = {0};
        mcp2515_read_rx_buffer(rxdata, sizeof(rxdata));
        memcpy(IBuffers[SPI_CH_MCP2515_DATA], rxdata, sizeof(rxdata));
    } else if (Sequence == SPI_SEQ_WRITE_MCP2515) {
        mcp2515_load_tx_buffer(IBuffers[SPI_CH_MCP2515_DATA],
                               IBuffLen[SPI_CH_MCP2515_DATA]);
        mcp2515_rts();
    }
    Spi_JobEndNotification(Sequence);
    Spi_SeqEndNotification(Sequence);
    Spi_GlobalStatus = SPI_IDLE;
    return E_OK;
}

Std_ReturnType Spi_SyncTransmit(Spi_SequenceType Sequence) {
    if (Spi_GlobalStatus == SPI_BUSY) return E_NOT_OK;
    Spi_GlobalStatus = SPI_BUSY;
    Spi_SeqResults[Sequence] = SPI_SEQ_PENDING;
    /* Thực thi ngay tại chỗ */
    if (Sequence == SPI_SEQ_READ_MCP2515) {
        uint8 rxdata[14] = {0};
        mcp2515_read_rx_buffer(rxdata, sizeof(rxdata));
        memcpy(IBuffers[SPI_CH_MCP2515_DATA], rxdata, sizeof(rxdata));
    } else if (Sequence == SPI_SEQ_WRITE_MCP2515) {
        mcp2515_load_tx_buffer(IBuffers[SPI_CH_MCP2515_DATA],
                               IBuffLen[SPI_CH_MCP2515_DATA]);
        mcp2515_rts();
    }
    Spi_JobEndNotification(Sequence);
    Spi_SeqEndNotification(Sequence);
    Spi_GlobalStatus = SPI_IDLE;
    return E_OK;
}

Spi_StatusType Spi_GetStatus(void) { return Spi_GlobalStatus; }
Spi_JobResultType Spi_GetJobResult(Spi_JobType Job) { return Spi_JobResults[Job]; }
Spi_SeqResultType Spi_GetSequenceResult(Spi_SequenceType Sequence) { return Spi_SeqResults[Sequence]; }

void Spi_GetVersionInfo(Std_VersionInfoType* versioninfo) {
    versioninfo->vendorID = 0;
    versioninfo->moduleID = 123;
    versioninfo->sw_major_version = 1;
    versioninfo->sw_minor_version = 0;
    versioninfo->sw_patch_version = 0;
}

Spi_StatusType Spi_GetHWUnitStatus(Spi_HWUnitType HWUnit) { return Spi_GlobalStatus; }
void Spi_Cancel(Spi_SequenceType Sequence) { Spi_SeqResults[Sequence] = SPI_SEQ_CANCELLED; Spi_GlobalStatus = SPI_IDLE; }
void Spi_SetAsyncMode(Spi_AsyncModeType Mode) { Spi_CurrentMode = Mode; }

void Spi_MainFunction_Handling(void) {
    if (Spi_GlobalStatus == SPI_BUSY) {
        /* Xử lý sequence đã queue ở đây */
        if (queuedSequence == SPI_SEQ_READ_MCP2515) {
            uint8 rxdata[14] = {0};
            mcp2515_read_rx_buffer(rxdata, sizeof(rxdata));
            memcpy(IBuffers[SPI_CH_MCP2515_DATA], rxdata, sizeof(rxdata));
        } else if (queuedSequence == SPI_SEQ_WRITE_MCP2515) {
            mcp2515_load_tx_buffer(IBuffers[SPI_CH_MCP2515_DATA],
                                   IBuffLen[SPI_CH_MCP2515_DATA]);
            mcp2515_rts();
        }
        Spi_JobEndNotification(queuedSequence);
        Spi_SeqEndNotification(queuedSequence);
        Spi_GlobalStatus = SPI_IDLE;
    }
}

/* ==== Callback ==== */
void Spi_JobEndNotification(Spi_JobType Job) {
    Spi_JobResults[Job] = SPI_JOB_OK;
    /* Notify CAN Driver ở đây nếu cần */
}

void Spi_SeqEndNotification(Spi_SequenceType Sequence) {
    Spi_SeqResults[Sequence] = SPI_SEQ_OK;
    /* Notify CAN Driver ở đây nếu cần */
}
