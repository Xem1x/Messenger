#pragma once


#include <Messanger_nmsp.h>
using asio::ip::tcp;

namespace Messanger {
    template <typename T>
    class client_interface {
    public:
        client_interface() {}
        virtual ~client_interface() { disconnect_from(); }

    public:

        bool connect_to(const std::string& host, const uint16_t port)
        {
            try {

                tcp::resolver resolver(client_io_context);
                tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));
               
                connect_ptr = std::make_unique<connection<T>>(connection<T>::owner::client, client_io_context, tcp::socket(client_io_context), __q_messages_in);

                
                connect_ptr->Connect_to_server(endpoints);

                
                client_thread = std::thread([this]() { client_io_context.run(); });
                
            }
            catch (std::exception& e) {
                std::cerr << "Client Exception: " << e.what() << '\n';
                return false;
            }
            return true;
        }

        void disconnect_from()
        {
            if (is_connected()) {
                connect_ptr->Disconnect();
            }

            client_io_context.stop();
            if (client_thread.joinable())
                client_thread.join();

            connect_ptr.release();
        }

        bool is_connected()
        {
            if (connect_ptr)
                return connect_ptr->Is_connected();
            else
                return false;
        }

    public:
        void send(const Message<T>& msg)
        {
            
            if (is_connected())
            {
                
                connect_ptr->Send(msg);
            }
        }

        ts_queue<Owned_Message<T>>& get_in_comming()
        {
            return __q_messages_in;
        }

    protected:
        asio::io_context client_io_context;
        std::thread client_thread;
        std::unique_ptr<connection<T>> connect_ptr;

    private:
        ts_queue<Owned_Message<T>> __q_messages_in;
    };
}