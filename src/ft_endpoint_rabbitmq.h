
#ifndef END_POINT_RABBITMQ_H
#define END_POINT_RABBITMQ_H 1
#include "ft_endpoint.h"
#include "ft_type.h"
#include "ft_utility.h"
#include <fstream>
#include <iostream>
#include <math.h>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

#ifdef HAS_RABBITMQ_END_POINT
#include <rabbitmq-c/tcp_socket.h>

#define RABBITMQ_SET_HEADER (OP_USER_DEFINED_START + 0)
#define RABBITMQ_GET_HEADER (OP_USER_DEFINED_START + 1)

//
// I/O layer
class EndpointRabbitMQ : public Endpoint {
private:
  std::string endpoint_info;
  std::string hostname;
  std::string port;
  std::string username;
  std::string password;
  std::string vhost;
  std::string queue_name;

  std::unordered_map<std::string, std::string> headertable;

public:
  EndpointRabbitMQ(std::string endpoint_info_p);
  EndpointRabbitMQ();
  ~EndpointRabbitMQ();

  int ParseEndpointInfo() override;
  int ExtractMeta() override;
  int PrintInfo() override;
  int Create() override;
  int Open() override;
  int Read(std::vector<unsigned long long> start,
           std::vector<unsigned long long> end, void *data) override;
  int Write(std::vector<unsigned long long> start,
            std::vector<unsigned long long> end, void *data) override;
  int Close() override;
  void Map2MyType() override;
  int Control(int opt_code, std::vector<std::string> &parameter_v) override;

  amqp_connection_state_t conn;
};

#else

class EndpointRabbitMQ : public Endpoint {
private:
public:
  /**
   * @brief Construct a new EndpointHDF5 object
   *
   * @param data_endpoint contains the info of the endpoint, e.g., file type +
   * file info
   */
  EndpointRabbitMQ(std::string endpoint_info_p) {
    std::cout << "EndpointRabbitMQ is not configured and compiled ! \n";
  }
  ~EndpointRabbitMQ() { Close(); }

  int ParseEndpointInfo() override { return -1; }

  int ExtractMeta() override { return -1; }

  int PrintInfo() override { return -1; }

  int Create() override { return -1; }

  int Open() override { return -1; }

  void Map2MyType() override {}

  int Read(std::vector<unsigned long long> start,
           std::vector<unsigned long long> end, void *data) override {
    return -1;
  }

  int Write(std::vector<unsigned long long> start,
            std::vector<unsigned long long> end, void *data) override {
    return -1;
  }

  int Close() override { return -1; }

  int Control(int opt_code, std::vector<std::string> &parameter_v) {
    return -1;
  }
};
#endif

#endif