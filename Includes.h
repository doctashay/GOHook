#pragma once

#include "Interface.h"
#include "SDK.h"
#include "Menu.h"
#include <time.h>

using namespace std;

extern CEntity* Me(void);

extern void FillRGBA(int x, int y, int w, int h, int r, int g, int b, int a);
extern void Border(int x, int y, int w, int h, int line, int r, int g, int b, int a);
extern void Text(int x, int y, int r, int g, int b, int a, bool bCenter, unsigned long font, const char *fmt, ...);
extern void Text(int x, int y, int r, int g, int b, int a, bool bCenter, unsigned long font, const wchar_t *fmt, ...);

extern void __stdcall new_PaintTraverse(unsigned int vguiPanel, bool forceRepaint, bool allowForce);
extern void __stdcall new_CreateMove(int sequence_number, float input_sample_frametime, bool active);


