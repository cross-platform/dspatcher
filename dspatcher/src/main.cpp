#include <QtpMain.h>

#include <QApplication>

#include <DSPatch.h>

int main(int argv, char* args[])
{
    QApplication app(argv, args);
    QtpMain mainWindow;
    mainWindow.show();

    DspComponent c;

    QtpComp::CompInfo comp;
    comp.typeId = 0;
    comp.typeName = "Wave Decoder";
    comp.outPins.append("L Channel");
    comp.outPins.append("R Channel");
    mainWindow.registerComp(comp);

    comp.typeId = 1;
    comp.typeName = "Mp3 Decoder";
    comp.inPins.clear();
    comp.outPins.clear();
    comp.outPins.append("L Channel");
    comp.outPins.append("R Channel");
    mainWindow.registerComp(comp);

    comp.typeId = 2;
    comp.typeName = "Gain";
    comp.inPins.clear();
    comp.outPins.clear();
    comp.inPins.append("Input");
    comp.outPins.append("Output");
    mainWindow.registerComp(comp);

    comp.typeId = 3;
    comp.typeName = "Ambisonix";
    comp.inPins.clear();
    comp.outPins.clear();
    comp.inPins.append("Mono Input");
    comp.outPins.append("FL Output");
    comp.outPins.append("FR Output");
    comp.outPins.append("RL Output");
    comp.outPins.append("RR Output");
    mainWindow.registerComp(comp);

    comp.typeId = 4;
    comp.typeName = "Audio Device";
    comp.inPins.clear();
    comp.outPins.clear();
    comp.inPins.append("L Input");
    comp.inPins.append("R Input");
    comp.outPins.append("L Output");
    comp.outPins.append("R Output");
    mainWindow.registerComp(comp);

    return app.exec();
}
