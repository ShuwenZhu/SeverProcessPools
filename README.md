# Project #3 - ServerProcessPools
Project #3 (no link, as requirements come from a word doc) takes the [project #2 code](https://github.com/ShuwenZhu/LocalTestHarness) and modifies it to be a client/server test harness model. Uses worker subprocesses and a message passing system to coordinate testing.

## Authors
Shuwen Zhu, Kevin Wren

# How to Run
Using the git plugin in Visual Studio, pull the project down.
Then Build & Run the project! By default, the project will run the xml file "test1.xml" contained inside ServerProcessPools\xmlFiles\test1.xml. This test will run two valid DLLs and one invalid DLL to demonstrate our TestHarness and its new message passing ability.

## File Descriptions

### ServerProcessPools/main.cpp
This file is our "Executive" and demonstrates the runtime of our code and how clients would interact. It instanstates the Logger and TestHarness, and starts up the new TestHarness server. Then for a demonstration, the Executive launches a Client subprocess that reads in an xml file, and sends test requests at our server. The server then routes these requestst to individual worker processes who will test and return pass/fail (via message) to the Server.

### ServerProcessPools/Worker
The Worker is a seperate process & Exceutable, spawned from the Test Harness when initialized. These workers report to a specified server port and send a READY message. When they get a REQUEST message (with a dll name in the message) he attempts to dynamically load and run the iTest function interface. He then responds to the server with the result of the test in a RESULT message.

### ServerProcessPools/Client
The Client takes a server port and a XML file. he reads the xml file, parsees out the DLL names inside the file and sends messages to the server asking the server to properly test those dlls.

### ServerProcessPools/TestHarness/LoggerTH.cpp & LoggerTH.h.
 No changes to the logger since last project, except a rename to LoggerTH.cpp & LoggerTH.h.

### ServerProcessPools/TestHarness/TestHarness.cpp & TestHarness.h

This is the main Test Harness class. A test Harness is instanciate with a logger, and works in its own thread by running a comm server. This server takes messages with the name "REQUEST"/"READY"/"RESULT" and processes them appropriately. A REQUEST message is a client/worker asking the server to handle a dll name. A READY message is a worker saying he is ready for a request message. A Result is the reseult of testing an associated DLL.If passed a REQUEST, the main thread adds this request to a queue. A READY message gets added to another queue, and a RESULT is printed out immedately.

Another thread processes the queues. This thread blocks until a message shows up in the request Queue, removes that request then  blocks until a worker shows up in the READY queue. Once we have a request and a message, we send a new request to the READY worker, and loop back to proccessing for another request/ready worker pair.

### DivideTestDll & LamdaTestDll
These are two DLLs we use to test our TestHarness. Our project builds them (doesnt link them) and then we load them explictely and execute the "Test()" function inside.

### TestHarness/XmlDocument/*
[XmlDocument is located here.](https://ecs.syr.edu/faculty/fawcett/handouts/CSE687-OnLine/Repository/XmlDocument/) It is a suite of tools written by professor Fawcett for creating, parsing and understanding XML. We use the XmlParser module to read and understand our xml file, then XmlDocument to parse through our created abstract syntax tree.

### TestHarness/Comm/*
[Comm is located here.](https://ecs.syr.edu/faculty/fawcett/handouts/CSE687-OnLine/Repository/Comm/). It provides a nice entry into sockets with message passing capability in a plug and play way. The Server, worker and clients all use this package to navigate sockets.

## Requirements
Your Remote Code Repository Prototypes Project:
1.	Shall be prepared using the latest version of Visual Studio, and written in the standard C++ programming language, using the standard C++ libraries.
2.	Shall implement a message-passing communication channel using Sockets, based on the Channel prototype found in the Repository.
3.	Shall define a Message class that provides:
4.	Shall define methods for sending and processing messages.
5.	Each communication EndPoint1 shall provide both a Sender and a Receiver, wrapped in a Comm class2.
6.	Shall provide a TestHarness Package that creates, at startup, a specified number of child processes. The child processes post Ready messages to the TestHarness on startup.
7.	Shall provide a Client package that posts one or more TestRequest messages to the TestHarness. You will find it convenient to have the TestHarness provide a BlockingQueue for TestRequests and a BlockingQueue for Ready messages. It uses a child thread to extract messages from its Comm using getMessage(), and enqueue the message in the appropriate queue. The main TestHarness thread dequeues these and sends the test request to the child tester that sent the Ready message3.
8.	Shall program the Executive to provide several TestRequests in quick succession to demonstrate that the process pool executes TestRequests concurrently and that each will, on completion, post a Ready status message to the TestHarness and await the next TestRequest message.
9.	Shall log test results, demonstrating that the requirements, above, are met.
