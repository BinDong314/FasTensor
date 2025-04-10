
#ifndef END_POINT_RABBITMQ__RESTAPI_H
#define END_POINT_RABBITMQ__RESTAPI_H 1
#include "ft_endpoint.h"
#include "ft_type.h"
#include <fstream>
#include <iostream>
#include <math.h>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

#ifdef HAS_RABBITMQ_RESTAPI_END_POINT

#include <curl/curl.h>
#include <nlohmann/json.hpp> // Use nlohmann/json library (single header JSON parser)

#define RABBITMQ_SET_HEADER (OP_USER_DEFINED_START + 0)
#define RABBITMQ_GET_HEADER (OP_USER_DEFINED_START + 1)

//
// I/O layer
class EndpointRabbitMQRestAPI : public Endpoint {
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
  EndpointRabbitMQRestAPI(std::string endpoint_info_p);
  EndpointRabbitMQRestAPI();
  ~EndpointRabbitMQRestAPI();

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

class EndpointRabbitMQRestAPI : public Endpoint {
private:
public:
  /**
   * @brief Construct a new EndpointHDF5 object
   *
   * @param data_endpoint contains the info of the endpoint, e.g., file type +
   * file info
   */
  EndpointRabbitMQRestAPI(std::string endpoint_info_p) {
    std::cout << "EndpointRabbitMQRestAPI is not configured and compiled ! \n";
  }
  ~EndpointRabbitMQRestAPI() { Close(); }

  int ParseEndpointInfo() override { return 0; }

  int ExtractMeta() override { return 0; }

  int PrintInfo() override { return 0; }

  int Create() override { return 0; }

  int Open() override { return 0; }

  void Map2MyType() override {}

  int Read(std::vector<unsigned long long> start,
           std::vector<unsigned long long> end, void *data) override {
    return 0;
  }

  int Write(std::vector<unsigned long long> start,
            std::vector<unsigned long long> end, void *data) override {
    return 0;
  }

  int Close() override { return 0; }

  int Control(int opt_code, std::vector<std::string> &parameter_v) { return 0; }
};
#endif

#endif