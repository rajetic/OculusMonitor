////////////////////////////////////////////////////////////
// Oculus Monitor
// Copyright (C) 2018 Kojack (rajetic@gmail.com)
//
// KF is released under the MIT License  
// https://opensource.org/licenses/MIT
////////////////////////////////////////////////////////////

#include "vrstate.h"
#include <fstream>
#include <string>

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
		for (int i = 0; i < 2; ++i)
		{
			state.touchHandTrigger[i] = temp.HandTrigger[i];
			state.touchHandTriggerNDZ[i] = temp.HandTriggerNoDeadzone[i];
			state.touchHandTriggerRaw[i] = temp.HandTriggerRaw[i];
			state.touchIndexTrigger[i] = temp.IndexTrigger[i];
			state.touchIndexTriggerNDZ[i] = temp.IndexTriggerNoDeadzone[i];
			state.touchIndexTriggerRaw[i] = temp.IndexTriggerRaw[i];
			state.touchThumbStick[i] = temp.Thumbstick[i];
			state.touchThumbStickNDZ[i] = temp.ThumbstickNoDeadzone[i];
			state.touchThumbStickRaw[i] = temp.ThumbstickRaw[i];
		}
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

void StateManager::exportCSV(const std::string &filename)
{
	std::fstream out(filename, std::ios::out);
	out << "Time,RemoteButtons,TouchButtons,TouchTouches,LeftIndexTrigger,RightIndexTrigger,LeftHandTrigger,RightHandTrigger,LeftTouchPosX,LeftTouchPosY,LeftTouchPosZ,LeftTouchOrientationW,LeftTouchOrientationX,LeftTouchOrientationY,LeftTouchOrientationZ,RightTouchPosX,RightTouchPosY,RightTouchPosZ,RightTouchOrientationW,RightTouchOrientationX,RightTouchOrientationY,RightTouchOrientationZ,HeadPosX,HeadPosY,HeadPosZ,HeadOrientationW,HeadOrientationX,HeadOrientationY,HeadOrientationZ,Sensor0PosX, Sensor0PosY, Sensor0PosZ, Sensor0OrientationW, Sensor0OrientationX, Sensor0OrientationY, Sensor0OrientationZ,Sensor1PosX, Sensor1PosY, Sensor1PosZ, Sensor1OrientationW, Sensor1OrientationX, Sensor1OrientationY, Sensor1OrientationZ,Sensor2PosX, Sensor2PosY, Sensor2PosZ, Sensor2OrientationW, Sensor2OrientationX, Sensor2OrientationY, Sensor2OrientationZ,Sensor3PosX, Sensor3PosY, Sensor3PosZ, Sensor3OrientationW, Sensor3OrientationX, Sensor3OrientationY, Sensor3OrientationZ" << std::endl;
	for (int i = 0; i < m_samples.size(); ++i)
	{
		VRState &s = m_samples[i];
		out << s.time << ",";
		out << s.remoteButtons << ",";
		out << s.touchButtons << ",";
		out << s.touchTouch << ",";
		out << s.touchIndexTrigger[0] << ",";
		out << s.touchIndexTrigger[1] << ",";
		out << s.touchHandTrigger[0] << ",";
		out << s.touchHandTrigger[1] << ",";
		out << s.trackingState.HandPoses[0].ThePose.Position.x << ",";
		out << s.trackingState.HandPoses[0].ThePose.Position.y << ",";
		out << s.trackingState.HandPoses[0].ThePose.Position.z << ",";
		out << s.trackingState.HandPoses[0].ThePose.Orientation.w << ",";
		out << s.trackingState.HandPoses[0].ThePose.Orientation.x << ",";
		out << s.trackingState.HandPoses[0].ThePose.Orientation.y << ",";
		out << s.trackingState.HandPoses[0].ThePose.Orientation.z << ",";
		out << s.trackingState.HandPoses[1].ThePose.Position.x << ",";
		out << s.trackingState.HandPoses[1].ThePose.Position.y << ",";
		out << s.trackingState.HandPoses[1].ThePose.Position.z << ",";
		out << s.trackingState.HandPoses[1].ThePose.Orientation.w << ",";
		out << s.trackingState.HandPoses[1].ThePose.Orientation.x << ",";
		out << s.trackingState.HandPoses[1].ThePose.Orientation.y << ",";
		out << s.trackingState.HandPoses[1].ThePose.Orientation.z << ",";
		out << s.trackingState.HeadPose.ThePose.Position.x << ",";
		out << s.trackingState.HeadPose.ThePose.Position.y << ",";
		out << s.trackingState.HeadPose.ThePose.Position.z << ",";
		out << s.trackingState.HeadPose.ThePose.Orientation.w << ",";
		out << s.trackingState.HeadPose.ThePose.Orientation.x << ",";
		out << s.trackingState.HeadPose.ThePose.Orientation.y << ",";
		out << s.trackingState.HeadPose.ThePose.Orientation.z << ",";
		for (int j = 0; j < s.sensorCount; ++j)
		{
			out << s.sensorPose[j].Pose.Position.x << ",";
			out << s.sensorPose[j].Pose.Position.y << ",";
			out << s.sensorPose[j].Pose.Position.z << ",";
			out << s.sensorPose[j].Pose.Orientation.w << ",";
			out << s.sensorPose[j].Pose.Orientation.x << ",";
			out << s.sensorPose[j].Pose.Orientation.y << ",";
			out << s.sensorPose[j].Pose.Orientation.z << ",";
		}
		out << std::endl;
	}
}

void StateManager::writeDAECamera(std::fstream &out, std::string name, float hfov, float vfov, float near, float far)
{
	//tan(FOV_H / 2) / screen_width = tan(FOV_V / 2) / screen_height
	out << "		<camera id=\"" << name << "\" name=\"" << name << "\">" << std::endl;
	out << "			<optics>" << std::endl;
	out << "				<technique_common>" << std::endl;
	out << "					<perspective>" << std::endl;
	out << "						<xfov sid=\"xfov\">" << hfov * (180.0 / 3.14159265) << "</xfov>" << std::endl;
	out << "						<yfov sid=\"yfov\">" << vfov * (180.0 / 3.14159265) << "</yfov>" << std::endl;
	out << "						<znear sid=\"znear\">" << near << "</znear>" << std::endl;
	out << "						<zfar sid=\"zfar\">" << far << "</zfar>" << std::endl;
	out << "					</perspective>" << std::endl;
	out << "				</technique_common>" << std::endl;
	out << "			</optics>" << std::endl;
	out << "			<extra>" << std::endl;
	out << "				<technique profile=\"blender\">" << std::endl;
	out << "					<shiftx sid=\"shiftx\" type=\"float\">0</shiftx>" << std::endl;
	out << "					<shifty sid=\"shifty\" type=\"float\">0</shifty>" << std::endl;
	out << "					<YF_dofdist sid=\"YF_dofdist\" type=\"float\">0</YF_dofdist>" << std::endl;
	out << "				</technique>" << std::endl;
	out << "			</extra>" << std::endl;
	out << "		</camera>" << std::endl;
}

void StateManager::writeDAEPositions(std::fstream &out, std::vector<Keyframe> &keys, std::string name)
{
	int count = keys.size();
	// X
	out << "	<animation id=\"" << name << "_location_X\">" << std::endl;
	out << "		<source id=\"" << name << "_location_X-input\">" << std::endl;
	out << "			<float_array id=\"" << name << "_location_X-input-array\" count=\"" << count << "\">";
	for (int i = 0; i < count; ++i)
	{
		out << keys[i].time << " ";
	}
	out << "</float_array>" << std::endl;
	out << "			<technique_common>" << std::endl;
	out << "				<accessor source=\"#" << name << "_location_X-input-array\" count=\"" << count << "\" stride=\"1\">" << std::endl;
	out << "					<param name=\"TIME\" type=\"float\"/>" << std::endl;
	out << "				</accessor>" << std::endl;
	out << "			</technique_common>" << std::endl;
	out << "		</source>" << std::endl;
	out << "		<source id=\"" << name << "_location_X-output\">" << std::endl;
	out << "			<float_array id=\"" << name << "_location_X-output-array\" count=\"" << count << "\">";
	for (int i = 0; i < count; ++i)
	{
		out << keys[i].position.x << " ";
	}
	out << "</float_array>" << std::endl;
	out << "			<technique_common>" << std::endl;
	out << "				<accessor source=\"#" << name << "_location_X-output-array\" count=\"" << count << "\" stride=\"1\">" << std::endl;
	out << "					<param name=\"X\" type=\"float\"/>" << std::endl;
	out << "				</accessor>" << std::endl;
	out << "			</technique_common>" << std::endl;
	out << "		</source>" << std::endl;
	out << "		<source id=\"" << name << "_location_X-interpolation\">" << std::endl;
	out << "			<Name_array id=\"" << name << "_location_X-interpolation-array\" count=\"" << count << "\">";
	for (int i = 0; i < count; ++i)
	{
		"LINEAR ";
	}
	out << "</Name_array>" << std::endl;
	out << "			<technique_common>" << std::endl;
	out << "				<accessor source=\"#" << name << "_location_X-interpolation-array\" count=\"" << count << "\" stride=\"1\">" << std::endl;
	out << "					<param name=\"INTERPOLATION\" type=\"name\"/>" << std::endl;
	out << "				</accessor>" << std::endl;
	out << "			</technique_common>" << std::endl;
	out << "		</source>" << std::endl;
	out << "		<sampler id=\"" << name << "_location_X-sampler\">" << std::endl;
	out << "			<input semantic=\"INPUT\" source=\"#" << name << "_location_X-input\"/>" << std::endl;
	out << "			<input semantic=\"OUTPUT\" source=\"#" << name << "_location_X-output\"/>" << std::endl;
	out << "			<input semantic=\"INTERPOLATION\" source=\"#" << name << "_location_X-interpolation\"/>" << std::endl;
	out << "		</sampler>" << std::endl;
	out << "		<channel source=\"#" << name << "_location_X-sampler\" target=\"" << name << "/location.X\"/>" << std::endl;
	out << "	</animation>" << std::endl;

	// Y
	out << "	<animation id=\"" << name << "_location_Y\">" << std::endl;
	out << "		<source id=\"" << name << "_location_Y-input\">" << std::endl;
	out << "			<float_array id=\"" << name << "_location_Y-input-array\" count=\"" << count << "\">";
	for (int i = 0; i < count; ++i)
	{
		out << keys[i].time << " ";
	}
	out << "</float_array>" << std::endl;
	out << "			<technique_common>" << std::endl;
	out << "				<accessor source=\"#" << name << "_location_Y-input-array\" count=\"" << count << "\" stride=\"1\">" << std::endl;
	out << "					<param name=\"TIME\" type=\"float\"/>" << std::endl;
	out << "				</accessor>" << std::endl;
	out << "			</technique_common>" << std::endl;
	out << "		</source>" << std::endl;
	out << "		<source id=\"" << name << "_location_Y-output\">" << std::endl;
	out << "			<float_array id=\"" << name << "_location_Y-output-array\" count=\"" << count << "\">";
	for (int i = 0; i < count; ++i)
	{
		out << -keys[i].position.z << " ";
	}
	out << "</float_array>" << std::endl;
	out << "			<technique_common>" << std::endl;
	out << "				<accessor source=\"#" << name << "_location_Y-output-array\" count=\"" << count << "\" stride=\"1\">" << std::endl;
	out << "					<param name=\"Y\" type=\"float\"/>" << std::endl;
	out << "				</accessor>" << std::endl;
	out << "			</technique_common>" << std::endl;
	out << "		</source>" << std::endl;
	out << "		<source id=\"" << name << "_location_Y-interpolation\">" << std::endl;
	out << "			<Name_array id=\"" << name << "_location_Y-interpolation-array\" count=\"" << count << "\">";
	for (int i = 0; i < count; ++i)
	{
		"LINEAR ";
	}
	out << "</Name_array>" << std::endl;
	out << "			<technique_common>" << std::endl;
	out << "				<accessor source=\"#" << name << "_location_Y-interpolation-array\" count=\"" << count << "\" stride=\"1\">" << std::endl;
	out << "					<param name=\"INTERPOLATION\" type=\"name\"/>" << std::endl;
	out << "				</accessor>" << std::endl;
	out << "			</technique_common>" << std::endl;
	out << "		</source>" << std::endl;
	out << "		<sampler id=\"" << name << "_location_Y-sampler\">" << std::endl;
	out << "			<input semantic=\"INPUT\" source=\"#" << name << "_location_Y-input\"/>" << std::endl;
	out << "			<input semantic=\"OUTPUT\" source=\"#" << name << "_location_Y-output\"/>" << std::endl;
	out << "			<input semantic=\"INTERPOLATION\" source=\"#" << name << "_location_Y-interpolation\"/>" << std::endl;
	out << "		</sampler>" << std::endl;
	out << "		<channel source=\"#" << name << "_location_Y-sampler\" target=\"" << name << "/location.Y\"/>" << std::endl;
	out << "	</animation>" << std::endl;

	// Z
	out << "	<animation id=\"" << name << "_location_Z\">" << std::endl;
	out << "		<source id=\"" << name << "_location_Z-input\">" << std::endl;
	out << "			<float_array id=\"" << name << "_location_Z-input-array\" count=\"" << count << "\">";
	for (int i = 0; i < count; ++i)
	{
		out << keys[i].time << " ";
	}
	out << "</float_array>" << std::endl;
	out << "			<technique_common>" << std::endl;
	out << "				<accessor source=\"#" << name << "_location_Z-input-array\" count=\"" << count << "\" stride=\"1\">" << std::endl;
	out << "					<param name=\"TIME\" type=\"float\"/>" << std::endl;
	out << "				</accessor>" << std::endl;
	out << "			</technique_common>" << std::endl;
	out << "		</source>" << std::endl;
	out << "		<source id=\"" << name << "_location_Z-output\">" << std::endl;
	out << "			<float_array id=\"" << name << "_location_Z-output-array\" count=\"" << count << "\">";
	for (int i = 0; i < count; ++i)
	{
		out << keys[i].position.y << " ";
	}
	out << "</float_array>" << std::endl;
	out << "			<technique_common>" << std::endl;
	out << "				<accessor source=\"#" << name << "_location_Z-output-array\" count=\"" << count << "\" stride=\"1\">" << std::endl;
	out << "					<param name=\"Z\" type=\"float\"/>" << std::endl;
	out << "				</accessor>" << std::endl;
	out << "			</technique_common>" << std::endl;
	out << "		</source>" << std::endl;
	out << "		<source id=\"" << name << "_location_Z-interpolation\">" << std::endl;
	out << "			<Name_array id=\"" << name << "_location_Z-interpolation-array\" count=\"" << count << "\">";
	for (int i = 0; i < count; ++i)
	{
		"LINEAR ";
	}
	out << "</Name_array>" << std::endl;
	out << "			<technique_common>" << std::endl;
	out << "				<accessor source=\"#" << name << "_location_Z-interpolation-array\" count=\"" << count << "\" stride=\"1\">" << std::endl;
	out << "					<param name=\"INTERPOLATION\" type=\"name\"/>" << std::endl;
	out << "				</accessor>" << std::endl;
	out << "			</technique_common>" << std::endl;
	out << "		</source>" << std::endl;
	out << "		<sampler id=\"" << name << "_location_Z-sampler\">" << std::endl;
	out << "			<input semantic=\"INPUT\" source=\"#" << name << "_location_Z-input\"/>" << std::endl;
	out << "			<input semantic=\"OUTPUT\" source=\"#" << name << "_location_Z-output\"/>" << std::endl;
	out << "			<input semantic=\"INTERPOLATION\" source=\"#" << name << "_location_Z-interpolation\"/>" << std::endl;
	out << "		</sampler>" << std::endl;
	out << "		<channel source=\"#" << name << "_location_Z-sampler\" target=\"" << name << "/location.Z\"/>" << std::endl;
	out << "	</animation>" << std::endl;
}

void StateManager::writeDAEOrientation(std::fstream &out, std::vector<Keyframe> &keys, std::string name)
{
	int count = keys.size();
	// X
	out << "	<animation id=\"" << name << "_rotation_euler_X\">" << std::endl;
	out << "		<source id=\"" << name << "_rotation_euler_X-input\">" << std::endl;
	out << "			<float_array id=\"" << name << "_rotation_euler_X-input-array\" count=\"" << count << "\">";
	for (int i = 0; i < count; ++i)
	{
		out << keys[i].time << " ";
	}
	out << "</float_array>" << std::endl;
	out << "			<technique_common>" << std::endl;
	out << "				<accessor source=\"#" << name << "_rotation_euler_X-input-array\" count=\"" << count << "\" stride=\"1\">" << std::endl;
	out << "					<param name=\"TIME\" type=\"float\"/>" << std::endl;
	out << "				</accessor>" << std::endl;
	out << "			</technique_common>" << std::endl;
	out << "		</source>" << std::endl;
	out << "		<source id=\"" << name << "_rotation_euler_X-output\">" << std::endl;
	out << "			<float_array id=\"" << name << "_rotation_euler_X-output-array\" count=\"" << count << "\">";
	for (int i = 0; i < count; ++i)
	{
		float yaw, pitch, roll;
		keys[i].orientation.GetYawPitchRoll(&yaw, &pitch, &roll);
		out << pitch * (180.0 / 3.14159265) << " ";
	}
	out << "</float_array>" << std::endl;
	out << "			<technique_common>" << std::endl;
	out << "				<accessor source=\"#" << name << "_rotation_euler_X-output-array\" count=\"" << count << "\" stride=\"1\">" << std::endl;
	out << "					<param name=\"ANGLE\" type=\"float\"/>" << std::endl;
	out << "				</accessor>" << std::endl;
	out << "			</technique_common>" << std::endl;
	out << "		</source>" << std::endl;
	out << "		<source id=\"" << name << "_rotation_euler_X-interpolation\">" << std::endl;
	out << "			<Name_array id=\"" << name << "_rotation_euler_X-interpolation-array\" count=\"" << count << "\">";
	for (int i = 0; i < count; ++i)
	{
		out << "LINEAR ";
	}
	out << "</Name_array>" << std::endl;
	out << "			<technique_common>" << std::endl;
	out << "				<accessor source=\"#" << name << "_rotation_euler_X-interpolation-array\" count=\"" << count << "\" stride=\"1\">" << std::endl;
	out << "					<param name=\"INTERPOLATION\" type=\"name\"/>" << std::endl;
	out << "				</accessor>" << std::endl;
	out << "			</technique_common>" << std::endl;
	out << "		</source>" << std::endl;
	out << "		<sampler id=\"" << name << "_rotation_euler_X-sampler\">" << std::endl;
	out << "			<input semantic=\"INPUT\" source=\"#" << name << "_rotation_euler_X-input\"/>" << std::endl;
	out << "			<input semantic=\"OUTPUT\" source=\"#" << name << "_rotation_euler_X-output\"/>" << std::endl;
	out << "			<input semantic=\"INTERPOLATION\" source=\"#" << name << "_rotation_euler_X-interpolation\"/>" << std::endl;
	out << "		</sampler>" << std::endl;
	out << "		<channel source=\"#" << name << "_rotation_euler_X-sampler\" target=\"" << name << "/rotationX.ANGLE\"/>" << std::endl;
	out << "	</animation>" << std::endl;

	// Y
	out << "	<animation id=\"" << name << "_rotation_euler_Y\">" << std::endl;
	out << "		<source id=\"" << name << "_rotation_euler_Y-input\">" << std::endl;
	out << "			<float_array id=\"" << name << "_rotation_euler_Y-input-array\" count=\"" << count << "\">";
	for (int i = 0; i < count; ++i)
	{
		out << keys[i].time << " ";
	}
	out << "</float_array>" << std::endl;
	out << "			<technique_common>" << std::endl;
	out << "				<accessor source=\"#" << name << "_rotation_euler_Y-input-array\" count=\"" << count << "\" stride=\"1\">" << std::endl;
	out << "					<param name=\"TIME\" type=\"float\"/>" << std::endl;
	out << "				</accessor>" << std::endl;
	out << "			</technique_common>" << std::endl;
	out << "		</source>" << std::endl;
	out << "		<source id=\"" << name << "_rotation_euler_Y-output\">" << std::endl;
	out << "			<float_array id=\"" << name << "_rotation_euler_Y-output-array\" count=\"" << count << "\">";
	for (int i = 0; i < count; ++i)
	{
		float yaw, pitch, roll;
		keys[i].orientation.GetYawPitchRoll(&yaw, &pitch, &roll);

		out << -roll * (180.0 / 3.14159265) << " ";
	}
	out << "</float_array>" << std::endl;
	out << "			<technique_common>" << std::endl;
	out << "				<accessor source=\"#" << name << "_rotation_euler_Y-output-array\" count=\"" << count << "\" stride=\"1\">" << std::endl;
	out << "					<param name=\"ANGLE\" type=\"float\"/>" << std::endl;
	out << "				</accessor>" << std::endl;
	out << "			</technique_common>" << std::endl;
	out << "		</source>" << std::endl;
	out << "		<source id=\"" << name << "_rotation_euler_Y-interpolation\">" << std::endl;
	out << "			<Name_array id=\"" << name << "_rotation_euler_Y-interpolation-array\" count=\"" << count << "\">";
	for (int i = 0; i < count; ++i)
	{
		"LINEAR ";
	}
	out << "</Name_array>" << std::endl;
	out << "			<technique_common>" << std::endl;
	out << "				<accessor source=\"#" << name << "_rotation_euler_Y-interpolation-array\" count=\"" << count << "\" stride=\"1\">" << std::endl;
	out << "					<param name=\"INTERPOLATION\" type=\"name\"/>" << std::endl;
	out << "				</accessor>" << std::endl;
	out << "			</technique_common>" << std::endl;
	out << "		</source>" << std::endl;
	out << "		<sampler id=\"" << name << "_rotation_euler_Y-sampler\">" << std::endl;
	out << "			<input semantic=\"INPUT\" source=\"#" << name << "_rotation_euler_Y-input\"/>" << std::endl;
	out << "			<input semantic=\"OUTPUT\" source=\"#" << name << "_rotation_euler_Y-output\"/>" << std::endl;
	out << "			<input semantic=\"INTERPOLATION\" source=\"#" << name << "_rotation_euler_Y-interpolation\"/>" << std::endl;
	out << "		</sampler>" << std::endl;
	out << "		<channel source=\"#" << name << "_rotation_euler_Y-sampler\" target=\"" << name << "/rotationY.ANGLE\"/>" << std::endl;
	out << "	</animation>" << std::endl;

	// Z
	out << "	<animation id=\"" << name << "_rotation_euler_Z\">" << std::endl;
	out << "		<source id=\"" << name << "_rotation_euler_Z-input\">" << std::endl;
	out << "			<float_array id=\"" << name << "_rotation_euler_Z-input-array\" count=\"" << count << "\">";
	for (int i = 0; i < count; ++i)
	{
		out << keys[i].time << " ";
	}
	out << "</float_array>" << std::endl;
	out << "			<technique_common>" << std::endl;
	out << "				<accessor source=\"#" << name << "_rotation_euler_Z-input-array\" count=\"" << count << "\" stride=\"1\">" << std::endl;
	out << "					<param name=\"TIME\" type=\"float\"/>" << std::endl;
	out << "				</accessor>" << std::endl;
	out << "			</technique_common>" << std::endl;
	out << "		</source>" << std::endl;
	out << "		<source id=\"" << name << "_rotation_euler_Z-output\">" << std::endl;
	out << "			<float_array id=\"" << name << "_rotation_euler_Z-output-array\" count=\"" << count << "\">";
	for (int i = 0; i < count; ++i)
	{
		float yaw, pitch, roll;
		keys[i].orientation.GetYawPitchRoll(&yaw, &pitch, &roll);

		out << yaw * (180.0 / 3.14159265) << " ";
	}
	out << "</float_array>" << std::endl;
	out << "			<technique_common>" << std::endl;
	out << "				<accessor source=\"#" << name << "_rotation_euler_Z-output-array\" count=\"" << count << "\" stride=\"1\">" << std::endl;
	out << "					<param name=\"ANGLE\" type=\"float\"/>" << std::endl;
	out << "				</accessor>" << std::endl;
	out << "			</technique_common>" << std::endl;
	out << "		</source>" << std::endl;
	out << "		<source id=\"" << name << "_rotation_euler_Z-interpolation\">" << std::endl;
	out << "			<Name_array id=\"" << name << "_rotation_euler_Z-interpolation-array\" count=\"" << count << "\">";
	for (int i = 0; i < count; ++i)
	{
		"LINEAR ";
	}
	out << "</Name_array>" << std::endl;
	out << "			<technique_common>" << std::endl;
	out << "				<accessor source=\"#" << name << "_rotation_euler_Z-interpolation-array\" count=\"" << count << "\" stride=\"1\">" << std::endl;
	out << "					<param name=\"INTERPOLATION\" type=\"name\"/>" << std::endl;
	out << "				</accessor>" << std::endl;
	out << "			</technique_common>" << std::endl;
	out << "		</source>" << std::endl;
	out << "		<sampler id=\"" << name << "_rotation_euler_Z-sampler\">" << std::endl;
	out << "			<input semantic=\"INPUT\" source=\"#" << name << "_rotation_euler_Z-input\"/>" << std::endl;
	out << "			<input semantic=\"OUTPUT\" source=\"#" << name << "_rotation_euler_Z-output\"/>" << std::endl;
	out << "			<input semantic=\"INTERPOLATION\" source=\"#" << name << "_rotation_euler_Z-interpolation\"/>" << std::endl;
	out << "		</sampler>" << std::endl;
	out << "		<channel source=\"#" << name << "_rotation_euler_Z-sampler\" target=\"" << name << "/rotationZ.ANGLE\"/>" << std::endl;
	out << "	</animation>" << std::endl;
}

void StateManager::exportDAE(ovrSession hmd, const std::string &filename)
{
	std::fstream out(filename, std::ios::out);
	int sensorMaxCount = 0;
	for (int i = 0; i < m_samples.size(); ++i)
	{
		if (m_samples[i].sensorCount > sensorMaxCount)
			sensorMaxCount = m_samples[i].sensorCount;
	}

	out << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << std::endl;
	out << "<COLLADA xmlns=\"http://www.collada.org/2005/11/COLLADASchema\" version=\"1.4.1\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">" << std::endl;
	out << "<asset>" << std::endl;
	out << "	<unit name=\"meter\" meter=\"1\"/>" << std::endl;
	out << "	<up_axis>Y_UP</up_axis>" << std::endl;
	out << "</asset>" << std::endl;


	out << "	<library_cameras>" << std::endl;
	for (int i = 0; i < sensorMaxCount; ++i)
	{
		ovrTrackerDesc &desc = m_samples.front().sensorDesc[i];
		writeDAECamera(out, "Sensor" + std::to_string(i) + "-camera", desc.FrustumHFovInRadians, desc.FrustumVFovInRadians, desc.FrustumNearZInMeters, desc.FrustumFarZInMeters);
	}

	ovrHmdDesc hd = ovr_GetHmdDesc(hmd);
	writeDAECamera(out, "Head-camera", atan(hd.MaxEyeFov[0].LeftTan) * (180.0 / 3.14159265), atan(hd.MaxEyeFov[0].UpTan) * (180.0 / 3.14159265), 0.01, 100);
	out << "	</library_cameras>" << std::endl;

	out << "<library_animations>" << std::endl;
	std::vector<Keyframe> frames(m_samples.size());
	for (int i = 0; i < m_samples.size(); ++i)
	{
		frames[i].time = m_samples[i].time;
		frames[i].position = m_samples[i].trackingState.HandPoses[0].ThePose.Position;
		frames[i].orientation = m_samples[i].trackingState.HandPoses[0].ThePose.Orientation;
	}
	writeDAEPositions(out, frames, "Left");
	writeDAEOrientation(out, frames, "Left");
	for (int i = 0; i < m_samples.size(); ++i)
	{
		frames[i].position = m_samples[i].trackingState.HandPoses[1].ThePose.Position;
		frames[i].orientation = m_samples[i].trackingState.HandPoses[1].ThePose.Orientation;
	}
	writeDAEPositions(out, frames, "Right");
	writeDAEOrientation(out, frames, "Right");
	for (int i = 0; i < m_samples.size(); ++i)
	{
		frames[i].position = m_samples[i].trackingState.HeadPose.ThePose.Position;
		frames[i].orientation = m_samples[i].trackingState.HeadPose.ThePose.Orientation;
	}
	writeDAEPositions(out, frames, "Head");
	writeDAEOrientation(out, frames, "Head");

	for (int s = 0; s < sensorMaxCount; ++s)
	{
		for (int i = 0; i < m_samples.size(); ++i)
		{
			frames[i].position = m_samples[i].sensorPose[s].Pose.Position;
			frames[i].orientation = m_samples[i].sensorPose[s].Pose.Orientation;
		}
		writeDAEPositions(out, frames, "Sensor" + std::to_string(s));
		writeDAEOrientation(out, frames, "Sensor" + std::to_string(s));
	}

	out << "</library_animations>" << std::endl;
	out << "<library_visual_scenes>" << std::endl;
	out << "	<visual_scene id=\"Scene\" name=\"Scene\">" << std::endl;
	out << "      <node id=\"Left\" name=\"Left\" type=\"NODE\">" << std::endl;
	out << "        <translate sid=\"location\">0 0 0</translate>" << std::endl;
	out << "        <rotate sid=\"rotationZ\">0 0 1 0</rotate>" << std::endl;
	out << "        <rotate sid=\"rotationY\">0 1 0 0</rotate>" << std::endl;
	out << "        <rotate sid=\"rotationX\">1 0 0 0</rotate>" << std::endl;
	out << "        <scale sid=\"scale\">1 1 1</scale>" << std::endl;
	out << "      </node>" << std::endl;
	out << "      <node id=\"Right\" name=\"Right\" type=\"NODE\">" << std::endl;
	out << "        <translate sid=\"location\">0 0 0</translate>" << std::endl;
	out << "        <rotate sid=\"rotationZ\">0 0 1 0</rotate>" << std::endl;
	out << "        <rotate sid=\"rotationY\">0 1 0 0</rotate>" << std::endl;
	out << "        <rotate sid=\"rotationX\">1 0 0 0</rotate>" << std::endl;
	out << "        <scale sid=\"scale\">1 1 1</scale>" << std::endl;
	out << "      </node>" << std::endl;
	out << "      <node id=\"Head\" name=\"Head\" type=\"NODE\">" << std::endl;
	out << "        <translate sid=\"location\">0 0 0</translate>" << std::endl;
	out << "        <rotate sid=\"rotationZ\">0 0 1 0</rotate>" << std::endl;
	out << "        <rotate sid=\"rotationY\">0 1 0 0</rotate>" << std::endl;
	out << "        <rotate sid=\"rotationX\">1 0 0 0</rotate>" << std::endl;
	out << "        <scale sid=\"scale\">1 1 1</scale>" << std::endl;
	out << "	      <node id=\"HeadCamera\" name=\"HeadCamera\" type=\"NODE\">" << std::endl;
	out << "	        <matrix sid=\"transform\">-1 0 0 0 0 0 1 0 0 1 0 0 0 0 0 1</matrix>" << std::endl;
	out << "			<instance_camera url=\"#Head-camera\"/>" << std::endl;
	out << "	     </node>" << std::endl;
	out << "      </node>" << std::endl;

	for (int i = 0; i < sensorMaxCount; ++i)
	{
		std::string name = "Sensor" + std::to_string(i);
		out << "      <node id=\"" << name << "\" name=\"" << name << "\" type=\"NODE\">" << std::endl;
		out << "        <translate sid=\"location\">0 0 0</translate>" << std::endl;
		out << "        <rotate sid=\"rotationZ\">0 0 1 0</rotate>" << std::endl;
		out << "        <rotate sid=\"rotationY\">0 1 0 0</rotate>" << std::endl;
		out << "        <rotate sid=\"rotationX\">1 0 0 0</rotate>" << std::endl;
		out << "        <scale sid=\"scale\">1 1 1</scale>" << std::endl;
		out << "	      <node id=\"" << name + "-inner" << "\" name=\"" << name + "-inner" << "\" type=\"NODE\">" << std::endl;
		out << "	        <matrix sid=\"transform\">-1 0 0 0 0 0 1 0 0 1 0 0 0 0 0 1</matrix>" << std::endl;
		out << "			<instance_camera url=\"" << "#Sensor" + std::to_string(i) + "-camera" << "\"/>" << std::endl;
		out << "	     </node>" << std::endl;
		out << "      </node>" << std::endl;
	}

	out << "    </visual_scene>" << std::endl;
	out << "  </library_visual_scenes>" << std::endl;
	out << "  <scene>" << std::endl;
	out << "    <instance_visual_scene url=\"#Scene\"/>" << std::endl;
	out << "  </scene>" << std::endl;
	out << "</COLLADA>" << std::endl;
	out.close();
}
