//g++ server.cpp -o server
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
void start_server() {

	int sockfd;
	struct sockaddr_in servaddr;
    	vector<struct emp>v;


	sockfd = socket(AF_INET, SOCK_DGRAM, 0);


	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(6000);


	bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) ;


    while(1) {

        socklen_t len;
        struct sockaddr_in cliaddr;
        len = sizeof(cliaddr);
	struct emp s2 ;
   
	
        recvfrom( sockfd, &s2, sizeof(struct emp), MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len );
		int flag = 0;
		if (s2.a == 1) {
			for (int i = 0; i < v.size(); i++) {
				if (strcmp(s2.node1.i , v[i].node1.i) == 0) {
					flag = 1;
					strcpy(v[i].node1.msg, inet_ntoa(cliaddr.sin_addr));
					v[i].node1.p = cliaddr.sin_port;
					break;
				}
			}
				if (flag == 1) {
cout<<"updated succesfully"<<endl;				
				/*char *buff1 = "Details of user updated successfully";
				sendto(sockfd, (char *)buff1, strlen(buff1), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, sizeof(cliaddr));
				*/
					flag = 0;
				} else {
					struct emp s3 ;
					s3.a = s2.a;
					strcpy(s3.node1.i , s2.node1.i);
					strcpy(s3.node1.msg, inet_ntoa(cliaddr.sin_addr));
					s3.node1.p = cliaddr.sin_port;
					v.push_back(s3);
cout<<"new user registered "<<s3.node1.msg<<" "<<s3.node1.p<<endl;	

				/*char *buff2 = "New User <ID> registered successfully!";
				sendto(sockfd, (char *)buff2, strlen(buff2), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, sizeof(cliaddr));*/

			}
		} else if (s2.a == 2) {	
		
			for (int i = 0; i < v.size(); i++) {
//cout<<s2.node1.con_user<<" "<<strlen(s2.node1.con_user)<<" "<<v[i].node1.i<<strlen(v[i].node1.i)<<endl;				
				if (strcmp(s2.node1.con_user, v[i].node1.i) == 0) {
cout<<v[i].node1.i<<" "<<v[i].node1.p<<endl;				
					strcpy(s2.node1.msg, v[i].node1.msg );
					s2.node1.p = v[i].node1.p;
//cout<<s2.node1.msg<<endl;
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
