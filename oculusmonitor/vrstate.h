////////////////////////////////////////////////////////////
// Oculus Monitor
// Copyright (C) 2018 Kojack (rajetic@gmail.com)
//
// KF is released under the MIT License  
// https://opensource.org/licenses/MIT
////////////////////////////////////////////////////////////

#pragma once
#include "OVR_CAPI.h"
#include "Extras/OVR_Math.h"
#include <vector>

struct VRState
{
	float time;
	unsigned int remoteButtons;
	unsigned int touchButtons;
	unsigned int touchTouch;
	ovrVector2f touchThumbStick[2];
	float touchHandTrigger[2];
	float touchIndexTrigger[2];
	ovrTrackingState trackingState;
	unsigned int sensorCount;
	ovrTrackerPose sensorPose[4];
	ovrTrackerDesc sensorDesc[4];
};

struct Keyframe
{
	double time;
	OVR::Vector3f position;
	OVR::Quatf orientation;
};

class StateManager
{
public:
	enum PollState
	{
		e_live,
		e_playback,
		e_record
	};

	std::vector<VRState> m_samples;
	double m_time;
	PollState m_pollState;
	int m_current;

	StateManager();
	VRState poll(ovrSession hmd, double time);
	void reset();
	void writeDAECamera(std::fstream &out, std::string name, float hfov, float vfov, float near, float far);
	void writeDAEPositions(std::fstream &out, std::vector<Keyframe> &keys, std::string name);
	void writeDAEOrientation(std::fstream &out, std::vector<Keyframe> &keys, std::string name);
	void exportCSV(const std::string &filename);
	void exportDAE(ovrSession hmd, const std::string &filename);

};
