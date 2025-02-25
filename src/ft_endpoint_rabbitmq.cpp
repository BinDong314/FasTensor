#include "ft_endpoint_rabbitmq.h"
#include <iostream>
#include <string>

#ifdef HAS_RABBITMQ_END_POINT

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

  conn = amqp_new_connection();
  amqp_socket_t *socket = amqp_tcp_socket_new(conn);
  if (!socket) {
    std::cerr << "Failed to create TCP socket" << std::endl;
    return -1;
  }

  int status = amqp_socket_open(socket, hostname.c_str(), std::stoi(port));
  if (status) {
    std::cerr << "Failed to open TCP socket" << std::endl;
    return -1;
  }

  amqp_rpc_reply_t rpc_reply =
      amqp_login(conn, vhost.c_str(), 0, 131072, 0, AMQP_SASL_METHOD_PLAIN,
                 username.c_str(), password.c_str());
  if (rpc_reply.reply_type != AMQP_RESPONSE_NORMAL) {
    std::cerr << "Failed to login to RabbitMQ" << std::endl;
    return -1;
  }

  amqp_channel_open(conn, 1);
  rpc_reply = amqp_get_rpc_reply(conn);
  if (rpc_reply.reply_type != AMQP_RESPONSE_NORMAL) {
    std::cerr << "Failed to open channel" << std::endl;
    return -1;
  }

  amqp_queue_declare(conn, 1, amqp_cstring_bytes(queue_name.c_str()), 0, 0, 0,
                     1, amqp_empty_table);
  rpc_reply = amqp_get_rpc_reply(conn);
  if (rpc_reply.reply_type != AMQP_RESPONSE_NORMAL) {
    std::cerr << "Failed to declare queue" << std::endl;
    return -1;
  }

  create_flag = true;
  open_flag = true;
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

void print_vector(std::string info,
                  const std::vector<unsigned long long> &vec) {
  std::cout << info << ": ";
  for (const auto &val : vec) {
    std::cout << val << " "; // Print each element followed by a space
  }
  std::cout << std::endl; // Print a newline after the vector is printed
}

int EndpointRabbitMQ::Write(std::vector<unsigned long long> start,
                            std::vector<unsigned long long> end, void *data) {
  // Implement writing logic specific to RabbitMQ
  std::cout << "Writing to RabbitMQ endpoint" << std::endl;

  print_vector("start: ", start);
  print_vector("end: ", end);

  if (!open_flag) {
    std::cerr << "Connection is not open" << std::endl;
    return -1;
  }

  // Convert data to string for publishing
  std::string message(static_cast<char *>(data), end[0] - start[0] + 1);

  amqp_bytes_t message_bytes;
  message_bytes.len = message.size();
  message_bytes.bytes = const_cast<char *>(message.c_str());

  int status = amqp_basic_publish(conn, 1, amqp_cstring_bytes(""),
                                  amqp_cstring_bytes(queue_name.c_str()), 0, 0,
                                  nullptr, message_bytes);
  if (status < 0) {
    std::cerr << "Failed to publish message" << std::endl;
    return -1;
  }

  return 0;
}

int EndpointRabbitMQ::Close() {
  // Implement closing logic specific to RabbitMQ
  std::cout << "Closing RabbitMQ endpoint" << std::endl;

  if (!open_flag) {
    return 0;
  }

  amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS);
  amqp_connection_close(conn, AMQP_REPLY_SUCCESS);
  amqp_destroy_connection(conn);

  open_flag = false;
  return 0;
}

void EndpointRabbitMQ::Map2MyType() {
  // Implement mapping logic specific to RabbitMQ
  std::cout << "Mapping to RabbitMQ specific type" << std::endl;
  // Dummy implementation
}

#endif