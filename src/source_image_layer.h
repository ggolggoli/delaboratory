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

#ifndef _DE_SOURCE_IMAGE_LAYER_H
#define _DE_SOURCE_IMAGE_LAYER_H

#include "layer.h"
class deSourceImage;
#include "size.h"

class deSourceImageLayer:public deLayer
{
    private:
        const deSourceImage& sourceImage;
        deSize previewSize;
    public:
        deSourceImageLayer(const deSourceImage& _sourceimage, const std::string& _name);
        virtual ~deSourceImageLayer();

        void setPreviewSize(const deSize& size);
        
//        virtual void generatePreview();
        virtual void changeSourceLayer(int id, const deLayerStack& layerStack);
        virtual dePreview* createPreview(dePreviewStack& previewStack);
        
        virtual void changeColorSpace(deColorSpace _colorSpace, const deLayerStack& layerStack);

        virtual wxDialog* createDialog(wxWindow* parent, int layerNumber, deProject* project);
        virtual deActionFrame* createActionFrame(wxWindow* parent, int layerNumber, deProject* project);

        virtual const std::string getDescription() const {return "source image";};
};

#endif
