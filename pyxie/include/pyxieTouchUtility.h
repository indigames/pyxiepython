///////////////////////////////////////////////////////////////
//Pyxie game engine
//
//  Copyright Kiharu Shishikura 2019. All rights reserved.
///////////////////////////////////////////////////////////////
#pragma once

typedef struct SceTouchPanelInfo {
	int16_t minAaX;   /*E Minimum Active Area X Pos */
	int16_t minAaY;   /*E Minimum Active Area Y Pos */
	int16_t maxAaX;   /*E Maximum Active Area X Pos */
	int16_t maxAaY;   /*E Maximum Active Area Y Pos */
	int16_t minDispX; /*E Minimum Display X Pos     */
	int16_t minDispY; /*E Minimum Display Y Pos     */
	int16_t maxDispX; /*E Maximum Display X Pos     */
	int16_t maxDispY; /*E Maximum Display Y Pos     */
	uint8_t minForce; /*E Minimum touch force value */
	uint8_t maxForce; /*E Maximum touch force value */
	uint8_t rsv[30];  /*E reserved                  */
} SceTouchPanelInfo;

typedef struct SceTouchReport {
	uint32_t  id;		/*E Touch ID    */
	uint8_t  force;		/*E Touch Force */
	int16_t  x;			/*E X position  */
	int16_t  y;			/*E Y position  */
	uint64_t finger;
	int8_t   rsv[2];	/*E reserved    */
} SceTouchReport;

#define SCE_TOUCH_MAX_REPORT		10
typedef struct SceTouchData {
	uint64_t      timeStamp;					/*E Time stamp of this data. */
	uint32_t      status;						/*E Currently unused.        */
	uint32_t      reportNum;					/*E Num of touch reports     */
	SceTouchReport report[SCE_TOUCH_MAX_REPORT];/*E Touch reports            */
} SceTouchData;


#include "pyxieTouchManager.h"

/*E px per mm */
#define TOUCH_UTIL_PIXELS_PER_MM (10.0f)		// (px/mm)

/*E Maximum number of basic event */
#define TOUCH_UTIL_MAX_EVENT   (SCE_TOUCH_MAX_REPORT * 2)

/*E Default value of move threshold */
#ifndef __ANDROID__
#define TOUCH_UTIL_THRESHOLD_MOVE  2	// (mm)
#else
#define TOUCH_UTIL_THRESHOLD_MOVE  6	// (mm)
#endif
#define TOUCH_UTIL_THRESHOLD_TAP_TIME 400		// (msec)
#define TOUCH_UTIL_THRESHOLD_DTAP_TIME 0		// (msec)
#define TOUCH_UTIL_THRESHOLD_DTAP_DIST 40		// (mm)
#define TOUCH_UTIL_THRESHOLD_PINCH_DIST 20		// (mm)
#define TOUCH_UTIL_THRESHOLD_FLICK_VELO 30		// (mm/sec)
#define TOUCH_UTIL_THRESHOLD_ROTATE_DEG 5		// (deg)
#define TOUCH_UTIL_THRESHOLD_LONG_PRESS_TIME 1000 // (msec)


namespace pyxie
{
	/*E Basic Touch Event Information */
	typedef struct TouchUtilInfo
	{
		// fixed properties
		float			pixelPerMM;
		float			diagonalLen;

		// variable properties
		uint32_t		reportNum;
		uint64_t		updateTime;		/*E Last update time              */

		SingleFingerEvent* pFree;			/*E Free Resources                */
		SingleFingerEvent* pPress;			/*E "PRESS"   state Touch.        */
		SingleFingerEvent* pHold;			/*E "HOLD"    state Touch.        */
		SingleFingerEvent* pMove;			/*E "MOVE"    state Touch.        */
		SingleFingerEvent* pRelease;		/*E "RELEASE" state Touch         */
		SingleFingerEvent  eventElem[TOUCH_UTIL_MAX_EVENT];

		SingleFingerEvent* pSingleFingerEvent;	/*E Single Finger Event Queue */
		DualFingerEvent dualFingerEventInfo;	/*E Dual Finger Event Info */

		// settings
		uint8_t			useMultiTap;
		uint32_t		moveThreshold;			/*E The threshold of "MOVE" state */
		uint32_t		tapTimeThreshold;		/*E threshold of "Tap" (time(msec) between PRESS and RELEASE) */
		uint32_t		dtapTimeThreshold;      /*E threshold of "Double Tap" (time(msec) between 1st Tap and 2nd Tap's PRESS) */
		uint32_t		dtapDistThreshold;      /*E threshold of "Double Tap" (distance(mm) between taps) */
		uint32_t		pinchDistThreshold;     /*E threshold of "Pinch" (ditance(mm) change to start PINCH) */
		uint32_t		flickVeloThreshold;     /*E threshold of "Flick" (square of velocity (mm/sec) to start FLICK) */
		uint32_t		rotateThreshold;        /*E threshold of "Rotate" (rotation(deg) to start ROTATE) */
		uint32_t		longPressTimeThreshold; /*E threshold of "Press&Hold" (time(msec) for being "long") */
		uint32_t		tapMicroTimeThreshold;  /*E tapTimeThreshold in microseconds */
		uint32_t		dtapMicroTimeThreshold;         /*E dtapTimeThreshold in microseconds */
		uint32_t		moveThresholdSq;                /*E square of moveThreshold */
		uint32_t		dtapDistThresholdSq;            /*E square of dtapDistThreshold */
		uint32_t		pinchDistThresholdInPx;         /*E pinchDistThreshold (px) */
		float			flickVeloThresholdSq;           /*E square of flickVeloThreshold(mm/microsec) */
		float			rotateThresholdRad;             /*E rotateion threshold (rad) */
		uint32_t		longPressMicroTimeThreshold;	/*E threshold of "Press&Hold" (microsec) */
	} TouchUtilInfo;


	/*E Initialize the TouchUtilInfo Structure. */
	int touchUtilInit(TouchUtilInfo* pUilInfo, SceTouchPanelInfo* pPanelInfo);

	/*E Updates the event information. */
	int touchUtilUpdate(TouchUtilInfo* pUilInfo, const SceTouchData* pData);


	int touchUtilSetMultiTapAvailability(TouchUtilInfo* pUilInfo, uint8_t useMultiTap);

	/*E Set threshold to judge "MOVE" or "HOLD" event. (Default:10) */
	int touchUtilSetMoveThreshold(TouchUtilInfo* pUilInfo, uint32_t threshold);
	int touchUtilSetTapThreshold(TouchUtilInfo* pUilInfo, uint32_t threshold);
	int touchUtilSetDoubleTapThreshold(TouchUtilInfo* pUilInfo, uint32_t timeThreshold, uint32_t distThreshold);
	int touchUtilSetPinchThreshold(TouchUtilInfo* pUilInfo, uint32_t threshold);
	int touchUtilSetFlickThreshold(TouchUtilInfo* pUilInfo, uint32_t threshold);
	int touchUtilSetRotateThreshold(TouchUtilInfo* pUilInfo, uint32_t threshold);
	int touchUtilSetLongPressThreshold(TouchUtilInfo* pUtilInfo, uint32_t threshold);

	float touchUtilGetPinchValueAbsolute(TouchUtilInfo* pUilInfo);
	float touchUtilGetPinchValueRelative(TouchUtilInfo* pUilInfo);

	float touchUtilGetRotationValueAbsolute(TouchUtilInfo* pUilInfo);
	float touchUtilGetRotationValueRelative(TouchUtilInfo* pUilInfo);


	/*E Dump Event Element */
	void touchUtilDumpElemQueue(SingleFingerEvent* pEventElem);

	int touchUtilGetNumInQueue(SingleFingerEvent* pEventElem);
}

