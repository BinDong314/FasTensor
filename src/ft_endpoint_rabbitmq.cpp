#include "ft_endpoint_rabbitmq.h"

#include <string>

#include <iostream>

EndpointRabbitMQ::EndpointRabbitMQ(std::string endpoint_info_p) {
  // Constructor implementation
  endpoint_info = endpoint_info_p;
  ParseEndpointInfo();
}

EndpointRabbitMQ::EndpointRabbitMQ() {
  // Constructor implementation
}

EndpointRabbitMQ::~EndpointRabbitMQ() {
  // Destructor implementation
}

int EndpointRabbitMQ::ParseEndpointInfo() {
  // Implement parsing logic specific to RabbitMQ

  std::stringstream ss(endpoint_info);
  std::string item;
  std::vector<std::string> tokens;

  // Split the endpoint_info string by ':'
  while (std::getline(ss, item, ':')) {
    tokens.push_back(item);
  }

  // Check if we have the correct number of tokens
  if (tokens.size() != 6) {
    std::cerr << "Invalid endpoint_info format: " << endpoint_info << std::endl;
    for (const auto &token : tokens) {
      std::cout << token << std::endl;
    }
    return -1;
  }

  hostname = tokens[0];
  port = tokens[1];
  username = tokens[2];
  password = tokens[3];
  vhost = tokens[4];
  queue_name = tokens[5];

  return 0;
}

int EndpointRabbitMQ::ExtractMeta() {
  // Implement metadata extraction logic specific to RabbitMQ
  std::cout << "Extracting RabbitMQ metadata" << std::endl;
  // Dummy implementation
  return 0;
}

int EndpointRabbitMQ::PrintInfo() {
  // Implement printing of endpoint information
  std::cout << "RabbitMQ Endpoint Info:" << std::endl;
  std::cout << "Endpoint Type: " << endpoint_type << std::endl;
  std::cout << "Endpoint Info: " << endpoint_info << std::endl;
  std::cout << "Hostname: " << hostname << std::endl;
  std::cout << "Port: " << port << std::endl;
  std::cout << "Username: " << username << std::endl;
  std::cout << "Password: " << password << std::endl;
  std::cout << "VHost: " << vhost << std::endl;
  std::cout << "Queue Name: " << queue_name << std::endl;
  std::cout << "Endpoint Ranks: " << endpoint_ranks << std::endl;
  std::cout << "Data Element Type: " << data_element_type << std::endl;
  return 0;
}

int EndpointRabbitMQ::Create() {
  // Implement creation logic specific to RabbitMQ
  std::cout << "Creating RabbitMQ endpoint" << std::endl;
  // Dummy implementation
  create_flag = true;
  return 0;
}

int EndpointRabbitMQ::Open() {
  // Implement opening logic specific to RabbitMQ
  std::cout << "Opening RabbitMQ endpoint" << std::endl;
  // Dummy implementation
  open_flag = true;
  return 0;
}

int EndpointRabbitMQ::Read(std::vector<unsigned long long> start,
                           std::vector<unsigned long long> end, void *data) {
  // Implement reading logic specific to RabbitMQ
  std::cout << "Reading from RabbitMQ endpoint" << std::endl;
  // Dummy implementation
  return 0;
}

int EndpointRabbitMQ::Write(std::vector<unsigned long long> start,
                            std::vector<unsigned long long> end, void *data) {
  // Implement writing logic specific to RabbitMQ
  std::cout << "Writing to RabbitMQ endpoint" << std::endl;
  // Dummy implementation
  return 0;
}

int EndpointRabbitMQ::Close() {
  // Implement closing logic specific to RabbitMQ
  std::cout << "Closing RabbitMQ endpoint" << std::endl;
  // Dummy implementation
  open_flag = false;
  return 0;
}

void EndpointRabbitMQ::Map2MyType() {
  // Implement mapping logic specific to RabbitMQ
  std::cout << "Mapping to RabbitMQ specific type" << std::endl;
  // Dummy implementation
}