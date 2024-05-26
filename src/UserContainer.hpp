#ifndef USER_CONTAINER_H_
#define USER_CONTAINER_H_

#include "User.hpp"

#include <fstream>
#include <vector>

std::vector<user> users;

int add_user(const user &usr)
{
	users.push_back(usr);
	return 0;
}

int get_privileges(std::string name, std::string pass)
{
	for (auto i : users)
		if ((i.name == name && i.password == pass))
			return i.priv;
	return -1;
}

#endif // USER_CONTAINER_H_
