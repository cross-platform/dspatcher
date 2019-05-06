/************************************************************************
DSPatcher - Cross-Platform Graphical Tool for DSPatch
Copyright (c) 2014-2019 Marcus Tomlinson

This file is part of DSPatcher.

GNU Lesser General Public License Usage
This file may be used under the terms of the GNU Lesser General Public
License version 3.0 as published by the Free Software Foundation and
appearing in the file LICENSE included in the packaging of this file.
Please review the following information to ensure the GNU Lesser
General Public License version 3.0 requirements will be met:
http://www.gnu.org/copyleft/lgpl.html.

Other Usage
Alternatively, this file may be used in accordance with the terms and
conditions contained in a signed written agreement between you and
Marcus Tomlinson.

DSPatch is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
************************************************************************/

#pragma once

#include <DSPatch.h>

class QWidget;

namespace DSPatch
{
namespace DSPatcher
{

class DLLEXPORT UiComponent : public Component
{
public:
    NONCOPYABLE( UiComponent );
    DEFINE_PTRS( UiComponent );

    UiComponent( ProcessOrder processOrder = ProcessOrder::InOrder );
    virtual ~UiComponent();

    virtual QWidget* widget() = 0;
};

}  // namespace DSPatcher
}  // namespace DSPatch
