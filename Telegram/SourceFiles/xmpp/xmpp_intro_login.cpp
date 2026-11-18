/*
This file is part of gram (Telegram Desktop UI with an XMPP backend).

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
#include "xmpp/xmpp_intro_login.h"

#include "xmpp/xmpp_client.h"
#include "intro/intro_widget.h"
#include "lang/lang_keys.h"
#include "ui/widgets/fields/input_field.h"
#include "ui/widgets/fields/password_input.h"
#include "styles/style_intro.h"

namespace Xmpp {
namespace {

[[nodiscard]] bool IsValidJid(const QString &jid) {
	const auto at = jid.indexOf('@');
	return (at > 0)
		&& (at == jid.lastIndexOf('@'))
		&& (at + 1 < jid.size())
		&& !jid.contains(' ');
}

} // namespace

IntroLoginWidget::IntroLoginWidget(
	QWidget *parent,
	not_null<Main::Account*> account,
	not_null<Intro::details::Data*> data)
: Step(parent, account, data)
, _jid(this, st::introName, tr::lng_xmpp_jid())
, _password(this, st::introPassword, tr::lng_xmpp_password()) {
	_jid->changes(
	) | rpl::on_next([=] {
		hideError();
	}, _jid->lifetime());
	_jid->submits(
	) | rpl::on_next([=] {
		_password->setFocus();
	}, _jid->lifetime());
	connect(_password, &Ui::PasswordInput::changed, [=] { hideError(); });

	setTitleText(tr::lng_xmpp_login_title());
	setDescriptionText(tr::lng_xmpp_login_desc());
	setMouseTracking(true);
}

IntroLoginWidget::~IntroLoginWidget() = default;

void IntroLoginWidget::resizeEvent(QResizeEvent *e) {
	Step::resizeEvent(e);
	updateControlsGeometry();
}

void IntroLoginWidget::updateControlsGeometry() {
	// Same placement as the first / last name fields of the sign-up step.
	const auto firstTop = contentTop() + st::introStepFieldTop;
	const auto secondTop = firstTop
		+ st::introName.heightMin
		+ st::introPhoneTop;
	_jid->moveToLeft(contentLeft(), firstTop);
	_password->moveToLeft(contentLeft(), secondTop);
}

void IntroLoginWidget::setInnerFocus() {
	if (_jid->getLastText().trimmed().isEmpty()) {
		_jid->setFocusFast();
	} else {
		_password->setFocusFast();
	}
}

void IntroLoginWidget::activate() {
	Step::activate();
	_jid->show();
	_password->show();
	setInnerFocus();
}

void IntroLoginWidget::cancelled() {
	_clientLifetime.destroy();
	if (_client) {
		_client->disconnectFromServer();
		_client = nullptr;
	}
	_connecting = false;
}

void IntroLoginWidget::submit() {
	if (_connecting) {
		return;
	}
	const auto jid = _jid->getLastText().trimmed();
	const auto password = _password->getLastText();
	if (!IsValidJid(jid)) {
		_jid->setFocus();
		_jid->showError();
		showError(tr::lng_xmpp_bad_jid());
		return;
	} else if (password.isEmpty()) {
		_password->setFocus();
		_password->showError();
		return;
	}
	hideError();
	cancelled();

	_connecting = true;
	_connectingJid = jid;
	setDescriptionText(tr::lng_connecting());

	_client = std::make_unique<Client>();
	_client->stateChanges(
	) | rpl::on_next([=](ConnectionState state) {
		if (_connecting && state == ConnectionState::Connected) {
			connected();
		}
	}, _clientLifetime);
	_client->errors(
	) | rpl::on_next([=](const QString &error) {
		failed(error);
	}, _clientLifetime);
	_client->connectToServer(jid, password);
}

void IntroLoginWidget::connected() {
	_connecting = false;
	// Next step: hand the client to an XMPP-backed Main::Session and open
	// the chat list. Until then, confirm the sign-in on this screen.
	setDescriptionText(tr::lng_xmpp_connected(
		lt_jid,
		rpl::single(_connectingJid)));
}

void IntroLoginWidget::failed(const QString &error) {
	if (!_connecting) {
		return;
	}
	// We are inside a signal of _client here, so it must not be destroyed
	// now. The next submit() or cancelled() releases it.
	_connecting = false;
	setDescriptionText(tr::lng_xmpp_login_desc());
	_password->selectAll();
	_password->setFocus();
	_password->showError();
	showError(tr::lng_xmpp_connect_failed(
		lt_error,
		rpl::single(error)));
}

rpl::producer<QString> IntroLoginWidget::nextButtonText() const {
	return tr::lng_intro_submit();
}

} // namespace Xmpp
