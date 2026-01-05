/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2025 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/
#include "SDL_internal.h"

#include "../../events/SDL_pen_c.h"

#ifdef SDL_VIDEO_DRIVER_WINDOWS

#include "../../core/windows/SDL_windows.h"

// #define NOWTBASICFXNS
// #include "wintab.h"

#ifndef SDL_windowswintab_h_
#define SDL_windowswintab_h_

// Set up for C function definitions, even when using C++
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

DECLARE_HANDLE(HCTX); /* context handle */

#ifndef NOWTMESSAGES

#define WT_DEFBASE			0x7FF0
#define WT_MAXOFFSET			0xF

#define _WT_PACKET(b)		((b)+0)
#define _WT_CTXOPEN(b)		((b)+1)
#define _WT_CTXCLOSE(b)		((b)+2)
#define _WT_CTXUPDATE(b)	((b)+3)
#define _WT_CTXOVERLAP(b)	((b)+4)
#define _WT_PROXIMITY(b)	((b)+5)
#define _WT_INFOCHANGE(b)	((b)+6)
#define _WT_CSRCHANGE(b)	((b)+7) /* 1.1 */
#define _WT_PACKETEXT(b)	((b)+8) /* 1.4 */
#define _WT_MAX(b)			((b)+WT_MAXOFFSET)

#define WT_PACKET				_WT_PACKET(WT_DEFBASE) /* The WT_PACKET message is posted to the context-owning windows that have requested messaging for their context. */
#define WT_CTXOPEN			_WT_CTXOPEN(WT_DEFBASE) /* The WT_CTXOPEN message is sent to the owning window and to any manager windows when a context is opened. */
#define WT_CTXCLOSE			_WT_CTXCLOSE(WT_DEFBASE) /* The WT_CTXCLOSE message is sent to the owning window and to any manager windows when a context is about to be closed. */
#define WT_CTXUPDATE			_WT_CTXUPDATE(WT_DEFBASE) /* The WT_CTXUPDATE message is sent to the owning window and to any manager windows when a context is changed. */
#define WT_CTXOVERLAP		_WT_CTXOVERLAP(WT_DEFBASE) /* The WT_CTXOVERLAP message is sent to the owning window and to any manager windows when a context is moved in the overlap order. */
#define WT_PROXIMITY			_WT_PROXIMITY(WT_DEFBASE) /* The WT_PROXIMITY message is posted to the owning window and any manager windows when the cursor enters or leaves context proximity. */
#define WT_INFOCHANGE		_WT_INFOCHANGE(WT_DEFBASE) /* The WT_INFOCHANGE message is sent to all manager and context-owning windows when the number of connected tablets has changed */
#define WT_CSRCHANGE			_WT_CSRCHANGE(WT_DEFBASE) /* 1.1  The WT_CSRCHANGE message is posted to the owning window when a new cursor enters the context.*/
#define WT_PACKETEXT			_WT_PACKETEXT(WT_DEFBASE) /* 1.4 Packet notification for Wintab extensions data */ 
#define WT_MAX					_WT_MAX(WT_DEFBASE)

#endif


// typedef struct tagWTTILT { /* 1.1 */
// 	int tiltX;
// 	int tiltY;
// } WTTILT;

typedef struct tagWTORIENTATION {
	int orAzimuth;
	int orAltitude;
	int orTwist;
} WTORIENTATION;//, *PWTORIENTATION, NEAR *NPWTORIENTATION, FAR *LPWTORIENTATION;

typedef struct tagWTROTATION { /* 1.1 */
	int roPitch;
	int roRoll;
	int roYaw;
} WTROTATION;//, *PWTROTATION, NEAR *NPWTROTATION, FAR *LPWTROTATION;

//  SDL_PEN_AXIS_PRESSURE,  /**< Pen pressure.  Unidirectional: 0 to 1.0 */
//  SDL_PEN_AXIS_XTILT,     /**< Pen horizontal tilt angle.  Bidirectional: -90.0 to 90.0 (left-to-right). */
//  SDL_PEN_AXIS_YTILT,     /**< Pen vertical tilt angle.  Bidirectional: -90.0 to 90.0 (top-to-down). */
//  SDL_PEN_AXIS_DISTANCE,  /**< Pen distance to drawing surface.  Unidirectional: 0.0 to 1.0 */
//  SDL_PEN_AXIS_ROTATION,  /**< Pen barrel rotation.  Bidirectional: -180 to 179.9 (clockwise, 0 is facing up, -180.0 is facing down). */
//  SDL_PEN_AXIS_SLIDER,    /**< Pen finger wheel or slider (e.g., Airbrush Pen).  Unidirectional: 0 to 1.0 */
//  SDL_PEN_AXIS_TANGENTIAL_PRESSURE,    /**< Pressure from squeezing the pen ("barrel pressure"). */
//  SDL_PEN_AXIS_COUNT       /**< Total known pen axis types in this version of SDL. This number may grow in future releases! */

// #define PACKETDATA (PK_CURSOR | PK_X | PK_Y | PK_Z | PK_ROTATION | PK_NORMAL_PRESSURE |  PK_BUTTONS |  | PK_TANGENT_PRESSURE | PK_ORIENTATION )

typedef struct tagWINTAB_PACKET {
   // UINT           pkCursor;   
   DWORD          pkButtons;
   LONG           pkX;
   LONG           pkY;
   // LONG           pkZ;
   UINT           pkNormalPressure;
   // UINT           pkTangentPressure;
   // WTORIENTATION  pkOrientation; /* replace tilt (needs convertion) */
   // WTROTATION		pkRotation;
   // WTTILT         pkTilt;
} WINTAB_PACKET;

extern bool SDL_IsWintabEnabled(void);

extern bool SDL_InitWintab(void);
extern void SDL_QuitWintab(void);

extern bool SDL_OpenWintabCtx(HWND hwnd);
extern void SDL_CloseWintabCtx(HWND hwnd);

extern SDL_PenInfo SDL_GetWintabPenInfo();
extern bool SDL_GetWintabPacket(HCTX hCtx, UINT wSerial, WINTAB_PACKET* packet);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // SDL_windowswintab_h_

#endif // SDL_VIDEO_DRIVER_WINDOWS