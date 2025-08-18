#include "Can.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>


static Can_ControllerStateType Can_ControllerState[CAN_MAX_CONTROLLER] = { CAN_CS_UNINIT };
static int canSocket[CAN_MAX_CONTROLLER] = { -1 };
static struct sockaddr_can addr[CAN_MAX_CONTROLLER];
static struct ifreq ifr[CAN_MAX_CONTROLLER];
static const Can_ConfigType* Can_ConfigPtr = NULL;
extern void CanIf_RxIndication(uint8 Hrh, uint32 CanId, uint8 CanDlc, const uint8 *CanSduPtr);


void Can_Init(const Can_ConfigType* Config)
{
    Can_ConfigPtr = Config;
    for (uint8 i = 0; i < Config->NumControllers; i++) {
        const Can_ControllerConfigType* cfg = &Config->ControllerConfigs[i];

        /* Create CAN RAW socket */
        canSocket[i] = socket(PF_CAN, SOCK_RAW, CAN_RAW);
        if (canSocket[i] < 0) {
            perror("CAN socket");
            Can_ControllerState[i] = CAN_CS_UNINIT;
            continue;
        }

        /* Get interface index */
        strncpy(ifr[i].ifr_name, cfg->ifName, IFNAMSIZ - 1);
        if (ioctl(canSocket[i], SIOCGIFINDEX, &ifr[i]) < 0) {
            perror("SIOCGIFINDEX");
            close(canSocket[i]);
            canSocket[i] = -1;
            Can_ControllerState[i] = CAN_CS_UNINIT;
            continue;
        }

        /* Bind socket to CAN interface */
        addr[i].can_family = AF_CAN;
        addr[i].can_ifindex = ifr[i].ifr_ifindex;
        if (bind(canSocket[i], (struct sockaddr *)&addr[i], sizeof(addr[i])) < 0) {
            perror("CAN bind");
            close(canSocket[i]);
            canSocket[i] = -1;
            Can_ControllerState[i] = CAN_CS_UNINIT;
            continue;
        }
        Can_ControllerState[i] = CAN_CS_STOPPED;
    }
}

void Can_DeInit(void)
{
    if (Can_ConfigPtr == NULL) return;
    for (uint8 i = 0; i < Can_ConfigPtr->NumControllers; i++) {
        if (canSocket[i] >= 0) {
            close(canSocket[i]);
            canSocket[i] = -1;
        }
        Can_ControllerState[i] = CAN_CS_UNINIT;
    }
    Can_ConfigPtr = NULL;
}

Std_ReturnType Can_SetControllerMode(uint8 Controller, Can_ControllerStateType Transition)
{
    if (Can_ConfigPtr == NULL || Controller >= Can_ConfigPtr->NumControllers) return E_NOT_OK;

    /* Đơn giản chỉ update trạng thái, thực tế có thể dùng ioctl để up/down interface nếu cần */
    Can_ControllerState[Controller] = Transition;
    return E_OK;
}

Std_ReturnType Can_Write(Can_HwHandleType Hth, const Can_PduType* PduInfo)
{
    if (Can_ConfigPtr == NULL || Hth >= CAN_MAX_CONTROLLER) return CAN_NOT_OK;
    if (canSocket[Hth] < 0 || PduInfo == NULL) return CAN_NOT_OK;

    struct can_frame frame;
    memset(&frame, 0, sizeof(frame));
    frame.can_id = PduInfo->id;
    frame.can_dlc = PduInfo->length;
    memcpy(frame.data, PduInfo->sdu, PduInfo->length);

    int nbytes = write(canSocket[Hth], &frame, sizeof(frame));
    if (nbytes != sizeof(frame)) {
        perror("CAN Write");
        return CAN_NOT_OK;
    }
    return CAN_OK;
}

void Can_MainFunction_Write(void)
{
    /* Không cần xử lý đặc biệt cho SocketCAN, nếu cần confirm thì add queue/callback */
}

void Can_MainFunction_Read(void)
{
    struct can_frame frame;
    for (uint8 i = 0; i < CAN_MAX_CONTROLLER; i++) {
        if (canSocket[i] < 0) continue;
        int nbytes = read(canSocket[i], &frame, sizeof(frame));
        if (nbytes > 0) {
            // Gọi callback lên CanIf (hoặc nếu chưa có, tạm thời print ra)
            CanIf_RxIndication(i, frame.can_id, frame.can_dlc, frame.data);
        }
    }
}


void Can_MainFunction_BusOff(void)
{
    /* Có thể kiểm tra trạng thái bus nếu cần bằng netlink hoặc tools khác */
}

void Can_MainFunction_Wakeup(void)
{
    /* Không dùng cho Linux CAN */
}

void Can_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
    if (versioninfo == NULL) return;
    versioninfo->vendorID = 0;
    versioninfo->moduleID = 0x80;
    versioninfo->sw_major_version = CAN_SW_MAJOR_VERSION;
    versioninfo->sw_minor_version = CAN_SW_MINOR_VERSION;
    versioninfo->sw_patch_version = CAN_SW_PATCH_VERSION;
}
