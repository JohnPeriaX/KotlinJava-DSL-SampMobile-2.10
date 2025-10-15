/*===========================================================================
	DSP (Digital Signal Processing)
===========================================================================*/

/*
	Multi-channel order of each channel is as follows:
	 3 channels       left-front, right-front, center.
	 4 channels       left-front, right-front, left-rear/side, right-rear/side.
	 5 channels       left-front, right-front, center, left-rear/side, right-rear/side.
	 6 channels (5.1) left-front, right-front, center, LFE, left-rear/side, right-rear/side.
	 8 channels (7.1) left-front, right-front, center, LFE, left-rear/side, right-rear/side, left-rear center, right-rear center.
*/

// DSP channels flags
#define BASS_BFX_CHANALL	-1				// all channels at once (as by default)
#define BASS_BFX_CHANNONE	0				// disable an effect for all channels
#define BASS_BFX_CHAN1		1				// left-front channel
#define BASS_BFX_CHAN2		2				// right-front channel
#define BASS_BFX_CHAN3		4				// see above info
#define BASS_BFX_CHAN4		8				// see above info
#define BASS_BFX_CHAN5		16				// see above info
#define BASS_BFX_CHAN6		32				// see above info
#define BASS_BFX_CHAN7		64				// see above info
#define BASS_BFX_CHAN8		128				// see above info

// if you have more than 8 channels (7.1), use this macro
#define BASS_BFX_CHANNEL_N(n) (1<<((n)-1))

// DSP effects
enum {
	BASS_FX_BFX_ROTATE = 0x10000,			// A channels volume ping-pong	/ multi channel
	BASS_FX_BFX_ECHO,						// Echo							/ 2 channels max	(deprecated)
	BASS_FX_BFX_FLANGER,					// Flanger						/ multi channel		(deprecated)
	BASS_FX_BFX_VOLUME,						// Volume						/ multi channel
	BASS_FX_BFX_PEAKEQ,						// Peaking Equalizer			/ multi channel
	BASS_FX_BFX_REVERB,						// Reverb						/ 2 channels max	(deprecated)
	BASS_FX_BFX_LPF,						// Low Pass Filter 24dB			/ multi channel		(deprecated)
	BASS_FX_BFX_MIX,						// Swap, remap and mix channels	/ multi channel
	BASS_FX_BFX_DAMP,						// Dynamic Amplification		/ multi channel
	BASS_FX_BFX_AUTOWAH,					// Auto Wah						/ multi channel
	BASS_FX_BFX_ECHO2,						// Echo 2						/ multi channel		(deprecated)
	BASS_FX_BFX_PHASER,						// Phaser						/ multi channel
	BASS_FX_BFX_ECHO3,						// Echo 3						/ multi channel		(deprecated)
	BASS_FX_BFX_CHORUS,						// Chorus/Flanger				/ multi channel
	BASS_FX_BFX_APF,						// All Pass Filter				/ multi channel		(deprecated)
	BASS_FX_BFX_COMPRESSOR,					// Compressor					/ multi channel		(deprecated)
	BASS_FX_BFX_DISTORTION,					// Distortion					/ multi channel
	BASS_FX_BFX_COMPRESSOR2,				// Compressor 2					/ multi channel
	BASS_FX_BFX_VOLUME_ENV,					// Volume envelope				/ multi channel
	BASS_FX_BFX_BQF,						// BiQuad filters				/ multi channel
	BASS_FX_BFX_ECHO4,						// Echo	4						/ multi channel
	BASS_FX_BFX_PITCHSHIFT,					// Pitch shift using FFT		/ multi channel		(not available on mobile)
	BASS_FX_BFX_FREEVERB					// Reverb using "Freeverb" algo	/ multi channel
};

// BiQuad Filters
enum {
	BASS_BFX_BQF_LOWPASS,
	BASS_BFX_BQF_HIGHPASS,
	BASS_BFX_BQF_BANDPASS,					// constant 0 dB peak gain
	BASS_BFX_BQF_BANDPASS_Q,				// constant skirt gain, peak gain = Q
	BASS_BFX_BQF_NOTCH,
	BASS_BFX_BQF_ALLPASS,
	BASS_BFX_BQF_PEAKINGEQ,
	BASS_BFX_BQF_LOWSHELF,
	BASS_BFX_BQF_HIGHSHELF
};

typedef struct {
	int   lFilter;							// BASS_BFX_BQF_xxx filter types
	float fCenter;							// [1Hz..<info.freq/2] Cutoff (central) frequency in Hz
	float fGain;							// [-15dB...0...+15dB] Used only for PEAKINGEQ and Shelving filters in dB (can be above/below these limits)
	float fBandwidth;						// [0.1...........<10] Bandwidth in octaves (fQ is not in use (fBandwidth has a priority over fQ))
											// 						(between -3 dB frequencies for BANDPASS and NOTCH or between midpoint
											// 						(fGgain/2) gain frequencies for PEAKINGEQ)
	float fQ;								// [0.1.............1] The EE kinda definition (linear) (if fBandwidth is not in use)
	float fS;								// [0.1.............1] A "shelf slope" parameter (linear) (used only with Shelving filters)
											// 						when fS = 1, the shelf slope is as steep as you can get it and remain monotonically
											// 						increasing or decreasing gain with frequency.
	int   lChannel;							// BASS_BFX_CHANxxx flag/s
} BASS_BFX_BQF;

// Compressor 2
typedef struct {
	float fGain;							// output gain of signal after compression	[-60....60] in dB
	float fThreshold;						// point at which compression begins		[-60.....0] in dB
	float fRatio;							// compression ratio						[1.......n]
	float fAttack;							// attack time in ms						[0.01.1000]
	float fRelease;							// release time in ms						[0.01.5000]
	int   lChannel;							// BASS_BFX_CHANxxx flag/s
} BASS_BFX_COMPRESSOR2;