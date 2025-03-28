#ifndef MIC_H
#define MIC_H

#include <stdint.h>

// Number of calibration samples used to calculate ADC offset
#define CALIBRATION_SAMPLES 1000

// Minimum and maximum ADC values for normalization
#define MIN_ADC_VALUE 0
#define MAX_ADC_VALUE 4095

// Global variables to store the number of samples, ADC offset, and sample buffer
extern uint16_t num_samples;
extern uint16_t offset;
extern float* samples;

// Function prototypes

/**
 * @brief Calculates the ADC offset by averaging a fixed number of calibration samples.
 * This function repeatedly reads ADC values and calculates their average
 * to set the offset used for normalization.
 */
void calculate_offset(void);

/**
 * @brief Initializes the microphone.
 * Sets the number of samples and configures the ADC for reading the microphone input.
 * @param num_samples_ Number of samples to collect per second.
 */
void init_mic(uint16_t num_samples_);

/**
 * @brief Reads a single microphone sample.
 * Reads raw ADC data, applies offset correction, and normalizes the result
 * to a floating-point value between 0 and 1.
 * @return Normalized microphone sample.
 */
float read_mic(void);

/**
 * @brief Records microphone samples over a given duration.
 * Collects samples at regular intervals for the specified number of seconds.
 * Allocates memory for storing the samples.
 * @param seconds Number of seconds to record.
 */
void record_mic(uint16_t seconds);

/**
 * @brief Returns the collected microphone samples.
 * Provides access to the buffer containing the recorded samples.
 * @return Pointer to the array of collected samples.
 */
float* get_samples_mic(void);

/**
 * @brief Deinitializes the microphone.
 * Frees any allocated memory for the sample buffer.
 */
void deinit_mic(void);

#endif // MIC_H
