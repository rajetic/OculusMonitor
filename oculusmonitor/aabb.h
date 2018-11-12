////////////////////////////////////////////////////////////
// Oculus Monitor
// Copyright (C) 2018 Kojack (rajetic@gmail.com)
//
// KF is released under the MIT License  
// https://opensource.org/licenses/MIT
////////////////////////////////////////////////////////////

#pragma once
#include "Extras/OVR_Math.h"
#include <algorithm>
#include "imgui.h"

template<typename T>
inline T remap(const T &src_range_start, const T &src_range_end, const T &dst_range_start, const T &dst_range_end, const T &value_to_remap)
{
	return ((dst_range_end - dst_range_start) * (value_to_remap - src_range_start)) / (src_range_end - src_range_start) + dst_range_start;
}

class AABB
{
public:
	OVR::Vector3f minCorner;
	OVR::Vector3f maxCorner;

	inline AABB() :minCorner(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()), maxCorner(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min())
	{
	}

	inline AABB(const OVR::Vector3f &v) : minCorner(v), maxCorner(v)
	{
	}

	static OVR::Vector3f minimum(const OVR::Vector3f &a, const OVR::Vector3f &b)
	{
		return OVR::Vector3f(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
	}

	static OVR::Vector3f maximum(const OVR::Vector3f &a, const OVR::Vector3f &b)
	{
		return OVR::Vector3f(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
	}

	inline AABB &merge(const OVR::Vector3f &v)
	{
		minCorner = minimum(minCorner, v);
		maxCorner = maximum(maxCorner, v);
		return *this;
	}

	inline float width() const
	{
		return (maxCorner.x - minCorner.x);
	}

	inline float height() const
	{
		return (maxCorner.y - minCorner.y);
	}

	inline float length() const
	{
		return (maxCorner.z - minCorner.z);
	}

	inline OVR::Vector3f size() const
	{
		return (maxCorner - minCorner);
	}

	inline OVR::Vector3f centre() const
	{
		return (maxCorner + minCorner)*0.5;
	}

	inline ImVec2 remap(const ImVec2 &size, const ImVec2 &offset, const OVR::Vector3f &p)
	{
		ImVec2 out;
		float aspectOculus = width() / length();
		float aspectImGui = size.x / size.y;
		if (aspectOculus > aspectImGui)
		{
			out.x = ::remap<float>(minCorner.x, maxCorner.x, 0, size.x, p.x) + offset.x;
			out.y = ::remap<float>(minCorner.z, maxCorner.z, 0, size.x / aspectOculus, p.z) + (size.y - (size.x / aspectOculus)) / 2.0 + offset.y;
		}
		else
		{
			out.x = ::remap<float>(minCorner.x, maxCorner.x, 0, size.y * aspectOculus, p.x) + (size.x - (size.y * aspectOculus)) / 2.0 + offset.x;
			out.y = ::remap<float>(minCorner.z, maxCorner.z, 0, size.y, p.z) + offset.y;
		}
		return out;
	}
};



