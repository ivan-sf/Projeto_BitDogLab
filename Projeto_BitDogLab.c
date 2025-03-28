#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include "Fonts.h"
#include "DisplaySSD1306.h"
#include "hardware/uart.h"
#include "hardware/adc.h"
#include "joystick_lib.h"
#include "direcoes.h"
#include "mic.h"
#include "PDSFunctions.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15

extern bool select; //Variable that indicates the button click
int position = 0; // Global variable to store cursor's position
int screen = 0; // Global variable to store the selected screen

#define SAMPLES 20000 
#define DOWNSAMPLE (SAMPLES/SSD1306_WIDTH)

#define BUFFER_SIZE 1024
#define BAR_WIDTH 1

int start_index = 30;

int end_index = 158;

volatile bool timer_fired = false;
void att_position();
void att_box();
void att_screen();

// Callback function
bool repeating_timer_callback(__unused struct repeating_timer *t) {
    att_position();
    return true;
}

void plot_fft(SSD1306_COLOR_t color);
void plot_waveform(SSD1306_COLOR_t color);

struct RenderArea FrameArea = {
    .StartColumn = 0,
    .EndColumn = SSD1306_WIDTH - 1,
    .StartPage = 0,
    .EndPage = SSD1306_NumberOfPages - 1
};

uint8_t ssd[SSD1306_BufferLength];

int main(){
    stdio_init_all();

    // I2C initialization
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    
    Calculate_RenderArea_BufferLength(&FrameArea);

    int fs = 10000;
    init_joystick(); // Joystick initialization
    SSD1306_Init(); // Display initialization
    init_mic(fs); // Microphone initialization

    struct repeating_timer timer;
    add_repeating_timer_ms(143, repeating_timer_callback, NULL, &timer);

    // Screen setup
    SSD1306_Clear(ssd, &FrameArea);
    SSD1306_DrawRectangle(ssd, 2,2 , 124, 62, SSD1306_COLOR_WHITE);
    SSD1306_SetString(ssd, 42, 10, "Gravar", &Font_7x10, SSD1306_COLOR_WHITE); // Record
    SSD1306_SetString(ssd, 44, 26, "Tempo", &Font_7x10, SSD1306_COLOR_WHITE); // Time domain plotting
    SSD1306_SetString(ssd, 25, 42, "Frequencia", &Font_7x10, SSD1306_COLOR_WHITE); // Frequency domain plotting (FFT)
    
    position=1;
    while(1){
        att_box();
        att_screen();
    }
}

// Function that updates selection box position
void att_position() {
    if(select ==false){
        joystick_captura();
        normalizar_joystick();
        switch (get_direcao())
        {
            case CIMA1:   
            case CIMA2:
            position = (position - 1 + 3) % 3;
            break;

            case BAIXO1:
            case BAIXO2:
            position = (position + 1) % 3;
            break;

            default:
            break;
        }
    }  
}

// Function that updates selection box
void att_box(){
    if (select==false){
        switch (position){
            case 0 :
                SSD1306_DrawRectangle(ssd, 20, 6+16, 66+20, 16, SSD1306_COLOR_BLACK);
                SSD1306_DrawRectangle(ssd, 20, 6+16*2, 66+20, 16, SSD1306_COLOR_BLACK);
                SSD1306_DrawRectangle(ssd, 20, 6, 66+20, 16, SSD1306_COLOR_WHITE);
                break;

            case 1:
                SSD1306_DrawRectangle(ssd, 20, 6, 66+20, 16, SSD1306_COLOR_BLACK);
                SSD1306_DrawRectangle(ssd, 20, 6+16*2, 66+20, 16, SSD1306_COLOR_BLACK); 
                SSD1306_DrawRectangle(ssd, 20, 6+16, 66+20, 16, SSD1306_COLOR_WHITE);
                break;
            case 2:
                SSD1306_DrawRectangle(ssd, 20, 6, 66+20, 16, SSD1306_COLOR_BLACK);
                SSD1306_DrawRectangle(ssd, 20, 6+16, 66+20, 16, SSD1306_COLOR_BLACK);
                SSD1306_DrawRectangle(ssd, 20, 6+16*2, 66+20, 16, SSD1306_COLOR_WHITE); 
                break;

            default:
                break;
        }

    SSD1306_Render(ssd, &FrameArea);
    }    
}

// Functions that uptades screen
void att_screen(){
    if(select == false && screen !=0){
        screen = 0;
        SSD1306_Clear(ssd, &FrameArea);
        SSD1306_DrawRectangle(ssd, 2,2 , 124, 62, SSD1306_COLOR_WHITE);
        SSD1306_SetString(ssd, 42, 10, "Gravar", &Font_7x10, SSD1306_COLOR_WHITE); // Record
        SSD1306_SetString(ssd, 44, 26, "Tempo", &Font_7x10, SSD1306_COLOR_WHITE); // Time domain plotting
        SSD1306_SetString(ssd, 25, 42, "Frequencia", &Font_7x10, SSD1306_COLOR_WHITE); // Frequency domain plotting
    }
    else if (select == true){
        switch (position){
            case 0:
                screen =1;
                SSD1306_Clear(ssd, &FrameArea);
                SSD1306_SetString(ssd, 20, 23, "Gravando", &Font_11x18, SSD1306_COLOR_WHITE); // Recording
                SSD1306_Render(ssd, &FrameArea);
                record_mic(2); // Time in seconds
                select=false;
                break;

            case 1:
                screen = 2;
                plot_waveform(SSD1306_COLOR_WHITE);
                sleep_ms(5000);
                select = false;
                break;

            case 2:
                screen = 3;
                plot_fft(SSD1306_COLOR_WHITE);
                sleep_ms(5000);
                select = false;
                break;
        
            default:
                break;
        }
    }
}

// Function that plots the recorded sound wave in time domain
void plot_waveform(SSD1306_COLOR_t color) {
    SSD1306_Clear(ssd, &FrameArea); 

    float* original = get_samples_mic(); // Gets samples array
    float plot[SAMPLES]; // Declaration of new array for plotting

    // Copies original samples array to plot array
    for (int i = 0; i < SAMPLES; i++){
        plot[i] = original[i];
    }

    // Normalization of samples values
    for (int i =0; i<SAMPLES; i++){
        plot[i] = (plot[i]+1)* 4095/2;
    }

    // Determines maximum and minimum values of the given samples
    int min_val = 4095, max_val = 0;
    for (int i = 0; i < SAMPLES; i++) {
        if (plot[i] < min_val) min_val = plot[i];
        if (plot[i] > max_val) max_val = plot[i];
    }

    // Wave plotting
    for (int x = 0; x < SSD1306_WIDTH - 1; x++) {
        int idx1 = x * DOWNSAMPLE; // Initial line point index
        int idx2 = (x + 1) * DOWNSAMPLE; // Final line point index

        // Values normalization to fit in the display
        int y1 = 63 - ((plot[idx1] - min_val) * 63) / (max_val - min_val);
        int y2 = 63 - ((plot[idx2] - min_val) * 63) / (max_val - min_val);

        SSD1306_DrawLine(ssd, x, y1, x + 1, y2, color);
    }

    SSD1306_SetString(ssd, 127-7*6, 0, "Tempo", &Font_7x10, SSD1306_COLOR_WHITE);

    SSD1306_Render(ssd, &FrameArea);

    sleep_ms(50);
}

// Function that plots the recorded sound wave in frequency domain
void plot_fft(SSD1306_COLOR_t color){
    SSD1306_Clear(ssd, &FrameArea);

    float* original = get_samples_mic(); // Gets samples array
    float plot[SAMPLES]; // Declaration of new array for plotting
    
    // Copies original samples array to plot array
    for (int i = 0; i < SAMPLES; i++){
        plot[i] = original[i];
    }

    // Normalization of samples values
    for (int i = 0; i <SAMPLES; i++){
        plot[i] = (plot[i]+1)*4095/2;
    }

    // Declaration of array for FFT plotting
    _Complex double complex_buffer[BUFFER_SIZE];
    double magnitude_buffer[BUFFER_SIZE/2];
    float converted_buffer[BUFFER_SIZE];

    convert_to_complex(plot, complex_buffer, BUFFER_SIZE); // Conversion of data to complex domain
   
    fft_radix2(complex_buffer, BUFFER_SIZE); // FFT
   
    // Calculates magnitudes and normalizes it
    double max_magnitude = 0;
    for (int i = 0; i < BUFFER_SIZE / 2; i++) {
        magnitude_buffer[i] = cabs(complex_buffer[i]);
        if (magnitude_buffer[i] > max_magnitude) {
            max_magnitude = magnitude_buffer[i];
        }
    }
    if (max_magnitude > 0) {
        for (int i = 0; i < BUFFER_SIZE / 2; i++) {
            magnitude_buffer[i] = (magnitude_buffer[i] / max_magnitude) * 16*SSD1306_HEIGHT;
        }
     }
   
    int bar_count = end_index - start_index + 1;

    SSD1306_Clear(ssd, &FrameArea);
   
    // Draws the bars corresponding to the FFT
    for (int i = 0; i < bar_count; i++) {
        int index = start_index + i; // Magnitude buffer index
        int bar_height = (int)magnitude_buffer[index]; // Normalized height

        if (bar_height > SSD1306_HEIGHT) {
            bar_height = SSD1306_HEIGHT; // Limitates to the height of the display
        }
   
    SSD1306_DrawFilledRectangle(ssd, i * BAR_WIDTH, SSD1306_HEIGHT - bar_height, BAR_WIDTH, bar_height, SSD1306_COLOR_WHITE);
    }

    SSD1306_SetString(ssd, 127-7*4, 0, "FFT", &Font_7x10, SSD1306_COLOR_WHITE);

    SSD1306_Render(ssd, &FrameArea);

    sleep_ms(50);
}
