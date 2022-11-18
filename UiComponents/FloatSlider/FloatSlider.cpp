/******************************************************************************
DSPatcher - Cross-Platform Graphical Tool for DSPatch
Copyright (c) 2021, Marcus Tomlinson

BSD 2-Clause License

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
******************************************************************************/

#include <FloatSlider.h>

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

class FloatSlider
{
public:
    FloatSlider()
    {
        widget = new QWidget();

        slider = new QSlider( Qt::Horizontal, widget );
        slider->setRange( 0, 100 );
        slider->setValue( 100 );

        QLabel* label = new QLabel( widget );
        label->setText( QString::number( (float)slider->sliderPosition() / 100 ) );
        label->setFixedWidth( 30 );

        slider->connect( slider, &QSlider::valueChanged,
                         [label]( int value ) { label->setText( QString::number( (float)value / 100 ) ); } );

        slider->connect( slider, &QSlider::valueChanged, [this]( int value ) { currentValue = (float)value / 100; } );

        QHBoxLayout* layout = new QHBoxLayout( widget );
        layout->addWidget( slider );
        layout->addWidget( label );
        layout->setContentsMargins( 15, 15, 15, 15 );
        widget->resize( layout->sizeHint() );
    }

    ~FloatSlider()
    {
        slider->disconnect();
        widget->deleteLater();
    }

    QSlider* slider;
    QWidget* widget;

    float currentValue = 1.0f;
};

}  // namespace internal
}  // namespace DSPatcher
}  // namespace DSPatch

FloatSlider::FloatSlider()
    : UiComponent( ProcessOrder::OutOfOrder )
    , p( new internal::FloatSlider() )
{
    SetOutputCount_( 1 );
}

QWidget* FloatSlider::widget()
{
    return p->widget;
}

void FloatSlider::Process_( SignalBus&, SignalBus& outputs )
{
    outputs.SetValue( 0, p->currentValue );
}
