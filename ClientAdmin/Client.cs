﻿using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Linq;

namespace Chatclient {
  class Client {

    private Socket sender;
    private byte[] bytes;
    private bool connected;
    private string serverIP = "10.27.6.127";

    public Client() {
      bytes = new byte[1024];
      connected = false;
      Connect();
    }

    public void Connect() {
      while (!connected) {
        try {
          IPHostEntry ipHostInfo = Dns.Resolve(serverIP);
          IPAddress ipAddress = ipHostInfo.AddressList[0];
          IPEndPoint remoteEP = new IPEndPoint(ipAddress, 6666);

          sender = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

          try {
            sender.Connect(remoteEP);
            Console.WriteLine("Socket connected to {0}", sender.RemoteEndPoint.ToString());
            connected = true;

          } catch (ArgumentNullException ane) {
            Console.WriteLine("ArgumentNullException : {0}", ane.ToString());
          } catch (SocketException se) {
            Console.WriteLine("SocketException : {0}", se.ToString());
          } catch (Exception e) {
            Console.WriteLine("Unexpected exception : {0}", e.ToString());
          }
        } catch (Exception e) {
          Console.WriteLine(e.ToString());
        }
      }
    }

    public void Run() {
      string message;
      message = "";
      while (true) {
        Receive();
        if (Console.KeyAvailable) {
          message = Console.ReadLine();
          Send(message);
        }
      }
    }

    private void Send(string message) {
      message += ",10";
      try {
        string[] msg = message.Split(',');
        byte[] intBytes = new byte[msg.Length];
        for (int i = 0; i < msg.Length; i++) {
          intBytes[i] = (byte)Int32.Parse(msg[i]);
        }
        int bytesSent = sender.Send(intBytes);
      } catch (ArgumentNullException ane) {
        Console.WriteLine("ArgumentNullException : {0}", ane.ToString());
      } catch (SocketException se) {
        Console.WriteLine("SocketException : {0}", se.ToString());
      } catch (Exception e) {
        Console.WriteLine("Unexpected exception : {0}", e.ToString());
      }
    }

    private void Receive() {
      try {
        while (sender.Available > 0) {
          int bytesRec = sender.Receive(bytes);
          Console.Write(Encoding.ASCII.GetString(bytes, 0, bytesRec) + '\n');
        }
      } catch (ArgumentNullException ane) {
        Console.WriteLine("ArgumentNullException : {0}", ane.ToString());
      } catch (SocketException se) {
        Console.WriteLine("SocketException : {0}", se.ToString());
      } catch (Exception e) {
        Console.WriteLine("Unexpected exception : {0}", e.ToString());
      }
    }

    ~Client() {
      try {
        sender.Shutdown(SocketShutdown.Both);
        sender.Close();
      } catch (ArgumentNullException ane) {
        Console.WriteLine("ArgumentNullException : {0}", ane.ToString());
      } catch (SocketException se) {
        Console.WriteLine("SocketException : {0}", se.ToString());
      } catch (Exception e) {
        Console.WriteLine("Unexpected exception : {0}", e.ToString());
      }
    }
  }
}