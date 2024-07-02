# SimpleHttp

A simple Http server written for learning purposes with C that utilizes the Linux Socket API.

## Core Functionality

An HTTP server is responsible for handling client requests and serving responses over the HTTP protocol. Here are the core functionalities of an HTTP server:

1. Listening for Incoming Connections
The server listens on a specified port for incoming TCP connections from clients.

2. Accepting Client Connections
Once a connection request is detected, the server accepts the connection, establishing a communication link with the client.

3. Receiving HTTP Requests
The server reads the HTTP request from the client. This includes:
- **Request Line:** Method (GET, POST, etc.), URL, and HTTP version.
- **Headers:** Metadata about the request (e.g., Host, User-Agent, Content-Type).
- **Body:** Data sent by the client (primarily in POST requests).

4. Parsing HTTP Requests
The server parses the incoming HTTP request to understand what the client is requesting.

5. Handling Requests
The server processes the request based on the method and URL. This involves:
- **Static Content:** Serving files from the file system (e.g., HTML, CSS, images).
- **Dynamic Content:** Executing server-side scripts or applications (e.g., CGI, PHP).

6. Generating HTTP Responses
The server creates an HTTP response to send back to the client. This includes:
- **Status Line:** HTTP version, status code (e.g., 200 OK, 404 Not Found).
- **Headers:** Metadata about the response (e.g., Content-Type, Content-Length).
- **Body:** The content being returned (e.g., HTML, JSON).

7. Sending HTTP Responses
The server sends the generated response back to the client over the established connection.

8. Logging
Recording details about incoming requests and outgoing responses for monitoring and debugging purposes.

9. Connection Management
- **Keep-Alive:** Managing persistent connections to improve performance.
- **Closing Connections:** Properly closing connections after the response is sent or when the client disconnects.

### 10. Concurrency Handling
Managing multiple simultaneous client connections, often using multi-threading, multi-processing, or asynchronous I/O.
