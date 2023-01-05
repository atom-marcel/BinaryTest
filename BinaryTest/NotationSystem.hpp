#pragma once

#ifndef NOTATION_H
#define NOTATION_H

#include <string>
#include <math.h>
#include <fstream>
#include "Node.hpp"

namespace BaseNotation
{
	enum class Base {
		Binary,
		Octal,
		Hexadecimal,
		Decimal
	};

	template<typename T = int>
	class Notation
	{
	private:
		Base sys;

		std::string ALP_DECIMAL = "0123456789";
		std::string ALP_BINARY = "01";
		std::string ALP_OCTAL = "01234567";
		std::string ALP_HEXADECIMAL = "0123456789ABCDEF0123456789abcdef";

		void Evaluate_decimal()
		{
			int dec = 0;
			int index = 0;
			for (int i = this->notation.size() - 1; i >= 0; i--)
			{
				dec += this->find_index(this->alphabet, this->notation[i]) * pow(this->base, index);
				index++;
			}
			this->value = dec;
		}

		void SetBase()
		{
			switch (this->sys)
			{
			case Base::Binary:
				this->base = 2;
				this->alphabet = this->ALP_BINARY;
				break;
			case Base::Decimal:
				this->base = 10;
				this->alphabet = this->ALP_DECIMAL;
				break;
			case Base::Octal:
				this->base = 8;
				this->alphabet = this->ALP_OCTAL;
				break;
			case Base::Hexadecimal:
				this->base = 16;
				this->alphabet = this->ALP_HEXADECIMAL;
				break;
			default:
				this->base = 10;
				this->alphabet = this->ALP_DECIMAL;
				break;
			}
		}
	public:
		int base;
		T value;
		std::string notation;
		std::string alphabet;

		Notation(Base system, std::string value)
		{
			this->sys = system;
			this->notation = value;
			this->SetBase();
			this->Evaluate_decimal();
		}

		Notation(T value)
		{
			this->sys = Base::Decimal;
			this->value = value;
			this->notation = std::to_string(this->value);
		}

		int find_index(std::string str, char find)
		{
			int index = 0;
			for (int i = 0; i < str.size(); i++)
			{
				if (index >= this->base) index = 0;

				if (str[i] == find)
				{
					return index;
				}

				index++;
			}
		}

		int ConvertTo(Base system)
		{
			// Default process:
			this->sys = system;
			this->SetBase();

			int rem = this->value % this->base;
			int div = this->value / this->base;
			Node<int>* node = new Node(rem);
			while (div != 0)
			{
				rem = div % this->base;
				div = div / this->base;
				node->add(rem);
			}
			Node<int>* reversed = node->reverse();
			this->notation = "";
			for (Node<int>* n = reversed; n != NULL; n = n->next)
			{
				this->notation += this->alphabet[n->value];
			}

			return 0;
		}

		Base GetBase()
		{
			return this->sys;
		}

		void print()
		{
			std::cout << "Notation: " << this->notation << " Value: " << (int)this->value << " Character: " << this->value << "\n";
		}
	};

	class Byte : public Notation<uint8_t>
	{
	private:
		void Fill_zeros()
		{
			while ((this->notation.length() % 8) != 0)
			{
				this->notation = '0' + this->notation;
			}
		}
	public:
		Byte(uint8_t b) : Notation(b) 
		{ 
			this->ConvertTo(Base::Binary); 
			this->Fill_zeros(); 
		}
		Byte(char b) : Notation((uint8_t)b) 
		{ 
			this->ConvertTo(Base::Binary); 
			this->Fill_zeros(); 
		}
		Byte(std::string str) : Notation(Base::Binary, str)
		{
			this->Fill_zeros();
		}
	};

	class ByteList
	{
	private:
		std::string ALP_BASE64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	public:
		Node<Byte>* list;
		int size;

		ByteList(uint8_t value)
		{
			Byte b(value);
			this->list = new Node(b);
			this->size = 1;
		}

		ByteList(std::string str)
		{
			this->size = str.length();
			char c = str[0];
			Byte b(c);
			this->list = new Node(b);
			for (int i = 1; i < this->size; i++)
			{
				char c = str[i];
				Byte b(c);
				this->list->add(b);
			}
		}

		int add(uint8_t value)
		{
			Byte b(value);
			this->list->add(b);
			this->size++;

			return 0;
		}

		int add(Byte b)
		{
			this->list->add(b);
			this->size++;

			return 0;
		}

		int remove(int index)
		{
			this->list->remove(index);
			this->size--;
		}

		Byte operator[](int i)
		{
			Byte value = this->list->get(i);
			return value;
		}

		std::string To_Ascii()
		{
			std::string str = "";
			for (Node<Byte>* n = this->list; n != NULL; n = n->next)
			{
				str += (char)n->value.value;
			}
			return str;
		}

		void SetListFromBase64(std::string base64_str)
		{
			this->list = NULL;
			std::string raw_bytes = "";
			for (int i = 0; i < base64_str.length(); i++)
			{
				if (base64_str[i] != '=')
				{
					uint8_t decimal = (uint8_t)this->find_index_base64(base64_str[i]);
					Notation<uint8_t>* _6Bit = new Notation(decimal);
					_6Bit->ConvertTo(Base::Binary);
					while ((_6Bit->notation.length() % 6) != 0)
					{
						_6Bit->notation = '0' + _6Bit->notation;
					}
					raw_bytes += _6Bit->notation;
				}
			}

			int x = 0;
			std::string _8BitGroup = "";
			for (int i = 0; i < raw_bytes.length(); i++)
			{
				_8BitGroup += raw_bytes[i];
				if (x >= 7)
				{
					Byte b(_8BitGroup);
					if (this->list == NULL) this->list = new Node(b);
					else this->list->add(b);
					_8BitGroup = "";
					x = 0;
				}
				else x++;
			}
		}

		int find_index_base64(char find)
		{
			int index = 0;
			std::string str = this->ALP_BASE64;
			for (int i = 0; i < str.size(); i++)
			{
				if (index >= 64) index = 0;

				if (str[i] == find)
				{
					return index;
				}

				index++;
			}
		}

		std::string Get_Base64_String()
		{
			std::string raw_bytes = "";
			for (Node<Byte>* n = this->list; n != NULL; n = n->next)
			{
				raw_bytes += n->value.notation;
			}

			while ((raw_bytes.length() % 6) != 0)
			{
				raw_bytes += '0';
			}

			Node<uint8_t>* list_6bit = NULL;
			int x = 0;
			uint8_t decimal = 0;
			for (int i = raw_bytes.length() - 1; i >= 0; i--)
			{
				decimal += int(raw_bytes[i] - '0') * pow(2, x);
				if (x >= 5)
				{
					if (list_6bit == NULL) list_6bit = new Node(decimal);
					else list_6bit->add(decimal);
					decimal = 0;
					x = 0;
				}
				else x++;
			}

			list_6bit = list_6bit->reverse();

			std::string base64 = "";
			for (Node<uint8_t>* n = list_6bit; n != NULL; n = n->next)
			{
				base64 += this->ALP_BASE64[n->value];
			}

			while ((base64.length() % 4) != 0)
			{
				base64 += '=';
			}

			return base64;
		}

		void SaveFile(std::string filename)
		{
			std::ofstream MyFile;
			MyFile.open(filename);

			for (int i = 0; i < this->list->length(); i++)
			{
				MyFile << this->list->get(i).value;
			}

			MyFile.close();
		}

		void LoadFile(std::string filename)
		{
			this->list = NULL;
			std::ifstream MyFile;
			MyFile.open(filename);
			std::string line;

			while (std::getline(MyFile, line))
			{
				for (int i = 0; i < line.length(); i++)
				{
					Byte b(line[i]);
					if (this->list == NULL) this->list = new Node(b);
					else this->list->add(b);
				}
			}

			this->size = this->list->length();

			MyFile.close();
		}
	};
}

#endif