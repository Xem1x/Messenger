#pragma once

#include "Messanger_common.h"
#include "Message.h"
#include "Queue.h"
#include "Connection.h"
using asio::ip::tcp;
namespace Messanger
{
	template<typename T>
	class Server_interface
	{
	public:
		Server_interface(uint16_t port)
			: server_acceptor(server_io_context, tcp::endpoint(tcp::v4(),port))
		{

		}

		~Server_interface()
		{
			stop_server();
		}
		
		bool start_server()
		{
			try {
				waitForNewConnection();
				server_thread = std::thread(
					[this]()
					{
						server_io_context.run();
					});
			}
			catch (std::exception& e)
			{
				std::cerr << " Server Exception :" << e.what() << std::endl;
				return false;
			}
			std::cout << "Started";
			return true;
		}

		void stop_server()
		{
			server_io_context.stop();
			if (server_thread.joinable()) server_thread.join();
			std::cout << "Server stopped" << std::endl;
		}

		void waitForNewConnection()
		{
			server_acceptor.async_accept(
				[this](std::error_code ec, tcp::socket socket)
				{
					if (!ec)
					{
						std::shared_ptr<connection<T>> new_connect =
							std::make_shared<connection<T>>(connection<T>::owner::server,
								server_io_context,
								std::move(socket),
								q_message_in);
						if (onClientConnect(new_connect)) {
							connection_deque.push_back(std::move(new_connect));

							connection_deque.back()->Connect_to_client(IDCounter++);

						}
						else {

							std::cout << "[-----] Connection Denied...!\n";
						}
					}
					else
					{
						std::cout << " New connection error:" << ec.message() << std::endl;
					}
					waitForNewConnection();
				});
		}
		void messageClient(std::shared_ptr<connection<T>> client, const Messanger::Message<T>& msg)
		{
			if (client && client->Is_connected())
			{
				client->Send(msg);
				std::cout << "Msg sent back to"<< msg.header.sender_name.data() << std::endl;

			}
			else
			{
				onClientDisconnect(client);
				std::cout << "Msg didnt send back" << std::endl;
				client.reset();
				connection_deque.erase(
					std::remove(
						connection_deque.begin(),
						connection_deque.end(),
						client),
					connection_deque.end()
				);	
			}
		}


		void message_all_clients(const Message<T>& msg, std::shared_ptr<connection<T>> ignored_client = nullptr)
		{
			bool invalid_client_exists = false;

			for (auto& client : connection_deque) {
				if (client && client->Is_connected()) {
					if (client != ignored_client)
						client->Send(msg);
				}
				else {
					onClientDisconnect(client);
					client.reset();

					invalid_client_exists = true;
				}
			}
			if (invalid_client_exists)
				connection_deque.erase(
					std::remove(connection_deque.begin(), connection_deque.end(), nullptr), connection_deque.end());
		}
		void update(std::size_t max_messages = -1, bool wait = false)
		{
			if (wait)
				q_message_in.wait();

			std::size_t message_count = 0;

			while (message_count < max_messages && !q_message_in.empty()) {
				auto msg = q_message_in.pop_front();
				onMessage(msg.remote, msg.msg);
				message_count++;
			}
		}
		std::deque<std::shared_ptr<connection<T>>> get_connection_queue()
		{
			return connection_deque;
		}
		int get_conn_queue_size()
		{
			return connection_deque.size();
		}
	protected:
		virtual bool onClientConnect(std::shared_ptr<connection<T>> client)
		{
			return false;
		}

		virtual void onClientDisconnect(std::shared_ptr<connection<T>> client)
		{

		}

		virtual void onMessage(std::shared_ptr<connection<T>> client, Messanger::Message<T> &msg)
		{
			std::cout << "in";
		}
	private:
		ts_queue <Owned_Message<T>> q_message_in;
		std::deque<std::shared_ptr<connection<T>>> connection_deque;

		asio::io_context server_io_context;
		std::thread server_thread;
		tcp::acceptor server_acceptor;

		uint32_t IDCounter = 0;


	};
}