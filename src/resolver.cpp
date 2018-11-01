#include <boost/asio.hpp>
#include <string_view>
#include <iostream>

using tcp = boost::asio::ip::tcp;

void resolve_request(boost::asio::io_context& ioc,
		     std::string_view host,
		     std::string_view port)
{
    tcp::resolver  asio_resolver {ioc};
    auto const results = asio_resolver.resolve(host, port);
    for (const auto& result: results){
	std::cout << result.endpoint() << std::endl;
    }
}

void async_resolve_request(boost::asio::io_context& ioc,
			   std::string_view host,
			   std::string_view port)
{
    tcp::resolver  asio_resolver {ioc};
    asio_resolver.async_resolve(host, port, []
				(const boost::system::error_code& ec,
				 tcp::resolver::results_type results){
				    if (ec){
					std::cout << "error " << ec.message() << std::endl;
					return;
				    }
				    for  (const auto& result: results){
					std::cout << result.endpoint() << std::endl;
				    }
				});
}
int main(int argc, char *argv[])
{
    if (argc != 3 && argc != 2){
	std::cerr << "Usage: resolver <host> [port]";
	return -1;
    }
    std::string port;
    if (argc == 3)
	port = argv[2];
	
    boost::asio::io_context ioc;
    resolve_request(ioc, argv[1], port);
    async_resolve_request(ioc, argv[1], port);
    ioc.run();
    
}
