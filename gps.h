
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <queue.h>

#include "uart1.h"
#include "i2c.h"

#include "ogn.h"

#include "lowpass2.h"

extern          uint32_t GPS_UnixTime;      // [sec] UTC time in Unix format
extern          uint32_t GPS_FatTime;       // [2 sec] UTC time in FAT format (for FatFS)
extern volatile  uint8_t GPS_Sec;           // [sec]
extern           int32_t GPS_Altitude;      // [0.1m] altitude (height above Geoid)
extern           int32_t GPS_Latitude;      // [0.0001/60 deg]
extern           int32_t GPS_Longitude;     // [0.0001/60 deg]
extern           int16_t GPS_GeoidSepar;    // [0.1m]
extern          uint16_t GPS_LatCosine;     // [1.0/(1<<12)] Latitude Cosine for distance calculations
extern          uint32_t GPS_TimeSinceLock; // [sec] time since GPS has a valid lock
// extern           uint8_t GPS_FreqPlan;      // [] frequency (hopping) plan: 1 = Europe/Africa, 2 = USA/Canada, 3 = Australia/South America

extern TickType_t PPS_TickCount;            // [ms] time since the most recent rising edge of PPS pulse
uint16_t PPS_Phase(void);                   // [ms]

#ifdef WITH_PPS_IRQ
extern TickType_t PPS_IRQ_TickCount;
extern uint32_t PPS_IRQ_TickTime;
extern  int32_t PPS_IRQ_TickTimeDiff;
extern uint32_t PPS_IRQ_Period;
extern LowPass2<int32_t,6,4,8> PPS_IRQ_AverPeriod;
#endif

GPS_Position *GPS_getPosition(void);
GPS_Position *GPS_getPosition(int8_t Sec);

// extern xQueueHandle xQueuePacket;

void GPS_Configuration (void);

#ifdef __cplusplus
  extern "C"
#endif
void vTaskGPS(void* pvParameters);

