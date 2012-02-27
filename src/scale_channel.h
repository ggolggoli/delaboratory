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

#ifndef _DE_SCALE_CHANNEL_H
#define _DE_SCALE_CHANNEL_H

#include "size.h"
#include "value.h"

void scaleChannel(deValue* src, deValue* dst, int maxx, int ox, deValue scaleW, int maxy, int oy, deValue scaleH, int ws, int hs, int wd);

void scaleChannel(deValue* src, deValue* dst, int x1, int y1, int x2, int y2, int w, int h, int ws);

#endif
