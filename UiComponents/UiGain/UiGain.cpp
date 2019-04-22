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

#include <UiGain.h>

#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QWidget>

using namespace DSPatch;
using namespace DSPatcher;

namespace DSPatch
{
namespace DSPatcher
{
namespace internal
{

class UiGain
{
public:
    UiGain()
    {
        widget = new QWidget();

        slider = new QSlider( Qt::Horizontal, widget );
        slider->setRange( 0, 100 );
        slider->setValue( 100 );

        QLabel* label = new QLabel( widget );
        label->setText( QString::number( (float)slider->sliderPosition() / 100 ) );
        label->setFixedWidth( 30 );

        widget->connect( slider, &QSlider::valueChanged,
                         [label]( int value ) { label->setText( QString::number( (float)value / 100 ) ); } );

        widget->connect( slider, &QSlider::valueChanged, [this]( int value ) { gain = (float)value / 100; } );

        QHBoxLayout* layout = new QHBoxLayout( widget );
        layout->addWidget( slider );
        layout->addWidget( label );
        widget->resize( layout->sizeHint() );
    }

    ~UiGain()
    {
        slider->disconnect();

        if ( deleteWidget )
        {
            delete widget;
        }
    }

    QSlider* slider;
    QWidget* widget;
    bool deleteWidget = true;

    float gain = 1.0f;
};

}  // namespace internal
}  // namespace DSPatcher
}  // namespace DSPatch

UiGain::UiGain()
    : UiComponent( ProcessOrder::OutOfOrder )
    , p( new internal::UiGain() )
{
    SetInputCount_( 1, {"in"} );
    SetOutputCount_( 1, {"out"} );
}

QWidget* UiGain::widget()
{
    p->deleteWidget = false;
    return p->widget;
}

void UiGain::Process_( SignalBus const& inputs, SignalBus& outputs )
{
    auto in = inputs.GetValue<std::vector<short>>( 0 );
    if ( !in )
    {
        return;
    }

    for ( auto& inSample : *in )
    {
        inSample *= p->gain;  // apply gain sample-by-sample
    }

    outputs.MoveSignal( 0, inputs.GetSignal( 0 ) );  // move gained input signal to output
}
