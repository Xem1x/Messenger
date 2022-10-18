#include "Messanger_common.h"
#pragma once

namespace Messanger
{

	template <typename T>
	struct Message_header
	{
		T id{}; 
		uint32_t size = 0; 
		std::array<wchar_t, 256> sender_name{};
		std::array<wchar_t, 256> reciever_name{};
	};


	template <typename T>
	struct Message
	{
		Message_header<T> header{};
		std::array<wchar_t, 256> body{};
		std::chrono::system_clock::time_point time = std::chrono::system_clock::now();


		size_t size() const
		{
			return sizeof(Message_header<T>) + body.size();
		}

		
	};

	template <typename T>
	class connection;

	template <typename T>
	struct Owned_Message {
		std::shared_ptr<connection<T>> remote = nullptr;
		Message<T> msg;

		friend std::ostream& operator<<(std::ostream& os, const Owned_Message<T>& msg)
		{
			os << msg.msg;
			return os;
		}
	};

}