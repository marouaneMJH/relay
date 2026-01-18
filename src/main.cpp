#include "core/node.hpp"
#include "core/cli_manager.hpp"
#include "benchmark/benchmark.hpp"

#define PORT_BASE 10000
#define MINUTES(X) ((X) * 60)

int main(int argc, char *argv[])
{
  int nbr_nodes = 10;
  if (argc >= 2)
    nbr_nodes = std::stoi(argv[1]);

  CliManager cli;

  cli.run();

  // --- BENCHMARK ---
  // auto nodes = Benchmark::generate_nodes(nbr_nodes, PORT_BASE);
  // Benchmark bench(nodes, MINUTES(1));

  // bench.start();
  // auto result = bench.wait_and_collect();

  // std::cout
  //     << "sent=" << result.sent
  //     << " recv=" << result.received
  //     << " drop=" << result.dropped
  //     << " msg/s=" << result.msg_per_sec
  //     << " p50(ns)=" << result.p50_ns
  //     << " p95(ns)=" << result.p95_ns
  //     << " p99(ns)=" << result.p99_ns
  //     << "\n";

  return 0;
}
