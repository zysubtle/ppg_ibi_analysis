#include "ppg_ibi.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

static void test_header_constants(void)
{
    assert(PPG_IBI_CHANNEL_COUNT == 4u);
    assert(PPG_IBI_SAMPLE_RATE_HZ == 50u);
    assert(PPG_IBI_SAMPLE_PERIOD_MS == 20u);
    assert(PPG_IBI_ADC_BITS == 24u);
    assert(PPG_IBI_RAM_BUDGET_BYTES == 20480u);
    assert(ppg_ibi_version() == PPG_IBI_VERSION_U32);
}

static void test_fixture_path_exists(void)
{
    FILE *fixture = fopen("tests/fixtures/sample_ppg.csv", "r");

    assert(fixture != (FILE *)0);
    (void)fclose(fixture);
}

static void test_api_compile_and_link(void)
{
    ppg_ibi_config_t config;
    ppg_ibi_context_t ctx;
    ppg_ibi_sample_t sample = {
        0u,
        {3338182, 3708818, 3536433, 3134694},
        true
    };
    ppg_ibi_event_t event;
    bool event_generated = true;

    ppg_ibi_config_default(&config);

    assert(config.sample_rate_hz == 50u);
    assert(config.sample_period_ms == 20u);
    assert(config.timestamp_tolerance_ms == PPG_IBI_TIMESTAMP_TOLERANCE_MS);
    assert(sizeof(ppg_ibi_context_t) < PPG_IBI_RAM_BUDGET_BYTES);
    assert(ppg_ibi_init(&ctx, &config) == PPG_IBI_STATUS_OK);

    assert(ppg_ibi_process(&ctx, &sample, &event, &event_generated) == PPG_IBI_STATUS_OK);
    assert(event_generated == false);
    assert(event.timestamp_ms == sample.timestamp_ms);
    assert(event.ibi_ms == 0u);
    assert(event.confidence == 0.0f);
    assert(event.signal_quality == 0.0f);
    assert(event.selected_channel == PPG_IBI_SELECTED_CHANNEL_NONE);
    assert(event.state == PPG_IBI_STATE_ACQUIRE);
    assert(event.reject_reason == PPG_IBI_REJECT_NOT_READY);

    sample.timestamp_ms = 20u;
    sample.allow_measure = false;
    assert(ppg_ibi_process(&ctx, &sample, &event, &event_generated) == PPG_IBI_STATUS_OK);
    assert(event_generated == false);
    assert(event.state == PPG_IBI_STATE_INVALID);
    assert(event.reject_reason == PPG_IBI_REJECT_MEASURE_NOT_ALLOWED);

    sample.timestamp_ms = 40u;
    sample.allow_measure = true;
    assert(ppg_ibi_process(&ctx, &sample, &event, &event_generated) == PPG_IBI_STATUS_OK);
    assert(event_generated == false);
    assert(event.state == PPG_IBI_STATE_REACQUIRE);
    assert(event.reject_reason == PPG_IBI_REJECT_NOT_READY);

    assert(ppg_ibi_reset(&ctx) == PPG_IBI_STATUS_OK);
    assert(ctx.state == PPG_IBI_STATE_INIT);
}

int main(void)
{
    test_header_constants();
    test_fixture_path_exists();
    test_api_compile_and_link();

    return 0;
}
