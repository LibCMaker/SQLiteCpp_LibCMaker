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

// The code is from:
// http://zetcode.com/db/sqlitec/

#include <sqlite3.h>
#include <stdio.h>

int main(void) {

  sqlite3 *db;
  char *err_msg = 0;

  int rc = sqlite3_open(":memory:", &db);

  if (rc != SQLITE_OK) {

    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);

    return 1;
  }

  const char *sql = "CREATE TABLE Friends(Id INTEGER PRIMARY KEY, Name TEXT);"
              "INSERT INTO Friends(Name) VALUES ('Tom');"
              "INSERT INTO Friends(Name) VALUES ('Rebecca');"
              "INSERT INTO Friends(Name) VALUES ('Jim');"
              "INSERT INTO Friends(Name) VALUES ('Roger');"
              "INSERT INTO Friends(Name) VALUES ('Robert');";

  rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

  if (rc != SQLITE_OK) {

    fprintf(stderr, "Failed to create table\n");
    fprintf(stderr, "SQL error: %s\n", err_msg);
    sqlite3_free(err_msg);
    return 1;

  } else {

    fprintf(stdout, "Table Friends created successfully\n");
  }

  int last_id = sqlite3_last_insert_rowid(db);
  printf("The last Id of the inserted row is %d\n", last_id);

  if (last_id != 5) {
    printf("ERROR: the last Id of the inserted row is NOT 5\n");
    return 1;
  }

  sqlite3_close(db);

  return 0;
}
