#pragma once
#include <string>
using namespace std;

class User
{
private:
	const string login_; // логин пользователя
	string password_;  // пароль
	string name_; // имя пользователя

public:
	User(const string& login, const string& password, const string& name)
		:login_(login), password_(password), name_(name) {}

	const string& getUserLogin() const noexcept { return this->login_; } /// прочитать логин пользователя

	const string& getUserPassword() const { return this->password_; } /// прочитать пароль пользователя
	void setUserPassword(const string& password) { this->password_ = password; } /// изменить пароль пользователя

	const string& getUserName() const { return this->name_; } // прочитать имя пользователя
	void setUserName(const string& name) { this->name_ = name; } // изменить имя пользователя
};

