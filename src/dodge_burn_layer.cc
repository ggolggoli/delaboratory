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

#include "dodge_burn_layer.h"
#include "project.h"
#include <iostream>
#include "blur.h"
#include "str.h"
#include "xml.h"
#include "frame_factory.h"
#include "copy_channel.h"
#include "blend_channel.h"
#include "process_linear.h"
#include "layer_processor.h"
#include "preset.h"
#include "channel_manager.h"

deDodgeBurnLayer::deDodgeBurnLayer(deColorSpace _colorSpace, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, deViewManager& _viewManager)
:deLayer(_colorSpace, _sourceLayer, _layerStack, _channelManager),
 alternate("alternate"),
 viewManager(_viewManager)
{
    blurRadiusIndex = registerPropertyValue("blur_radius");
    blur2RadiusIndex = registerPropertyValue("blur2_radius");
    dodgeAmountIndex = registerPropertyValue("dodge_amount");
    dodgeMinIndex = registerPropertyValue("dodge_min");
    dodgeMaxIndex = registerPropertyValue("dodge_max");
    burnAmountIndex = registerPropertyValue("burn_amount");
    burnMinIndex = registerPropertyValue("burn_min");
    burnMaxIndex = registerPropertyValue("burn_max");

    dePropertyValue* blurRadius = getPropertyValue(blurRadiusIndex);
    dePropertyValue* blur2Radius = getPropertyValue(blur2RadiusIndex);
    dePropertyValue* dodgeAmount = getPropertyValue(dodgeAmountIndex);
    dePropertyValue* dodgeMin = getPropertyValue(dodgeMinIndex);
    dePropertyValue* dodgeMax = getPropertyValue(dodgeMaxIndex);
    dePropertyValue* burnAmount = getPropertyValue(burnAmountIndex);
    dePropertyValue* burnMin = getPropertyValue(burnMinIndex);
    dePropertyValue* burnMax = getPropertyValue(burnMaxIndex);

    blurRadius->setLabel("blur 1");
    blurRadius->setMin(1);
    blurRadius->setMax(50);

    blur2Radius->setLabel("blur 2");
    blur2Radius->setMin(1);
    blur2Radius->setMax(50);

    dodgeAmount->setLabel("dodge amount");
    dodgeMin->setLabel("dodge min");
    dodgeMax->setLabel("dodge max");

    burnAmount->setLabel("burn amount");
    burnMin->setLabel("burn min");
    burnMax->setLabel("burn max");

    alternate.setLabel("use screen / multiply instead dodge / burn");
    alternate.set(false);

    {
        dePresetLayer* p = new dePresetLayer(*this);
        p->addPresetValue(new dePresetValue("blur_radius", 5.0));
        p->addPresetValue(new dePresetValue("blur2_radius", 5.0));
        p->addPresetValue(new dePresetValue("dodge_amount", 0.4));
        p->addPresetValue(new dePresetValue("dodge_min", 0.6));
        p->addPresetValue(new dePresetValue("dodge_max", 0.95));
        p->addPresetValue(new dePresetValue("burn_amount", 0.4));
        p->addPresetValue(new dePresetValue("burn_min", 0.05));
        p->addPresetValue(new dePresetValue("burn_max", 0.4));
        presets["reset"] = p;
    }        

    {
        dePresetLayer* p = new dePresetLayer(*this);
        p->addPresetValue(new dePresetValue("blur_radius", 20.0));
        p->addPresetValue(new dePresetValue("blur2_radius", 5.0));
        presets["radius big"] = p;
    }        

    {
        dePresetLayer* p = new dePresetLayer(*this);
        p->addPresetValue(new dePresetValue("blur_radius", 5.0));
        p->addPresetValue(new dePresetValue("blur2_radius", 5.0));
        presets["radius small"] = p;
    }        

    {
        dePresetLayer* p = new dePresetLayer(*this);
        p->addPresetValue(new dePresetValue("dodge_amount", 0.0));
        presets["dodge off"] = p;
    }        

    {
        dePresetLayer* p = new dePresetLayer(*this);
        p->addPresetValue(new dePresetValue("dodge_amount", 0.4));
        p->addPresetValue(new dePresetValue("dodge_min", 0.6));
        p->addPresetValue(new dePresetValue("dodge_max", 0.95));
        presets["dodge small"] = p;
    }        

    {
        dePresetLayer* p = new dePresetLayer(*this);
        p->addPresetValue(new dePresetValue("dodge_amount", 0.5));
        p->addPresetValue(new dePresetValue("dodge_min", 0.5));
        p->addPresetValue(new dePresetValue("dodge_max", 0.95));
        presets["dodge medium"] = p;
    }        

    {
        dePresetLayer* p = new dePresetLayer(*this);
        p->addPresetValue(new dePresetValue("dodge_amount", 0.6));
        p->addPresetValue(new dePresetValue("dodge_min", 0.3));
        p->addPresetValue(new dePresetValue("dodge_max", 0.95));
        presets["dodge big"] = p;
    }        

    {
        dePresetLayer* p = new dePresetLayer(*this);
        p->addPresetValue(new dePresetValue("burn_amount", 0.0));
        presets["burn off"] = p;
    }        

    {
        dePresetLayer* p = new dePresetLayer(*this);
        p->addPresetValue(new dePresetValue("burn_amount", 0.4));
        p->addPresetValue(new dePresetValue("burn_min", 0.05));
        p->addPresetValue(new dePresetValue("burn_max", 0.4));
        presets["burn small"] = p;
    }        

    {
        dePresetLayer* p = new dePresetLayer(*this);
        p->addPresetValue(new dePresetValue("burn_amount", 0.5));
        p->addPresetValue(new dePresetValue("burn_min", 0.05));
        p->addPresetValue(new dePresetValue("burn_max", 0.5));
        presets["burn medium"] = p;
    }        

    {
        dePresetLayer* p = new dePresetLayer(*this);
        p->addPresetValue(new dePresetValue("burn_amount", 0.6));
        p->addPresetValue(new dePresetValue("burn_min", 0.05));
        p->addPresetValue(new dePresetValue("burn_max", 0.7));
        presets["burn big"] = p;
    }        

    applyPreset("reset");

    disableNotLuminance();
}

deDodgeBurnLayer::~deDodgeBurnLayer()
{
}

bool deDodgeBurnLayer::processDB(const deChannel& sourceChannel, deChannel& channel)
{
    deSize size = mainLayerImage.getChannelSize();
    logMessage("dodge/burn start");

    deBlendMode mode1 = deBlendDodge;
    deBlendMode mode2 = deBlendBurn;

    if (alternate.get())
    {
        mode1 = deBlendScreen;
        mode2 = deBlendMultiply;
    }

    const deValue* source = sourceChannel.getPixels();
    deValue* destination = channel.getPixels();

    deValue* blurMap = NULL;
    deValue* dodgeBurnMap = NULL;
    deValue* firstStage = NULL;

    try
    {
        blurMap = new deValue [size.getN()];
        dodgeBurnMap = new deValue [size.getN()];
        firstStage = new deValue [size.getN()];
    }
    catch (std::bad_alloc)
    {
        logError("ERROR allocating memory in dodge/burn");
        if (blurMap)
        {
            delete [] blurMap;
        }
        if (dodgeBurnMap)
        {
            delete [] dodgeBurnMap;
        }
        if (firstStage)
        {
            delete [] firstStage;
        }
        return false;
    }

    dePropertyValue* blurRadius = getPropertyValue(blurRadiusIndex);
    dePropertyValue* blur2Radius = getPropertyValue(blur2RadiusIndex);
    dePropertyValue* dodgeAmount = getPropertyValue(dodgeAmountIndex);
    dePropertyValue* dodgeMin = getPropertyValue(dodgeMinIndex);
    dePropertyValue* dodgeMax = getPropertyValue(dodgeMaxIndex);
    dePropertyValue* burnAmount = getPropertyValue(burnAmountIndex);
    dePropertyValue* burnMin = getPropertyValue(burnMinIndex);
    dePropertyValue* burnMax = getPropertyValue(burnMaxIndex);

    deValue r = viewManager.getRealScale() * blurRadius->get();
    deValue r2 = viewManager.getRealScale() * blur2Radius->get();

    deBlurType type = deGaussianBlur;

    blurChannel(source, blurMap, size, r, r, type, 0.0);

    deValue dmin = dodgeMin->get();
    deValue dmax = dodgeMax->get();

    processLinear(blurMap, dodgeBurnMap, size.getN(), dmin, dmax, false);
    blurChannel(dodgeBurnMap, dodgeBurnMap, size, r2, r2, type, 0.0);

    deValue da = dodgeAmount->get(); 
    blendChannel(source, source, firstStage, dodgeBurnMap, mode1, da, size.getN());

    deValue bmin = burnMin->get();
    deValue bmax = burnMax->get();
    processLinear(blurMap, dodgeBurnMap, size.getN(), bmin, bmax, true);
    blurChannel(dodgeBurnMap, dodgeBurnMap, size, r2, r2, type, 0.0);

    deValue ba = burnAmount->get(); 
    blendChannel(firstStage, source, destination, dodgeBurnMap, mode2, ba, size.getN());

    delete [] blurMap;
    delete [] dodgeBurnMap;
    delete [] firstStage;

    logMessage("dodge/burn end");

    return true;
}


bool deDodgeBurnLayer::isChannelNeutral(int index)
{
    return false;
}    

void deDodgeBurnLayer::save(xmlNodePtr root)
{
    saveCommon(root);
    saveBlend(root);
    saveValueProperties(root);

    alternate.save(root);
}

void deDodgeBurnLayer::load(xmlNodePtr root)
{
    loadBlend(root);

    xmlNodePtr child = root->xmlChildrenNode;

    while (child)
    {
        loadValueProperties(child);
        alternate.load(child);

        child = child->next;
    }        
}

bool deDodgeBurnLayer::updateMainImageSingleChannel(int i)
{
    const deImage& sourceImage = getSourceImage();

    int s = sourceImage.getChannelIndex(i);

    if ((isChannelNeutral(i)) || (!isChannelEnabled(i)))
    {
        mainLayerImage.disableChannel(i, s);
        return true;
    }

    bool actionResult = false;

    deChannel* sourceChannel = channelManager.getChannel(s);
    if (sourceChannel)
    {
        mainLayerImage.enableChannel(i);
        int c = mainLayerImage.getChannelIndex(i);
        deChannel* channel = channelManager.getChannel(c);

        if (channel)
        {
            channel->lockWrite();
            sourceChannel->lockRead();

            actionResult = processDB(*sourceChannel, *channel);

            sourceChannel->unlockRead();
            channel->unlockWrite();
        }
    }

    return actionResult;

}
