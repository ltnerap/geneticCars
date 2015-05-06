#include <QGraphicsItem>
#include <QGraphicsView>
#include <QTimer>
#include <QWidget>

//graphics scene class
class QGraphicsScene;

//window view class
class WindowView : public QGraphicsView
{
	Q_OBJECT
public:
	WindowView(QGraphicsScene*, QWidget*x=  NULL);
protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
};

//timer handler class
class TimerHandler : public QWidget
{
Q_OBJECT
public:
	TimerHandler(int);
protected slots:
	void onTimer();
};
