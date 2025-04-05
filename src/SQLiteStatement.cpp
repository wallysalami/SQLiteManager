#include "SQLiteStatement.h"

SQLiteStatement::SQLiteStatement(sqlite3 *db, const String &query) : _db(db)
{
	int result = sqlite3_prepare_v2(_db, query.c_str(), -1, &_stmt, nullptr);

	if (result != SQLITE_OK)
		throw std::runtime_error(sqlite3_errmsg(_db));
}

JsonDocument SQLiteStatement::getRowData()
{
	JsonDocument rowData;

	for (size_t col = 0, colNumber = sqlite3_column_count(_stmt); col < colNumber; ++col)
	{
		const char *colName = sqlite3_column_name(_stmt, col);
		switch (sqlite3_column_type(_stmt, col))
		{
		case SQLITE_INTEGER:
			rowData[String(colName)] = sqlite3_column_int(_stmt, col);
			break;

		case SQLITE_TEXT:
			rowData[String(colName)] = String(reinterpret_cast<const char *>(sqlite3_column_text(_stmt, col)));
			break;

		case SQLITE_FLOAT:
			rowData[String(colName)] = sqlite3_column_double(_stmt, col);
			break;

		default:
			throw std::runtime_error("cannot handle type");
		}
	}

	return rowData;
}

SQLiteStatement::~SQLiteStatement()
{
	sqlite3_finalize(_stmt);
}

void SQLiteStatement::bind() {}

void SQLiteStatement::bind(char arg)
{
	bind((int) arg);
}

void SQLiteStatement::bind(unsigned char arg)
{
	bind((int) arg);
}

void SQLiteStatement::bind(short arg)
{
	bind((int) arg);
}

void SQLiteStatement::bind(unsigned short arg)
{
	bind((int) arg);
}

void SQLiteStatement::bind(int arg)
{
	int result = sqlite3_bind_int(_stmt, ++_paramCounter, arg);

	if (result != SQLITE_OK)
		throw std::runtime_error(sqlite3_errmsg(_db));
}

void SQLiteStatement::bind(unsigned int arg)
{
	bind((long long) arg);
}

void SQLiteStatement::bind(long arg)
{
	bind((long long) arg);
}

void SQLiteStatement::bind(unsigned long arg)
{
	bind((long long) arg);
}

void SQLiteStatement::bind(long long arg)
{
	sqlite_int64 arg64 = arg; 
	int result = sqlite3_bind_int64(_stmt, ++_paramCounter, arg64);
	
	if (result != SQLITE_OK)
		throw std::runtime_error(sqlite3_errmsg(_db));
}

void SQLiteStatement::bind(unsigned long long arg)
{
	bind((long long) arg);
}

void SQLiteStatement::bind(float arg)
{
	bind((double) arg);
}

void SQLiteStatement::bind(double arg)
{
	int result = sqlite3_bind_double(_stmt, ++_paramCounter, arg);

	if (result != SQLITE_OK)
		throw std::runtime_error(sqlite3_errmsg(_db));
}

void SQLiteStatement::bind(const String &arg)
{
	bind(arg.c_str());
}

void SQLiteStatement::bind(const char* arg)
{
	int result = sqlite3_bind_text(_stmt, ++_paramCounter, arg, -1, SQLITE_TRANSIENT);

	if (result != SQLITE_OK)
		throw std::runtime_error(sqlite3_errmsg(_db));
}

JsonDocument SQLiteStatement::evaluate()
{
	JsonDocument result;

	int stepResult;
	for (size_t index = 0; (stepResult = sqlite3_step(_stmt)) == SQLITE_ROW; ++index)
	{
		JsonDocument rowResult = getRowData();
		result[index] = rowResult;
	}

	if (stepResult != SQLITE_DONE)
		throw std::runtime_error(sqlite3_errmsg(_db));

	return result;
}
