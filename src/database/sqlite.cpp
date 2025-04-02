#include "database/sqlite.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <cstdlib>
#include <iostream>
#include <yaml-cpp/yaml.h>

std::unique_ptr<SQLite::Database>
NewSQLiteClient(const std::string &dataSourceName) {
  try {

    auto db_ptr = std::make_unique<SQLite::Database>(
        dataSourceName, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

    CreateTables(db_ptr);

    return db_ptr;
  } catch (SQLite::Exception &e) {
    std::cerr << "SQLite error: " << e.what() << std::endl;
    throw;
  }
}

void CreateTables(std::unique_ptr<SQLite::Database> &db) {
  if (!db) {
    throw std::runtime_error("Database pointer is null");
  }

  SQLite::Statement songsTable(
      *db, "CREATE TABLE IF NOT EXISTS songs ("
           "song_id INTEGER PRIMARY KEY AUTOINCREMENT, "
           "original_filepath TEXT NOT NULL UNIQUE, "
           "processed_filepath TEXT, "
           "processing_status TEXT NOT NULL DEFAULT 'NEW',"
           // --Tracks workflow state('NEW', 'CONVERTED', 'SPECTROGRAM_PENDING',
           //                         'SPECTROGRAM_DONE', 'ERROR_CONVERSION',
           //                         'ERROR_SPECTROGRAM')

           "last_processed_timestamp INTEGER, " // --Unix timestamp of last
                                                // processing action
           "title TEXT NOT NULL, "
           "artist TEXT NOT NULL, "
           "ytID TEXT, "
           "mel_spectrogram BLOB"
           ")");

  try {
    songsTable.exec();
  } catch (SQLite::Exception &e) {
    std::cerr << "SQLite error creating table 'songs': " << e.what()
              << std::endl;
    throw;
  } catch (std::exception &e) {
    std::cerr << "Standard error during table creation: " << e.what()
              << std::endl;
    throw;
  }
}
