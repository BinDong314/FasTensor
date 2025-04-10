#include "ft_endpoint_rabbitmq_restapi.h"

#ifdef HAS_RABBITMQ_RESTAPI_END_POINT

EndpointRabbitMQRestAPI::EndpointRabbitMQRestAPI(std::string endpoint_info_p) {
  // Constructor implementation
  endpoint_info = endpoint_info_p;
  SetEndpointType(EP_RabbitMQ_RESTAPI);
  ParseEndpointInfo();
}

EndpointRabbitMQRestAPI::EndpointRabbitMQRestAPI() {
  // Constructor implementation
}

EndpointRabbitMQRestAPI::~EndpointRabbitMQRestAPI() {
  // Destructor implementation
}

int EndpointRabbitMQRestAPI::ParseEndpointInfo() {
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

int EndpointRabbitMQRestAPI::ExtractMeta() {
  // Implement metadata extraction logic specific to RabbitMQ
  std::cout << "Extracting RabbitMQ metadata" << std::endl;
  // Dummy implementation
  return 0;
}

int EndpointRabbitMQRestAPI::PrintInfo() {
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

int EndpointRabbitMQRestAPI::Create() {
  // Implement creation logic specific to RabbitMQ

  CURL *curl = curl_easy_init();
  if (!curl) {
    std::cerr << "Failed to initialize CURL\n";
    return -1;
  }

  std::string encoded_vhost = (vhost == "/") ? "%2f" : vhost;
  std::string url =
      "http://" + hostname + "/api/queues/" + encoded_vhost + "/" + queue_name;

  // Create JSON payload
  nlohmann::json payload = {{"durable", true},
                            {"auto_delete", false},
                            {"arguments", nlohmann::json::object()}};
  std::string json_payload = payload.dump(); // Dump JSON to string

  struct curl_slist *headers = nullptr;
  headers = curl_slist_append(headers, "Content-Type: application/json");

  // Set CURL options
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_USERPWD, (username + ":" + password).c_str());
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
  curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_payload.c_str());

  // Perform request
  CURLcode res = curl_easy_perform(curl);

  long response_code = 0;
  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

  if (res != CURLE_OK) {
    std::cerr << "CURL error: " << curl_easy_strerror(res) << "\n";
  } else if (response_code == 201) {
    std::cout << "Queue '" << queue_name << "' created successfully!\n";
  } else if (response_code == 204) {
    std::cout << "Queue '" << queue_name << "' already exists.\n";
  } else {
    std::cerr << "Queue creation failed. HTTP code: " << response_code << "\n";
  }

  // Cleanup
  curl_slist_free_all(headers);
  curl_easy_cleanup(curl);

  return 0;
}

int EndpointRabbitMQRestAPI::Open() {
  open_flag = true;
  return 0;
}

volatile sig_atomic_t EndpointRabbitMQRestAPI_read_signal_handler_should_stop =
    0;

void EndpointRabbitMQRestAPI_read_signal_handler(int signal) {
  EndpointRabbitMQRestAPI_read_signal_handler_should_stop = 1;
}

// Helper function to capture HTTP response into a string
static size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                            void *userp) {
  ((std::string *)userp)->append((char *)contents, size * nmemb);
  return size * nmemb;
}

int EndpointRabbitMQRestAPI::Read(std::vector<unsigned long long> start,
                                  std::vector<unsigned long long> end,
                                  void *data) {
  std::cout << "Reading from RabbitMQ endpoint via HTTP REST API" << std::endl;

  signal(SIGINT, EndpointRabbitMQRestAPI_read_signal_handler);

  CURL *curl;
  CURLcode res;
  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();

  if (!curl) {
    std::cerr << "Failed to initialize curl" << std::endl;
    return -1;
  }

  // Construct the URL
  std::string url = "http://" + hostname + ":" + port + "/api/queues/" + vhost +
                    "/" + queue_name + "/get";

  // Prepare the POST data (JSON)
  nlohmann::json post_data = {
      {"count", 1}, {"ackmode", "ack_requeue_false"}, {"encoding", "auto"}};
  std::string post_fields = post_data.dump();

  std::string readBuffer;

  struct curl_slist *headers = nullptr;
  headers = curl_slist_append(headers, "Content-Type: application/json");

  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_fields.c_str());

  // Authentication
  curl_easy_setopt(curl, CURLOPT_USERNAME, username.c_str());
  curl_easy_setopt(curl, CURLOPT_PASSWORD, password.c_str());

  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

  while (!EndpointRabbitMQRestAPI_read_signal_handler_should_stop) {
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res)
                << std::endl;
      curl_easy_cleanup(curl);
      curl_global_cleanup();
      return -1;
    }

    if (readBuffer.empty()) {
      std::cerr << "No response received" << std::endl;
      break;
    }

    // Parse the JSON response
    auto response = nlohmann::json::parse(readBuffer);
    if (response.empty()) {
      std::cerr << "No messages available" << std::endl;
      break;
    }

    // Should be an array of messages
    if (!response.is_array() || response.size() == 0) {
      std::cerr << "No messages received" << std::endl;
      break;
    }

    auto message = response[0];

    // Extract headers
    headertable.clear();
    if (message.contains("properties") &&
        message["properties"].contains("headers")) {
      auto headers_json = message["properties"]["headers"];
      for (auto it = headers_json.begin(); it != headers_json.end(); ++it) {
        headertable[it.key()] = it.value().get<std::string>();
        std::cout << "receiving key = " << it.key()
                  << ", value = " << it.value() << "\n";
      }
    }

    // Extract payload
    if (message.contains("payload_bytes")) {
      std::vector<unsigned char> payload =
          message["payload_bytes"].get<std::vector<unsigned char>>();
      size_t message_size = payload.size();
      std::memcpy(data, payload.data(), message_size);
    } else if (message.contains("payload")) {
      std::string payload = message["payload"];
      size_t message_size = payload.size();
      std::memcpy(data, payload.data(), message_size);
    }

    break; // Only consume one message
  }

  curl_easy_cleanup(curl);
  curl_global_cleanup();

  std::cout << "Stopping message consumption." << std::endl;
  return 0;
}

void print_vector2(std::string info,
                   const std::vector<unsigned long long> &vec) {
  std::cout << info << ": ";
  for (const auto &val : vec) {
    std::cout << val << " "; // Print each element followed by a space
  }
  std::cout << std::endl; // Print a newline after the vector is printed
}

// Helper to base64 encode (OpenSSL version)
std::string base64_encode(const unsigned char *input, int length) {
  BIO *bmem = nullptr;
  BIO *b64 = nullptr;
  BUF_MEM *bptr = nullptr;

  b64 = BIO_new(BIO_f_base64());
  BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL); // No newlines
  bmem = BIO_new(BIO_s_mem());
  b64 = BIO_push(b64, bmem);
  BIO_write(b64, input, length);
  BIO_flush(b64);
  BIO_get_mem_ptr(b64, &bptr);

  std::string encoded(bptr->data, bptr->length);
  BIO_free_all(b64);

  return encoded;
}

// Helper to generate Basic Authorization header
std::string create_basic_auth(const std::string &username,
                              const std::string &password) {
  std::string auth = username + ":" + password;
  return "Authorization: Basic " +
         base64_encode(reinterpret_cast<const unsigned char *>(auth.c_str()),
                       auth.length());
}

int EndpointRabbitMQRestAPI::Write(std::vector<unsigned long long> start,
                                   std::vector<unsigned long long> end,
                                   void *data) {
  std::cout << "Writing to RabbitMQ HTTP API endpoint" << std::endl;

  size_t total_bytes;
  COUNT_CELLS(start, end, total_bytes);
  total_bytes = total_bytes * GetAuEndpointDataTypeSize(data_element_type);
  print_vector2("start: ", start);
  print_vector2("end: ", end);
  std::cout << "total_bytes: " << total_bytes << ", data_element_size= "
            << GetAuEndpointDataTypeSize(data_element_type) << "\n";

  // Base64 encode the payload
  std::string base64_data =
      base64_encode(reinterpret_cast<unsigned char *>(data), total_bytes);

  // Build JSON body
  std::ostringstream json_body;
  json_body << "{";
  json_body << "\"properties\": {";
  json_body << "\"headers\": {";
  bool first = true;
  for (const auto &header : headertable) {
    if (!first)
      json_body << ",";
    json_body << "\"" << header.first << "\":\"" << header.second << "\"";
    first = false;
  }
  json_body << "}";
  json_body << "},";
  json_body << "\"routing_key\": \"" << queue_name << "\",";
  json_body << "\"payload\": \"" << base64_data << "\",";
  json_body << "\"payload_encoding\": \"base64\"";
  json_body << "}";

  // Prepare CURL
  CURL *curl = curl_easy_init();
  if (!curl) {
    std::cerr << "Failed to initialize CURL" << std::endl;
    return -1;
  }

  // Build URL
  std::ostringstream url;
  url << "http://" << hostname << "/api/exchanges/"
      << curl_easy_escape(curl, vhost.c_str(), 0)
      << "/amq.default/publish"; // assuming "amq.default" exchange

  // Set CURL options
  curl_easy_setopt(curl, CURLOPT_URL, url.str().c_str());

  struct curl_slist *headers = NULL;
  headers = curl_slist_append(headers, "Content-Type: application/json");

  // Add Authorization header
  std::string auth_header = create_basic_auth(username, password);
  headers = curl_slist_append(headers, auth_header.c_str());

  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

  std::string json = json_body.str();
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json.c_str());

  CURLcode res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    std::cerr << "CURL request failed: " << curl_easy_strerror(res)
              << std::endl;
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    return -1;
  }

  long response_code;
  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
  if (response_code != 200) {
    std::cerr << "HTTP POST failed, response code: " << response_code
              << std::endl;
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    return -1;
  }

  curl_slist_free_all(headers);
  curl_easy_cleanup(curl);
  return 0;
}

int EndpointRabbitMQRestAPI::Close() {
  // Implement closing logic specific to RabbitMQ
  return 0;
}

void EndpointRabbitMQRestAPI::Map2MyType() {
  // Implement mapping logic specific to RabbitMQ
  std::cout << "Mapping to RabbitMQ specific type" << std::endl;
  // Dummy implementation
}

// Function to parse a long vector into a header table
void ParseHeaders2(std::unordered_map<std::string, std::string> &headers,
                   const std::vector<std::string> &vs) {
  if (vs.size() % 2 != 0) {
    std::cerr
        << "Invalid header format! Each key must have exactly one value.\n";
    return;
  }

  for (size_t i = 0; i < vs.size(); i += 2) {
    headers[vs[i]] = vs[i + 1]; // First string is key, second is value
  }
}

// Function to flatten a header table into a single vector
std::vector<std::string>
FlattenHeaders2(const std::unordered_map<std::string, std::string> &headers) {
  std::vector<std::string> result;
  std::cout << "calling FlattenHeaders..  headertable.size() = "
            << headers.size() << std::endl;

  for (const auto &[key, value] : headers) {
    result.push_back(key);
    result.push_back(value);
    std::cout << "FlattenHeaders key = " << key << ", value = " << value
              << std::endl;
  }

  return result;
}

int EndpointRabbitMQRestAPI::Control(int opt_code,
                                     std::vector<std::string> &parameter_v) {
  switch (opt_code) {
  case RABBITMQ_SET_HEADER:
    if (parameter_v.size() < 2) {
      AU_EXIT("SET_HEADER  needs at least 2 parameter: key-value \n");
    }
    ParseHeaders2(headertable, parameter_v);
    break;
  case RABBITMQ_GET_HEADER:
    parameter_v = FlattenHeaders2(headertable);
    break;
  default:
    std::cout << "Unsupported datatype in " << __FILE__ << " : " << __LINE__
              << std::endl;
    std::flush(std::cout);
    std::exit(EXIT_FAILURE);
  }

  return 0;
}
#endif