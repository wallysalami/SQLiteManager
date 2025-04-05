#pragma once

#include <ArduinoJson.h>
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

	JsonDocument evaluate();

	friend class SQLiteManager;

private:
	sqlite3_stmt *_stmt;
	sqlite3 *_db;
	uint32_t _paramCounter = 0;

	void bind();
	void bind(char i);
	void bind(unsigned char i);
	void bind(short i);
	void bind(unsigned short i);
	void bind(int i);
	void bind(unsigned int i);
	void bind(long i);
	void bind(unsigned long i);
	void bind(long long i);
	void bind(unsigned long long i);
	void bind(float i);
	void bind(double i);
	void bind(const String &str);
	void bind(const char *str);
	JsonDocument getRowData();
};
