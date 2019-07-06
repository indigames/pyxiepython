///////////////////////////////////////////////////////////////
//Pyxie game engine
//
//  Copyright Kiharu Shishikura 2019. All rights reserved.
///////////////////////////////////////////////////////////////

#pragma once
#include "pyxieTypes.h"
#include <vector>

namespace pyxie
{
	enum TOUCH_STATE
	{
		TOUCH_STATE_FREE = 0,
		TOUCH_STATE_PRESS = 1 << 0,
		TOUCH_STATE_HOLD = 1 << 1,
		TOUCH_STATE_MOVE = 1 << 2,
		TOUCH_STATE_RELEASE = 1 << 3,
		TOUCH_STATE_TAP = 1 << 4,
		TOUCH_STATE_LONGPRESS = 1 << 5,
		TOUCH_STATE_FLICK = 1 << 6,
	};

	struct SingleFingerEvent
	{
		uint32_t	state;
		uint32_t	id;				/*E Touch ID           */

		int16_t		org_x;			/*E Pressed x position */
		int16_t		org_y;			/*E Pressed y position */

		int16_t		cur_x;			/*E Current x position */
		int16_t		cur_y;			/*E Current y position */

		int16_t		delta_x;		/*E Delta x position   */
		int16_t		delta_y;		/*E Delta y position   */
		uint64_t	delta_t;		/*E Delta time         */
		uint64_t	elapsed_t;		/*E Elapsed time       */
		uint8_t		fast_motion_t;

		uint8_t		num_tap;		/*E number of taps */

		uint8_t		is_moved;		/*E is moved or not */
		uint8_t		is_tapped;		/*E is tapped or not */
		uint8_t		is_longpressed;
		uint8_t		is_tap_candidate;
		uint8_t		is_flicked;

		uint8_t		force;

		struct SingleFingerEvent* pNext;
		uint64_t	finger;
	};

	enum DUAL_FINGER_STATE
	{
		DUAL_FINGER_STATE_FREE = 0,
		DUAL_FINGER_STATE_BEGAN,
		DUAL_FINGER_STATE_HOLD,
		DUAL_FINGER_STATE_MOVE,
		DUAL_FINGER_STATE_ENDED,
	};

	struct DualFingerEvent
	{
		uint32_t			state;
		SingleFingerEvent*	pPair[2];
		float				pinchValueAbsolute;
		float				pinchValueRelative;
		float				rotateValueAbsolute;
		float				rotateValueRelative;
	};


	struct TouchEvent {
		uint64_t timestamp;
		uint32_t uid;
		SingleFingerEvent single;
		std::vector<SingleFingerEvent> singlepnextsave;
		void Fix()
		{
			for (size_t i = 0; i < singlepnextsave.size(); ++i)
			{
				if (i == 0)
				{
					single.pNext = &singlepnextsave[i];
				}
				else
				{
					singlepnextsave[i - 1].pNext = &singlepnextsave[i];
				}
				if (i == singlepnextsave.size() - 1)
				{
					singlepnextsave[i].pNext = 0;
				}
			}
		}
	};

	class PYXIE_EXPORT pyxieTouchManager
	{
	public:
		static pyxieTouchManager& Instance();
		uint32_t GetEventsNum();
		bool GetTouchEvent(uint32_t idx, TouchEvent& outevent);
		void ClearTouchEvents();
		~pyxieTouchManager();
		void Update();
		const SingleFingerEvent* GetFirstSingleFingerEvent();
		const SingleFingerEvent* GetNextSingleFingerEvent(const SingleFingerEvent* fingerEvent);
		const DualFingerEvent*	GetDualFingerEvent();
		void EnableTouchForce(int port, bool enable);
		bool IsTapped(int& x, int& y);
		void SetTouchParam(int taptime, int longpresstime, int move);
		void UpdateTouches(int command, int x, int y, long fingerid);
	protected:
		pyxieTouchManager();
		void TouchScreenUpdate();
	};

}

