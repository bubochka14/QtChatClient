#pragma once
#include <qobject.h>
#include "roommodel.h"
#include "messagemodel.h"
#include "clientmethoddispatcher.h"
#include "servermethodcaller.h"
#include "userinfo.h"
#include "qfuturewatcher.h"
#include "usersmodel.h"
#include <qloggingcategory.h>
#include "network_include.h"
#include "chatcontroller.h"
Q_DECLARE_LOGGING_CATEGORY(LC_CROOM_CONTROLLER);
Q_DECLARE_LOGGING_CATEGORY(LC_CUSER_CONTROLLER);
Q_DECLARE_LOGGING_CATEGORY(LC_CMESSAGE_CONTROLLER);

class CC_NETWORK_EXPORT CallerRoomController : public RoomController
{
	Q_OBJECT;
public:
	explicit CallerRoomController(ServerMethodCaller* caller,ClientMethodDispatcher* disp, QObject* parent = nullptr);
	QFuture<void> createGroup(const QString& name) override;
	QFuture<void> createDirect(int userID) override;
	QFuture<void> addUserToRoom(int userID, int roomId) override;
	QFuture<void> deleteRoom(int id) override;
	QFuture<void> updateRoom(const QVariantHash& data) override;
	QFuture<bool> initialize(UserInfo* user) override;

protected:
	void connectToDispatcher();
private:
	ServerMethodCaller* _caller;
	ClientMethodDispatcher* _disp;
};
class CC_NETWORK_EXPORT CallerMessageController : public MessageController
{
	Q_OBJECT;

public:
	explicit CallerMessageController(ServerMethodCaller* caller, ClientMethodDispatcher* disp, QObject* parent = nullptr);
	QFuture <MessageModel*> getRoomHistory(int roomId) override;
	QFuture<void> updateMessage(const QVariantHash& data) override;
	QFuture<void> createMessage(const QString& body, int roomId) override;
	QFuture<void> deleteMessage(int roomid, int messId) override;
	QFuture<void> markAsRead(int roomID, size_t count) override;
	QFuture<bool> initialize(UserInfo* user) override;
protected:
	void connectToDispatcher();
	void reset() override;
private:
	ServerMethodCaller* _caller;
	ClientMethodDispatcher* _disp;
	QMap<int, MessageModel*> _history;
	int _tempMessageCounter;

};
class CC_NETWORK_EXPORT CallerUserController : public UserController
{
	Q_OBJECT;
;
public:
	explicit CallerUserController(ServerMethodCaller* caller, ClientMethodDispatcher* disp, QObject* parent = nullptr);
	QFuture<UsersModel*> findUsers(const QVariantHash& pattern, int limit) override;
	QFuture<UserInfo*> getUserInfo(int userId) override;
	QFuture<void> updateUser(const QVariantHash& data) override;
	QFuture<void> deleteUser() override;
protected:
	void reset() override;
private:
	ClientMethodDispatcher* _disp;
	ServerMethodCaller* _caller;
	QMap<int, UserInfo*> _users;
	QMap<int, QFuture<UserInfo*>> _calls;

};
class CC_NETWORK_EXPORT CallerControllerManager : public ControllerManager
{
public:
	explicit CallerControllerManager(ServerMethodCaller* caller,ClientMethodDispatcher* disp, QObject* parent = nullptr);
	RoomController* roomController() override;
	MessageController* messageController() override;
	UserController* userController() override;
	Calls::Controller* callController() override;
private:
	RoomController* room;
	MessageController* message;
	UserController* user;
	Calls::Controller* call;
};