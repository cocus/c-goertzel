/**
 * @file goertzel-dtmf.h
 * @author Santiago Hormazabal (santiagohssl@gmail.com)
 * @brief Implementation of a DTMF symbol detector using the optimized Goertzel
 *        algorithm.
 * @version 0.1
 */
#ifndef _GOERTZEL_DTMF_H
#define _GOERTZEL_DTMF_H

#include "goertzel.h"

#include <stdint.h> // For uint* typedefs
#include <stddef.h> // For size_t

/**
 * @brief Global defines for the goertzel-dtmf library.
 *        There are a total of 8 tones that can be detected on DTMF. Two groups
 *        of 4 tones create a 4x4 matrix that indicates which DTMF symbol is
 *        being detected.
 */
#define DTMF_GROUP_COUNT 2
#define DTMF_FREQS_PER_GROUP 4
#define DTMF_TOTAL_FREQS 8

/**
 * @brief Structure that holds the status of a DTMF detector.
 *        Contains 8 Goertzel state structures, each one related
 *        to the DTMF frequencies matrix.
 */
struct goertzel_dtmf_state
{
    struct goertzel_state g[DTMF_TOTAL_FREQS];  /*!< Goertzel struct for each DTMF tone */
};

/**
 * @brief Initializes a Goertzel based DTMF detector structure with the given
 *        arguments for its proper usage. The algorithm needs to know from the
 *        start the value of the wanted (target) frequency, the sampling
 *        frequency and the number of samples to be processed in one go.
 *
 * @param dtmf DTMF detector structure.
 * @param sampling_frequency Sampling frequency in Hz.
 * @param target_frequency Frequency to look for in Hz.
 * @param samples Number of samples to use on the detect method.
 */
void goertzel_dtmf_init(struct goertzel_dtmf_state *dtmf,
    uint32_t sampling_frequency, size_t samples);

/**
 * @brief Adds (process) a single sample (out of the configured 'samples' on
 *        the init). Uses an uint16_t as the storage type.
 *
 * @param dtmf DTMF detector structure.
 * @param sample Value of the sample to process. Range [0, 65535] centered
 *               on 32768.
 */
void goertzel_dtmf_add_sample_pcm16(struct goertzel_dtmf_state *dtmf,
    uint16_t sample);

/**
 * @brief Adds (process) a single sample (out of the configured 'samples' on
 *        the init). Uses an uint8_t as the storage type.
 *
 * @param dtmf DTMF detector structure.
 * @param sample Value of the sample to process. Range [0, 255] centered
 *               on 128.
 */
void goertzel_dtmf_add_sample_pcm8(struct goertzel_dtmf_state *dtmf,
    uint8_t sample);

/**
 * @brief Detects a tone based upon the state of the dtmf structure.
 *        The structure needs to have the exact amount of samples added to it.
 *        This amount of samples is configured on the initialization of the
 *        structure. Call the _add_sample functions with the appropriate
 *        set of samples (sampled at the configured frequency) before calling
 *        this function.
 *        Two threshold values are used, and these determine if the frequencies
 *        of the tone group 0 or 1 are found or not.
 *        After calling this function, the state of the Goertzel detectors is
 *        reset, so it's not possible to call this function more than once
 *        without calling the _add_sample functions in between.
 *        This value needs to be tweaked depending on the amplitude of the
 *        samples and sampling frequency being used. In an ideal world, both of
 *        them should be equal.
 *        If a DTMF symbol is detected, its ASCII representation (uppercase for
 *        A, B, C and D) it's returned. Otherwise, a zero char ('\0') is
 *        returned.
 *
 * @param dtmf DTMF detector structure.
 * @param threshold_a Threshold for the first group (697, 770, 852, 941)
 * @param threshold_b Threshold for the second group (1209, 1336, 1477, 1633)
 * @return char ASCII representation of the DTMF symbol, or '\0' if no symbol
 *                    is detected with the given thresholds.
 */
char goertzel_dtmf_detect(struct goertzel_dtmf_state *dtmf,
    goertzel_storage_type threshold_a, goertzel_storage_type threshold_b);

#endif // _GOERTZEL_DTMF_H
