#include "database/db.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <cstdlib>
#include <iostream>

int main(int argc, char **argv) {
  std::unique_ptr<SQLite::Database> db;
  try {
    db = NewDBClient();
    if (db != nullptr) {
      std::cout << "DB COnnection success: " << db->getFilename() << std::endl;
    }

  } catch (SQLite::Exception &e) {
    std::cout << "Error with DB: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
