////////////////////////////////////////////////////////////
// Oculus Monitor
// Copyright (C) 2018 Kojack (rajetic@gmail.com)
//
// KF is released under the MIT License  
// https://opensource.org/licenses/MIT
////////////////////////////////////////////////////////////

#include "vrstate.h"

StateManager::StateManager() : m_pollState(e_live), m_current(0)
{
	m_samples.resize(216000);
}

VRState StateManager::poll(ovrSession hmd, double time)
{
	VRState state;
	if (m_pollState != e_playback)
	{
		state.trackingState = ovr_GetTrackingState(hmd, 0, false);
		ovrInputState temp;
		ovr_GetInputState(hmd, ovrControllerType::ovrControllerType_Remote, &temp);
		state.remoteButtons = temp.Buttons;
		ovr_GetInputState(hmd, ovrControllerType::ovrControllerType_Touch, &temp);
		state.touchButtons = temp.Buttons;
		state.touchTouch = temp.Touches;
		state.touchHandTrigger[0] = temp.HandTrigger[0];
		state.touchHandTrigger[1] = temp.HandTrigger[1];
		state.touchIndexTrigger[0] = temp.IndexTrigger[0];
		state.touchIndexTrigger[1] = temp.IndexTrigger[1];
		state.touchThumbStick[0] = temp.Thumbstick[0];
		state.touchThumbStick[1] = temp.Thumbstick[1];

		state.sensorCount = ovr_GetTrackerCount(hmd);
		for (unsigned int i = 0; i < state.sensorCount; ++i)
		{
			state.sensorDesc[i] = ovr_GetTrackerDesc(hmd, i);
			state.sensorPose[i] = ovr_GetTrackerPose(hmd, i);
		}
		state.time = time;
	}
	else
	{
		while (true)
		{
			if (m_samples[m_current].time <= time)
			{
				if (m_samples[m_current + 1].time > time)
				{
					state = m_samples[m_current];
					break;
				}
				else
				{
					m_current++;
					if (m_current > m_samples.size() - 2)
					{
						m_current = m_samples.size() - 2;
						state = m_samples[m_current];
						break;
					}
				}
			}
			else
			{
				m_current--;
				if (m_current < 0)
				{
					m_current = 0;
					state = m_samples[m_current];
					break;
				}
			}
		}
	}

	if (m_pollState == e_record)
	{
		m_samples.push_back(state);
	}

	return state;
}

void StateManager::reset()
{
	m_samples.clear();
	m_current = 0;
	m_pollState = e_live;
}