#pragma once
#include "network_include.h"
#include <qobject.h>
#include <qqmlengine.h>
#include <qfuture.h>
#include "usermodel.h"
#include "api/user.h"
#include "api/group.h"
#include "networkcoordinator.h"
#include "abstractcontroller.h"
#include "userhandle.h"
#include "unordered_map"
#include "stack"
#include <algorithm>
namespace User{
	class CC_NETWORK_EXPORT  Controller : public AbstractController
	{
		Q_OBJECT;
		Q_PROPERTY(Handle* empty READ empty CONSTANT)
	public:
		explicit Controller(QObject* parent = nullptr);
		Q_INVOKABLE virtual QFuture<Model*> find(const QString& pattern, int limit) = 0;
		Q_INVOKABLE virtual QFuture<Handle*> get(int userID) = 0;
		Q_INVOKABLE virtual QFuture<Handle*> get() = 0;
		Q_INVOKABLE virtual QFuture<void> update(const QVariantHash& data) = 0;
		Q_INVOKABLE virtual QFuture<void> create(const QString& password,const QString& log) = 0;
		Q_INVOKABLE virtual QFuture<Model*> getGroupUsers(int groupID) =0;
		//Q_INVOKABLE virtual Handler* currentUser() = 0;
		Q_INVOKABLE virtual QFuture<void> remove() = 0;
		Handle* empty() const;
	protected:
		virtual Handle* getEmpty() const =0;

	};
	class CC_NETWORK_EXPORT CallerController final: public Controller
	{
		Q_OBJECT;
	public:
		explicit CallerController(std::shared_ptr<NetworkCoordinator> manager,
			QObject* parent = nullptr);
		QFuture<Model*> find(const QString& pattern, int limit) override;
		QFuture<Model*> getGroupUsers(int groupID) override;
		QFuture<Handle*> get(int userID) override;
		QFuture<Handle*> get() override;
		QFuture<void> create(const QString& password, const QString& log);
		QFuture<void> update(const QVariantHash& data) override;
		QFuture<void> remove() override;
		QFuture<void> initialize() override;
		void reset() override;
	protected:
		Handle* getEmpty() const override;
		Handle* getFreeHandle();
		void growHandlePool(size_t size);
		bool parseSearchString(const QString& pattern, Api::Find& req);
	private:
		QtEventLoopEmplacer _emp;
		std::shared_ptr<NetworkCoordinator> _manager;
		std::unordered_map<int, Model*> _usersInRooms;
		std::unordered_map<int, Handle*> _userHandlers;
		QHash<int, QFuture<Handle*>> _pendingRequests;
		Handle* _empty;
		std::stack<Handle*> _freeHandlePool;
		std::mutex _handleMutex;
	};
}