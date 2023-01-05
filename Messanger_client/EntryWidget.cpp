#include "EntryWidget.h"
#include "ui_EntryWidget.h"
EntryWidget::EntryWidget(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::EntryWidget)
{
	ui->setupUi(this);
	
	QObject::connect(ui->ForwardButton, &QPushButton::clicked, this, &EntryWidget::ForwardToMessageWidget);

}

EntryWidget::~EntryWidget()
{
	delete ui;
}

void EntryWidget::ForwardToMessageWidget()
{

	SetLogin();
	emit loginning();
	this->close();
}

void EntryWidget::SetLogin()
{
	if (ui->LoginTextBox->text() != "")
		login = ui->LoginTextBox->text();
	else
		ui->LoginTextBox->setStyleSheet("border: 1px solid red;");
}

QString EntryWidget::GetLogin()
{
	return login;
}
