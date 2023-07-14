//g++ dns_client3.cpp -o dns_client3 -pthread
//./dns_client3 "name of user"
#include <bits/stdc++.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include<thread>
#include<pthread.h>

#define MAXLINE 1024
#define IP "127.0.0.1"  //this is for local hosting , we can also add server ip

using namespace std;
struct emp {
	int a; //this is identifier used for identifying packate type on the server side
	struct node 
	{
		char i[256]; // source id 
		char msg[256]; //ip
		int p; //port number
		char con_user[256]; //connetcted user destination id
	} node1;
	char chat[256]; //this is the chat or message
};
int query(string id) {   //this function gives the random port number on which the user was registered first time assigned by the os
	struct emp s1;
	s1.a = 2;
	strcpy(s1.node1.con_user, id.c_str());
    int sockfd;
	struct sockaddr_in	 servaddr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(6000);
	servaddr.sin_addr.s_addr = inet_addr(IP);

	
	sendto(sockfd, &s1, sizeof(struct emp), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
	struct emp s2;
		socklen_t len;
	len = sizeof(servaddr);
	recvfrom(sockfd, &s2, sizeof(struct emp), MSG_WAITALL, ( struct sockaddr *) &servaddr, &len);
	close(sockfd);
	return s2.node1.p;

}
void con_msg(int port ) {    //this function acts as server from the client side so that it can message from other client and see the chat!!
	int sockfd;
	struct sockaddr_in servaddr;
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);


	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(port);

cout<<"server listening port"<<port<<endl;

	bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) ;

	struct emp s3;
	struct sockaddr_in con;
	socklen_t len;
	len = sizeof(con);
	int n = recvfrom(sockfd, &s3, sizeof(emp), MSG_WAITALL, ( struct sockaddr *) &con, &len);
	cout<<s3.chat<<"    message received from     "<<s3.node1.i<<" "<<inet_ntoa(con.sin_addr)<<"  "<< con.sin_port<<endl;	
	close(sockfd);
	
	
}

void call2(int port) {   //this is for keeping the server of client always in running state
	while(1) {
		con_msg(port);
	}
}
	
struct emp Send_msg(void *s1) {	// this function is for receiving the node information from actual dns server about the other client to whom sending message
        int sockfd;
	    struct sockaddr_in	 servaddr;

    	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(6000);
	servaddr.sin_addr.s_addr = inet_addr(IP);

	
	sendto(sockfd, s1, sizeof(struct emp), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
	cout << "Message delivered successfully.\n";
		struct emp s2;
		socklen_t len;
	len = sizeof(servaddr);
	recvfrom(sockfd, &s2, sizeof(struct emp), MSG_WAITALL, ( struct sockaddr *) &servaddr, &len);
	close(sockfd);
	return s2;	//returing the target client's details packate
}


void func2(string id) {  //this function has two major functionality one is calling send_msg() for retreving other client ip port etc info and other is
	    int sockfd;  								// establishing a socket connection with the target client  
		while(1) {
		struct emp temp;
		string ch;
		strcpy(temp.node1.i, id.c_str());
		temp.a = 2;
		cin>>temp.node1.con_user;
		getline(cin, ch);
		strcpy(temp.chat, ch.c_str());
		struct emp temp2;
		temp2 = Send_msg(&temp);
			
		if (temp2.node1.p == 0) {           //if the dns returns a packate with port 0 it means the target client is not registered in the server
			cout<<"invalid user"<<endl;
		} else {
		struct sockaddr_in	 servaddr;   // here we are  establishing connection with the other client  

    	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
		servaddr.sin_family = AF_INET;
		servaddr.sin_port = htons(temp2.node1.p);
		servaddr.sin_addr.s_addr = inet_addr(temp2.node1.msg);

cout<<"dest port = "<<temp2.node1.p<<endl;	
		sendto(sockfd, &temp2, sizeof(struct emp), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
		
		close(sockfd);	
		}
		
		}
		
		
}
void register_user(string id)  //this is for registering user first time
{
	int sockfd;
	struct sockaddr_in	 servaddr;   //here we are sending a packate to the server for registering 
   	 struct emp s1;
	strcpy(s1.node1.i, id.c_str());
	s1.a = 1;
    	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(IP);
	servaddr.sin_port = htons(6000);
	
	sendto(sockfd, &s1, sizeof(struct emp), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
	close(sockfd);
	
}
int main(int argc, char **argv) {   //here we are passing command line argument for getting the client user id while running the code
	string id = "";
	for (int i =1; i < argc; i++) {   //storing in a string
		id = id + argv[i];
	}
	register_user(id);  //calling function for regsiter
	
	int port = query(id);  //callling query() for getting the main port on which this user was registered
	
	cout<<"main port = "<<port<<endl;
	thread th1(func2, id);  //threading the function so that we can use run several function at a same time
	thread th2(call2, port);
	
	th2.join();   //calling the threading function same time
	th1.join();
	
	return 0;
}
