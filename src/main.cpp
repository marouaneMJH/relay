#include "core/node.hpp"
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iterator>
#include <random>
#include <vector>
#include <atomic>
#include <thread>
#include <string_view>
#define PORT_BASE 10000

class Benchmark
{
public:
  explicit Benchmark(std::size_t node_count)
  {
    nodes_.reserve(node_count);
    threads_.reserve(node_count);

    init_nodes_(node_count);
    start_nodes_();
    connect_nodes_();
  }

  ~Benchmark()
  {

    for (auto &t : threads_)
      if (t.joinable())
        t.join();
  }

  [[noreturn]] void send_message_to_server() noexcept
  {
    constexpr std::string_view message{"ping"};
    std::size_t index = 0;
    std::size_t count = 0;
    constexpr std::size_t MAX_PER_SECOND = 100'000;
    std::atomic<std::size_t> tokens{MAX_PER_SECOND};

    std::thread refill([&]
                       {
    while (true) {
        tokens.store(MAX_PER_SECOND, std::memory_order_relaxed);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    } });

    while (true)
    {
      if (tokens.fetch_sub(1, std::memory_order_relaxed) > 0)
      {
        nodes_[index].send(PORT_BASE, message);
        index = (index + 1) % nodes_.size();
      }
      else
      {
        std::this_thread::yield();
      }
    }
  }

private:
  std::vector<Node> nodes_;
  std::vector<std::thread> threads_;

private:
  void init_nodes_(std::size_t count)
  {
    for (std::size_t i = 0; i < count; ++i)
    {
      nodes_.emplace_back(i, PORT_BASE + i);
    }
  }

  void start_nodes_()
  {
    for (auto &node : nodes_)
    {
      threads_.emplace_back([&node]
                            { node.run(); });
    }
  }

  void connect_nodes_()
  {
    tcp::endpoint server(
        boost::asio::ip::make_address("127.0.0.1"),
        PORT_BASE);

    for (std::size_t i = 1; i < nodes_.size(); ++i)
    {
      nodes_[i].connect(server);
    }
  }
};

int main(int argc, char *argv[])
{
  size_t nbr_nodes = 10;

  Benchmark benchmark(nbr_nodes);

  benchmark.send_message_to_server();
  return 0;
}
