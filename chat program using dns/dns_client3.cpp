//g++ dns_client3.cpp -o dns_client3 -pthread
//./dns_client3
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
#define IP "127.0.0.1"

using namespace std;
struct emp {
	int a;
	struct node 
	{
		char i[256]; // source id 
		char msg[256]; //ip
		int p;
		char con_user[256]; //connetcted user destination id
	} node1;
	char chat[256];
};
int query(string id) {
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
void con_msg(int port ) {
		
	//	int port = query(id);
//cout<<"port = "<<port<<endl;
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

void call2(int port) {
	while(1) {
		con_msg(port);
	}
}
	
struct emp Send_msg(void *s1) {	
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
	return s2;	
}


void func2(string id) {
	    int sockfd;
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
		if (temp2.node1.p == 0) {
			cout<<"invalid user"<<endl;
		} else {

		
		struct sockaddr_in	 servaddr;

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


int main(int argc, char **argv) {
	string id = "";
	for (int i =1; i < argc; i++) {
		id = id + argv[i];
	}
	int sockfd;
	struct sockaddr_in	 servaddr;
    struct emp s1;
	strcpy(s1.node1.i, id.c_str());
	s1.a = 1;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(IP);
	servaddr.sin_port = htons(6000);
	
	sendto(sockfd, &s1, sizeof(struct emp), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
	close(sockfd);
	int port = query(id);
	cout<<"main port = "<<port<<endl;
	thread th1(func2, id);
	thread th2(call2, port);
	
	th2.join();
	th1.join();
	
	return 0;
}
