#if !defined(_SQLiteStatement_h)
#define _SQLiteStatement_h

#include <Arduino_JSON.h>
#include <Arduino.h>
#include <sqlite3.h>

class SQLiteStatement
{
public:
	SQLiteStatement(sqlite3 *db, const String &query);
	~SQLiteStatement();
	template <typename T, typename... Args>
	void bind(T first, Args... rest)
	{
		bind(first);
		bind(rest...);
	}

	JSONVar evaluate();

	friend class DatabaseConnection;

private:
	sqlite3_stmt *_stmt;
	sqlite3 *_db;
	uint32_t _paramCounter = 0;

	void bind();
	void bind(int i);
	void bind(const String &str);
	JSONVar getRowData();
};

#endif // _SQLiteStatement_h
