#include <iostream>
#include <list>
#include <vector>
#include <sqlite3.h>
#include <functional>
#include <type_traits>

#include <db_handler/db_handler.hpp>
namespace sane {
    sqlite3 *DBHandler::m_db = nullptr;

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

        rc = sqlite3_prepare_v2(m_db, t_sql.c_str(), -1, &sqlite3PreparedStatement, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Error preparing SQLite3 statement: " << sqlite3_errmsg(m_db) << std::endl;
            sqlite3_close(m_db);
            updateStatus(rc);
            return sqlite3PreparedStatement;
        }
        updateStatus(SQLITE_OK);

        //  Optional, but will most likely increase performance.
        rc = sqlite3_exec(m_db, "BEGIN TRANSACTION", nullptr, nullptr, nullptr);
        updateStatus(rc);

        // Return the prepared statement to be stepped through and run custom code on.
        return sqlite3PreparedStatement;
    }

    int DBHandler::executeSqlStatement(const std::string &t_sql) {
//        sqlite3_exec(m_db, t_sql, );
        return 0;
    }

    int DBHandler::finalizePreparedSqlStatement (int t_rcStatusCode, sqlite3_stmt *t_sqlite3PreparedStatement) {
        // FIXME: Check triggers false positive "not an error" rc status.
//        if (t_rcStatusCode != SQLITE_DONE) {
//            std::cerr << "Error sqlite3_step ended but does not have status SQLITE_DONE: " << sqlite3_errmsg(m_db) <<
//                      std::endl;
//            updateStatus(SQLITE_NOT_DONE);
//            return t_rcStatusCode;
//        }
        char * zErrMsg;
        int rc = sqlite3_exec(m_db, "END TRANSACTION", nullptr, nullptr, &zErrMsg );   //  End the transaction.

        if (rc != SQLITE_OK) {
            std::cerr << zErrMsg << std::endl;
            return rc;
        }

        updateStatus(rc);

        // Finalize the prepared statement object.
        sqlite3_finalize(t_sqlite3PreparedStatement);

        updateStatus(SQLITE_OK);
        return SQLITE_OK;
    }
} // namespace sane