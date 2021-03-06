//
//  main.c
//  MostSimpleSynth
//
//  Created by Mathias Dietrich on 6/29/14.
//  Copyright (c) 2014 Sensorem. All rights reserved.
//
#include <stdlib.h>
#include <math.h>

#include <AudioToolbox/AudioQueue.h>
#include <CoreAudio/CoreAudioTypes.h>
#include <CoreFoundation/CFRunLoop.h>
#include <ApplicationServices/ApplicationServices.h>
#include <stdio.h>

#define NUM_BUFFERS 3
#define BUFFER_SIZE 4096
#define SAMPLE_TYPE float
#define SAMPLE_RATE 44100
#define BYTESPERSAMPLE 4

#define DPI 3.14159265358979323846  * 2.0

float volume0;
float phase0;
float freq0;

float volume1;
float phase1;

float volume2;
float phase2;

int bytesPerSample;

void callback(void *custom_data, AudioQueueRef queue, AudioQueueBufferRef buffer);



int main(int argc, const char * argv[])
{
    
    
    int midiNote = 69;
    
    freq0 = 27.5 * 4.0 * ((midiNote - 21) / 12.0);

    bytesPerSample = sizeof(float);

    volume0 = 0.5;
    phase0 = 0;

    volume1 = 0.3;
    phase1 = 10;
    
    volume2 = 0.3;
    phase2 = 20;
    
    AudioStreamBasicDescription format = {0};
    AudioQueueRef queue;
    AudioQueueBufferRef buffers[NUM_BUFFERS];
    
    format.mSampleRate       = 44100;
    format.mFormatID         = kAudioFormatLinearPCM;
    format.mFormatFlags      = kAudioFormatFlagIsFloat | kAudioFormatFlagIsPacked;
    format.mBitsPerChannel = 8 * sizeof(float);
    format.mFramesPerPacket = 1;
    format.mChannelsPerFrame = 1;
    format.mBytesPerPacket = sizeof(float) * format.mFramesPerPacket;
    format.mBytesPerFrame = sizeof(float) * format.mChannelsPerFrame;
    format.mSampleRate = SAMPLE_RATE;
    
    AudioQueueNewOutput(&format, callback, NULL, CFRunLoopGetCurrent(), kCFRunLoopCommonModes, 0, &queue);

    for (int i = 0; i < NUM_BUFFERS; i++)
    {
        AudioQueueAllocateBuffer(queue, BUFFER_SIZE, &buffers[i]);
        
        buffers[i]->mAudioDataByteSize = BUFFER_SIZE;
        
        callback(NULL, queue, buffers[i]);
    }
	
    AudioQueueStart(queue, NULL);
    CFRunLoopRun();
    return 0;
}


// ------------------------------------------------------------------------------------------
void callback(void *custom_data, AudioQueueRef queue, AudioQueueBufferRef buffer)
{
    float *casted_buffer = (float *)buffer->mAudioData;
    
    for (int i = 0; i < BUFFER_SIZE / bytesPerSample ; i++)
    {

        double float_sample0 = sin( phase0) * volume0;
        double float_sample1 = sin( phase1) * volume1;
        double float_sample2 = sin( phase2) * volume2;
        
        casted_buffer[i ] = float_sample0 + float_sample1 + float_sample2;
        
        phase0 += freq0 * DPI / SAMPLE_RATE;
        phase1 += (freq0 / 2 - 0.5 ) * DPI / SAMPLE_RATE;
        phase2 += (freq0 * 3 + 0.2 ) * DPI / SAMPLE_RATE;
    }
    
    AudioQueueEnqueueBuffer(queue, buffer, 0, NULL);
    
}


    

