#ifndef __FPAU_SWITCHES
#define __FPAU_SWITCHES

//------- HW Specific Switches ---------//
// Must be activated from top to bottom
#define UART
#define FPAU

//------ Cycles Profiling Switches -----//
//#define PROFILING_NTT_INTT
//#define PROFILING_MAC
//#define PROFILING_MULT
//#define PROFILING_ADD_SUB
#define PROFILING_STAGES

#ifdef STEEL                     // defined in compilation flags (makefile)
#define TICKS_REGISTER  "cycle"
#else
#define TICKS_REGISTER  "time"
#endif

//--- Testing Vectors Output Switch ----//
//#define NTT_TESTING_HW


#endif //__FPAU_SWITCHES