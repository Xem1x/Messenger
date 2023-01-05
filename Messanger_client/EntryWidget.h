#pragma once
#ifndef EntryWidget_H
#define EntryWidget_H
#include <QtWidgets/QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class EntryWidget; };
QT_END_NAMESPACE

class EntryWidget : public QWidget
{
	Q_OBJECT

public:
	EntryWidget(QWidget *parent = nullptr);
	~EntryWidget();

	void ForwardToMessageWidget();

	void SetLogin();
	QString GetLogin();

signals:
	void loginning();

private:
	Ui::EntryWidget *ui;
	QString login;
	
};
#endif
