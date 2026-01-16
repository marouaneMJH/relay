#include "core/node.hpp"
#include "benchmark/benchmark.hpp"
#define PORT_BASE 10000

int main(int argc, char *argv[])
{
  int nbr_nodes = 10;

  if (argc >= 2)
    nbr_nodes = std::stoi(argv[1]);

  auto nodes = Benchmark::generate_nodes(nbr_nodes, 10000);

  Benchmark bench(nodes, 10); // 10 seconds
  bench.start();

  auto result = bench.wait_and_collect();

  std::cout
      << "sent=" << result.sent
      << " recv=" << result.received
      << " drop=" << result.dropped
      << " msg/s=" << result.msg_per_sec
      << " p50(ns)=" << result.p50_ns
      << " p95(ns)=" << result.p95_ns
      << " p99(ns)=" << result.p99_ns
      << "\n";

  return 0;
}
