#include <DsprParam.h>

#include <iostream>

DsprParam::DsprParam(std::string const& name, DspParameter const& param, QMenu* contextMenu)
    : _param(param)
    , _contextMenu(contextMenu)
{
    if (_param.Type() == DspParameter::Bool)
    {
        _checkbox = new QCheckBox(_contextMenu);
        _checkbox->setText(name.c_str());
        QWidgetAction* customAction = new QWidgetAction(_contextMenu);
        customAction->setDefaultWidget(_checkbox);
        _action = customAction;

        connect(_checkbox, SIGNAL(stateChanged(int)), this, SLOT(paramChanged(int)));
    }
    else if (_param.Type() == DspParameter::Int)
    {
        QWidget* intSlider = new QWidget(_contextMenu);

        _slider = new QSlider(Qt::Horizontal, intSlider);
        if (_param.GetIntRange())
        {
            _slider->setRange(_param.GetIntRange()->first, _param.GetIntRange()->second);
        }
        else
        {
            _slider->setRange(0, *_param.GetInt() * 2);
        }
        _slider->setValue(*_param.GetInt());

        QLabel* label = new QLabel(intSlider);
        label->setNum(_slider->sliderPosition());

        connect(_slider, SIGNAL(valueChanged(int)), label, SLOT(setNum(int)));

        QHBoxLayout* layout = new QHBoxLayout(intSlider);
        layout->addWidget(_slider);
        layout->addWidget(label);

        QWidgetAction* intSliderAction = new QWidgetAction(_contextMenu);
        intSliderAction->setDefaultWidget(intSlider);
        _action = intSliderAction;

        connect(_slider, SIGNAL(valueChanged(int)), this, SLOT(paramChanged(int)));
    }
    else if (_param.Type() == DspParameter::Float)
    {
        QWidget* floatSlider = new QWidget(_contextMenu);

        _slider = new QSlider(Qt::Horizontal, floatSlider);
        if (_param.GetFloatRange())
        {
            _slider->setRange(_param.GetFloatRange()->first, _param.GetFloatRange()->second);
        }
        else
        {
            _slider->setRange(0, *_param.GetFloat() * 2);
        }
        _slider->setValue(*_param.GetFloat());

        QLabel* label = new QLabel(floatSlider);
        label->setNum(_slider->sliderPosition());

        connect(_slider, SIGNAL(valueChanged(int)), label, SLOT(setNum(int)));

        QHBoxLayout* layout = new QHBoxLayout(floatSlider);
        layout->addWidget(_slider);
        layout->addWidget(label);

        QWidgetAction* intSliderAction = new QWidgetAction(_contextMenu);
        intSliderAction->setDefaultWidget(floatSlider);
        _action = intSliderAction;

        connect(_slider, SIGNAL(valueChanged(int)), this, SLOT(paramChanged(int)));
    }
    else if (_param.Type() == DspParameter::String)
    {
        _textBox = new QLineEdit(_contextMenu);
        _textBox->setText(_param.GetString()->c_str());

        QWidgetAction* customAction = new QWidgetAction(_contextMenu);
        customAction->setDefaultWidget(_textBox);
        _action = customAction;

        connect(_textBox, SIGNAL(textChanged(QString const&)), this, SLOT(paramChanged(QString const&)));
    }
    else if (_param.Type() == DspParameter::FilePath)
    {
        QWidget* fileBrowser = new QWidget(_contextMenu);

        _textBox = new QLineEdit(fileBrowser);
        _textBox->setEnabled(false);
        _textBox->setText(_param.GetString()->c_str());

        QPushButton* btnBrowse = new QPushButton(fileBrowser);
        btnBrowse->setText("Browse");

        QHBoxLayout* layout = new QHBoxLayout(fileBrowser);
        layout->addWidget(_textBox);
        layout->addWidget(btnBrowse);

        QWidgetAction* fileBrowserAction = new QWidgetAction(_contextMenu);
        fileBrowserAction->setDefaultWidget(fileBrowser);
        _action = fileBrowserAction;

        connect(_textBox, SIGNAL(textChanged(QString const&)), this, SLOT(paramChanged(QString const&)));
//        QFileDialog* custom = new QFileDialog(_contextMenu);
//        QWidgetAction* customAction = new QWidgetAction(_contextMenu);
//        customAction->setDefaultWidget(custom);
//        _contextMenu->addAction(customAction);
    }
    else if (_param.Type() == DspParameter::List)
    {
        _listBox = new QComboBox(_contextMenu);
        for (int i = 0; i < _param.GetList()->size(); ++i)
        {
            _listBox->addItem((*_param.GetList())[i].c_str());
        }

        QWidgetAction* customAction = new QWidgetAction(_contextMenu);
        customAction->setDefaultWidget(_listBox);
        _action = customAction;

        connect(_listBox, SIGNAL(currentIndexChanged(int)), this, SLOT(paramChanged(int)));
    }
    else if (_param.Type() == DspParameter::Trigger)
    {
        _button = new QPushButton(_contextMenu);
        _button->setText(name.c_str());

        QWidgetAction* customAction = new QWidgetAction(_contextMenu);
        customAction->setDefaultWidget(_button);
        _action = customAction;

        connect(_button, SIGNAL(released()), this, SLOT(paramChanged()));
    }
}

QWidgetAction* DsprParam::action()
{
    return _action;
}

DspParameter& DsprParam::param()
{
    return _param;
}

bool DsprParam::SetBool(bool const& value)
{
    bool result = _param.SetBool(value);
    if (result && _param.Type() == DspParameter::Bool)
    {
        _checkbox->setEnabled(value);
    }
    return result;
}

bool DsprParam::SetInt(int const& value)
{
    bool result = _param.SetInt(value);
    if (result && _param.Type() == DspParameter::Int)
    {
        _slider->setValue(value);
    }
    else if (result && _param.Type() == DspParameter::List)
    {
        _listBox->setCurrentIndex(value);
    }
    return result;
}

bool DsprParam::SetIntRange(std::pair<int, int> const& intRange)
{
    bool result = _param.SetIntRange(intRange);
    if (result && _param.Type() == DspParameter::Int)
    {
        _slider->setRange(intRange.first, intRange.second);
    }
    return result;
}

bool DsprParam::SetFloat(float const& value)
{
    bool result = _param.SetFloat(value);
    if (result && _param.Type() == DspParameter::Float)
    {
        _slider->setValue(value * 100);
    }
    return result;
}

bool DsprParam::SetFloatRange(std::pair<float, float> const& floatRange)
{
    bool result = _param.SetFloatRange(floatRange);
    if (result && _param.Type() == DspParameter::Float)
    {
        _slider->setRange(floatRange.first * 100, floatRange.second * 100);
    }
    return result;
}

bool DsprParam::SetString(std::string const& value)
{
    bool result = _param.SetString(value);
    if (result && _param.Type() == DspParameter::String)
    {
        _textBox->setText(value.c_str());
    }
    else if (result && _param.Type() == DspParameter::FilePath)
    {
        _textBox->setText(value.c_str());
    }
    else if (result && _param.Type() == DspParameter::List)
    {
        for (int i = 0; i < _listBox->children().size(); ++i)
        {
            if (_listBox->itemText(i) == value.c_str())
            {
                _listBox->setCurrentIndex(i);
                break;
            }
        }
    }
    return result;
}

bool DsprParam::SetList(std::vector<std::string> const& value)
{
    bool result = _param.SetList(value);
    if (result && _param.Type() == DspParameter::List)
    {
        _listBox->clear();
        for (size_t i = 0; i < value.size(); ++i)
        {
            _listBox->addItem(value[i].c_str());
        }
    }
    return result;
}

void DsprParam::paramChanged(int value)
{
    if (_param.Type() == DspParameter::Bool)
    {
        std::cerr << "Bool" << std::endl;
        emit paramUpdated(value != 0);
    }
    else if (_param.Type() == DspParameter::Int)
    {
        std::cerr << "Int" << std::endl;
        emit paramUpdated(value);
    }
    else if (_param.Type() == DspParameter::Float)
    {
        std::cerr << "Float" << std::endl;
        emit paramUpdated((float)value / 100.f);
    }
    else if (_param.Type() == DspParameter::List)
    {
        std::cerr << "List" << std::endl;
        emit paramUpdated(value);
    }
}

void DsprParam::paramChanged(QString const& newString)
{
    if (_param.Type() == DspParameter::String)
    {
        std::cerr << "String" << std::endl;
        emit paramUpdated(newString.toStdString());
    }
    else if (_param.Type() == DspParameter::FilePath)
    {
        std::cerr << "FilePath" << std::endl;
        emit paramUpdated(newString.toStdString());
    }
}

void DsprParam::paramChanged()
{
    if (_param.Type() == DspParameter::Trigger)
    {
        std::cerr << "Trigger" << std::endl;
        emit paramUpdated();
    }
}
