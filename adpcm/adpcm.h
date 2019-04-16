#ifndef ADPCM_H
#define ADPCM_H

struct adpcm_state {
    short	valprev;	/* Previous output value */
    char	index;		/* Index into stepsize table */
};

extern void adpcm_coder(short *indata, signed char *outdata, int len, struct adpcm_state *state, int channels);
extern void adpcm_decoder(signed char *indata, short *outdata, int len, struct adpcm_state *state, int channels);

#endif /*ADPCM_H*/
