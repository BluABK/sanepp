#include <iostream>
#include <list>
#include <vector>
#include <sqlite3.h>
#include <functional>
#include <type_traits>

#include <db_handler/db_handler.hpp>
namespace sane {
    /**
    * Prepare an sqlStatement
    *
    *                              Suggested step-method:
    *                                  while ((rc = sqlite3_step(t_sqlite3PreparedStatment)) == SQLITE_ROW) { //code }
    *
    * @param t_dbName              Filename of database.
    * @param t_sql                 An SQL statement.
    * @return                      List: [rc, sqlite3PreparedStatement]. if rc is not SQLITE_OK then you should abort.
    */
    sqlite3_stmt * DBHandler::prepareSqlStatement(const std::string &t_sql) {
        sqlite3_stmt *sqlite3PreparedStatement = nullptr;

        int rc;

        rc = sqlite3_prepare_v2(getDB(), t_sql.c_str(), -1, &sqlite3PreparedStatement, nullptr);
        std::cout << "sqlite3_prepare_v2: " << rc << std::endl;
        if (rc != SQLITE_OK) {
            std::cerr << "Error preparing SQLite3 statement: " << sqlite3_errmsg(getDB()) << std::endl;
            sqlite3_close(getDB());
            updateStatus(SQLITE_ERROR);
            return sqlite3PreparedStatement;
        }

        // Return the prepared statement to be stepped through and run custom code on.
        updateStatus(SQLITE_OK);

        return sqlite3PreparedStatement;
    }

    int DBHandler::finalizePreparedSqlStatement (int t_rcStatusCode, sqlite3_stmt *t_sqlite3PreparedStatement) {
        if (t_rcStatusCode != SQLITE_DONE) {
            std::cerr << "Error sqlite3_step ended but does not have status SQLITE_DONE: " << sqlite3_errmsg(getDB()) <<
                      std::endl;
            updateStatus(SQLITE_NOT_DONE);
            return t_rcStatusCode;
        }

        // Destroy the prepared statement object.
        sqlite3_finalize(t_sqlite3PreparedStatement);

        updateStatus(SQLITE_OK);
        return SQLITE_OK;
    }

    sqlite3 *DBHandler::getDB() {
        return m_db;
    }
} // namespace sane