#pragma once
#include <vector>
#include <atomic>
#include <chrono>
#include <cstdint>
#include <string>
#include <mutex>
#include <memory>
class Node;

class Benchmark
{
public:
    struct Result
    {
        uint64_t sent;
        uint64_t received;
        uint64_t dropped;
        double msg_per_sec;
        uint64_t p50_ns;
        uint64_t p95_ns;
        uint64_t p99_ns;
    };

    // Static node generator
    static std::vector<std::unique_ptr<Node>>
    generate_nodes(std::size_t count,
                   uint16_t base_port);

    // Connectors

    void connect_server_client(int server_index = 0);

    // void connect_cycle_topology();

    Benchmark(std::vector<std::unique_ptr<Node>> &nodes,
              uint64_t duration_seconds);

    void start();
    Result wait_and_collect();

private:
    void on_receive(uint64_t node_id,
                    uint64_t from_id,
                    const std::string &msg);

    void run_nodes();
    void send_tick();

private:
    std::vector<std::unique_ptr<Node>> &nodes_;
    const uint64_t duration_s_;

    std::atomic<uint64_t> sent_{0};
    std::atomic<uint64_t> received_{0};
    std::atomic<uint64_t> dropped_{0};

    std::mutex latency_mtx_;
    std::vector<uint64_t> latencies_ns_;

    std::atomic<bool> running_{false};
    std::chrono::steady_clock::time_point start_tp_;
};
