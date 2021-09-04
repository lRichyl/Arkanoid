#ifndef TIMER_H
#define TIMER_H
#include <windows.h>

long long get_performance_counter_frequency(){
     LARGE_INTEGER perf_count_frequency_result;
     QueryPerformanceFrequency(&perf_count_frequency_result);
     long long perf_count_frequency = perf_count_frequency_result.QuadPart;
     return perf_count_frequency;
}

LARGE_INTEGER get_time_counter(){
     LARGE_INTEGER counter;
     QueryPerformanceCounter(&counter);
     return counter;
}

float get_time_in_ms(LARGE_INTEGER start_counter, LARGE_INTEGER end_counter, long long perf_count_frequency, int *fps = nullptr){
     long long counter_elapsed = end_counter.QuadPart - start_counter.QuadPart;
     float time_in_ms = ((1000*(float)counter_elapsed) / (float)perf_count_frequency);
     if(fps != nullptr){
          *fps = perf_count_frequency / counter_elapsed;
     }
     //last_counter = end_counter;
     return time_in_ms;
}
#endif
