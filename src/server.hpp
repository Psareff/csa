#ifndef SERVER_H_
#define SERVER_H_

#include "User.hpp"
#include "UserContainer.hpp"
#include "Utility.hpp"

#define validate_user(custom) \
	auto basic_auth = req->GetHeader("Authorization"); \
	if (!basic_auth.empty()) \
	{ \
		auto splitted = utils::Split(basic_auth, " "); \
		if (splitted.size() == 2 && \
		    splitted.front() == "Basic") \
		{ \
			auto decode = utils::DecodeBase64(splitted.back()); \
			auto splitted_auth = utils::Split(decode, ":"); \
			if (splitted_auth.size() == 2) \
			{ \
				int priv; \
				if ((priv = get_privileges(splitted_auth.front(), splitted_auth.back())) != -1) \
				{ \
					user::privileges_e priv_e = user::privileges_e(priv); \
					custom; \
					resp->String("Forbidden"); \
					return 403; \
				} \
				resp->String("Logged user not found"); \
				return 404; \
			} \
		} \
	} \
	resp->String("Bad REQ"); \
	return 400; \

int check_permission(std::string self, std::string name, user::privileges_e priv)
{
	if (priv == user::admin)
		return 2;
	if (self == name)
		return 1;
	return 0;
}


#endif // SERVER_H_
