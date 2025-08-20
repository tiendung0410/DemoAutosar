#ifndef STD_TYPES_H_
#define STD_TYPES_H_
#include "stdio.h"
/* AUTOSAR version and vendor (optional) */
#define STD_TYPES_VENDOR_ID      (0xFFFFu)
#define STD_TYPES_MODULE_ID      (197u)
#define STD_TYPES_SW_MAJOR_VERSION   (1u)
#define STD_TYPES_SW_MINOR_VERSION   (0u)
#define STD_TYPES_SW_PATCH_VERSION   (0u)

/* Standard AUTOSAR types */
typedef unsigned char           uint8;
typedef signed char             sint8;
typedef unsigned short          uint16;
typedef signed short            sint16;
typedef unsigned long           uint32;
typedef signed long             sint32;
typedef unsigned long long      uint64;
typedef signed long long        sint64;

typedef float                   float32;
typedef double                  float64;

/* Boolean */
#ifndef FALSE
#define FALSE                   (0u)
#endif

#ifndef TRUE
#define TRUE                    (1u)
#endif

typedef uint8                   boolean;

/* Std_ReturnType */
typedef uint8                   Std_ReturnType;

#ifndef E_OK
#define E_OK                    (0u)
#endif

#ifndef E_NOT_OK
#define E_NOT_OK                (1u)
#endif

/* Version Info Type */
typedef struct {
    uint16 vendorID;
    uint16 moduleID;
    uint8  sw_major_version;
    uint8  sw_minor_version;
    uint8  sw_patch_version;
} Std_VersionInfoType;

#endif /* STD_TYPES_H_ */
