#include "User.h"
#include <cstring>

User::User()
{
	username = nullptr;
	password = nullptr;
}

User::~User() {
	delete[] username;
	delete[] password;
}
