#include "src/DatabaseConnection.h"
#include <SD.h>

DatabaseConnection db;

void setup()
{
	try
	{
		Serial.begin(115200);

		if (!SD.begin())
			throw std::runtime_error("cannot begin SD");

		if (SD.exists("/test.db"))
			SD.remove("/test.db");

		db.open("/sd/test.db");

		db.execute("CREATE TABLE IF NOT EXISTS users (name TEXT NOT NULL PRIMARY KEY, age INTEGER NOT NULL) WITHOUT ROWID");
	}
	catch (const std::exception &e)
	{
		Serial.println(e.what());
		while (true)
			;
	}
}

void loop()
{
	Serial.println("Name:");
	while (!Serial.available())
		;
	String name = Serial.readString();

	Serial.println("Age:");
	while (!Serial.available())
		;
	int age = Serial.parseInt();

	try
	{
		db.execute("INSERT INTO users VALUES (?, ?)", name, age);
		while (!Serial.available())
			;
		String query = Serial.readString();
		JSONVar result = db.execute(query);
		Serial.println(result);
	}
	catch (const std::exception &e)
	{
		Serial.println(e.what());
	}
}