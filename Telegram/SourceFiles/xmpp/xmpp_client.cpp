/*
This file is part of gram (Telegram Desktop UI with an XMPP backend).

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
#include "xmpp/xmpp_client.h"

#include <QXmppClient.h>
#include <QXmppError.h>
#include <QXmppMessage.h>

namespace Xmpp {

Client::Client()
: _client(std::make_unique<QXmppClient>()) {
	const auto client = _client.get();
	QObject::connect(client, &QXmppClient::stateChanged, [=](
			QXmppClient::State state) {
		switch (state) {
		case QXmppClient::DisconnectedState:
			setState(ConnectionState::Disconnected);
			break;
		case QXmppClient::ConnectingState:
			setState(ConnectionState::Connecting);
			break;
		case QXmppClient::ConnectedState:
			setState(ConnectionState::Connected);
			break;
		}
	});
	QObject::connect(client, &QXmppClient::messageReceived, [=](
			const QXmppMessage &message) {
		if (message.body().isEmpty()) {
			return; // Chat states, receipts and other payloads come later.
		}
		_incomingMessages.fire({
			.from = message.from(),
			.to = message.to(),
			.body = message.body(),
			.id = message.id(),
			.originId = message.originId(),
		});
	});
	QObject::connect(client, &QXmppClient::errorOccurred, [=](
			const QXmppError &error) {
		_errors.fire_copy(error.description);
	});
}

Client::~Client() = default;

void Client::connectToServer(const QString &jid, const QString &password) {
	_client->connectToServer(jid, password);
}

void Client::disconnectFromServer() {
	_client->disconnectFromServer();
}

void Client::sendMessage(const QString &bareJid, const QString &body) {
	auto message = QXmppMessage(QString(), bareJid, body);
	message.setType(QXmppMessage::Chat);
	_client->send(std::move(message));
}

ConnectionState Client::state() const {
	return _state;
}

rpl::producer<ConnectionState> Client::stateChanges() const {
	return _stateChanges.events();
}

rpl::producer<IncomingMessage> Client::incomingMessages() const {
	return _incomingMessages.events();
}

rpl::producer<QString> Client::errors() const {
	return _errors.events();
}

void Client::setState(ConnectionState state) {
	if (_state != state) {
		_state = state;
		_stateChanges.fire_copy(state);
	}
}

} // namespace Xmpp
