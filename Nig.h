#include <windows.h>

#define ShowMessage(msg) MessageBox(0, msg, "Oops", MB_OK)

#define random(n) ((float)rand()/(float)RAND_MAX*(float)n)
