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

#include "property_value.h"
#include "action_layer.h"
#include "str.h"
#include "xml.h"

dePropertyValue::dePropertyValue(deActionLayer& _layer, const std::string& _name)
:layer(_layer), name(_name)
{
    min = 0.0;
    max = 1.0;
    defaultValue = 0.0;

    value = defaultValue;
}

dePropertyValue::~dePropertyValue()
{
}

void dePropertyValue::set(deValue _value)
{
    value = _value;
}

void dePropertyValue::setAndUpdate(deValue _value)
{
    set(_value);
    layer.updateImage();
    layer.updateOtherLayers();
    layer.repaint();
}

deValue dePropertyValue::get() const
{
    return value;
}

void dePropertyValue::save(xmlNodePtr root) const
{
    saveChild(root, name, str(value));
}    

void dePropertyValue::load(xmlNodePtr child)
{
    if ((!xmlStrcmp(child->name, BAD_CAST(name.c_str())))) 
    {
        value = getValue(getContent(child));
    }
}    

std::string dePropertyValue::getName() const
{
    return name;
}

deValue dePropertyValue::getMin() const
{
    return min;
}

deValue dePropertyValue::getMax() const
{
    return max;
}

deValue dePropertyValue::getDefault() const
{
    return defaultValue;
}
