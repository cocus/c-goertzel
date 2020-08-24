/**
 * @file goertzel.h
 * @author Santiago Hormazabal (santiagohssl@gmail.com)
 * @brief Implementation of the optimized Goertzel algorithm per
 *        https://www.embedded.com/the-goertzel-algorithm/
 * @version 0.1
 */
#ifndef _GOERTZEL_H
#define _GOERTZEL_H

#include <stdint.h> // For uint* typedefs
#include <stddef.h> // For size_t

/**
 * @brief Define a custom assert() here. Leave unused for the maximum speed
 *        but take great care on the arguments passed on all the functions.
 */
#define GOERTZEL_ASSERT(k)

/**
 * @brief Defines the storage type to be used on the Goertzel algorithm.
 *        If maximum precision is required, this value can be changed to
 *        double or fixed point types (when available).
 */
typedef float goertzel_storage_type;

/**
 * @brief Structure that holds the state of the Q1 and Q2 variables used
 *        repeatedly on the Goertzel algorithm. It also holds the pre-computed
 *        cosine of k.
 */
struct goertzel_state
{
    goertzel_storage_type coeff;        /*!< 2 times the value of the cos(k) */
    goertzel_storage_type Q1;           /*!< block variable Q1 */
    goertzel_storage_type Q2;           /*!< block variable Q2 */
};

/**
 * @brief Resets a Goertzel structure to its default values.
 *
 * @param g Goertzel structure.
 */
void goertzel_reset(struct goertzel_state *g);

/**
 * @brief Initializes a Goertzel structure with the given arguments for
 *        its proper usage. The algorithm needs to know from the start
 *        the value of the wanted (target) frequency, the sampling
 *        frequency and the number of samples to be processed in one go.
 *
 * @param g Goertzel structure.
 * @param sampling_frequency Sampling frequency in Hz.
 * @param target_frequency Frequency to look for in Hz.
 * @param samples Number of samples to use on the detect method.
 */
void goertzel_init(struct goertzel_state *g, uint32_t sampling_frequency,
    uint32_t target_frequency, size_t samples);

/**
 * @brief Adds (process) a single sample (out of the configured 'samples' on
 *        the start up). Uses the goertzel_storage_type as the storage type.
 *
 * @param g Goertzel structure.
 * @param sample Value of the sample to process.
 */
void goertzel_process_sample(struct goertzel_state *g,
    goertzel_storage_type sample);

/**
 * @brief Adds (process) a single sample (out of the configured 'samples' on
 *        the start up). Uses an uint8_t as the storage type.
 *
 * @param g Goertzel structure.
 * @param sample Value of the sample to process. Range [0, 255] centered
 *               on 128.
 */
void goertzel_process_sample_pcm8(struct goertzel_state *g,
    uint8_t sample);

/**
 * @brief Adds (process) a single sample (out of the configured 'samples' on
 *        the start up). Uses an uint16_t as the storage type.
 *
 * @param g Goertzel structure.
 * @param sample Value of the sample to process. Range [0, 65535] centered
 *               on 32768.
 */
void goertzel_process_sample_pcm16(struct goertzel_state *g,
    uint16_t sample);

/**
 * @brief Returns the squared magnitude (complex^2 + real^2) of the processed
 *        block of samples.
 *
 * @param g Goertzel structure.
 * @return goertzel_storage_type Detected magnitude of the Goertzel Algorithm.
 */
goertzel_storage_type goertzel_get_squared_magnitude(struct goertzel_state *g);

/**
 * @brief Returns the magnitude (sqrt(complex^2 + real^2)) of the processed
 *        block of samples.
 *
 * @param g Goertzel structure.
 * @return goertzel_storage_type Detected magnitude of the Goertzel Algorithm.
 */
goertzel_storage_type goertzel_get_magnitude(struct goertzel_state *g);

#endif // _GOERTZEL_H
