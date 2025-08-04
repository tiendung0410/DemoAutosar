#ifndef SPI_H
#define SPI_H

#include "Std_Types.h"

/* ==== Types ==== */
typedef enum {
    SPI_UNINIT,
    SPI_IDLE,
    SPI_BUSY
} Spi_StatusType;

typedef enum {
    SPI_JOB_OK,
    SPI_JOB_PENDING,
    SPI_JOB_FAILED
} Spi_JobResultType;

typedef enum {
    SPI_SEQ_OK,
    SPI_SEQ_PENDING,
    SPI_SEQ_FAILED,
    SPI_SEQ_CANCELLED
} Spi_SeqResultType;

typedef uint8 Spi_ChannelType;
typedef uint8 Spi_JobType;
typedef uint8 Spi_SequenceType;
typedef uint8 Spi_HWUnitType;

typedef enum {
    SPI_POLLING_MODE,
    SPI_INTERRUPT_MODE
} Spi_AsyncModeType;

/* ==== API ==== */
void Spi_Init(const void* ConfigPtr);
void Spi_DeInit(void);

Std_ReturnType Spi_WriteIB(Spi_ChannelType Channel, const uint8* DataBufferPtr);
Std_ReturnType Spi_ReadIB(Spi_ChannelType Channel, uint8* DataBufferPtr);

Std_ReturnType Spi_SetupEB(Spi_ChannelType Channel,
                           const uint8* SrcDataBufferPtr,
                           uint8* DesDataBufferPtr,
                           uint16 Length);

Std_ReturnType Spi_AsyncTransmit(Spi_SequenceType Sequence);
Std_ReturnType Spi_SyncTransmit(Spi_SequenceType Sequence);

Spi_StatusType Spi_GetStatus(void);
Spi_JobResultType Spi_GetJobResult(Spi_JobType Job);
Spi_SeqResultType Spi_GetSequenceResult(Spi_SequenceType Sequence);

void Spi_GetVersionInfo(Std_VersionInfoType* versioninfo);
Spi_StatusType Spi_GetHWUnitStatus(Spi_HWUnitType HWUnit);

void Spi_Cancel(Spi_SequenceType Sequence);
void Spi_SetAsyncMode(Spi_AsyncModeType Mode);

void Spi_MainFunction_Handling(void);

/* ==== Callback ==== */
void Spi_JobEndNotification(Spi_JobType Job);
void Spi_SeqEndNotification(Spi_SequenceType Sequence);

#endif /* SPI_H */
