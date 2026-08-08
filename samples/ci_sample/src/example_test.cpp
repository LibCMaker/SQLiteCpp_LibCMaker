/*****************************************************************************
 * Project:  LibCMaker
 * Purpose:  A CMake build scripts for build libraries with CMake
 * Author:   NikitaFeodonit, nfeodonit@yandex.com
 *****************************************************************************
 *   Copyright (c) 2017-2026 NikitaFeodonit
 *
 *    This file is part of the LibCMaker project.
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published
 *    by the Free Software Foundation, either version 3 of the License,
 *    or (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *    See the GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program. If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/

// The code is based on the codes from:
// http://zetcode.com/db/sqlitec/
// https://github.com/SRombauts/SQLiteCpp#examples

#include <sqlite3.h>

#include <SQLiteCpp/SQLiteCpp.h>

#include "gtest/gtest.h"

int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    EXPECT_EQ(3, argc);

    EXPECT_STREQ("name_00", azColName[0]);
    EXPECT_STREQ("ԱաԲբԳգ", argv[0]);

    EXPECT_STREQ("name_01", azColName[1]);
    EXPECT_STREQ("ԱԱԲԲԳԳ", argv[1]);

    EXPECT_STREQ("name_02", azColName[2]);
    EXPECT_STREQ("աաբբգգ", argv[2]);

    return 0;
}

TEST(Examle, sqlite)
{
  sqlite3 *db;
  char *err_msg = NULL;

  int rc = sqlite3_open(":memory:", &db);
  EXPECT_EQ(rc, SQLITE_OK) << "Cannot open database: " << sqlite3_errmsg(db);
  if (rc != SQLITE_OK) {
    rc = sqlite3_close(db);
    EXPECT_EQ(rc, SQLITE_OK) << "Cannot close database";
    ASSERT_TRUE(false);
  }

  const char *sql =
      "CREATE TABLE Friends(Id INTEGER PRIMARY KEY, Name TEXT);"
      "INSERT INTO Friends(Name) VALUES ('Rebecca');"
      "INSERT INTO Friends(Name) VALUES ('ԱաԲբԳգ');";

  rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
  EXPECT_EQ(rc, SQLITE_OK) << err_msg;
  if (rc != SQLITE_OK) {
    sqlite3_free(err_msg);
    ASSERT_TRUE(false);
  }

  int last_id = sqlite3_last_insert_rowid(db);
  EXPECT_EQ(last_id, 2);

  const char *sql_select =
    "SELECT Name as name_00, "
      "upper(Name, 'hy_AM') as name_01, "
      "lower(Name, 'hy_AM') as name_02 "
      "FROM Friends WHERE Name = 'ԱաԲբԳգ';";

  rc = sqlite3_exec(db, sql_select, callback, NULL, &err_msg);
  EXPECT_EQ(rc, SQLITE_OK) << err_msg;
  if (rc != SQLITE_OK) {
    sqlite3_free(err_msg);
    ASSERT_TRUE(false);
  }

  rc = sqlite3_close(db);
  EXPECT_EQ(rc, SQLITE_OK) << "Cannot close database";
}

TEST(Examle, SQLiteCpp)
{
  try
  {
    SQLite::Database db(":memory:", SQLite::OPEN_READWRITE);

    int last_id = db.exec(
      "CREATE TABLE Friends(Id INTEGER PRIMARY KEY, Name TEXT);");
    EXPECT_EQ(0, last_id);

    last_id = db.exec(
      "INSERT INTO Friends(Name) VALUES ('Rebecca');");
    EXPECT_EQ(1, last_id);

    last_id = db.exec(
      "INSERT INTO Friends(Name) VALUES ('ԱաԲբԳգ');");
    EXPECT_EQ(1, last_id);

    SQLite::Statement sql_select(db,
      "SELECT Name as name_00, "
        "upper(Name, 'hy_AM') as name_01, "
        "lower(Name, 'hy_AM') as name_02 "
        "FROM Friends WHERE Name = 'ԱաԲբԳգ';");

    EXPECT_TRUE(sql_select.executeStep());
    EXPECT_EQ(3, sql_select.getColumnCount());

    EXPECT_STREQ("name_00", sql_select.getColumnName(0));
    EXPECT_STREQ("ԱաԲբԳգ", sql_select.getColumn(0));

    EXPECT_STREQ("name_01", sql_select.getColumnName(1));
    EXPECT_STREQ("ԱԱԲԲԳԳ", sql_select.getColumn(1));

    EXPECT_STREQ("name_02", sql_select.getColumnName(2));
    EXPECT_STREQ("աաբբգգ", sql_select.getColumn(2));

    EXPECT_FALSE(sql_select.executeStep());
    EXPECT_TRUE(sql_select.isDone());
  }
  catch (std::exception& e)
  {
    EXPECT_TRUE(false) << "exception: " << e.what();
  }
}
