/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*	Created: 2013-06-21 by Taylor Snead
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/

#include <GL/glew.h>

#define BOOST_COMPUTE_DEBUG_KERNEL_COMPILATION

#include <lfant/OpenCL.h>

// Internal
#include <lfant/Console.h>
#include <lfant/Game.h>
#include <lfant/FileSystem.h>

// External
#include <CL/cl.h>
#include <boost/compute.hpp>
#include <boost/compute/context.hpp>
#include <boost/compute/interop/opengl.hpp>


namespace lfant {

OpenCL::OpenCL(Game* game) :
	Subsystem(game),
	platform(compute::system::platforms()[0])
{
}

OpenCL::~OpenCL()
{
}

//void OpenCL::Init() {}
//void OpenCL::Deinit() {}

//OpenCL::Kernel* OpenCL::LoadFile(string path, string entry) { return nullptr; }

//OpenCL::Kernel::Kernel(const string& source, string entry) {}
//boost::compute::command_queue* OpenCL::Kernel::GetQueue() { return nullptr; }


void OpenCL::Init()
{
//	gpu = compute::system::default_device();
//	platform = compute::system::platforms()[0];
/*
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
		CL_CONTEXT_PLATFORM, (cl_context_properties) platform.id(),
		NULL
		};
#endif

	context = compute::context(gpu);*/
	context = compute::opengl_create_shared_context();
	gpu = context.get_device();

	GetGame()->Log("OpenCL device: gpu? ", gpu.type() == CL_DEVICE_TYPE_GPU, " '"+gpu.name()+" by "+gpu.vendor()+"'.");

	queue = compute::command_queue(context, gpu);
}

OpenCL::Kernel* OpenCL::LoadFile(string path, string entry)
{
//	compute::program* program = compute::program::create_with_source(result, context);
//	program->build();
//	compute::kernel kernel(*program, "main");
	Kernel* kernel = new Kernel(this, game->GetAssetPath(path).string(), entry);
	return kernel;
}

void OpenCL::Deinit()
{
	queue.flush();
}

OpenCL::Kernel::Kernel(OpenCL* cl, const string& source, string entry) :
	cl(cl)
{
	program = compute::program::create_with_source_file(source, cl->context);
	program.build();
	kernel = new compute::kernel(program, entry);
}

boost::compute::command_queue* OpenCL::Kernel::GetQueue()
{
	return &cl->queue;
}


}
