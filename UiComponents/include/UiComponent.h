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
#include <InOut/InOut.h>

namespace DSPatch
{
namespace DSPatcher
{

class DLLEXPORT UiComponent final : public DSPatchables::InOut
{
public:
    DEFINE_PTRS( UiComponent );

    enum ParamType
    {
        Bool,
        Int,
        Float,
        String,
        FilePath,  // this is essentially just a string, but helps when determining an appropriate user input method
        List,      // this type acts as a vector (available items), an int (index selected), and a string (item selected)
        Trigger    // this type has no value, SetParam(triggerParam) simply represents a trigger. E.g. a button press
    };

    UiComponent( ParamType paramType, int inCount, int outCount );
    UiComponent( ParamType paramType, int inCount, int outCount, std::pair<int, int> const& intRange );
    UiComponent( ParamType paramType, int inCount, int outCount, std::pair<float, float> const& floatRange );

    ~UiComponent();

    ParamType const paramType;
    std::pair<int, int> const intRange = std::make_pair( 0, 100 );
    std::pair<float, float> const floatRange = std::make_pair( 0.0f, 1.0f );
};

}  // namespace DSPatcher
}  // namespace DSPatch
