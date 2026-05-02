/*
 *  @name: adFFT.h
 *
 *  @author: Shauna Recto
 *
 *  @description: This header file contains all the necessary declarations and
 *      default definitions of members of the FFT class.
 *
 *      The default implementation of the program is using the Arch upstream
 *      version of FFTW, but can be substituted with a different implementation
 *      such as KissFFT.
 */

#include <fftw3.h>
#include <string>
#include <vector>

#ifndef AD_FFT_H
#define AD_FFT_H

class ADFFT {
  private:
	std::vector<float> rawAudio;	   // Sample buffers for all channels.
	std::vector<float> monoBuffer;	   // Mono mixdown buffer.
	std::vector<float> leftOverBuffer; // Unused samples for next cycle.

	/* FFTW related members */
	float *samples;			 // The samples that will be used with the FFT
	fftwf_complex *spectrum; // the spectrum of the samples
	fftwf_plan fft;			 // The plan to compute the FFT.

	/* magSpectrums Contains numFFTs FFTs of size spectrumSize */
	std::vector<std::vector<float>> magSpectrums;
	int spectrumSize;

	/* Other FFT Related members */
	std::vector<float> hannWindow;

	ADFFT();
	~ADFFT();
	void update();

	/* Supporting Member functions */
	float clampSample(float,
					  float); // Checks and forces sample to be within range.
};

#endif /* AD_FFT_H */