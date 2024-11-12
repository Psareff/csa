#include "hv/HttpServer.h"
#include <libpq-fe.h>

#include <iostream>
#include <vector>
#include <cstdlib>

#include "server.hpp"

using namespace hv;
using json = nlohmann::json;

PGconn* connectDB() {
    const char* host = getenv("POSTGRES_HOST");
    const char* dbname = getenv("POSTGRES_DB");
    const char* user = getenv("POSTGRES_USER");
    const char* password = getenv("POSTGRES_PASSWORD");

    std::string conninfo = "host=" + std::string(host ? host : "localhost") +
                           " dbname=" + std::string(dbname ? dbname : "mydatabase") +
                           " user=" + std::string(user ? user : "myuser") +
                           " password=" + std::string(password ? password : "mypassword");

    PGconn* conn = PQconnectdb(conninfo.c_str());

    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "Connection to database failed: "
                  << PQerrorMessage(conn) << std::endl;
        PQfinish(conn);
        return nullptr;
    }

    return conn;
}

void initDB() {
    PGconn* conn = connectDB();
    if (!conn) {
        std::cerr << "Failed to connect to the database for initialization." << std::endl;
        return;
    }

    const char* createTableQuery =
        "CREATE TABLE IF NOT EXISTS users ("
        "id SERIAL PRIMARY KEY,"
        "name TEXT UNIQUE NOT NULL,"
        "password TEXT NOT NULL,"
        "privilege TEXT NOT NULL"
        ");";

    PGresult* res = PQexec(conn, createTableQuery);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Failed to create users table: "
                  << PQerrorMessage(conn) << std::endl;
    }

    PQclear(res);
    PQfinish(conn);
}

int main() {
    initDB();

    HttpService router;

    router.GET("/users", [](HttpRequest* req, HttpResponse* resp) {
        PGconn* conn = connectDB();
        if (!conn) {
            resp->String("Database connection failed");
            return 500;
        }

        PGresult* res = PQexec(conn, "SELECT id, name, privilege FROM users;");
        if (PQresultStatus(res) != PGRES_TUPLES_OK) {
            resp->String("Failed to retrieve users");
            PQclear(res);
            PQfinish(conn);
            return 500;
        }

        int nrows = PQntuples(res);
        json users = json::array();

        for (int i = 0; i < nrows; i++) {
            json user;
            user["id"] = std::atoi(PQgetvalue(res, i, 0));
            user["name"] = PQgetvalue(res, i, 1);
            user["privilege"] = PQgetvalue(res, i, 2);
            users.push_back(user);
        }

        resp->Json(users);

        PQclear(res);
        PQfinish(conn);
        return 200;
    });

    router.GET("/user/{user_id}", [](HttpRequest* req, HttpResponse* resp) {
       std::string user_id = req->GetParam("user_id");

        PGconn* conn = connectDB();
        if (!conn) {
            resp->String("Database connection failed");
            return 500;
        }

        std::string query = "SELECT id, name, privilege FROM users WHERE name = $1;";
        const char* paramValues[1] = { user_id.c_str() };

        PGresult* res = PQexecParams(conn, query.c_str(), 1, NULL, paramValues, NULL, NULL, 0);
        if (PQresultStatus(res) != PGRES_TUPLES_OK) {
            resp->String("Failed to retrieve user");
            PQclear(res);
            PQfinish(conn);
            return 500;
        }
        int ret = 200;
        if (PQntuples(res) == 0) {
            ret = 404;
            resp->String("User not found");
        } else {
            json user;
            user["id"] = std::atoi(PQgetvalue(res, 0, 0));
            user["name"] = PQgetvalue(res, 0, 1);
            user["privilege"] = PQgetvalue(res, 0, 2);
            resp->Json(user);
        }

        PQclear(res);
        PQfinish(conn);
        return ret;
    });

router.POST("/user", [](HttpRequest* req, HttpResponse* resp) {
    std::cout << "POSTING USER" << std::endl;
    json u;
    try {
        u = json::parse(req->body);
    } catch (json::parse_error& e) {
        resp->String("Invalid JSON format");
        return 400;
    }

    if (!u.contains("name") || !u["name"].is_string() ||
        !u.contains("password") || !u["password"].is_string() ||
        !u.contains("privilege") || !u["privilege"].is_string()) {
        resp->String("Missing or invalid fields: name, password, and privilege are required");
        return 400;
    }

    std::string name = u["name"];
    std::string password = u["password"];
    std::string privilege = u["privilege"];

    PGconn* conn = connectDB();
    if (!conn) {
        resp->String("Database connection failed");
        return 500;
    }

    const char* insertQuery = "INSERT INTO users (name, password, privilege) VALUES ($1, $2, $3);";
    const char* paramValues[3] = { name.c_str(), password.c_str(), privilege.c_str() };

    PGresult* res = PQexecParams(conn, insertQuery, 3, NULL, paramValues, NULL, NULL, 0);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        resp->String("Failed to add user");
        PQclear(res);
        PQfinish(conn);
        return 500;
    }

    resp->String("User successfully added");

    PQclear(res);
    PQfinish(conn);
    return 200;
});

    router.PUT("/user/{user_id}", [](HttpRequest* req, HttpResponse* resp) {
        std::string user_id = req->GetParam("user_id");
        json u = json::parse(req->body);
        std::string name = u["name"];
        std::string password = u["password"];
        std::string privilege = u["privilege"];

        PGconn* conn = connectDB();
        if (!conn) {
            resp->String("Database connection failed");
            return 500;
        }

        const char* updateQuery = "UPDATE users SET name = $1, password = $2, privilege = $3 WHERE id = $4;";
        const char* paramValues[4] = { name.c_str(), password.c_str(), privilege.c_str(), user_id.c_str() };

        PGresult* res = PQexecParams(conn, updateQuery, 4, NULL, paramValues, NULL, NULL, 0);
        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            resp->String("Failed to update user");
            PQclear(res);
            PQfinish(conn);
            return 500;
        }

        resp->String("User successfully updated");

        PQclear(res);
        PQfinish(conn);
        return 200;
    });

    router.Delete("/user/{user_id}", [](HttpRequest* req, HttpResponse* resp) {
        std::string user_id = req->GetParam("user_id");

        PGconn* conn = connectDB();
        if (!conn) {
            resp->String("Database connection failed");
            return 500;
        }

        const char* deleteQuery = "DELETE FROM users WHERE id = $1;";
        const char* paramValues[1] = { user_id.c_str() };

        PGresult* res = PQexecParams(conn, deleteQuery, 1, NULL, paramValues, NULL, NULL, 0);
        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            resp->String("Failed to delete user");
            PQclear(res);
            PQfinish(conn);
            return 500;
        }

        resp->String("User successfully deleted");

        PQclear(res);
        PQfinish(conn);
        return 200;
    });

    HttpServer server;
    server.registerHttpService(&router);
    server.setPort(8080);
    server.run();

    return 0;
}
