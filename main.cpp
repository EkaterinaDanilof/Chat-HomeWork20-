#include <iostream>
#include "Chat.h"

int main()
{
	setlocale(LC_ALL, ""); // ��� ��������� � �������. 
	std::cout << "����� ���������� � ���." << std::endl;
	std::cout << "��� �� ������ �������?." << std::endl;
	std::cout << "=================================================================================================" << std::endl;

	Chat chat("127.0.0.1", 8080, 1024); // �������� ������� Chat � �����������

	chat.start(); //���������

	while (chat.isChatWork()) { //������� ���� ������ ����, ����� �� ���������� ��������� ����� �� ��������� 

		chat.showLoginMenu(); //���� �����������

		while (chat.getCurrentUser()) {
			chat.showUserMenu(); //���� ������������

		}

	}

	return 0;
}