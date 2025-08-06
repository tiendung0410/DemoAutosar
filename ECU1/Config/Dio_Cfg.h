#ifndef DIO_CFG_H
#define DIO_CFG_H

#include "Dio_Types.h"

/* === Versioning (theo AUTOSAR kiểu) === */
#define DIO_VENDOR_ID            (0u)
#define DIO_MODULE_ID            (120u)
#define DIO_SW_MAJOR_VERSION     (1u)
#define DIO_SW_MINOR_VERSION     (0u)
#define DIO_SW_PATCH_VERSION     (0u)

/* === Extern config instance (định nghĩa ở Dio_Cfg.c) === */
extern const Dio_ConfigType Dio_Config;

#endif /* DIO_CFG_H */
