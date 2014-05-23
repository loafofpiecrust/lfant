/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/
#pragma once

// internal
#include <lfant/Geometry.h>

// external

namespace lfant {

class Terrain : public Geometry
{
public:
	Terrain();
	~Terrain();

	virtual void Init();
	virtual void Update();

	virtual void BeginRender();
	virtual void Render();
	virtual void EndRender();

	void Generate();

protected:

private:

};

}
