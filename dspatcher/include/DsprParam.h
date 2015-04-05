#ifndef DSPRPARAM_H
#define DSPRPARAM_H

#include <DSPatch.h>

#include <QObject>
#include <QtWidgets>

class QtpWire;

class DsprParam : public QObject
{
    Q_OBJECT

public:
    DsprParam(int compId, int paramId, std::string const& name, DspParameter const& param, QMenu* contextMenu);
    QWidgetAction* action();
    DspParameter& param();

    int compId();
    int paramId();

    bool SetBool(bool const& value);
    bool SetInt(int const& value);
    bool SetIntRange(std::pair<int, int> const& intRange);
    bool SetFloat(float const& value);
    bool SetFloatRange(std::pair<float, float> const& floatRange);
    bool SetString(std::string const& value);
    bool SetList(std::vector<std::string> const& value);

signals:
    void boolUpdated(bool value);
    void intUpdated(int value);
    void floatUpdated(float value);
    void stringUpdated(std::string const& value);
    void triggerUpdated();

private slots:
    void paramChanged(int value);
    void paramChanged(QString const&);
    void paramChanged();

    void updateFloatSlider(int value);
    void browseForFile();

private:
    bool _settingParam;
    int _compId;
    int _paramId;
    DspParameter _param;
    QMenu* _contextMenu;
    QWidgetAction* _action;

    QCheckBox* _checkbox;
    QSlider* _slider;
    QLineEdit* _textBox;
    QComboBox* _listBox;
    QPushButton* _button;
    QLabel* _vlabel;
};

#endif  // DSPRPARAM_H
