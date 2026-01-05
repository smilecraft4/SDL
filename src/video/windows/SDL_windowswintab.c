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

#include "SDL_windowswintab.h"

#ifndef WTAPI
#ifndef WINAPI
#define WTAPI FAR PASCAL
#else
#define WTAPI WINAPI
#endif
#endif

typedef DWORD WTPKT; /* packet mask */
typedef DWORD FIX32; /* fixed-point arithmetic type */

/* info categories */
#define WTI_INTERFACE   1
#define IFC_WINTABID    1
#define IFC_SPECVERSION 2
#define IFC_IMPLVERSION 3
#define IFC_NDEVICES    4
#define IFC_NCURSORS    5
#define IFC_NCONTEXTS   6
#define IFC_CTXOPTIONS  7
#define IFC_CTXSAVESIZE 8
#define IFC_NEXTENSIONS 9
#define IFC_NMANAGERS   10
#define IFC_MAX         10

#define WTI_DEFCONTEXT 3
#define WTI_DEFSYSCTX  4
#define WTI_DDCTXS     400 /* 1.1 */
#define WTI_DSCTXS     500 /* 1.1 */
#define CTX_NAME       1
#define CTX_OPTIONS    2
#define CTX_STATUS     3
#define CTX_LOCKS      4
#define CTX_MSGBASE    5
#define CTX_DEVICE     6
#define CTX_PKTRATE    7
#define CTX_PKTDATA    8
#define CTX_PKTMODE    9
#define CTX_MOVEMASK   10
#define CTX_BTNDNMASK  11
#define CTX_BTNUPMASK  12
#define CTX_INORGX     13
#define CTX_INORGY     14
#define CTX_INORGZ     15
#define CTX_INEXTX     16
#define CTX_INEXTY     17
#define CTX_INEXTZ     18
#define CTX_OUTORGX    19
#define CTX_OUTORGY    20
#define CTX_OUTORGZ    21
#define CTX_OUTEXTX    22
#define CTX_OUTEXTY    23
#define CTX_OUTEXTZ    24
#define CTX_SENSX      25
#define CTX_SENSY      26
#define CTX_SENSZ      27
#define CTX_SYSMODE    28
#define CTX_SYSORGX    29
#define CTX_SYSORGY    30
#define CTX_SYSEXTX    31
#define CTX_SYSEXTY    32
#define CTX_SYSSENSX   33
#define CTX_SYSSENSY   34
#define CTX_MAX        34

#define WTI_DEVICES     100
#define DVC_NAME        1
#define DVC_HARDWARE    2
#define DVC_NCSRTYPES   3
#define DVC_FIRSTCSR    4
#define DVC_PKTRATE     5
#define DVC_PKTDATA     6
#define DVC_PKTMODE     7
#define DVC_CSRDATA     8
#define DVC_XMARGIN     9
#define DVC_YMARGIN     10
#define DVC_ZMARGIN     11
#define DVC_X           12
#define DVC_Y           13
#define DVC_Z           14
#define DVC_NPRESSURE   15
#define DVC_TPRESSURE   16
#define DVC_ORIENTATION 17
#define DVC_ROTATION    18 /* 1.1 */
#define DVC_PNPID       19 /* 1.1 */
#define DVC_MAX         19

#ifndef NOWTCAPABILITIES

/* hardware capabilities */
#define HWC_INTEGRATED     0x0001
#define HWC_TOUCH          0x0002
#define HWC_HARDPROX       0x0004
#define HWC_PHYSID_CURSORS 0x0008 /* 1.1 */
#endif

#ifndef NOWTCURSORS

#define WTI_CURSORS      200
#define CSR_NAME         1
#define CSR_ACTIVE       2
#define CSR_PKTDATA      3
#define CSR_BUTTONS      4
#define CSR_BUTTONBITS   5
#define CSR_BTNNAMES     6
#define CSR_BUTTONMAP    7
#define CSR_SYSBTNMAP    8
#define CSR_NPBUTTON     9
#define CSR_NPBTNMARKS   10
#define CSR_NPRESPONSE   11
#define CSR_TPBUTTON     12
#define CSR_TPBTNMARKS   13
#define CSR_TPRESPONSE   14
#define CSR_PHYSID       15 /* 1.1 */
#define CSR_MODE         16 /* 1.1 */
#define CSR_MINPKTDATA   17 /* 1.1 */
#define CSR_MINBUTTONS   18 /* 1.1 */
#define CSR_CAPABILITIES 19 /* 1.1 */
#define CSR_TYPE         20 /* 1.2 */
#define CSR_MAX          20

#endif

#ifndef NOWTPKT

/* WTPKT bits */
#define PK_CONTEXT          0x0001 /* reporting context */
#define PK_STATUS           0x0002 /* status bits */
#define PK_TIME             0x0004 /* time stamp */
#define PK_CHANGED          0x0008 /* change bit vector */
#define PK_SERIAL_NUMBER    0x0010 /* packet serial number */
#define PK_CURSOR           0x0020 /* reporting cursor */
#define PK_BUTTONS          0x0040 /* button information */
#define PK_X                0x0080 /* x axis */
#define PK_Y                0x0100 /* y axis */
#define PK_Z                0x0200 /* z axis */
#define PK_NORMAL_PRESSURE  0x0400 /* normal or tip pressure */
#define PK_TANGENT_PRESSURE 0x0800 /* tangential or barrel pressure */
#define PK_ORIENTATION      0x1000 /* orientation info: tilts */
#define PK_ROTATION         0x2000 /* rotation info; 1.1 */
#endif

#define LCNAMELEN  40
#define LC_NAMELEN 40
typedef struct tagLOGCONTEXTA
{
    char lcName[LCNAMELEN];
    UINT lcOptions;
    UINT lcStatus;
    UINT lcLocks;
    UINT lcMsgBase;
    UINT lcDevice;
    UINT lcPktRate;
    WTPKT lcPktData;
    WTPKT lcPktMode;
    WTPKT lcMoveMask;
    DWORD lcBtnDnMask;
    DWORD lcBtnUpMask;
    LONG lcInOrgX;
    LONG lcInOrgY;
    LONG lcInOrgZ;
    LONG lcInExtX;
    LONG lcInExtY;
    LONG lcInExtZ;
    LONG lcOutOrgX;
    LONG lcOutOrgY;
    LONG lcOutOrgZ;
    LONG lcOutExtX;
    LONG lcOutExtY;
    LONG lcOutExtZ;
    FIX32 lcSensX;
    FIX32 lcSensY;
    FIX32 lcSensZ;
    BOOL lcSysMode;
    int lcSysOrgX;
    int lcSysOrgY;
    int lcSysExtX;
    int lcSysExtY;
    FIX32 lcSysSensX;
    FIX32 lcSysSensY;
} LOGCONTEXTA, *PLOGCONTEXTA, NEAR *NPLOGCONTEXTA, FAR *LPLOGCONTEXTA;

/* context option values */
#define CXO_SYSTEM      0x0001
#define CXO_PEN         0x0002
#define CXO_MESSAGES    0x0004
#define CXO_MARGIN      0x8000
#define CXO_MGNINSIDE   0x4000
#define CXO_CSRMESSAGES 0x0008 /* 1.1 */

// TODO: Add supports for extensions
// #ifndef NOWTEXTENSIONS

// /* Extension tags. */
// #define WTX_TILT 2	/* Raw Cartesian tilt; 1.1 */
// #endif

#define PACKETDATA (PK_X | PK_Y | PK_BUTTONS | PK_NORMAL_PRESSURE)
#define PACKETMODE PK_BUTTONS

typedef struct tagAXIS
{
    LONG axMin;
    LONG axMax;
    UINT axUnits;
    FIX32 axResolution;
} AXIS, *PAXIS, NEAR *NPAXIS, FAR *LPAXIS;

typedef UINT(WTAPI *WTINFO)(UINT wCategory, UINT nIndex, LPVOID lpOutput);
typedef HCTX(WTAPI *WTOPEN)(HWND hWnd, LPLOGCONTEXTA lpLogCtx, BOOL fEnable);
typedef BOOL(WTAPI *WTCLOSE)(HCTX hCtx);
typedef BOOL(WTAPI* WTPACKET)(HCTX hCtx, UINT wSerial, LPVOID lpPkt);

static HINSTANCE wintab = NULL;

static WTINFO WTInfo = NULL;
static WTOPEN WTOpen = NULL;
static WTCLOSE WTClose = NULL;
static WTPACKET WTPacket = NULL;

static float sys_width = 0.0f;
static float sys_height = 0.0f;

typedef struct tagHCTX_INFO {
    LOGCONTEXTA lctx;
    char* name;
    HWND hwnd;
    HCTX hctx;
    AXIS x;
    AXIS y;
    AXIS pressure;
} HCTX_INFO, *PHCTX_INFO;

static UINT attached_devices = 0;
static SDL_HashTable *hctx_table = NULL;
static bool is_wintab_enabled = false;

bool SDL_IsWintabEnabled(void)
{
    return is_wintab_enabled;
}

bool SDL_InitWintab(void)
{
    SDL_assert(wintab == NULL && "Wintab32.dll already loaded");

    wintab = LoadLibraryA("Wintab32.dll");
    if (wintab == NULL) {
        SDL_SetError("Failed to load Wintab32.dll");
        return false;
    }

    WTInfo = (WTINFO)GetProcAddress(wintab, "WTInfoA");
    SDL_assert(WTInfo != NULL);
    WTOpen = (WTOPEN)GetProcAddress(wintab, "WTOpenA");
    SDL_assert(WTOpen != NULL);
    WTClose = (WTCLOSE)GetProcAddress(wintab, "WTClose");
    SDL_assert(WTClose != NULL);
    WTPacket = (WTPACKET)GetProcAddress(wintab, "WTPacket");
    SDL_assert(WTPacket != NULL);

    if (!WTInfo(0, 0, NULL)) {
        SDL_SetError("Wintab unavailable");
        return false;
    }

    hctx_table = SDL_CreateHashTable(0, true, SDL_HashPointer, SDL_KeyMatchPointer, NULL, NULL);
    is_wintab_enabled = true;

    return true;
}

void SDL_QuitWintab(void)
{
    SDL_assert(SDL_HashTableEmpty(hctx_table) && "Not all wintab context are closed");

    SDL_DestroyHashTable(hctx_table);
    is_wintab_enabled = false;

    if (wintab) {
        FreeLibrary(wintab);
        wintab = NULL;
    }

    WTInfo = NULL;
    WTOpen = NULL;
    WTClose = NULL;
    WTPacket = NULL;
}

bool SDL_OpenWintabCtx(HWND hwnd)
{
    // TODO: Respond to WT_CTXCLOSE & WT_CTXOPEN
    SDL_assert(hwnd != NULL && "hwnd must be valid");
    SDL_assert(hctx_table != NULL && "context table is invalid");

    int ctx_idx = 0;
    char *device_name = NULL;

    WTInfo(WTI_INTERFACE, IFC_NDEVICES, &attached_devices);
    SDL_Log("wintab attached devices num: %u", attached_devices);

    LOGCONTEXTA lctx = { 0 };
    const UINT ctx_found = WTInfo(WTI_DEFSYSCTX, 0, &lctx);
    if (ctx_found > 0) {
        UINT result = 0;
        UINT ret = 0;
        AXIS x_axis = { 0 };
        AXIS y_axis = { 0 };
        AXIS normal_pressure_axis = { 0 };

        ret = WTInfo(WTI_DEVICES + ctx_idx, DVC_HARDWARE, &result);
        const bool is_display_tablet = result & HWC_INTEGRATED;
        const bool unique_cursors = result & HWC_PHYSID_CURSORS;

        ret = WTInfo(WTI_DEVICES + ctx_idx, DVC_PKTRATE, &result);
        const UINT packet_rate = result;

        char name[1024];
        SDL_memset(name, '/0', sizeof(name));
        ret = WTInfo(WTI_DEVICES + ctx_idx, DVC_NAME, &name);
        SDL_Log("Context name: %s", name);

        lctx.lcPktData = PACKETDATA;
        lctx.lcOptions |= CXO_MESSAGES; // send WindowProc messsages
        lctx.lcOptions |= CXO_SYSTEM;   // move the system cursor
        lctx.lcPktMode = PACKETMODE;
        lctx.lcMoveMask = PACKETDATA;
        lctx.lcBtnUpMask = lctx.lcBtnDnMask;

        ret = WTInfo(WTI_DEVICES + ctx_idx, DVC_X, &x_axis);
        if (ret != sizeof(AXIS)) {
        SDL_SetError("Wintab does not have a X axis");
            return false;
        }

        ret = WTInfo(WTI_DEVICES + ctx_idx, DVC_Y, &y_axis);
        if (ret != sizeof(AXIS)) {
        SDL_SetError("Wintab does not have a Y axis");
            return false;
        }

        ret = WTInfo(WTI_DEVICES + ctx_idx, DVC_NPRESSURE, &normal_pressure_axis);
        if (ret != sizeof(AXIS)) {
        SDL_SetError("Wintab does not have a pressure axis");
            return false;
        }

        lctx.lcOutExtX = x_axis.axMax - x_axis.axMin + 1;
        lctx.lcOutExtY = y_axis.axMax - y_axis.axMin + 1;
        // lctx.lcOutExtX++;
        lctx.lcOutExtY = -lctx.lcOutExtY; // move origin to top left

        HCTX hctx = WTOpen(hwnd, &lctx, true);
        if (hctx) {

            PHCTX_INFO pinfo = (PHCTX_INFO)SDL_malloc(sizeof(HCTX_INFO));
            SDL_zero(*pinfo);

            pinfo->lctx = lctx;
            pinfo->hwnd = hwnd;
            pinfo->hctx = hctx;
            pinfo->name = SDL_strdup(name);
            pinfo->x = x_axis;
            pinfo->y = y_axis;
            pinfo->pressure = normal_pressure_axis;

            const bool success = SDL_InsertIntoHashTable(hctx_table, hwnd, pinfo, false);
            SDL_assert(success && "A wintab context already exists for this hwnd");
        }
    } else {
        SDL_SetError("No wintab context available");
        return false;
    }

    return true;
}

void SDL_CloseWintabCtx(HWND hwnd)
{
    PHCTX_INFO pinfo;
    if(SDL_FindInHashTable(hctx_table, hwnd, &pinfo)) {
        WTClose(pinfo->hctx);
        SDL_free(pinfo->name);
        SDL_RemoveFromHashTable(hctx_table, hwnd);
        SDL_free(pinfo);
    }
}

SDL_PenInfo SDL_GetWintabPenInfo()
{
    SDL_assert(SDL_IsWintabEnabled());

    SDL_PenInfo info;
    SDL_zero(info);
    info.capabilities = SDL_PEN_CAPABILITY_PRESSURE | SDL_PEN_CAPABILITY_XTILT | SDL_PEN_CAPABILITY_YTILT | SDL_PEN_CAPABILITY_DISTANCE | SDL_PEN_CAPABILITY_ROTATION | SDL_PEN_CAPABILITY_ERASER;
    info.max_tilt = 90.0f;
    info.num_buttons = 1;
    info.subtype = SDL_PEN_TYPE_PENCIL;

    // TODO: Add proper detections of pen capabalities
    // How to make sure we are checking the correct pen in multipen setup (if they exists)
    char name;
    WTInfo(WTI_CURSORS, CSR_NAME, &name);

    BYTE buttons;
    WTInfo(WTI_CURSORS, CSR_BUTTONS, &buttons);

    WTPKT packets;
    WTInfo(WTI_CURSORS, CSR_PKTDATA, &packets);

    DWORD id;
    WTInfo(WTI_CURSORS, CSR_PHYSID, &id);

    return info;
}

bool SDL_GetWintabPacket(HWND hwnd, HCTX hCtx, UINT wSerial, WINTAB_SDL_PACKET* packet)
{
    SDL_assert(SDL_IsWintabEnabled());

    WINTAB_PACKET wt_packet;
    if(!WTPacket(hCtx, wSerial, &wt_packet)) {
        SDL_SetError("Wintab packet not found in queue");
        return false;
    }

    PHCTX_INFO pinfo;
    bool found = SDL_FindInHashTable(hctx_table, hwnd, &pinfo);
    SDL_assert(found);

    // TODO: cache this 
	const float sysWidth = (float)GetSystemMetrics(SM_CXVIRTUALSCREEN);
	const float sysHeight = (float)GetSystemMetrics(SM_CYVIRTUALSCREEN);
	const float sysOrigX = (float)GetSystemMetrics(SM_XVIRTUALSCREEN);
	const float sysOrigY = (float)GetSystemMetrics(SM_YVIRTUALSCREEN);

    POINT position;
    position.x = 0;
    position.y = 0;
    ClientToScreen(hwnd, &position);

    packet->x = sysOrigX + (sysWidth * ((float)wt_packet.pkX / (float)pinfo->x.axMax)) - position.x;
    packet->y = sysOrigY + (sysHeight * ((float)wt_packet.pkY / (float)pinfo->y.axMax)) - position.y;
    packet->pressure = (float)wt_packet.pkNormalPressure / (float)pinfo->pressure.axMax;

    return true;
}
