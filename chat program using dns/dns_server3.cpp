//g++ dns_server3.cpp -o server
//./server
#include <bits/stdc++.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define MAXLINE 1024

using namespace std;
struct emp {
	int a;  
	struct node 
	{
		char i[256];//id
		char msg[256];//ip
		int p;//port
		char con_user[256]; //connetcted user
	} node1;
	
	char chat[256];
};
void start_server() {    //this function is for staring the server

	int sockfd;
	struct sockaddr_in servaddr;
    	vector<struct emp>v;   //this the data structure for storing the different user's data in format of structure packate


	sockfd = socket(AF_INET, SOCK_DGRAM, 0);


	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(6000);    //this is the unique port number of server you can give any port number as per your choice then also have to change in client side code port number


	bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) ;    //starting the server


    while(1) {     //for keeping the server always live

        socklen_t len;
        struct sockaddr_in cliaddr;
        len = sizeof(cliaddr);
	struct emp s2 ;
   
	
        recvfrom( sockfd, &s2, sizeof(struct emp), MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len );   // receiving packate from the client
		int flag = 0;
		if (s2.a == 1) {         // if (a == 1) it means the client want to register or update it's details 
			for (int i = 0; i < v.size(); i++) {      //searching in the vector
				if (strcmp(s2.node1.i , v[i].node1.i) == 0) {   //comparing with the vector's packate id with our packate id
					flag = 1;
					strcpy(v[i].node1.msg, inet_ntoa(cliaddr.sin_addr));  //inet_ntoa() converts inet datatypes to array datatype
					v[i].node1.p = cliaddr.sin_port;       //copying the os generated port to our packate
					break;
				}
			}
				if (flag == 1) { 
					cout<<"updated succesfully"<<endl;				
					flag = 0;
				} else {
					struct emp s3 ;
					s3.a = s2.a;
					strcpy(s3.node1.i , s2.node1.i);
					strcpy(s3.node1.msg, inet_ntoa(cliaddr.sin_addr));
					s3.node1.p = cliaddr.sin_port;     			//copying the os generated port to our packate 
					v.push_back(s3);          				//adding the new user structure packate into vector
					cout<<"new user registered "<<s3.node1.msg<<" "<<s3.node1.p<<endl;	

				/*char *buff2 = "New User <ID> registered successfully!";
				sendto(sockfd, (char *)buff2, strlen(buff2), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, sizeof(cliaddr));*/
				//this above part can be used to get a prompt message in the client side
			}
		} else if (s2.a == 2) {	 //this is for serching information about other client 
		
			for (int i = 0; i < v.size(); i++) {
				
//cout<<s2.node1.con_user<<" "<<strlen(s2.node1.con_user)<<" "<<v[i].node1.i<<strlen(v[i].node1.i)<<endl;
/*here we are searching the packate information of other user which is reffered by con_user(s2.con_user) in the sender packate 
and compare it with normal id(v[i].node1.i) in the dns database*/	
				
				if (strcmp(s2.node1.con_user, v[i].node1.i) == 0) {
					cout<<v[i].node1.i<<" "<<v[i].node1.p<<endl;				
					strcpy(s2.node1.msg, v[i].node1.msg );   //copying the ip of target client from his registered packate to our current packate
					s2.node1.p = v[i].node1.p;    //copying the port on which the connected user was registered 
					break;
				}
			}
				
		  	  sendto(sockfd, &s2, sizeof(struct emp), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, sizeof(cliaddr));	  
		}
        
      }


    close (sockfd);

}


int main() {
	start_server ();
	return 0;
}
