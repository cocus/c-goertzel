
#include "goertzel.h"

#define _USE_MATH_DEFINES
#include <math.h>

void goertzel_reset(struct goertzel_state *g)
{
    GOERTZEL_ASSERT(g && "g pointer is NULL");

    g->Q1 = 0;
    g->Q2 = 0;
}

void goertzel_init(struct goertzel_state *g, uint32_t sampling_frequency,
    uint32_t target_frequency, size_t samples)
{
    GOERTZEL_ASSERT(g && "g pointer is NULL");
    GOERTZEL_ASSERT((sampling_frequency > 0) && "sampling_frequency must be greater than zero");
    GOERTZEL_ASSERT((samples > 0) && "samples must be greater than zero");

    /**
     * k = (int) [ 0.5 + ( N*target_freq/sample_rate ) ]
     * omega = (2*PI/samples) * k
     * coeff = 2*cos(omega)
     */
    goertzel_storage_type k = samples;
    k *= target_frequency;
    k /= sampling_frequency;
    k = floor(0.5 + k);

    goertzel_storage_type omega = k * 2.0 * M_PI;
    omega /= samples;

    g->coeff = 2.0 * cos(omega);

    goertzel_reset(g);
}

void goertzel_process_sample(struct goertzel_state *g,
    goertzel_storage_type sample)
{
    GOERTZEL_ASSERT(g && "g pointer is NULL");

    goertzel_storage_type Q0 = g->coeff * g->Q1 - g->Q2 + sample;
    g->Q2 = g->Q1;
    g->Q1 = Q0;
}

void goertzel_process_sample_pcm8(struct goertzel_state *g,
    uint8_t sample)
{
    goertzel_process_sample(g, (goertzel_storage_type)sample - 128.0);
}

void goertzel_process_sample_pcm16(struct goertzel_state *g,
    uint16_t sample)
{
    goertzel_process_sample(g, (goertzel_storage_type)sample - 32768.0);
}

goertzel_storage_type goertzel_get_squared_magnitude(struct goertzel_state *g)
{
    GOERTZEL_ASSERT(g && "g pointer is NULL");

    goertzel_storage_type mag_sqrd;
    mag_sqrd = g->Q1 * g->Q1 + g->Q2 * g->Q2 - g->Q1 * g->Q2 * g->coeff;
    goertzel_reset(g);
    return mag_sqrd;
}

goertzel_storage_type goertzel_get_magnitude(struct goertzel_state *g)
{
    return sqrt(goertzel_get_squared_magnitude(g));
}
