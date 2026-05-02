/*
 *  @name: adFFT.cpp
 *
 *  @author: Shauna Recto
 *
 *  @description: This source file contains the implementation of computing
 *      of the FFT of a stream of data, outputting the frequency spectrum,
 *      comprising of frequency bins.
 *
 *      As previously mentioned in the header, this is an implementation using
 *      the FFTW library.
 *
 */

#include "adFFT.h"
#include <fftw3.h>

ADFFT::ADFFT() {}

ADFFT::~ADFFT() {
	fftwf_destroy_plan(fft);
	fftwf_free(spectrum);
	fftwf_free(samples);
}

void ADFFT::update() {}