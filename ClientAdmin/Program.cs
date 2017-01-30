using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

// C# chat client turned into echo client, a dummy client for smart home server
namespace Chatclient {
  class Program {

    public static int Main(String[] args) {
      Client myClient = new Client();
      myClient.Run();
      return 0;
    }

  }
}
