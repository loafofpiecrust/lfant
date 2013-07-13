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
#if LINUX
#include <GL/glx.h>
#elif WINDOWS
#include <GL/wgl.h>
#endif
#define BOOST_COMPUTE_DEBUG_KERNEL_COMPILATION

#include <lfant/OpenCL.h>

// Internal
#include <lfant/Console.h>
#include <lfant/Game.h>
#include <lfant/FileSystem.h>

// External
//#include <CL/cl.h>
//#include <boost/compute.hpp>


namespace lfant {

OpenCL::OpenCL()// :
//	platform(compute::system::platforms()[0])
{
}

OpenCL::~OpenCL()
{
}

void OpenCL::Init() {}
void OpenCL::OnDestroy() {}

OpenCL::Kernel* OpenCL::LoadFile(string path, string entry) { return nullptr; }

OpenCL::Kernel::Kernel(const string& source, string entry) {}
//boost::compute::command_queue* OpenCL::Kernel::GetQueue() { return nullptr; }

/*
void OpenCL::Init()
{
	gpu = compute::system::default_device();
//	platform = compute::system::platforms()[0];
	Log("OpenCL device: gpu? ", gpu.type() == CL_DEVICE_TYPE_GPU, " '"+gpu.name()+" by "+gpu.vendor()+"'.");
#if MACOSX
	cl_context_properties props[] = {
		CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE,
		(cl_context_properties) CGLGetShareGroup (CGLGetCurrentContext()),
		NULL
    	};
#elif WINDOWS
	cl_context_properties props[] = {
		CL_CONTEXT_PLATFORM, (cl_context_properties) platform.get(),
		CL_GL_CONTEXT_KHR, (cl_context_properties) wglGetCurrentContext(),
		CL_WGL_HDC_KHR, (cl_context_properties) wglGetCurrentDC(),
		NULL
	    };
#else
	cl_context_properties props[] = {
	    CL_GL_CONTEXT_KHR, (cl_context_properties) glXGetCurrentContext(),
	    CL_GLX_DISPLAY_KHR, (cl_context_properties) glXGetCurrentDisplay(),
	    CL_CONTEXT_PLATFORM, (cl_context_properties) platform.get(),
	    NULL
		};
#endif
	context = compute::context(gpu, props);
	queue = compute::command_queue(context, gpu);
}

OpenCL::Kernel* OpenCL::LoadFile(string path, string entry)
{
//	compute::program* program = compute::program::create_with_source(result, context);
//	program->build();
//	compute::kernel kernel(*program, "main");
	Kernel* kernel = new Kernel(game->fileSystem->GetGamePath(path).string(), entry);
	return kernel;
}

void OpenCL::OnDestroy()
{
}

OpenCL::Kernel::Kernel(const string& source, string entry) 
{
	program = compute::program::create_with_source_file(source, game->openCL->context);
	program.build();
	kernel = new compute::kernel(program, entry);
}

boost::compute::command_queue* OpenCL::Kernel::GetQueue()
{
	return &game->openCL->queue;
}
*/

}