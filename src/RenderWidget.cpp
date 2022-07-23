#include <QTabWidget>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QListView>
#include <QDoubleSpinBox>
#include "RenderWidget.h"
#include "StateMachine.h"
#include "Camera.h"
#include "Model.h"

RenderWidget::RenderWidget(QWidget* parent)
	: QWidget(parent), fps(0), firstMouseMove(true), mdTab(nullptr)
{
	ui.setupUi(this);

	// ��ʼ��״̬��
	sys = StateMachine::GetInstance();
	sys->Init(800, 600, 60);

	canvas = nullptr;
	renderLoop = new RenderLoop(nullptr);
	renderThread = new QThread(this);

	timer = new QTimer();
	connect(timer, &QTimer::timeout, this, &RenderWidget::GetFps);

	// ��Ⱦ�̺߳ͺ�����
	renderLoop->moveToThread(renderThread);
	connect(renderThread, &QThread::finished, renderLoop, &RenderLoop::deleteLater);
	connect(renderThread, &QThread::started, renderLoop, &RenderLoop::MainLoop);
	connect(renderLoop, &RenderLoop::Vsync, this, &RenderWidget::Vsync);

	renderThread->start();
	timer->start(1000);

	this->setMouseTracking(true);

	inputTimer = new QTimer();
	connect(inputTimer, &QTimer::timeout, this, &RenderWidget::DealInput);


	mdTab = new ModelTab(ui.tabWidget);
	ui.tabWidget->addTab(mdTab, QString::fromLocal8Bit("ģ������"));

	envTab = new EnvTab(ui.tabWidget);
	ui.tabWidget->addTab(envTab, QString::fromLocal8Bit("��������"));
}

RenderWidget::~RenderWidget()
{
	renderLoop->Stop();
	renderThread->quit();
	renderThread->wait();
	if (canvas)
		delete canvas;
	if (renderThread)
		delete renderThread;
	sys->Destroy();
	sys = nullptr;
	canvas = nullptr;
	renderLoop = nullptr;
	renderThread = nullptr;
	mdTab = nullptr;
}

void RenderWidget::GetFps()
{
	fps = sys->fps;
	sys->fps = 0;
}

// ����϶��¼�
void RenderWidget::mouseMoveEvent(QMouseEvent* event)
{

	if (!(event->buttons() & Qt::LeftButton))
		firstMouseMove = true;
	if (firstMouseMove)
	{
		firstMouseMove = false;
		preMousePos = event->pos();
	}
	else
	{
		QPoint delta = event->pos() - preMousePos;
		preMousePos = event->pos();
		if (event->buttons() & Qt::LeftButton) {
			sys->camera->RotateYaw(-delta.x());
			sys->camera->RotatePitch(-delta.y());
		}
	}
}

// �������¼�
void RenderWidget::wheelEvent(QWheelEvent* event)
{
	if (event->delta() > 0) {
		renderLoop->Zoom(0.1);
	}
	else {
		renderLoop->Zoom(-0.1);
	}
}

const int INPUT_RATE = 100;

// ���������¼�
void RenderWidget::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Escape) {
		renderLoop->Stop();
		QApplication* app;
		app->quit();
	}
	pressedKeys.append(static_cast<Qt::Key>(event->key()));
	if (!inputTimer->isActive()) {
		inputTimer->start(INPUT_RATE);
	}
}

void RenderWidget::keyReleaseEvent(QKeyEvent* event)
{
	if (inputTimer->isActive() && pressedKeys.isEmpty()) {
		inputTimer->stop();
		DealInput();
	}
	pressedKeys.removeAll(static_cast<Qt::Key>(event->key()));
}

void RenderWidget::DealInput()
{

	if (pressedKeys.contains(Qt::Key_W)) {
		sys->camera->MoveForward(0.05);
	}
	if (pressedKeys.contains(Qt::Key_S)) {
		sys->camera->MoveForward(-0.05);
	}
	if (pressedKeys.contains(Qt::Key_A)) {
		sys->camera->MoveRight(-0.05);
	}
	if (pressedKeys.contains(Qt::Key_D)) {
		sys->camera->MoveRight(0.05);
	}
	if (pressedKeys.contains(Qt::Key_Q)) {
		sys->camera->MoveUp(0.05);
	}
	if (pressedKeys.contains(Qt::Key_E)) {
		sys->camera->MoveUp(-0.05);
	}
}

// ��Ļˢ���ź�
void RenderWidget::Vsync(unsigned char* image, const unsigned int& num_triangles,
	const unsigned int& num_vertices)
{
	if (canvas)
		delete canvas;
	canvas = new QImage(image, 800, 600, QImage::Format_RGBA8888);
	this->setWindowTitle("Soft Renderer Ver.miHoYo by LZ@NWPU" + QString(" ==FPS %1==").arg(fps));
	update();
}

// ��Ļ�����¼�
void RenderWidget::paintEvent(QPaintEvent* event)
{
	if (canvas) {
		QPainter painter(this);
		painter.drawImage(0, 0, canvas->mirrored());
	}
	QWidget::paintEvent(event);
}
