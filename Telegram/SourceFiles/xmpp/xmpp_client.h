/*
This file is part of gram (Telegram Desktop UI with an XMPP backend).

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
#pragma once

#include <rpl/event_stream.h>
#include <rpl/producer.h>

#include <QtCore/QString>

#include <memory>

class QXmppClient;

namespace Xmpp {

enum class ConnectionState {
	Disconnected,
	Connecting,
	Connected,
};

struct IncomingMessage {
	QString from;
	QString to;
	QString body;
	QString id;
	QString originId;
};

// Thin wrapper over QXmppClient that exposes rpl streams, so the
// Telegram Desktop data layer can consume XMPP events the same way it
// consumes MTProto updates.
class Client final {
public:
	Client();
	~Client();

	void connectToServer(const QString &jid, const QString &password);
	void disconnectFromServer();
	void sendMessage(const QString &bareJid, const QString &body);

	[[nodiscard]] ConnectionState state() const;
	[[nodiscard]] rpl::producer<ConnectionState> stateChanges() const;
	[[nodiscard]] rpl::producer<IncomingMessage> incomingMessages() const;
	[[nodiscard]] rpl::producer<QString> errors() const;

private:
	void setState(ConnectionState state);

	const std::unique_ptr<QXmppClient> _client;
	ConnectionState _state = ConnectionState::Disconnected;
	rpl::event_stream<ConnectionState> _stateChanges;
	rpl::event_stream<IncomingMessage> _incomingMessages;
	rpl::event_stream<QString> _errors;

};

} // namespace Xmpp
