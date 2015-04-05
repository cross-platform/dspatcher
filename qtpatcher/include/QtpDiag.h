#ifndef QTPDIAG_H
#define QTPDIAG_H

#include <QtpComp.h>
#include <QGraphicsScene>

class QtpPin;

class QtpDiag : public QGraphicsScene
{
    Q_OBJECT

public:
    enum Mode
    {
        InsertComp,
        InsertLine,
        MoveComp
    };

    explicit QtpDiag(QObject* parent = 0);

    void setLineColor(const QColor& color);
    void setCompColor(const QColor& color);

    QColor compColor() const;
    QColor lineColor() const;

signals:
    void compInserted(QtpComp* comp);
    void compRemoved(int compId);
    void wireConnected(int fromComp, int fromPin, int toComp, int toPin);
    void wireDisconnected(int fromComp, int fromPin, int toComp, int toPin);

public slots:
    void setMode(Mode mode);
    void setNextComp(QtpComp::CompInfo const& compInfo);

    void bringToFront();
    void sendToBack();
    void deleteItem();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* mouseEvent);

private:
    bool isItemChange(int type);

    int _compId;
    QtpComp::CompInfo _nextComp;
    Mode _mode;
    bool _leftButtonDown;
    QPointF _startPoint;
    QGraphicsLineItem* _line;
    QColor _compColor;
    QColor _lineColor;
    QtpPin* _pinHovered;
};

#endif  // QTPDIAG_H
