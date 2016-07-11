#include "extcode.h"
#pragma pack(push)
#pragma pack(1)

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * Test_DLLSubVI
 */
void __stdcall Test_DLLSubVI(int16_t Numeric1, int16_t Numeric2, 
	int16_t Numeric3, int16_t Numeric4, int16_t Numeric5, int16_t Numeric6, 
	int16_t Array[], int32_t len);

MgErr __cdecl LVDLLStatus(char *errStr, int errStrLen, void *module);

#ifdef __cplusplus
} // extern "C"
#endif

#pragma pack(pop)

