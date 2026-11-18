/*
This file is part of gram (Telegram Desktop UI with an XMPP backend).

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
#pragma once

#include "intro/intro_step.h"

#include <memory>

namespace Ui {
class InputField;
class PasswordInput;
} // namespace Ui

namespace Xmpp {

class Client;

// XMPP sign-in step for the intro flow. Built only from the widgets and
// styles of the existing intro steps (same fields as the sign-up step,
// same password field as the cloud password step).
class IntroLoginWidget final : public Intro::details::Step {
public:
	IntroLoginWidget(
		QWidget *parent,
		not_null<Main::Account*> account,
		not_null<Intro::details::Data*> data);
	~IntroLoginWidget();

	void setInnerFocus() override;
	void activate() override;
	void cancelled() override;
	void submit() override;
	rpl::producer<QString> nextButtonText() const override;

	bool hasBack() const override {
		return true;
	}

protected:
	void resizeEvent(QResizeEvent *e) override;

private:
	void updateControlsGeometry();
	void connected();
	void failed(const QString &error);

	object_ptr<Ui::InputField> _jid;
	object_ptr<Ui::PasswordInput> _password;
	std::unique_ptr<Client> _client;
	rpl::lifetime _clientLifetime;
	QString _connectingJid;
	bool _connecting = false;

};

} // namespace Xmpp
