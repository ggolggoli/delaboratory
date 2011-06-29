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

#include "property_name.h"
#include "name_editor.h"
#include "preview_stack.h"
#include "project.h"
#include <wx/wx.h>

dePropertyName::dePropertyName(deLayer& _parent)
:deProperty(_parent)
{
    name = "";
}

dePropertyName::~dePropertyName()
{
}

void dePropertyName::setName(const std::string& _name)
{
    name = _name;
}

void dePropertyName::addPanelContent(wxPanel* panel, wxSizer* sizer)
{
    wxPanel* editor = new deNameEditor(panel, *this);
    sizer->Add(editor);
}    

void dePropertyName::saveContent(xmlNodePtr node)
{
    xmlNodeSetContent(node, xmlCharStrdup(name.c_str()));
}
