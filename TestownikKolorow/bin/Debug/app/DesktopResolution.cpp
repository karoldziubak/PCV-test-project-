#include "DesktopResolution.h"
#include <wtypes.h>

void GetDesktopResolution(int& horizontal, int& vertical)
{
   RECT desktop;
   const HWND hDesktop = GetDesktopWindow();
   GetWindowRect(hDesktop, &desktop);
   horizontal = desktop.right;
   vertical = desktop.bottom;
}
