#ifndef __USB_PHY_H__
#define __USB_PHY_H__

#include "ChibiOS.h"
#define PHY_READ_QUEUE_SIZE 16
#define PHY_READ_QUEUE_MASK (PHY_READ_QUEUE_SIZE - 1)

#ifdef __cplusplus
extern "C" {
#endif

struct USBMAC;

struct USBPHY {

  struct USBMAC *mac;     /* Pointer to the associated MAC */
  int initialized;

  /* USB D+ pin specification */
  uint32_t usbdpIAddr;
  uint32_t usbdpSAddr;
  uint32_t usbdpCAddr;
  uint32_t usbdpDAddr;
  uint32_t usbdpShift;

  /* USB D- pin specification */
  uint32_t usbdnIAddr;
  uint32_t usbdnSAddr;
  uint32_t usbdnCAddr;
  uint32_t usbdnDAddr;
  uint32_t usbdnShift;

  uint32_t usbdpMask;
  uint32_t usbdnMask;

  uint32_t epnum;

#if (CH_USE_RT == TRUE)
  thread_reference_t thread;
  THD_WORKING_AREA(waThread, 128);
#endif

  uint8_t read_queue_head;
  uint8_t read_queue_tail;
  uint16_t padding;

  /* pkt_size is cached in read_queue[x][11] */
  uint8_t read_queue[PHY_READ_QUEUE_SIZE][12];
} __attribute__((packed, aligned(4)));

int usbPhyResetStatistics(struct USBPHY *phy);

void usbPhyInit(struct USBPHY *phy, struct USBMAC *mac);
int usbPhyReadI(const struct USBPHY *phy, uint8_t samples[11]);
void usbPhyWriteI(const struct USBPHY *phy, const void *buffer, uint32_t count);
void usbPhyWriteTestPattern(const struct USBPHY *phy);
void usbPhyWriteTest(struct USBPHY *phy);
int usbProcessIncoming(struct USBPHY *phy);
int usbPhyQueue(struct USBPHY *phy, const uint8_t *buffer, int buffer_size);
int usbCapture(struct USBPHY *phy);
int usbPhyInitialized(struct USBPHY *phy);
int usbPhyWritePrepare(struct USBPHY *phy, int epnum, const void *buffer, int size);

void usbPhyAttach(struct USBPHY *phy);
void usbPhyDetach(struct USBPHY *phy);

struct USBPHY *usbPhyDefaultPhy(void);
struct USBPHY *usbPhyTestPhy(void);

#ifdef __cplusplus
};
#endif

#endif /* __USB_PHY_H__ */
