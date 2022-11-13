#include <iostream>
#include <thread>
#include <semaphore.h>
#include <fstream>
#include <string>

using namespace std;

sem_t sem1; //semaforo
string pack; 
string getIp(string command); //obtener el ip con string

void ipProve(string command); //comprobar el estado de la ip
int ipcont(string ipfile); //contador
int main(int argc, char *argv[]) {
	pack = argv[2];
	string filename = argv[1];	//obtiene los ip
	string listIp[ipcont(filename)];
	ifstream file(filename.c_str());
	int count = ipcont(filename);
	string s1;
	int ip = 0;
	sem_init(&sem1,0,1);
while(getline(file, s1)) { //a traves de una lista va guardando las ip
	listIp[ip] = s1;
	ip++;
}
	file.close();

	thread threads[count]; //creacion de las threads
	cout << "IP\t\tTrans.\tRec.\tPerd.\tEstado" << endl;
for(int i = 0; i < count; i++ ) {
	string command = "ping " + listIp[i] + " -c " + argv[2];
	threads[i] = thread(ipProve, command);
	}

for(int i = 0; i < count; i++) {
	threads[i].join();
	}
	return 0;
}

string getIp(string command) {
	string ip = "";
	for(int i = 5; command[i] != ' '; i++) {
	ip += command[i];
	}
	return ip;
}

void ipProve(string command) {
	char buffer[128];
	string x = "";
	FILE *pipe = popen(command.c_str(), "r");
	if(!pipe) cout << "error" << endl;
	while(!feof(pipe)) {
	if(fgets(buffer, 128, pipe) != NULL) {
		x += buffer;
	}
	}
	pclose(pipe);
	string In = "";
	int com = 0;
	for(int i = 0; i < x.length(); i++) {
		if(x[i-2] == ',') {
	 		com = 1;
		}
		if(com == 1 && x[i] == ' ') { 
			break;
		}
		if(com == 1) { 
			In += x[i];
		}
	}
	string ip = getIp(command);
	string mood;
	if(pack == In) {
		 mood = "UP";
	}else mood = "DOWN";
	int loss = stoi(pack) - stoi(In);
	sem_wait(&sem1);
	if(ip.length() < 10){
	cout << ip << "\t\t" << pack << "\t" << In << "\t" << loss << "\t" << mood << endl;
	}else cout << ip << "\t" << pack << "\t" << In << "\t" << loss << "\t" << mood << endl;
	sem_post(&sem1);
}

int ipcont(string ipfile) {
	ifstream file(ipfile.c_str());
	string c;
	int count2 = 0;
	while(getline(file, c)) {
		count2++;
	}
	file.close();
	return count2;
}
