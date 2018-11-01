#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>

#include <functional>
#include <string_view>
#include <iostream>

using tcp = boost::asio::ip::tcp;

void print(const tcp::resolver::results_type& results){
    for(const auto& result:results){
	std::cout << result.endpoint() << std::endl;
    }
}

void resolve_request(tcp::resolver& asio_resolver,
		     std::string_view host,
		     std::string_view port)
{
    auto const results = asio_resolver.resolve(host, port);
    print(results);
}

void async_resolve_request(tcp::resolver& asio_resolver,
			   std::string_view host,
			   std::string_view port)
{
    asio_resolver.async_resolve(host, port, []
				(const boost::system::error_code& ec,
				 tcp::resolver::results_type results){
				    if (ec){
					std::cout << __func__ << " error " << ec.message() << std::endl;
					return;
				    }
				    print(results);
				});
}

void coro_resolve_request(tcp::resolver& asio_resolver,
			  std::string_view host,
			  std::string_view port,
			  boost::asio::yield_context yield_){
    boost::system::error_code ec;
    const auto results = asio_resolver.async_resolve(host, port, yield_[ec]);
    if (ec){
	std::cout << __func__ << " error " << ec.message() << std::endl;
	return;
    }
    print(results);
    
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
    tcp::resolver  asio_resolver {ioc};
    resolve_request(asio_resolver, argv[1], port);
    async_resolve_request(asio_resolver, argv[1], port);

    // boost::asio::spawn(ioc, std::bind(
    // 			   &coro_resolve_request,
    // 			   asio_resolver,
    // 			   argv[1],
    // 			   port,
    // 			   std::placeholders::_1));
    ioc.run();
    
}
