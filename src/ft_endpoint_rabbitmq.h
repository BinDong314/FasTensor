
#ifndef END_POINT_RABBITMQ_H
#define END_POINT_RABBITMQ_H 1
#include "ft_endpoint.h"
#include "ft_type.h"
#include "ft_utility.h"
#include <fstream>
#include <iostream>
#include <math.h>
#include <regex>
#include <string.h>
#include <string>
#include <vector>

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
};
#endif