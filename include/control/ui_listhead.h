﻿#pragma once
/**
* Copyright (c) 2014-2018 dustpg   mailto:dustpg@gmail.com
*
* Permission is hereby granted, free of charge, to any person
* obtaining a copy of this software and associated documentation
* files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use,
* copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following
* conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
* HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*/

#include "ui_boxlayout.h"
#include <container/pod_vector.h>
//#include "../util/ui_double_click.h"

// ui namespace
namespace LongUI {
    // listhead control
    class UIListHead : public UIBoxLayout {
        // super class
        using Super = UIBoxLayout;
    protected:
        // ctor
        UIListHead(UIControl* parent, const MetaControl&) noexcept;
    public:
        // class meta
        static const  MetaControl   s_meta;
        // dtor
        ~UIListHead() noexcept;
        // ctor
        UIListHead(UIControl* parent = nullptr) noexcept : UIListHead(parent, UIListHead::s_meta) {}
    protected:
        // add child
        //void add_child(UIControl& child) noexcept override;
        // re-layout
        void relayout() noexcept override;
    private:
    };
    // get meta info for UIListHead
    LUI_DECLARE_METAINFO(UIListHead);
}

