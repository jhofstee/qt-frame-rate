#pragma once

#include <QDebug>
#include <QObject>
#include <QThread>
#include <QTimer>

// Count the fps in a seperate thread, so it has its own event loop, since
// timers are also affected by testing the gui eventloop.
class FpsThread : public QThread
{
	Q_OBJECT

public:
	FpsThread() : QThread() {
		mElapsedTimer.start();
	}

	int fps = 0;
	qint64 latency = -1;

	void newFrame() {
		qint64 ms = mElapsedTimer.restart();
		if (ms > _latency)
			_latency = ms;
		_fps++;
	}

private:
	void run() override {
		for (;;) {
			sleep(1);
			fps = _fps;
			_fps = 0;
			latency = _latency;
			_latency = -1;
		}
	}

	int _fps = 0;
	QElapsedTimer mElapsedTimer;
	qint64 _latency = -1;
};

class Test : public QObject
{
	Q_OBJECT
	Q_PROPERTY(bool queued READ getQueued WRITE setQueued NOTIFY queuedChanged)

public:
	Test() : QObject()
	{
		mWorkerThread = new FpsThread();
		mWorkerThread->start();

		connect(&mTimer, &QTimer::timeout, this, &Test::onTrigger);
		mTimer.start(500);
	}

	Q_INVOKABLE void frameUpdate() {
		mWorkerThread->newFrame();
	}

	Q_INVOKABLE int fps() {
		return mWorkerThread->fps;
	}

	Q_INVOKABLE qint64 latency() {
		return mWorkerThread->latency;
	}

	bool getQueued() {
		return queued;
	}

	void setQueued(bool val) {
		if (queued == val)
			return;
		queued = val;
		emit queuedChanged();
	}

signals:
	void queuedChanged();

private slots:
	void onTrigger() {
		for (int i=0; i < 40; i++) {
			if (!queued) {
				// now this will obviously block the gui...
				doWork();
			} else {
				// but what does this do?
				QMetaObject::invokeMethod(this, [this] { doWork(); }, Qt::QueuedConnection);
			}
		}
	}

	void doWork() {
		QThread::msleep(10);
	}

private:
	QTimer mTimer;
	FpsThread *mWorkerThread;
	bool queued = false;
};
