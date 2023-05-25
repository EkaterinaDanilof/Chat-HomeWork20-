#include <iostream>
#include "Chat.h"
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <cstring>

void Chat::start() {
	isChatWork_ = true; // Флаг для выхода из меню
}

std::shared_ptr<User> Chat::getUserByLogin(const std::string& login) const
{
	for (auto& user : users_) // цикл проходит по всему массиву пользователей
	{
		if (login == user.getUserLogin()) // проверка совпадения логина
			return std::make_shared<User>(user); /// возвращает указатель на текущего пользователя
	}

	return nullptr;

}

std::shared_ptr<User> Chat::getUserByName(const std::string& name) const
{
	for (auto& user : users_)
	{
		if (name == user.getUserName())
			return std::make_shared<User>(user); // возвращает указатель на текущего пользователя по имени
	}

	return nullptr;

}

void Chat::login() // Авторизация в чат по уже созданному логину и паролю
{
	std::string login, password;
	char operation;

	do
	{
		std::cout << "Логин: ";
		std::cin >> login;
		std::cout << "Пароль: ";
		std::cin >> password;

		currentUser_ = getUserByLogin(login);

		if (currentUser_ == nullptr || (password != currentUser_->getUserPassword()))
		{
			currentUser_ = nullptr;

			std::cout << "Ошибка входа..." << std::endl;
			std::cout << "Нажмите (0) чтобы выйти или другую кнопку чтобы повторить вход: ";
			std::cin >> operation;

			if (operation == '0')
				break;

		}

	} while (!currentUser_);
}

void Chat::signUp() // Регистрация пользователя в чате
{
	std::string login, password, name;

	std::cout << "Логин: ";
	std::cin >> login;
	std::cout << "Пароль: ";
	std::cin >> password;
	std::cout << "Имя пользователя: ";
	std::cin >> name;

	if (getUserByLogin(login) || login == "all")
	{
		throw UserLoginExp();
	}

	if (getUserByName(name) || name == "all")
	{
		throw UserNameExp();
	}

	User user = User(login, password, name);
	users_.push_back(user);
	currentUser_ = std::make_shared<User>(user);
}

void Chat::showChat() const // показывает отправленные сообщения, конкретному пользователю или всем
{
	std::string from;
	std::string to;

	std::cout << "- - - Чат - - -" << std::endl;

	for (auto& mess : messages_)
	{
		// Показывает сообщение: от текущего пользователя, для него и для всех
		if (currentUser_->getUserLogin() == mess.getFrom() || currentUser_->getUserLogin() == mess.getTo() || mess.getTo() == "all")
		{
			from = (currentUser_->getUserLogin() == mess.getFrom()) ? "Я" : getUserByLogin(mess.getFrom())->getUserName();

			if (mess.getTo() == "all")
			{
				to = "(Всем(all))";
			}
			else
			{
				to = (currentUser_->getUserLogin() == mess.getTo()) ? "Я" : getUserByLogin(mess.getTo())->getUserName();
			}

			std::cout << "Сообщение от: " << from << " Кому: " << to << std::endl;
			std::cout << "Текст: " << mess.getText() << std::endl;

		}
	}

	std::cout << "- - - - - - - - - - - -" << std::endl;
}

void Chat::showLoginMenu() // Отображение стартового меню программы - Меню верхнего уровня
{
	currentUser_ = nullptr;

	char operation;

	do
	{
		std::cout << "\033[33m" << "(1)Вход" << std::endl;
		std::cout << "(2)Регистрация" << std::endl;
		
		std::cout << "(0)Выход" << std::endl;
		std::cout << "\033[36m" << ">> " << "\033[0m";
		std::cin >> operation;

		switch (operation)
		{
		case '1':
			login();
			break;
		case '2':
			try
			{
				signUp();
			}
			catch (const std::exception& e)
			{
				std::cout << e.what() << std::endl;
			}
			break;
		
		case '0':
			isChatWork_ = false;
			break;
		default:
			std::cout << "1 или 2 " << std::endl;
			break;

		}
	} while (!currentUser_ && isChatWork_);
}

void Chat::showUserMenu() // отображает внутреннее меню чата, после авторизации пользователя
{
	char operation;

	std::cout << "Привет, " << currentUser_->getUserName() << std::endl;

	while (currentUser_)
	{
		std::cout << "(1)Показать чат | (2)Отправить сообщение | (3)Показать кто в чате | (0)Выйти ";

		std::cout << std::endl
			<< ">> ";
		std::cin >> operation;

		switch (operation)
		{
		case '1':
			showChat();
			break;
		case '2':
			addMessage();
			break;
		case '3':
			showAllUsersName();
			break;
		case '0':
			currentUser_ = nullptr;
			break;
		default:
			std::cout << "сделайте выбор.." << std::endl;
			break;
		}


	}
}

void Chat::showAllUsersName() const //Показать имена пользователей в чате
{
	std::cout << "- - - В чате - - -" << std::endl;
	for (auto& user : users_)
	{
		std::cout << user.getUserName();

		if (currentUser_->getUserLogin() == user.getUserLogin())
			std::cout << "(Я)";
		std::cout << std::endl;
	}
	std::cout << "- - - - - - - - -" << std::endl;
}

void Chat::addMessage() // Формирование сообщения
{
	std::string to, text;

	std::cout << "Кому (пользователю или всем(команда all)): ";
	std::cin >> to;
	std::cout << "Сообщение: ";
	std::cin.ignore(); // игнорировать любые введенные посторонние символы
	getline(std::cin, text); // ввод текстового сообщения с пробелами

	if (!(to == "all" || getUserByName(to))) // если не удалось найти получателя по имени
	{
		std::cout << "Такой пользователь не найден " << to << std::endl;
		return;
	}

	if (to == "all")
		messages_.push_back(Message{ currentUser_->getUserLogin(), "all", text });
	else
		messages_.push_back(Message{ currentUser_->getUserLogin(), getUserByName(to)->getUserLogin(), text });

}
Chat::Chat(const char* serverIp, int serverPort, int bufferLength)
	: serverIp(serverIp), serverPort(serverPort), bufferLength(bufferLength)
{
	// Инициализация сокета
	initSocket();

	// Задание адреса сервера
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	//serverAddr.sin_addr.s_addr = inet_addr(serverIp);
	inet_pton(AF_INET, serverIp, &serverAddr.sin_addr);
	serverAddr.sin_port = htons(serverPort);

	// Задание адреса клиента
	memset(&clientAddr, 0, sizeof(clientAddr));
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	clientAddr.sin_port = htons(0);

	// Вычисление длины структуры адреса клиента
	clientAddrLen = sizeof(clientAddr);

	// Выделение памяти для буферов приема/отправки данных
	receiveBuffer = new char[bufferLength];
	sendBuffer = new char[bufferLength];
}
Chat::~Chat()
{
}
void Chat::initSocket()
{
	// Инициализация Winsock
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		throw std::runtime_error("Failed to initialize Winsock");
	}

	// Создание UDP-сокета
	udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (udpSocket == INVALID_SOCKET)
	{
		WSACleanup();
		throw std::runtime_error("Failed to create UDP socket");
	}

	// Связывание сокета с адресом клиента
	if (bind(udpSocket, (SOCKADDR*)&clientAddr, sizeof(clientAddr)) == SOCKET_ERROR)
	{
		closesocket(udpSocket);
		WSACleanup();
		throw std::runtime_error("Failed to bind UDP socket");
	}
}
void Chat::sendMessage(const char* message)
{
	// Отправка сообщения на сервер
	int result = sendto(udpSocket, message, strlen(message) + 1, 0, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	if (result == SOCKET_ERROR)
	{
		closesocket(udpSocket);
		WSACleanup();
		throw std::runtime_error("Failed to send message");
	}
}
void Chat::receiveMessage()
{
	// Прием сообщения от сервера
	int result = recvfrom(udpSocket, receiveBuffer, bufferLength, 0, (SOCKADDR*)&clientAddr, &clientAddrLen);
	if (result == SOCKET_ERROR)
	{
		closesocket(udpSocket);
		WSACleanup();
		throw std::runtime_error("Error receiving data from server");
	}

	// Вывод сообщения на экран
	std::cout << "Received message from server: " << receiveBuffer << std::endl;
}
