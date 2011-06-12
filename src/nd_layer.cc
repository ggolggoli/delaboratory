/*
    delaboratory - color correction utility
    Copyright (C) 2011 Jacek Poplawski

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "nd_layer.h"
#include "nd_frame.h"
#include "blend.h"
#include "preview_stack.h"
#include "size.h"
#include "preview.h"
#include "project.h"

deNDLayer::deNDLayer(deLayerStack& _stack, const std::string& _name)
:deLayer(_stack, _name), nd(*this)
{
    mode = deBlendMultiply;
    clearEnabledChannels();
}

deNDLayer::~deNDLayer()
{
}

dePreview* deNDLayer::createPreview(dePreviewStack& previewStack)
{
    const dePreview* sourcePreview = previewStack.getPreview(sourceLayer.getIndex());

    if (!sourcePreview)
    {
        return NULL;
    }

    const deSize& sourceSize = sourcePreview->getSize();

    dePreview* preview = new dePreview(colorSpace.getColorSpace(), sourceSize);

    blend(*sourcePreview, nd.getND(), *preview, mode, enabledChannels);

    return preview;
}

deActionFrame* deNDLayer::createActionFrame(wxWindow* parent, int layerNumber, deProject* project)
{
    return new deNDFrame(parent, *this, project->getPreviewStack(), layerNumber);
}

void deNDLayer::onChangeColorSpace()
{
    clearEnabledChannels();
}

void deNDLayer::enableChannel(int c)
{
    enabledChannels.insert(c);
}

void deNDLayer::clearEnabledChannels()
{
    enabledChannels.clear();
}

const std::set<int>& deNDLayer::getEnabledChannels() const
{
    return enabledChannels;
}
