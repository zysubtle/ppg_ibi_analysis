#ifndef PPG_IBI_H
#define PPG_IBI_H

#include <stdbool.h>
#include <stdint.h>

#include "ppg_ibi_config.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PPG_IBI_VERSION_MAJOR (0u)
#define PPG_IBI_VERSION_MINOR (2u)
#define PPG_IBI_VERSION_PATCH (0u)
#define PPG_IBI_VERSION_U32 \
    (((uint32_t)PPG_IBI_VERSION_MAJOR << 16) | \
     ((uint32_t)PPG_IBI_VERSION_MINOR << 8) | \
     ((uint32_t)PPG_IBI_VERSION_PATCH))

typedef enum
{
    PPG_IBI_STATUS_OK = 0,
    PPG_IBI_STATUS_INVALID_ARGUMENT = -1,
    PPG_IBI_STATUS_INVALID_CONFIG = -2
} ppg_ibi_status_t;

typedef enum
{
    PPG_IBI_STATE_INIT = 0,
    PPG_IBI_STATE_ACQUIRE,
    PPG_IBI_STATE_TRACK,
    PPG_IBI_STATE_REACQUIRE,
    PPG_IBI_STATE_INVALID
} ppg_ibi_state_t;

typedef enum
{
    PPG_IBI_REJECT_NONE = 0,
    PPG_IBI_REJECT_NOT_READY,
    PPG_IBI_REJECT_MEASURE_NOT_ALLOWED,
    PPG_IBI_REJECT_LOW_SIGNAL_QUALITY,
    PPG_IBI_REJECT_TIMESTAMP_ERROR,
    PPG_IBI_REJECT_IBI_OUT_OF_RANGE,
    PPG_IBI_REJECT_CHANNEL_INVALID,
    PPG_IBI_REJECT_STATE_INVALID
} ppg_ibi_reject_reason_t;

typedef struct
{
    uint16_t sample_rate_hz;
    uint16_t sample_period_ms;
    uint16_t timestamp_tolerance_ms;
} ppg_ibi_config_t;

typedef struct
{
    uint32_t timestamp_ms;
    int32_t ppg[PPG_IBI_CHANNEL_COUNT];
    bool allow_measure;
} ppg_ibi_sample_t;

typedef struct
{
    uint32_t timestamp_ms;
    uint32_t ibi_ms;
    float confidence;
    float signal_quality;
    uint8_t selected_channel;
    ppg_ibi_state_t state;
    ppg_ibi_reject_reason_t reject_reason;
} ppg_ibi_event_t;

typedef struct
{
    ppg_ibi_config_t config;
    ppg_ibi_state_t state;
    ppg_ibi_reject_reason_t last_reject_reason;
    uint32_t sample_count;
    uint32_t last_timestamp_ms;
    bool has_last_timestamp;
    bool previous_allow_measure;
} ppg_ibi_context_t;

void ppg_ibi_config_default(ppg_ibi_config_t *config);
ppg_ibi_status_t ppg_ibi_init(ppg_ibi_context_t *ctx, const ppg_ibi_config_t *config);
ppg_ibi_status_t ppg_ibi_reset(ppg_ibi_context_t *ctx);
ppg_ibi_status_t ppg_ibi_process(ppg_ibi_context_t *ctx,
                                  const ppg_ibi_sample_t *sample,
                                  ppg_ibi_event_t *event,
                                  bool *event_generated);
uint32_t ppg_ibi_version(void);

#ifdef __cplusplus
}
#endif

#endif /* PPG_IBI_H */
