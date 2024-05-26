#include "hv/HttpServer.h"

#include "server.hpp"
#include "UserContainer.hpp"

#include "gtest/gtest.h"

using namespace hv;
using namespace std;

TEST(ServerTests, TestGetPrivileges) {
	struct user admin;
	admin.name = "admin";
	admin.password = "password";
	admin.priv = user::admin;

	add_user(admin);
	int priv = get_privileges("admin", "password");
    EXPECT_EQ(priv, 1);

}


int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
