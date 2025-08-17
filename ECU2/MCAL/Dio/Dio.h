#ifndef DIO_H
#define DIO_H

#include "Dio_Cfg.h"

#ifdef __cplusplus
extern "C" {
#endif

/* === DIO API theo AUTOSAR === */
void            Dio_Init(const Dio_ConfigType* ConfigPtr);
Dio_LevelType   Dio_ReadChannel(Dio_ChannelType ChannelId);
void            Dio_WriteChannel(Dio_ChannelType ChannelId, Dio_LevelType Level);
Dio_LevelType   Dio_FlipChannel(Dio_ChannelType ChannelId);

Dio_PortLevelType Dio_ReadPort(Dio_PortType PortId);
void              Dio_WritePort(Dio_PortType PortId, Dio_PortLevelType Level);
void              Dio_MaskedWritePort(Dio_PortType PortId, Dio_PortLevelType Level, Dio_PortLevelType Mask);

Dio_PortLevelType Dio_ReadChannelGroup(const Dio_ChannelGroupType* ChannelGroupIdPtr);
void              Dio_WriteChannelGroup(const Dio_ChannelGroupType* ChannelGroupIdPtr, Dio_PortLevelType Level);

void Dio_GetVersionInfo(uint16* major, uint16* minor, uint16* patch);

#ifdef __cplusplus
}
#endif

#endif /* DIO_H */
