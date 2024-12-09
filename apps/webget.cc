#include <cstdlib>
#include <iostream>
#include <span>
#include <string>
#include "socket.hh"

using namespace std;

void get_URL( const string& host, const string& path )
{
TCPSocket client_socket; 
client_socket.connect(Address(host, "http"));
client_socket.write(string("GET " + path + " HTTP/1.1\r"));

vector<string> buf;
client_socket.read(buf); 
if (buf.size() >= 3 && buf[1] == string("200") && buf[2] == string("OK")) {
  client_socket.write(string("Host: " + path + "\r"));
}
buf.clear();

while (!client_socket.eof()) {
  client_socket.read(buf);
  for (auto it = buf.begin(); it < --buf.end(); it++) cout << buf.front() << " ";
  cout << buf.back() << endl;
  buf.clear();
}
client_socket.write(string("Connection: close\r"));
client_socket.close();

}

int main( int argc, char* argv[] )
{
  try {
    if ( argc <= 0 ) {
      abort(); // For sticklers: don't try to access argv[0] if argc <= 0.
    }

    auto args = span( argv, argc );

    // The program takes two command-line arguments: the hostname and "path" part of the URL.
    // Print the usage message unless there are these two arguments (plus the program name
    // itself, so arg count = 3 in total).
    if ( argc != 3 ) {
      cerr << "Usage: " << args.front() << " HOST PATH\n";
      cerr << "\tExample: " << args.front() << " stanford.edu /class/cs144\n";
      return EXIT_FAILURE;
    }

    // Get the command-line arguments.
    const string host { args[1] };
    const string path { args[2] };

    // Call the student-written function.
    get_URL( host, path );
  } catch ( const exception& e ) {
    cerr << e.what() << "\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
