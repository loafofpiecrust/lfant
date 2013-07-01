/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2013-06-21 by Taylor Snead
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License at
*
*	http://www.apache.org/licenses/LICENSE-2.0
*
*	Unless required by applicable law or agreed to in writing, software
*	distributed under the License is distributed on an "AS IS" BASIS,
*	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*	See the License for the specific language governing permissions and
*	limitations under the License.
*
******************************************************************************/
#pragma once
#include <lfant/stdafx.h>

// Internal
#include <lfant/ptr.h>
#include <lfant/Subsystem.h>

// External
#define BOOST_COMPUTE_HAVE_GL
#include <boost/compute/device.hpp>
#include <boost/compute/context.hpp>
#include <boost/compute/command_queue.hpp>
#include <boost/compute/platform.hpp>

namespace compute = boost::compute;

namespace lfant {

class OpenCL : public Subsystem
{
public:
	class Kernel
	{
	public:
		Kernel(const string& source, string entry);

		boost::compute::kernel* Get() {return kernel;}
		boost::compute::command_queue* GetQueue();

	private:
		boost::compute::program program;
		ptr<boost::compute::kernel> kernel;
	};
	friend class Kernel;

	OpenCL();
	~OpenCL();

	Kernel* LoadFile(string path, string entry);

	void Init();
	void OnDestroy();

//protected:
	boost::compute::platform platform;
	boost::compute::device gpu;
	boost::compute::context context;
	boost::compute::command_queue queue;

private:
};

}