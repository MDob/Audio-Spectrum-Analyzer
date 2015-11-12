/*************************************************************************
 *  File Name:      fft_cfg.h
 *  Author:         Michael Dobinson
 *  Date Created:   2015-11-10
 *
 *  Original Creators: Tom Roberts, Malcolm Slaney, Dimitrios P. Bouras
 *
 *  Sub-files:
 *      fft.c
 *
 *  Brief:
 *      16-bit fixed point FFT implementation
 *
 *  [Compiled and tested with Atmel Studio 7]
 *
*************************************************************************/

#ifndef FFT_H_
#define FFT_H_

#define INVERSE_FFT 1

/*======================================================================*/
/*                         FUNCTION PROTOTYPES                          */
/*======================================================================*/
int fix_fft(short fr[], short fi[], short m, short inverse);
int fix_fftr(short f[], int m, int inverse);

#endif /* FFT_H_ */