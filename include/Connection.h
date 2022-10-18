#ifndef NET_CONNECTION
#define NET_CONNECTION
#endif //NET_CONNECTION
//#ifndef emit
//#define emit
//#endif // emit
//#include "ChatForm.h"


#include "Messanger_common.h"
#include "Queue.h"
#include "Message.h"

using asio::ip::tcp;





namespace Messanger {
    template <typename T>
    class connection : public std::enable_shared_from_this<connection<T>> {
    public:

        enum class owner {
            server,
            client
        };

    public:

        connection(owner parent, asio::io_context& asioContext, asio::ip::tcp::socket socket, ts_queue<Owned_Message <T>>& qIn)
            : connection_io_context(asioContext), connection_socket(std::move(socket)), connection_q_messages_in(qIn)
        {
            owner_type = parent;
        }

        virtual ~connection()
        {
        }

        uint32_t get_id() const
        {
            return id;
        }

    public:
        void Connect_to_client(uint32_t uid = 0)
        {
            if (owner_type == owner::server) {
                if (connection_socket.is_open()) {
                    id = uid;
                    read_data();
                }
            }
        }

        void Connect_to_server(const tcp::resolver::results_type& endpoints)
        {
            if (owner_type == owner::client) {

                asio::async_connect(connection_socket, endpoints,
                    [this](std::error_code ec, tcp::endpoint endpoint) {
                        if (!ec)
                            read_data();
                    });
            }
        }


        void Disconnect()
        {
            if (Is_connected())
                asio::post(connection_io_context, [this]() { connection_socket.close(); });
        }

        bool Is_connected() const
        {
            return connection_socket.is_open();
        }


        void Start_listening()
        {
        }
        
        std::array<wchar_t, 256> get_user_name()
        {
            return user_name;
        }
        void set_user_name(std::array<wchar_t, 256> new_user_name)
        {
            user_name = new_user_name;
        }
    public:
        // ASYNC - send a message, connections are one-to-one so no need to specifiy
        // the target, for a client, the target is the server and vice versa
        void Send(const Messanger::Message<T>& msg)
        {
            asio::post(connection_io_context,
                [this, msg]() {
                    // If the queue has a message in it, then we must
                    // assume that it is in the process of asynchronously being written.
                    // Either way add the message to the queue to be output. If no messages
                    // were available to be written, then start the process of writing the
                    // message at the front of the queue.
                    bool bWritingMessage = !connection_q_messages_out.empty();
                    try {
                        connection_q_messages_out.push_back(msg);
                    }
                    catch (std::exception& e) {
                    }
                    if (!bWritingMessage) {
                        write_data();
                    }
                });
        }



    private:
        // ASYNC - Prime context to write a message header
        void write_data()
        {
            // If this function is called, we know the outgoing message queue must have
            // at least one message to send. So allocate a transmission buffer to hold
            // the message, and issue the work - asio, send these bytes
            asio::async_write(connection_socket, asio::buffer(&connection_q_messages_out.front(), sizeof(Messanger::Message<T>)),
                [this](std::error_code ec, std::size_t length) {
                    if (!ec) {
                        connection_q_messages_out.pop_front();

                        if (!connection_q_messages_out.empty())
                            write_data();
                    }
                    else {
                        //std::cerr << "[" << id << "] Write Data Fail.\n";
                        connection_socket.close();
                    }
                });
        }

        // ASYNC - Prime context ready to read a message header
        void read_data()
        {
            // If this function is called, we are expecting asio to wait until it receives
            // enough bytes to form a header of a message. We know the headers are a fixed
            // size, so allocate a transmission buffer large enough to store it. In fact,
            // we will construct the message in a "temporary" message object as it's
            // convenient to work with.
            asio::async_read(connection_socket, asio::buffer(&temp_msg_in, sizeof(Messanger::Message<T>)),
                [this](std::error_code ec, std::size_t length) {
                    if (!ec) {
                        add_to_incomming_message_queue();
                        /*if (owner_type == owner::client)
                        {
                            emit incomingMessage();
                        }*/
                    }
                    else {
                        // Reading form the client went wrong, most likely a disconnect
                        // has occurred. Close the socket and let the system tidy it up later.
                        //std::cerr << "[" << id << "] Leave the server...\n";
                        connection_socket.close();
                    }
                });
        }

        // Once a full message is received, add it to the incoming queue
        void add_to_incomming_message_queue()
        {
            // Shove it in queue, converting it to an "owned message", by initialising
            // with the a shared pointer from this connection object
            if (owner_type == owner::server)
                connection_q_messages_in.push_back({ this->shared_from_this(), temp_msg_in });
            else
                connection_q_messages_in.push_back({ nullptr, temp_msg_in });

            // We must now prime the asio context to receive the next message. It
            // wil just sit and wait for bytes to arrive, and the message construction
            // process repeats itself. Clever huh?
            read_data();
        }

    protected:
        tcp::socket connection_socket;

        asio::io_context& connection_io_context;

        // This queue holds all messages to be sent to the remote side
        // of this connection
        ts_queue<Message<T>> connection_q_messages_out;

        // This references the incoming queue of the parent object
        ts_queue<Owned_Message <T>>& connection_q_messages_in;

        // Incoming messages are constructed asynchronously, so we will
        // store the part assembled message here, until it is ready
        Messanger::Message<T> temp_msg_in;

        // The "owner" decides how some of the connection behaves
        owner owner_type = owner::server;
        std::array<wchar_t, 256> user_name{};
        uint32_t id = 0;
    };
}    

