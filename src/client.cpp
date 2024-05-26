#include "hv/requests.h"

#include <string>
#include <iostream>

#include "Utility.hpp"

#define clear() std::cout << "\033[H\033[J";

int main() {
while (true)
{
	login:
	clear();
	std::string login, password;
	std::cout << "Login: ";
	std::cin >> login;
	std::cout << "Passwd: ";
	std::cin >> password;

	while (true)
	{
		clear();
		std::cout << "--- USR MGMT ---\n";
		std::cout << "Logged in as " + login + "\n";
		std::cout << "1. 'G'et all users (only admin)\n"
		             "2. get 'C'oncrete user (only yourself/admin)\n"
					 "3. 'A'dd new user (only admin)\n"
					 "4. 'E'dit existing user (only yourself/admin)\n"
					 "5. 'D'elete exitsing user (only yourself/admin)\n"
					 "q. 'Q'uit to login screen\n";
		char choice;
		std::cin >> choice;
		std::cin.ignore();
		switch (choice)
		{
			case 'G':
			case '1':
				{
					http_headers headers;

					headers["Content-Type"] = "application/json";
					headers["Authorization"] = "Basic " + utils::EncodeBase64(login + ":" + password);

					auto resp = requests::get("http://127.0.0.1:8080/users", headers);

					if (resp == NULL)
					    std::cout << "request failed!" << std::endl;
					else
					{
						std::cout << resp->status_code << " " << resp->status_message() << std::endl;
						std::cout << resp->body.c_str() << std::endl;
					}
				}
				break;
			case 'C':
			case '2':
				{
					std::string username;
					std::cout << "Enter username: ";
					std::cin >> username;
					std::cin.ignore();
					http_headers headers;

					headers["Content-Type"] = "application/json";
					headers["Authorization"] = "Basic " + utils::EncodeBase64(login + ":" + password);
					//auto resp = requests::post("http://127.0.0.1:8080/users", jroot.dump(), headers);

					auto resp = requests::get(("http://127.0.0.1:8080/user/" + username).c_str(), headers);

					if (resp == NULL)
					    std::cout << "request failed!" << std::endl;
					else
					{
						std::cout << resp->status_code << " " << resp->status_message() << std::endl;
						std::cout << resp->body.c_str() << std::endl;
					}
				}

				break;
			case 'A':
			case '3':
				{
					std::string usr, pass;
					int priv;
					std::cout << "Enter username: ";
					std::cin >> usr;
					std::cout << "Enter password: ";
					std::cin >> pass;
					std::cout << "Enter privileges: ";
					std::cin >> priv;
					std::cin.ignore();

					http_headers headers;

					headers["Content-Type"] = "application/json";
					headers["Authorization"] = "Basic " + utils::EncodeBase64(login + ":" + password);


					hv::Json jroot;
					jroot["name"] = usr;
					jroot["password"] = pass;
					jroot["privileges"] = priv;

					std::cout << jroot.dump() << std::endl;

					auto resp = requests::post("http://127.0.0.1:8080/user", jroot.dump(), headers);

					if (resp == NULL)
					    std::cout << "request failed!" << std::endl;
					else
					{
						std::cout << resp->status_code << " " << resp->status_message() << std::endl;
						std::cout << resp->body.c_str() << std::endl;
					}
				}

				break;
			case 'E':
			case '4':
				{
					std::string old_usr, new_usr, pass;
					std::cout << "Enter old username: ";
					std::cin >> old_usr;
					std::cout << "Enter new username: ";
					std::cin >> new_usr;
					std::cout << "Enter password: ";
					std::cin >> pass;
					std::cin.ignore();

					http_headers headers;

					headers["Content-Type"] = "application/json";
					headers["Authorization"] = "Basic " + utils::EncodeBase64(login + ":" + password);


					hv::Json jroot;
					jroot["name"] = new_usr;
					jroot["password"] = pass;

					std::cout << jroot.dump() << std::endl;

					auto resp = requests::put(("http://127.0.0.1:8080/user/" + old_usr).c_str(), jroot.dump(), headers);

					if (resp == NULL)
					    std::cout << "request failed!" << std::endl;
					else
					{
						std::cout << resp->status_code << " " << resp->status_message() << std::endl;
						std::cout << resp->body.c_str() << std::endl;
					}
				}

				break;
			case 'D':
			case '5':
				{
					std::string usr;
					std::cout << "Enter username: ";
					std::cin >> usr;
					std::cin.ignore();

					http_headers headers;

					headers["Content-Type"] = "application/json";
					headers["Authorization"] = "Basic " + utils::EncodeBase64(login + ":" + password);

					auto resp = requests::Delete(("http://127.0.0.1:8080/user/" + usr).c_str(), headers);

					if (resp == NULL)
					    std::cout << "request failed!" << std::endl;
					else
					{
						std::cout << resp->status_code << " " << resp->status_message() << std::endl;
						std::cout << resp->body.c_str() << std::endl;
					}
				}

				break;
			case 'Q':
			case 'q':
				goto login;
			default:
				break;

		}
		std::cout << "Press Enter to continue";
		std::cin.ignore();
	}
}
	return 0;
}
