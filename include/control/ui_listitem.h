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

// ui namespace
namespace LongUI {
    // listbox
    class UIListBox;
    // listitem
    class UIListItem : public UIBoxLayout {
        // super class
        using Super = UIBoxLayout;
        // private impl
        struct Private;
    public:
        // class meta
        static const  MetaControl   s_meta;
        // dtor
        ~UIListItem() noexcept;
        // ctor
        UIListItem(UIControl* parent = nullptr) noexcept : UIListItem(parent, UIListItem::s_meta) {}
    protected:
        // ctor
        UIListItem(UIControl* parent, const MetaControl&) noexcept;
    public:
        // clicked event
        static inline constexpr auto _clicked() noexcept { return GuiEvent::Event_Click; }
    public:
        // is selected?
        auto IsSelected() const noexcept { return m_oStyle.state.selected; }
    public:
        // get index
        auto GetIndex() const noexcept->uint32_t;
        // get text
        auto GetText() const noexcept ->const char16_t*;
        // get text- string object
        auto GetTextString() const noexcept -> const CUIString&;
        // set text
        void SetText(const CUIString& text) noexcept;
        // set text
        void SetText(CUIString&& text) noexcept;
        // set text
        void SetText(U16View text) noexcept;
    public:
        // do event
        auto DoEvent(UIControl * sender, const EventArg & e) noexcept->EventAccept override;
        // render
        //void Render() const noexcept override;
        // mouse event
        auto DoMouseEvent(const MouseEventArg& e) noexcept->EventAccept override;
        // update
        void Update() noexcept override;
    protected:
        // relayout
        void relayout() noexcept override;
        // add private child
        void add_private_child() noexcept;
        // add attribute
        void add_attribute(uint32_t key, U8View value) noexcept override;
#ifdef LUI_ACCESSIBLE
    protected:
        // accessible event
        //auto accessible(const AccessibleEventArg&) noexcept->EventAccept override;
#endif
    private:
        // parent box 
        UIListBox*          m_pListBox = nullptr;
        // private data
        Private*            m_private = nullptr;
    };
    // get meta info for UIListItem
    LUI_DECLARE_METAINFO(UIListItem);
}