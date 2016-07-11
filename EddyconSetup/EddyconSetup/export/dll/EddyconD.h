#include "extcode.h"
#pragma pack(push)
#pragma pack(1)

#ifdef __cplusplus
extern "C" {
#endif
typedef uint16_t  Enum;
#define Enum_Lowpass 0
#define Enum_Highpass 1

/*!
 * DLLFilter_calcFilterKoefs
 */
void __stdcall DLLFilter_calcFilterKoefs(Enum ftype2, double fsmp, double fc, 
	int16_t a_array[], int16_t b_array[], int32_t a_len, int32_t b_len);

MgErr __cdecl LVDLLStatus(char *errStr, int errStrLen, void *module);

#ifdef __cplusplus
} // extern "C"
#endif

#pragma pack(pop)

