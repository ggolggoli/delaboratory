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

#ifndef _DE_BLEND_H
#define _DE_BLEND_H

class dePreview;
class deND;
#include "value.h"
#include "blend_mode.h"
#include <set>

void blend(const dePreview& sourcePreview, const dePreview& overlayPreview, deValue alpha, dePreview& result, int oc, int dc, deBlendMode mode);
void blend(const dePreview& sourcePreview, const deND& nd, dePreview& resultPreview, deBlendMode mode, const std::set<int>& enabledChannels);

#endif    