#!/bin/bash
g++ ./patterns.cpp ./libs/functions.cpp ./libs/led-matrix.cc ./libs/gpio.cc ./libs/thread.cc ./libs/framebuffer.cc ./libs/pixel-mapper.cc ./libs/options-initialize.cc ./libs/multiplex-mappers.cc ./libs/hardware-mapping.c -pthread -o ./patterns.out
