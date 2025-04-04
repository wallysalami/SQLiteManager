#if !defined(_SQLiteDatabaseManager_h)
#define _SQLiteDatabaseManager_h

#include <Arduino.h>
#include <sqlite3.h>
#include <ArduinoJson.h>
#include "SQLiteStatement.h"
#include <mutex>

class DatabaseConnection
{
public:
	DatabaseConnection() = default;
	~DatabaseConnection();

	void open(const String &path);
	void close();
	template <typename... Args>
	JsonDocument execute(const String &query, Args... args)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		SQLiteStatement stmt(_db, query);
		stmt.bind(args...);
		return stmt.evaluate();
	}

	friend class SQLiteStatement;

private:
	sqlite3 *_db;
	std::mutex _mutex;
};

#endif // _SQLiteDatabaseManager_h
