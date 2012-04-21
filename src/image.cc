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

#include "image.h"
#include "channel_manager.h"
#include "logger.h"
#include "str.h"
#include "color_space_utils.h"

deImage::deImage(const deColorSpace& _colorSpace, deChannelManager& _channelManager)
:colorSpace(_colorSpace), channelManager(_channelManager)
{
    logMessage("create new image");
    int i;
    int s = getColorSpaceSize(colorSpace);
    for (i = 0; i < MAX_COLOR_SPACE_SIZE; i++)
    {
        if (i < s)
        {
            channelsAllocated[i] = channelManager.allocateNewChannel();
        }
        else
        {
            channelsAllocated[i] = -1;
        }
        channelsVisible[i] = -1;
    }        
}

deImage::~deImage()
{
    logMessage("destroy image");
    int i;
    int s = getColorSpaceSize(colorSpace);
    for (i = 0; i < s; i++)
    {
        assert (channelsAllocated[i] >= 0);
        channelManager.freeChannel(channelsAllocated[i]);
    }        
}

void deImage::enableChannel(int n)
{
    logMessage("enable channel " + str(n) + " in image");
    assert(n >= 0);
    assert(n < MAX_COLOR_SPACE_SIZE);
    assert (channelsAllocated[n] >= 0);
    channelManager.tryAllocateChannel(channelsAllocated[n]);
    channelsVisible[n] = channelsAllocated[n];
}

void deImage::disableChannel(int n, int c)
{
    logMessage("disable channel " + str(n) + " in image, replace with " +str(c));
    assert(n >= 0);
    assert(n < MAX_COLOR_SPACE_SIZE);
    channelsVisible[n] = c;
    channelManager.tryDeallocateChannel(channelsAllocated[n]);
}

int deImage::getChannelIndex(int n) const
{
    assert(n >= 0);
    assert(n < MAX_COLOR_SPACE_SIZE);
    return channelsVisible[n];
}

deColorSpace deImage::getColorSpace() const
{
    return colorSpace;
}

void deImage::disableAllChannels()
{
    int i;
    int s = getColorSpaceSize(colorSpace);
    for (i = 0; i < s; i++)
    {
        disableChannel(i, -1);
    }        
    
}

void deImage::enableAllChannels()
{
    int i;
    int s = getColorSpaceSize(colorSpace);
    for (i = 0; i < s; i++)
    {
        enableChannel(i);
    }        
}

void deImage::updateChannelUsage(std::map<int, int>& channelUsage, int index) const
{
    int i;
    int s = getColorSpaceSize(colorSpace);
    for (i = 0; i < s; i++)
    {
        int c = channelsVisible[i];
        channelUsage[c] = index;
    }        
}

const deValue* deImage::getValues(int channel) const
{
    deChannel* c = channelManager.getChannel(channelsVisible[channel]);
    if (!c)
    {
        return NULL;
    }
    return c->getPixels();
}

deValue* deImage::getValues(int channel) 
{
    deChannel* c = channelManager.getChannel(channelsVisible[channel]);
    if (!c)
    {
        return NULL;
    }
    return c->getPixels();
}

void deImage::lockRead() const
{
    int i;
    int s = getColorSpaceSize(colorSpace);
    for (i = 0; i < s; i++)
    {
        deChannel* c = channelManager.getChannel(channelsVisible[i]);
        if (c)
        {
            c->lockRead();
        }            
    }        
}

void deImage::unlockRead() const
{
    int i;
    int s = getColorSpaceSize(colorSpace);
    for (i = 0; i < s; i++)
    {
        deChannel* c = channelManager.getChannel(channelsVisible[i]);
        if (c)
        {
            c->unlockRead();
        }            
    }        
}

const deSize deImage::getChannelSize() const
{
    return channelManager.getChannelSizeFromChannelManager();
}
