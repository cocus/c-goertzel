#include "goertzel-dtmf.h"

static const uint32_t target_frequencies[DTMF_TOTAL_FREQS] = {
    697, 770, 852, 941,
    1209, 1336, 1477, 1633
};

static const char dtmf_map[DTMF_FREQS_PER_GROUP][DTMF_FREQS_PER_GROUP] = {
    { '1', '2', '3', 'A' },
    { '4', '5', '6', 'B' },
    { '7', '8', '9', 'C' },
    { '*', '0', '#', 'D' },
};

void goertzel_dtmf_init(struct goertzel_dtmf_state *dtmf,
    uint32_t sampling_frequency, size_t samples)
{
    GOERTZEL_ASSERT(dtmf && "dtmf pointer is NULL");
    GOERTZEL_ASSERT((sampling_frequency > 0) && "sampling_frequency must be greater than zero");
    GOERTZEL_ASSERT((samples > 0) && "samples must be greater than zero");

    for (size_t i = 0; i < DTMF_TOTAL_FREQS; i++)
    {
        goertzel_init(&dtmf->g[i],
            sampling_frequency,
            target_frequencies[i],
            samples);
    }
}

void goertzel_dtmf_add_sample_pcm8(struct goertzel_dtmf_state *dtmf,
    uint8_t sample)
{
    GOERTZEL_ASSERT(dtmf && "dtmf pointer is NULL");

    for (size_t i = 0; i < DTMF_TOTAL_FREQS; i++)
    {
        goertzel_process_sample_pcm8(&dtmf->g[i], sample);
    }
}

void goertzel_dtmf_add_sample_pcm16(struct goertzel_dtmf_state *dtmf,
    uint16_t sample)
{
    GOERTZEL_ASSERT(dtmf && "dtmf pointer is NULL");

    for (size_t i = 0; i < DTMF_TOTAL_FREQS; i++)
    {
        goertzel_process_sample_pcm16(&dtmf->g[i], sample);
    }
}

char goertzel_dtmf_detect(struct goertzel_dtmf_state *dtmf,
    goertzel_storage_type threshold_a, goertzel_storage_type threshold_b)
{
    GOERTZEL_ASSERT(dtmf && "dtmf pointer is NULL");

    size_t detected_grp_idx[DTMF_GROUP_COUNT] = { -1, -1 };
    goertzel_storage_type thresholds[DTMF_GROUP_COUNT];
    goertzel_storage_type max_magnitude;
    goertzel_storage_type magnitude;

    thresholds[0] = threshold_a;
    thresholds[1] = threshold_b;

    for (size_t group = 0; group < DTMF_GROUP_COUNT; group++)
    {
        max_magnitude = 0;
        for (size_t i = 0; i < DTMF_FREQS_PER_GROUP; i++)
        {
            magnitude = goertzel_get_squared_magnitude(
                &dtmf->g[(group * DTMF_FREQS_PER_GROUP) + i]);
            if ((magnitude > thresholds[group]) &&
                (magnitude > max_magnitude))
            {
                max_magnitude = magnitude;
                detected_grp_idx[group] = i;
            }
        }
    }

    if (detected_grp_idx[0] == -1 || detected_grp_idx[1] == -1)
    {
        return '\0';
    }

    return dtmf_map[detected_grp_idx[0]][detected_grp_idx[1]];
}