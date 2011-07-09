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

#include "wx/wx.h"
#include "project.h"
#include "main_frame.h"
#include "logger.h"

class deLaboratory: public wxApp
{	
    public:
        deLaboratory()
        :wxApp()
        {
        }

        ~deLaboratory()
        {
        }

    private:
    	virtual bool OnInit();
        deProject project;
};


IMPLEMENT_APP(deLaboratory)

bool deLaboratory::OnInit()
{
    project.logMessage("on init...");

    wxInitAllImageHandlers();

	int width = 1024;
	int height = 850;

    project.logMessage("creating main frame on init...");

	deMainFrame *frame = new deMainFrame( wxSize(width,height), &project);
    project.getGUI().start();

    if (argc > 1)
    {
        wxString arg = argv[1];
        char cstring[1024];
        strncpy(cstring, (const char*)arg.mb_str(wxConvUTF8), 1023);
        project.loadSourceImage(cstring);

    }

	frame->Show(TRUE);
	SetTopWindow(frame);

	return TRUE;
} 


	