#pragma once
#include <vector>
#include <exception>
#include <memory>
#include "Message.h"
#include <Winsock2.h>
#include "User.h"
using namespace std;

/*-----Исключение-----*/
struct UserLoginExp : public std::exception
{
	const char* what() const noexcept override { return "error: user login is busy"; }
};

struct UserNameExp : public std::exception
{
	const char* what() const noexcept override { return "error: user name is busy"; }
};
/*-----*/

class Chat {
	bool isChatWork_ = false; /// Флаг для выхода из меню
	std::vector<User> users_; /// Динамический массив (вектор) текущих пользователей
	std::vector<Message> messages_; /// Динамический массив (вектор) сообщений
	std::shared_ptr<User> currentUser_ = nullptr; /// показывает текущего пользователя

	void login();/// Авторизация в чат по уже созданному логину и паролю
	void signUp(); /// Регистрация пользователя в чате
	void showChat() const; // показывает отправленные сообщения, конкретному пользователю или всем
	void showAllUsersName() const; /// Показывает  список пользователей чата
	void addMessage(); //Формирования сообщения
	std::vector<User>& getAllUsers() { return users_; }
	std::vector<Message>& getAllMessage() { return messages_; }
	std::shared_ptr<User> getUserByLogin(const std::string& login) const;
	std::shared_ptr<User> getUserByName(const std::string& name) const;

public:
	Chat() = default;
	Chat(const char* serverIp, int serverPort, int bufferLength);
	~Chat();
	void start();/// запуск чата
	bool isChatWork() const { return isChatWork_; } /// проверка, работает чат или нет (если true - чат работает, если false - нет)
	std::shared_ptr<User> getCurrentUser() const { return currentUser_; }
	void showLoginMenu(); /// Отображение стартового меню программы - Меню верхнего уровня
	void showUserMenu(); // отображает внутреннее меню чата, после авторизации пользователя
	void initSocket();               // Инициализация сокета
	void receiveMessage();           // Прием сообщений
	void sendMessage(const char* message);  // Отправка сообщений
private:
	WSADATA wsaData;                 // Информация о версии используемой Winsock
	SOCKET udpSocket;                // UDP-сокет для обмена сообщениями
	sockaddr_in serverAddr;          // Адрес сервера
	sockaddr_in clientAddr;          // Адрес клиента
	int clientAddrLen;               // Длина структуры адреса клиента
	const char* serverIp;            // IP-адрес сервера
	const int serverPort;            // Порт сервера
	const int bufferLength;          // Размер буфера для приема/отправки данных
	char* receiveBuffer;             // Буфер для приема данных
	char* sendBuffer;                // Буфер для отправки данных
};


