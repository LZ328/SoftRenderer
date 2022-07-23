#include <QFileDialog>
#include <QImage>
#include <QPixmap>
#include <QSpinBox>
#include "StateMachine.h"
#include "MaterialDialog.h"
#include "MaterialBase.h"
#include "Texture.h"
#include "Image.h"

MaterialDialog::MaterialDialog(Material* m, QWidget* parent)
	: material(m), QDialog(parent)
{
	ui.setupUi(this);
	init = false;
	sys = StateMachine::GetInstance();
	connect(ui.moB, &QPushButton::clicked, this, &MaterialDialog::OpenMainTexture);
	connect(ui.mrB, &QPushButton::clicked, this, &MaterialDialog::RemoveMainTexture);
	connect(ui.cSBR, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &MaterialDialog::SetMaterial);
	connect(ui.cSBG, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &MaterialDialog::SetMaterial);
	connect(ui.cSBB, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &MaterialDialog::SetMaterial);
	connect(ui.sSBR, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &MaterialDialog::SetMaterial);
	connect(ui.sSBG, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &MaterialDialog::SetMaterial);
	connect(ui.sSBB, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &MaterialDialog::SetMaterial);
	connect(ui.gSB, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &MaterialDialog::SetMaterial);

	ShowTexture(material->mainTex.get(), ui.mCav);

	ui.cSBR->setValue(material->color.r * 255);
	ui.cSBG->setValue(material->color.g * 255);
	ui.cSBB->setValue(material->color.b * 255);

	ui.sSBR->setValue(material->specular.r * 255);
	ui.sSBG->setValue(material->specular.g * 255);
	ui.sSBB->setValue(material->specular.b * 255);

	ui.gSB->setValue(material->gloss);

	init = true;
}

MaterialDialog::~MaterialDialog() {}

void MaterialDialog::ShowTexture(Texture2D* t, QLabel* cav)
{
	if (t == nullptr)
		return;
	Image* img = t->GetImage();
	QImage* tmp;
	if (img->channel == 3)
		tmp = new QImage(img->data, img->width, img->height, QImage::Format_RGB888);
	else if (img->channel == 4)
		tmp = new QImage(img->data, img->width, img->height, QImage::Format_RGBA8888);
	else if (img->channel == 1)
		tmp = new QImage(img->data, img->width, img->height, QImage::Format_Grayscale8);
	QImage image = tmp->scaled(100, 100);
	delete tmp;
	cav->setPixmap(QPixmap::fromImage(image));
}

void MaterialDialog::OpenMainTexture() {
	sys->mutex.lock();
	QFileDialog* fileDialog = new QFileDialog(this);
	fileDialog->setWindowTitle(QStringLiteral("´ò¿ªÎÆÀí"));
	fileDialog->setDirectory(".");
	fileDialog->setNameFilter(tr("Texture File(*.bmp *.jpg *.jpeg *.png *.tga *.hdr)"));
	fileDialog->setFileMode(QFileDialog::ExistingFiles);
	fileDialog->setViewMode(QFileDialog::Detail);
	QStringList fileNames;
	if (fileDialog->exec()) {
		fileNames = fileDialog->selectedFiles();
		Texture2D* tex = new Texture2D(fileNames[0].toStdString());
		material->SetTexture(tex);
		ShowTexture(tex, ui.mCav);
		delete tex;
	}
	sys->mutex.unlock();
}

void MaterialDialog::RemoveMainTexture()
{
	sys->mutex.lock();
	material->mainTex = nullptr;
	ui.mCav->clear();
	sys->mutex.unlock();
}

void MaterialDialog::SetMaterial()
{
	if (!init)
		return;
	material->color.r = ui.cSBR->value() / 255.0f;
	material->color.g = ui.cSBG->value() / 255.0f;
	material->color.b = ui.cSBB->value() / 255.0f;
	material->specular.r = ui.sSBR->value() / 255.0f;
	material->specular.g = ui.sSBG->value() / 255.0f;
	material->specular.b = ui.sSBB->value() / 255.0f;
	material->gloss = ui.gSB->value();
}
