#ifndef USER_H_
#define USER_H_

#include <string>
#include "nlohmann/json.hpp"


struct user
{
		enum privileges_e { common, admin, invalid };
		std::string name;
		privileges_e priv;
		std::string password;
};

std::string user_privileges_to_string(user::privileges_e priv)
{
	switch(priv)
	{
		case user::common:
			return std::string("common");
		case user::admin:
			return std::string("admin");
		case user::invalid:
			return std::string("invalid");
	}
	return std::string("invalid");
}

user::privileges_e user_string_to_privileges(std::string priv)
{
	if (priv == "common")
		return user::common;
	if (priv == "admin")
		return user::admin;
	return user::invalid;
}

void user_cast_to_json(nlohmann::json &json, const user &user)
{
	json = nlohmann::json
	{
		{"name", user.name},
		{"password", user.password},
		{"privileges", user_privileges_to_string(user.priv)}
	};
}

void user_cast_from_json(user &user, const nlohmann::json &json)
{
	user.name = json["name"];
	user.password = json["password"];
	user.priv = json["privileges"];
}

bool user_is_admin(const user &user)
{
	return user.priv == user::admin ? true : false;
}

#endif // USER_H_
