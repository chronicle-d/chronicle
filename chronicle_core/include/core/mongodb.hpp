#ifndef CHRONICLE_MONGODB_HPP
#define CHRONICLE_MONGODB_HPP
/*
 * MongoDB driver wrapper for easier use inside chronicle
 *
 * Database name:   chronicle_db
 * Databse user:    chronicle_user
 * Database passwd: w8K4<2Y~snE}n_+l}+
 * Collections:
 *  - devices  | All devices and their information
 *  - users    | All chroniucle users
 *  - settings | Chronicle settings
 */

#include "mongocxx/collection-fwd.hpp"
#include "mongocxx/database-fwd.hpp"
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/client.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <mongocxx/exception/exception.hpp>
#include <bsoncxx/json.hpp>

#include <mongocxx/collection.hpp>
#include <mongocxx/database.hpp>
#include <mongocxx/options/find.hpp>

#include <string>

#include "core/error_handler.hpp"

class MongoDB {
  private:
    static void ensureInstance();
    void initDatabase();
    inline static const std::string databaseName_ = "chronicle_db";
    inline static const std::string databaseUser_ = "chronicle_user";
    inline static const std::string databaseUserPassword_ = "w8K4<2Y~snE}n_+l}+";
    mongocxx::client client_;
    mongocxx::database db_;
  public:
    MongoDB();
    void insertDocument(mongocxx::collection& collection, const bsoncxx::document::view_or_value& doc);
    void updateDocument(mongocxx::collection& collection, const std::string& deviceNickname, const bsoncxx::document::view_or_value& data);
    void deleteDocument(mongocxx::collection& collection, const std::string& deviceNickname);
    std::vector<bsoncxx::document::value> findDocuments(
      mongocxx::collection& collection,
      const bsoncxx::document::view_or_value& filter,
      const bsoncxx::document::view_or_value& projection,
      std::optional<int> limit = std::nullopt
    );

    mongocxx::collection devices_c;
    mongocxx::collection users_c;
    mongocxx::collection settings_c;
};
#endif // CHRONICLE_MONGODB_HPP
