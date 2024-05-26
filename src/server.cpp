#include "hv/HttpServer.h"

#include <iostream>
#include <vector>
#include <string>

#include "server.hpp"


using namespace hv;
using namespace std;

void init()
{
	struct user admin;
	admin.name = "admin";
	admin.password = "password";
	admin.priv = user::admin;

	add_user(admin);
}

int main()
{
	init();

	HttpService router;

	router.GET("/users", [](HttpRequest* req, HttpResponse* resp) {
		validate_user(
			if (check_permission(splitted_auth.front(), "", priv_e) == 2)
			{
				nlohmann::json j;
				for (auto i : users)
				{
					nlohmann::json u;
					user_cast_to_json(u, i);
					j["users"].push_back(u);
				}
				resp->Json(j);
				return 200;
			}
		);
	});

	router.GET("/user/{user_id}", [](HttpRequest* req, HttpResponse* resp)
	{
		std::string param = req->GetParam("user_id");
		validate_user(
			if (check_permission(splitted_auth.front(), param, priv_e) >= 1)
			{
				nlohmann::json u;
				for (auto i : users)
				{
					if (i.name == param)
					{
						user_cast_to_json(u, i);
						resp->Json(u);
						return 200;
					}
				}
				resp->String("Requested user not found");
				return 404;
			}
		);
	});

	router.POST("/user", [](HttpRequest* req, HttpResponse* resp)
	{
		validate_user(
			if (check_permission(splitted_auth.front(), "", priv_e) == 2)
			{

				nlohmann::json u = nlohmann::json::parse(req->body);
				user usr;
				user_cast_from_json(usr, u);
				users.push_back(usr);
				resp->String("User successfully added");
				return 200;
			}
		);
	});

	router.Delete("/user/{user_id}", [](HttpRequest* req, HttpResponse* resp)
	{
		std::string param = req->GetParam("user_id");
		validate_user(
			if (check_permission(splitted_auth.front(), param, priv_e) >= 1)
			{
				users.erase(std::remove_if( users.begin(), users.end(), [param](user& x) { return x.name == param;}), users.end());
				return 200;
			}
		);
	});

	router.PUT("/user/{user_id}", [](HttpRequest* req, HttpResponse* resp)
	{
		std::string param = req->GetParam("user_id");
		validate_user(
			if (check_permission(splitted_auth.front(), param, priv_e) >= 1)
			{
				nlohmann::json u = nlohmann::json::parse(req->body);
				for (auto iter = users.begin(); iter != users.end(); ++iter)
				{
					user usr = *iter;
					if ( usr.name == param)
					{
						(*iter).name = u["name"];
						(*iter).password = u["password"];
						resp->String("User successfully changed");
						return 200;
					}
				}
				resp->String("Requested user not found");
				return 404;
			}
		);
	});

	HttpServer server(&router);
	server.setPort(8080);
	server.setThreadNum(4);
	server.run();
	return 0;
}
