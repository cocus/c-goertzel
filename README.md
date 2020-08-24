# c-goertzel
C version of the Optimized Goertzel algorithm (https://www.embedded.com/the-goertzel-algorithm/) with DTMF detector code.

# Usage
## DTMF detector
Please refer to the example used as a test on the `test/test.c` file. In short terms:
1. Init a `goertzel_dtmf_state` structure by calling `goertzel_dtmf_detect` with the sample rate and the number of samples to process.
2. Call `goertzel_dtmf_add_sample_pcm8` or `goertzel_dtmf_add_sample_pcm16` to add the configured number of samples.
3. Detect the DTMF symbol by reading the return value of `goertzel_dtmf_detect`. The thresholds should be tweaked accordingly to the amplitude and sampling frequency that's being used.

## Tone detector (pure Goertzel)
1. Init a `goertzel_state` structure by calling `goertzel_init` with the sample rate and the number of samples to process.
2. Call `goertzel_process_sample_pcm8` or `goertzel_process_sample_pcm16` to add the configured number of samples.
3. Detect the squared Goertzel magnitude with the return value of `goertzel_get_squared_magnitude`. The thresholds should be tweaked accordingly to the amplitude and sampling frequency that's being used. If the magnitude value is required, `goertzel_get_magnitude` is available.
