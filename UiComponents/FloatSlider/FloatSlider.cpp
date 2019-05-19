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

        slider->connect( slider, &QSlider::valueChanged, [this]( int value ) { floatValue = (float)value / 100; } );

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

    float floatValue = 1.0f;
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

void FloatSlider::Process_( SignalBus const&, SignalBus& outputs )
{
    outputs.SetValue( 0, p->floatValue );
}
