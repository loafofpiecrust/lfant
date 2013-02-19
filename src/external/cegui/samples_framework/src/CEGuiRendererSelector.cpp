/***********************************************************************
    filename:   CEGuiRendererSelector.cpp
    created:    24/9/2004
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
 *
 *   Permission is hereby granted, free of charge, to any person obtaining
 *   a copy of this software and associated documentation files (the
 *   "Software"), to deal in the Software without restriction, including
 *   without limitation the rights to use, copy, modify, merge, publish,
 *   distribute, sublicense, and/or sell copies of the Software, and to
 *   permit persons to whom the Software is furnished to do so, subject to
 *   the following conditions:
 *
 *   The above copyright notice and this permission notice shall be
 *   included in all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *   OTHER DEALINGS IN THE SOFTWARE.
 ***************************************************************************/
#include "CEGuiRendererSelector.h"


/*************************************************************************
    Constructor.
*************************************************************************/
CEGuiRendererSelector::CEGuiRendererSelector()
{
    d_lastSelected = InvalidGuiRendererType;

    // make all renderer types un-available.  sub-classes should enable appropriate types.
    for (int i = 0; i < RendererTypeCount; ++i)
    {
        d_rendererAvailability[i] = false;
    }
}


/*************************************************************************
    Destructor.
*************************************************************************/
CEGuiRendererSelector::~CEGuiRendererSelector()
{}


/*************************************************************************
    Return the CEGuiRendererType value of the renderer most recently
    selected by the user.
*************************************************************************/
CEGuiRendererType CEGuiRendererSelector::getSelectedRendererType()
{
    return d_lastSelected;
}


/*************************************************************************
    Set whether or not a specific renderer type will be available for
    selection from the dialog the next time it is displayed.
*************************************************************************/
void CEGuiRendererSelector::setRendererAvailability(CEGuiRendererType rendererType, bool available)
{
    if (rendererType < RendererTypeCount)
    {
        d_rendererAvailability[rendererType] = available;
    }
}
