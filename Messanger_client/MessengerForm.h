#pragma once
#ifndef MessageForm_H
#define MessageForm_H
#include <QtWidgets/QMainWindow>
#include "EntryWidget.h"
#include <ChatWidget.h>

namespace Ui { class MessengerForm; }
class MessengerForm  : public QMainWindow
{
	Q_OBJECT

public:
	MessengerForm(QWidget* parent = nullptr);
	~MessengerForm();
	void LogIn();
	ChatWidget* chat;
private:
	Ui::MessengerForm* ui;
	QString login;
	

	EntryWidget* entry;
};
#endif
