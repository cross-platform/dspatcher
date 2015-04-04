#ifndef QTPPARAM_H
#define QTPPARAM_H

#include <QObject>
#include <QtWidgets>

#include <DSPatch.h>

class QtpWire;

class DsprParam : public QObject
{
    Q_OBJECT

public:
    DsprParam(DspParameter const& param, QMenu* contextMenu);
    QWidgetAction* action();
    DspParameter& param();

    bool SetBool(bool const& value);
    bool SetInt(int const& value);
    bool SetIntRange(std::pair<int, int> const& intRange);
    bool SetFloat(float const& value);
    bool SetFloatRange(std::pair<float, float> const& floatRange);
    bool SetString(std::string const& value);
    bool SetList(std::vector<std::string> const& value);

signals:
    void paramUpdated(bool value);
    void paramUpdated(int value);
    void paramUpdated(float value);
    void paramUpdated(std::string const& value);
    void paramUpdated();

private slots:
    void paramChanged(int value);
    void paramChanged(QString const&);
    void paramChanged();

private:
    DspParameter _param;
    QMenu* _contextMenu;
    QWidgetAction* _action;

    QCheckBox* _checkbox;
    QSlider* _slider;
    QLineEdit* _textBox;
    QComboBox* _listBox;
    QPushButton* _button;
};

#endif  // QTPPARAM_H
