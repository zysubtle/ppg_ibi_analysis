#include "ppg_ibi.h"

#include "ppg_ibi_internal.h"

static bool ppg_ibi_config_is_valid(const ppg_ibi_config_t *config)
{
    bool valid = false;

    if (config != (const ppg_ibi_config_t *)0)
    {
        valid = (config->sample_rate_hz == (uint16_t)PPG_IBI_SAMPLE_RATE_HZ) &&
                (config->sample_period_ms == (uint16_t)PPG_IBI_SAMPLE_PERIOD_MS) &&
                (config->timestamp_tolerance_ms <= (uint16_t)PPG_IBI_SAMPLE_PERIOD_MS);
    }

    return valid;
}

static void ppg_ibi_context_reset_state(ppg_ibi_context_t *ctx)
{
    ctx->state = PPG_IBI_STATE_INIT;
    ctx->last_reject_reason = PPG_IBI_REJECT_NOT_READY;
    ctx->sample_count = 0u;
    ctx->last_timestamp_ms = 0u;
    ctx->has_last_timestamp = false;
    ctx->previous_allow_measure = false;
}

static void ppg_ibi_fill_placeholder_event(const ppg_ibi_context_t *ctx,
                                           const ppg_ibi_sample_t *sample,
                                           ppg_ibi_event_t *event)
{
    event->timestamp_ms = sample->timestamp_ms;
    event->ibi_ms = PPG_IBI_PLACEHOLDER_IBI_MS;
    event->confidence = PPG_IBI_PLACEHOLDER_CONFIDENCE;
    event->signal_quality = PPG_IBI_PLACEHOLDER_SIGNAL_QUALITY;
    event->selected_channel = (uint8_t)PPG_IBI_SELECTED_CHANNEL_NONE;
    event->state = ctx->state;
    event->reject_reason = ctx->last_reject_reason;
}

void ppg_ibi_config_default(ppg_ibi_config_t *config)
{
    if (config != (ppg_ibi_config_t *)0)
    {
        config->sample_rate_hz = (uint16_t)PPG_IBI_SAMPLE_RATE_HZ;
        config->sample_period_ms = (uint16_t)PPG_IBI_SAMPLE_PERIOD_MS;
        config->timestamp_tolerance_ms = (uint16_t)PPG_IBI_TIMESTAMP_TOLERANCE_MS;
    }
}

ppg_ibi_status_t ppg_ibi_init(ppg_ibi_context_t *ctx, const ppg_ibi_config_t *config)
{
    ppg_ibi_config_t local_config;

    if (ctx == (ppg_ibi_context_t *)0)
    {
        return PPG_IBI_STATUS_INVALID_ARGUMENT;
    }

    if (config == (const ppg_ibi_config_t *)0)
    {
        ppg_ibi_config_default(&local_config);
        config = &local_config;
    }

    if (!ppg_ibi_config_is_valid(config))
    {
        return PPG_IBI_STATUS_INVALID_CONFIG;
    }

    ctx->config = *config;
    ppg_ibi_context_reset_state(ctx);

    return PPG_IBI_STATUS_OK;
}

ppg_ibi_status_t ppg_ibi_reset(ppg_ibi_context_t *ctx)
{
    if (ctx == (ppg_ibi_context_t *)0)
    {
        return PPG_IBI_STATUS_INVALID_ARGUMENT;
    }

    ppg_ibi_context_reset_state(ctx);

    return PPG_IBI_STATUS_OK;
}

ppg_ibi_status_t ppg_ibi_process(ppg_ibi_context_t *ctx,
                                  const ppg_ibi_sample_t *sample,
                                  ppg_ibi_event_t *event,
                                  bool *event_generated)
{
    bool recovering_from_gate = false;

    if ((ctx == (ppg_ibi_context_t *)0) ||
        (sample == (const ppg_ibi_sample_t *)0) ||
        (event == (ppg_ibi_event_t *)0) ||
        (event_generated == (bool *)0))
    {
        return PPG_IBI_STATUS_INVALID_ARGUMENT;
    }

    *event_generated = false;

    if (!sample->allow_measure)
    {
        ctx->state = PPG_IBI_STATE_INVALID;
        ctx->last_reject_reason = PPG_IBI_REJECT_MEASURE_NOT_ALLOWED;
    }
    else
    {
        recovering_from_gate = (ctx->sample_count > 0u) && (!ctx->previous_allow_measure);

        if (recovering_from_gate)
        {
            ctx->state = PPG_IBI_STATE_REACQUIRE;
        }
        else if (ctx->state == PPG_IBI_STATE_INIT)
        {
            ctx->state = PPG_IBI_STATE_ACQUIRE;
        }
        else
        {
            /* M2 only wires the API. Later milestones add detection logic. */
        }

        ctx->last_reject_reason = PPG_IBI_REJECT_NOT_READY;
    }

    ctx->last_timestamp_ms = sample->timestamp_ms;
    ctx->has_last_timestamp = true;
    ctx->previous_allow_measure = sample->allow_measure;
    ctx->sample_count++;

    ppg_ibi_fill_placeholder_event(ctx, sample, event);

    return PPG_IBI_STATUS_OK;
}

uint32_t ppg_ibi_version(void)
{
    return PPG_IBI_VERSION_U32;
}
