// Server side code
#include <arpa/inet.h> 
#include <string.h>
#include <unistd.h> 
#include <iostream> 
#include <string>
#define PORT 5555


using namespace std;
class Chat{
    public:
       void messaging(int new_sock);

    private:
       char buffer[1024]; // Create a char array that will contain the returned message
       int new_sock;
       int recvd_input;
       
};
void Chat::messaging(int new_sock){
    cout<< "Here at Chat::messaging(int new_sock)" << endl;
    
    // Read recieved message
    recvd_input = read(new_sock, buffer, 1024);
    cout<< buffer << endl;

    // Return a message
    char * out_msg = "FROM SERVER: How are you?"; // This will be coming from whatever DB querry
    send(new_sock, out_msg, strlen(out_msg), 0);
    cout<< "Response sent to client\n" << endl;


}
       
int main(){
    int master_sock, new_sock; // declare variables for file descriptor(socket), new socket formed and port number.
    struct sockaddr_in server_addr,client_addr; // Objects out of struct sockaddr_in
    socklen_t cli_addr_len; // obj out of struct socklen_t for connecting client address length.
   
    // Create socket file descriptor using socket() function. This is the Master socket
    master_sock = socket(AF_INET, SOCK_STREAM,0);
    if (master_sock < 0){ 
       cout<< "ERROR. Socket creation failed"<< endl;
       exit(EXIT_FAILURE);
       }

    else {
        cout<< "SUCCESS: Master socket created"<< endl;
    }

    // Bind created socket to the server IP address and port number.
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT); 

    if (bind(master_sock, (struct sockaddr*)&server_addr, sizeof(server_addr))< 0){
        cout<< "ERROR. Socket bind fail"<< endl;
        exit(EXIT_FAILURE);
    }
    
    else{
        cout<< "SUCCESS: Socket bind"<< endl;
    }

    int i = 0;
    while (i<1){

        // Get into Listening state
        if (listen(master_sock, 5) < 0) {
            perror("listen failed");
            exit(EXIT_FAILURE);
        }
    
        else{
            cout<< "SUCCESS: Listening to incoming connections ....."<< endl;
        }
        
        // Accepting incoming connections &forming a 2nd new socket that takes over the server<>client communication.
       
        cli_addr_len = sizeof(client_addr);
        new_sock = accept(master_sock, (struct sockaddr *)&client_addr, &cli_addr_len);

        if ( new_sock < 0) {
            perror("ERROR on accept");
            exit(EXIT_FAILURE);
        }

        else{
            // current date/time based on current system
            time_t now = time(0);
            // convert now to string form
            char* dt = ctime(&now);
            cout<<"\nTIME : " << dt << endl;
            cout<<"Connection from: IP = " << inet_ntoa(client_addr.sin_addr) << " , port = " << ntohs(client_addr.sin_port) << endl;

            // At this point , the processing of requests to be handled by classes.
            // Class could be about DB querries or any other process.
            
            Chat myChat;
            myChat.messaging(new_sock);
            close(new_sock);
        }

    }
    
    return 0;
}