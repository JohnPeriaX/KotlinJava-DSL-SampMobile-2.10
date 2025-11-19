#pragma once

// Lightweight compatibility layer to allow building without SpeexDSP.
// If SPEEXDSP_ENABLED is defined, real SpeexDSP headers are used.
// Otherwise, provide no-op stubs to avoid link errors while keeping code paths intact.

#ifdef SPEEXDSP_ENABLED
  #include "../vendor/speex/speex_echo.h"
  #include "../vendor/speex/speex_preprocess.h"
#else
  #include <stdint.h>

  typedef int16_t spx_int16_t;

  // Forward-declare opaque state structs
  typedef struct SpeexEchoState_ SpeexEchoState;
  typedef struct SpeexPreprocessState_ SpeexPreprocessState;

  // Define control request constants used in code paths
  #ifndef SPEEX_ECHO_SET_SAMPLING_RATE
  #define SPEEX_ECHO_SET_SAMPLING_RATE 1
  #endif

  #ifndef SPEEX_PREPROCESS_SET_ECHO_STATE
  #define SPEEX_PREPROCESS_SET_ECHO_STATE 2
  #endif

  // Provide no-op implementations
  static inline SpeexEchoState* speex_echo_state_init(int /*frame_size*/, int /*filter_length*/) { return nullptr; }
  static inline int speex_echo_ctl(SpeexEchoState* /*st*/, int /*request*/, void* /*ptr*/) { return 0; }
  static inline void speex_echo_playback(SpeexEchoState* /*st*/, const spx_int16_t* /*play*/) {}
  static inline void speex_echo_capture(SpeexEchoState* /*st*/, const spx_int16_t* /*rec*/, spx_int16_t* out) { (void)out; }

  static inline SpeexPreprocessState* speex_preprocess_state_init(int /*frame_size*/, int /*sample_rate*/) { return nullptr; }
  static inline int speex_preprocess_ctl(SpeexPreprocessState* /*st*/, int /*request*/, void* /*ptr*/) { return 0; }
  static inline int speex_preprocess_run(SpeexPreprocessState* /*st*/, spx_int16_t* /*x*/) { return 1; }
#endif

