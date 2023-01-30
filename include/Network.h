#pragma once

#include <iostream>
#include <vector>
#include <thread>
#include <asio.hpp>

#include "Definition.h"
#include "MessageHeader.h"

namespace MCRCON
{
	class BaseServer
	{
	public:
		BaseServer() = default;
		~BaseServer() = default;

		virtual asio::awaitable<void> listener(asio::io_context& io_context, const unsigned short& serverPort)
		{
            auto executor = co_await asio::this_coro::executor;
            std::cout << std::format("服务端端口: {}\n", serverPort);

            //建立acceptor
            //acceptor
            asio::ip::tcp::acceptor acceptor(executor, { asio::ip::tcp::v4(), asio::ip::port_type(serverPort) });
            for (;;)
            {
                //accept socket
                asio::ip::tcp::socket socket = co_await acceptor.async_accept(asio::use_awaitable);
                //获取ip
                auto ip = socket.remote_endpoint().address().to_string();

                asio::co_spawn(executor, echo(std::move(socket)), asio::detached);
            }
		}

		virtual asio::awaitable<void> echo(asio::ip::tcp::socket socket)
		{
            bool isEndOfFile = false;
            try
            {
                char data[8192]{ 0 };
                //调用客户端连接服务端的处理函数
                co_await socketConnected(socket);
                for (;;)
                {
                    //读取消息直到客户端断开链接
                    std::size_t n = co_await socket.async_read_some(asio::buffer(data), asio::use_awaitable);
                    //调用客户端发送消息给服务端的处理函数
                    co_await socketSendedMessage(socket, std::move(std::string(data, n)));
                }
            }
            catch (std::exception& e)
            {
                //判断是否是客户端断开连接
                if (!strcmp(e.what(), "End of file"))
                {
                    isEndOfFile = true;
                }
                else
                {
                    std::cout << e.what() << '\n';
                }
            }
            if (isEndOfFile)
            {
                //调用客户端断开连接的处理函数
                co_await socketDisconnected(socket);
            }
            co_return;
		}

        virtual void run(asio::io_context& io_context, const unsigned short& serverPort)
        {
            //创建listener的协程
            asio::co_spawn(io_context, listener(io_context, serverPort), asio::detached);

            //创建新线程处理
            std::thread thread([&io_context]() {io_context.run(); });
            thread.detach();
        }

		virtual asio::awaitable<void> socketConnected(asio::ip::tcp::socket& socket) = 0;
		virtual asio::awaitable<void> socketSendedMessage(asio::ip::tcp::socket& socket,
            std::string data
            ) = 0;
		virtual asio::awaitable<void> socketDisconnected(asio::ip::tcp::socket& socket) = 0;
	};

	class MCRCONServer : public BaseServer
	{
	public:
        MCRCONServer(MCRCON::GlobalVariable& v)
            :p_globalVariable(v)
        {

        }

        asio::awaitable<void> socketConnected(asio::ip::tcp::socket& socket)
        {
            co_return;
        }

        asio::awaitable<void> socketSendedMessage(asio::ip::tcp::socket& socket,
            std::string data
        )
        {
            //p_globalVariable.connections[socket.remote_endpoint().address().to_string()+ std::to_string(static_cast<int>(socket.remote_endpoint().port()))].
            if (!p_globalVariable.connections[socket.remote_endpoint().address().to_string() +
                std::to_string(static_cast<int>(socket.remote_endpoint().port()))
            ].getPluginName)
            {

            }
            co_return;
        }

        asio::awaitable<void> socketDisconnected(asio::ip::tcp::socket& socket)
        {
            co_return;
        }
		
	private:
        MCRCON::GlobalVariable& p_globalVariable;
	};
}