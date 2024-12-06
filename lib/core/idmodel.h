#pragma once
#include <QAbstractItemModel>
#include <unordered_map>
#include <deque>
template<class T, template<class>class C = std::deque>
class IdentifyingModel : public QAbstractListModel
{
public:
	explicit IdentifyingModel(const QHash<int,QByteArray> & roles,QObject* parent)
		:QAbstractListModel(parent)
	{
		_roles.insert(roles);
	}
	QVariant data(const QModelIndex& index, int role) const override final
	{
		if (!index.isValid() || index.row() >= rowCount() || index.row() < 0)
			return QVariant();
		return read(_data[index.row()], index.row(), role);

	}
	QVariant data(int id, int role) const 
	{
		if (!_index.contains(id))
			return QVariant();
		return read(_data[id],_index[id], role);

	}
	bool setData(const QModelIndex& index, const QVariant& value, int role) override final
	{
		if (!index.isValid() || !value.isValid() || index.row() >= rowCount() || index.row() < 0)
		{
			return false;
		}
		if (role == IDRole() && value.canConvert<int>())
		{
			int lastID = read(_data[index.row()], index.row(), role).toInt();
			if (lastID != value.toInt())
			{
				if (!edit(_data[index.row()], value, index.row(), role))
					return false;
				_index[value.toInt()] = index.row();
				_index.remove(lastID);
				return true;
			}
		}
		if (edit(_data[index.row()], value, index.row(), role))
		{

			emit dataChanged(index, index);
			return true;
		}
		return false;

	}
	QModelIndex idToIndex(int id) const
	{
		if (!_index.contains(id))
		{
			qWarning() << "Specified id " << id << " not found";
			return QModelIndex();
		}
		return index(_index[id]);
	}
	bool setData(int id, const QVariant& value, int role)
	{
		if (!_index.contains(id))
		{
			return false;
		}
		if (edit(_data[id],value, _index[id], role))
		{
			if(role==IDRole() && value.canConvert<int>())
			{
				if(id != value.toInt())
				{
					_index[value.toInt()] = _index[id];
					_index.remove(id);
				}
			}
			QModelIndex changed = index(_index[id]);
			emit dataChanged(changed, changed);
			return true;
		}
		return false;
	}
	int rowCount(const QModelIndex& parent = QModelIndex()) const override final
	{
		return _data.size();
	}
	bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex()) override final
	{
		Q_UNUSED(parent);
		if (row < 0 || row >rowCount() || count <= 0)
			return false;
		beginInsertRows(parent, row, row + count - 1);
		if (row + count < _data.size())
		{
			for (auto i = _data.begin() + row + 1; i < _data.end(); ++i)
			{
				_index[read(*i,0,IDRole()).toInt()] += count;
			}
		}
		_data.insert(_data.begin() + row, count, std::move(create()));
		endInsertRows();
		return true;
	}
	bool insertRows(int row, int count, QList<int> id)
	{
		if (row < 0 || row >rowCount() || count <= 0 || count != id.size())
			return false;
		beginInsertRows(QModelIndex(), row, row + count - 1);
		if (row + count < _data.size())
		{
			for (auto i = _data.begin() + row + 1; i < _data.end(); ++i)
			{
				_index[read(*i, 0, IDRole()).toInt()] += count;
			}
		}
		_data.insert(_data.begin() + row, count, std::move(create()));
		for (int i = row; i < row + count; i++)
		{
			edit(_data.at(i), id[i - row], i, IDRole());
		}
		endInsertRows();
		return true;
	}
	bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override final
	{
		Q_UNUSED(parent);
		if (row<0 || row + count > rowCount() || count <= 0)
			return false;
		beginRemoveRows(parent, row, row + count - 1);
		_index.remove(read(_data[row], row, IDRole()).toInt());
		free(std::move(_data[row]));
		_data.erase(_data.begin() + row, _data.begin() + row + count);
		endRemoveRows();
		return true;
	}
	QHash<int, QByteArray> roleNames() const
	{
		return _roles;
	}
	static constexpr int IDRole() { return 0; }
protected:
	virtual bool edit(T&,const QVariant&, int row, int role) = 0;
	virtual QVariant read(const T&, int row, int role) const = 0;
	virtual T create() { return T(); };
	virtual void free(T&&) {};
private:
	//id to row
	QHash<int, int> _index;
	QHash<int, QByteArray> _roles;
	C<T> _data;
};