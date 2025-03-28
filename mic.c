#include "mic.h"

#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/timer.h"

// Number of samples to collect
uint16_t num_samples = 0;
// Offset for ADC calibration
uint16_t offset = 2048;
// Pointer to store microphone samples
float* samples = NULL;

// Function to calculate ADC offset by averaging a set number of calibration samples
void calculate_offset() 
{
    uint32_t sum = 0;
    for (int i = 0; i < CALIBRATION_SAMPLES; i++) {
        sum += adc_read(); // Read ADC value
        sleep_us(100);     // Small delay between readings
    }
    offset = sum / CALIBRATION_SAMPLES; // Calculate the average offset
}

// Function to initialize the microphone, setting the number of samples and ADC configuration
void init_mic(uint16_t num_samples_)
{
    num_samples = num_samples_; // Store the number of samples
    adc_gpio_init(28);          // Initialize GPIO 28 for ADC
    adc_init();                 // Initialize the ADC hardware
}

// Function to read a single microphone sample and normalize it
float read_mic()
{
    uint16_t raw_value;
    adc_select_input(2); // Select ADC input channel 2
    raw_value = adc_read(); // Read the raw ADC value
    // Normalize the value between 0 and 1
    return ((raw_value - offset) - MIN_ADC_VALUE) / (float)(MAX_ADC_VALUE - MIN_ADC_VALUE);
}

// Function to record microphone samples for a given number of seconds
void record_mic(uint16_t seconds)
{
    absolute_time_t next_sample = get_absolute_time(); // Get the current time
    uint16_t raw_value;

    // Free previously allocated memory, if any
    if (samples != NULL)
        free(samples);

    // Allocate memory for the new set of samples
    samples = (float*)malloc(sizeof(float) * num_samples * seconds);

    adc_select_input(2); // Select ADC input channel 2

    // Collect samples at regular intervals
    for (int i = 0; i < num_samples * seconds; i++)
    {
        raw_value = adc_read(); // Read the raw ADC value
        // Normalize the value and store it in the samples array
        samples[i] = ((raw_value - offset) - MIN_ADC_VALUE) / (float)(MAX_ADC_VALUE - MIN_ADC_VALUE);
        // Calculate the time for the next sample
        next_sample = delayed_by_us(next_sample, 1000000 / num_samples);
        sleep_until(next_sample); // Wait until the next sampling time
    }
}

// Function to return the collected samples
float* get_samples_mic()
{
    return samples; // Return the pointer to the samples array
}

// Function to deinitialize the microphone, freeing allocated memory
void deinit_mic()
{
    free(samples); // Free the allocated memory
}
