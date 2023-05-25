#include <iostream>
#include "Chat.h"

int main()
{
	setlocale(LC_ALL, ""); // Для кириллицы в консоли. 
	std::cout << "Добро пожаловать в чат." << std::endl;
	std::cout << "Что вы хотите сделать?." << std::endl;
	std::cout << "=================================================================================================" << std::endl;

	Chat chat("127.0.0.1", 8080, 1024); // Создание объекта Chat с параметрами

	chat.start(); //Запускаем

	while (chat.isChatWork()) { //Внешний цикл работы чата, когда он закончится состоится выход из программы 

		chat.showLoginMenu(); //Меню регистрации

		while (chat.getCurrentUser()) {
			chat.showUserMenu(); //Меню пользователя

		}

	}

	return 0;
}