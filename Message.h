#pragma once
#include "string"
using namespace std;

class Message
{
private:
	const string _from; // От кого отправлено сообщение
	const string _to; // Кому отпралено сообщение
	const string _text; // Текст сообщения

public:
	Message(const string& from, const string& to, const string& text)
		: _from(from), _to(to), _text(text) {}

	const string& getFrom() const { return this->_from; }
	const string& getTo() const { return this->_to; }
	const string& getText() const { return this->_text; }
};


