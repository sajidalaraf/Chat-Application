
# Chat Application

A simple, multi-client chat application built using C and Winsock2 on Windows. This project enables users to communicate in a group chat, send private messages, engage in one-on-one private chats, list online users, and interact with an AI, all within a password-protected server using a server-client architecture.

## Features

- **Group Chat**: Connect multiple clients to chat together in a shared group environment.
- **Private Chat**: Users can initiate one-on-one private chats with others using `/<username>`. Type `quit` to exit private chat.
- **Direct Messaging**: Send direct messages to specific users with `//<username> <message>`.
- **AI Integration**: Chat with Gemini AI using /ai <your question> for answers and problem-solving.
- **Password Protection**: Server access requires a password (`WCHAT5` by default).
- **Online User List**: View users currently in the public chat with /list.
- **Threaded Communication**: Separate threads for sending and receiving messages ensure smooth interaction.
- **Graceful Exit**: Use `#exit` to leave the chat, or press Ctrl+C to terminate the client.
- **Critical Section Synchronization**: Ensures thread-safe operations on the server for managing clients.

## Requirements

- Windows OS
- C compiler (e.g., MinGW or Visual Studio)
- Winsock2 library (`ws2_32.lib`)
- libcurl library (libcurl.lib) for AI functionality
- cJSON library for parsing AI responses

## Setup Instructions

1. **Clone the Repository**:
 
2. **Compile the Code**:
   - Compile the server:
     ```bash
     gcc server.c -o server -lws2_32
     ```
   - Compile the client:
     ```bash
     gcc client.c -o client -lws2_32
     ```

3. **Run the Server**:
   ```bash
   server
   ```
   The server will listen on `localhost:8888`.

4. **Run the Client**:
   - Open a new terminal for each client.
   - Run the client:
     ```bash
     client
     ```
   - Enter the server password (`WCHAT5`), then your username.

5. **Chat Commands**:
   - Start a private chat: `/<username>`
   - Send a direct message: `//<username> <message>`
   - Chat with AI: /ai <your question>
   - List online users: /list
   - Show commands: /help
   - Exit private chat: `quit`
   - Exit the application: `#exit`

## Project Structure

- `server.c`: Manages client connections, message broadcasting, private chats, and AI integration.
- `client.c`: Handles user interaction, message display, and command processing.

## Areas for Future Improvements

1. **AI Enhancements**: Improve AI integration with more advanced models or features.
2. **Cross-Platform Support**: Port the application to Linux/macOS using POSIX sockets for broader compatibility.
3. **GUI Interface**: Replace the console interface with a graphical user interface using a library like Qt or SDL.
4. **Persistent Chat History**: Save chat messages to a file or database for users to review past conversations.
5. **User Authentication**: Implement a more robust authentication system (e.g., username/password pairs) instead of a single server password.
6. **Encryption**: Add end-to-end encryption for private messages to enhance security.
7. **Online User List**: Display a list of currently online users with a command like `/users`.
8. **Customizable Colors**: Allow users to choose their display color instead of a fixed cyan for names.
9. **File Sharing**: Enable users to share files or images through the chat.






