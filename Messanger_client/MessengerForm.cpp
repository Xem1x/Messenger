#include "MessengerForm.h"

#include "ui_MessengerForm.h"
MessengerForm::MessengerForm(QWidget* parent)
	: QMainWindow(parent),
	ui(new Ui::MessengerForm)
{
	ui->setupUi(this);
	entry = new EntryWidget();
	chat = new ChatWidget();
	
	
	this->layout()->addWidget(entry);
	entry->show();
	QObject::connect(entry, &EntryWidget::loginning, this, &MessengerForm::LogIn);
	
}

MessengerForm::~MessengerForm()
{
	delete ui;
}

void MessengerForm::LogIn()
{

	login = entry->GetLogin();
	//entry->close();
	this->layout()->addWidget(chat);
	chat->show();
	chat->setAutoFillBackground(true);
	chat->setUserLogin(login);
	chat->connect_to("127.0.0.1", 25565);
	chat->join_server();
	


}



