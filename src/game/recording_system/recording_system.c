#include "recording_system.h"
#include <stdio.h>

static recording_system_t recording_system = {0};

void recording_system_init(void){
    recording_system.centiseconds_per_frame = 5;
    recording_system.bit_depth = 24;
}

void recording_system_stop_recording(void){
    if(recording_system.state == RECORDING_SYSTEM_STATE_RECORDING){
        recording_system_add_frame();
        msf_gif_end(&recording_system.gif_state);
    }
    recording_system.state = RECORDING_SYSTEM_STATE_READY;
}

void recording_system_start_recording(void){
    if(recording_system.state == RECORDING_SYSTEM_STATE_RECORDING)
        return;

    unsigned int const FILENAME_LENGTH = 100;
    char file_name[FILENAME_LENGTH];

    {
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        snprintf(file_name, FILENAME_LENGTH, "recording%d%d%d%d%d%d.gif", 
        tm.tm_year + 1900, 
        tm.tm_mon + 1,
        tm.tm_mday, 
        tm.tm_hour, tm.tm_min, tm.tm_sec);
    }
    recording_system.img = GetScreenData();
    msf_gif_begin(&recording_system.gif_state, file_name, recording_system.img.width, recording_system.img.height);
    recording_system_add_frame();
    recording_system.base_time = clock();
    recording_system.state = RECORDING_SYSTEM_STATE_RECORDING;
}

void recording_system_add_frame() {
    recording_system.img = GetScreenData();
    msf_gif_frame(&recording_system.gif_state, (uint8_t *)recording_system.img.data, recording_system.bit_depth, recording_system.centiseconds_per_frame, recording_system.img.width * 4, false);
}

void recording_system_update(void){
    switch(recording_system.state){
        case RECORDING_SYSTEM_STATE_RECORDING:
        {
            double elapsed_time = ((double)clock() - recording_system.base_time) / CLOCKS_PER_SEC;

            if(elapsed_time >= (double)recording_system.centiseconds_per_frame / 100){
                recording_system.base_time = clock();
                printf("adding frame...\n");
                recording_system_add_frame();
            }
        }
        break;
    }
}

void recording_system_fini(void){
    recording_system_stop_recording();
}

bool recording_system_is_ready(void){
    return recording_system.state == RECORDING_SYSTEM_STATE_READY;
}

unsigned int recording_system_state() {
    return recording_system.state;
}