#include "SQLiteManager.h"

SQLiteManager::~SQLiteManager()
{
	close();
}

void SQLiteManager::open(const String &path)
{
	int result = sqlite3_open(path.c_str(), &_db);

	if (result != SQLITE_OK)
		throw std::runtime_error(sqlite3_errmsg(_db));
}

void SQLiteManager::close()
{
	int result = sqlite3_close(_db);

	if (result != SQLITE_OK)
		throw std::runtime_error(sqlite3_errmsg(_db));
}
