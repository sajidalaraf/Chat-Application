
# Chat Application

A simple, multi-client chat application built using C and Winsock2 on Windows. This project allows multiple users to communicate in a group chat, send private messages, and engage in one-on-one private chats with a password-protected server, implementing a server-client architecture.

## Features

- **Group Chat**: Multiple clients can join and communicate in a shared group chat.
- **Private Chat**: Users can initiate one-on-one private chats with others using `/<username>`. Type `quit` to exit private chat.
- **Direct Messaging**: Send direct messages to specific users with `//<username> <message>`.
- **Password Protection**: Server access requires a password (`WCHAT5` by default).
- **Threaded Communication**: Separate threads for sending and receiving messages ensure smooth interaction.
- **Graceful Exit**: Use `#exit` to leave the chat, or press Ctrl+C to terminate the client.
- **Critical Section Synchronization**: Ensures thread-safe operations on the server for managing clients.

## Requirements

- Windows OS
- C compiler (e.g., MinGW or Visual Studio)
- Winsock2 library (`ws2_32.lib`)

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
   - Exit private chat: `quit`
   - Exit the application: `#exit`

## Project Structure

- `server.c`: Server-side code handling client connections and message broadcasting.
- `client.c`: Client-side code for user interaction and message handling.

## Areas for Future Improvements

1. **AI Integration**: Include an AI in the chat, capable of answering questions and solving problems when prompted by users.
2. **Cross-Platform Support**: Port the application to Linux/macOS using POSIX sockets for broader compatibility.
3. **GUI Interface**: Replace the console interface with a graphical user interface using a library like Qt or SDL.
4. **Persistent Chat History**: Save chat messages to a file or database for users to review past conversations.
5. **User Authentication**: Implement a more robust authentication system (e.g., username/password pairs) instead of a single server password.
6. **Encryption**: Add end-to-end encryption for private messages to enhance security.
7. **Online User List**: Display a list of currently online users with a command like `/users`.
8. **Customizable Colors**: Allow users to choose their display color instead of a fixed cyan for names.
9. **File Sharing**: Enable users to share files or images through the chat.













