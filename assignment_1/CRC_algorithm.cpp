#include <iostream>
#include <fstream>
#include <ctgmath>
#include <stdlib.h>
#include <bitset>
using namespace std;

int  messageSize, polynomialSize, transmittedMessageSize;
unsigned long long int message, polynomial, reminder, receivedMessage, transmittedMessage;
string inputFilePath= "message_generator.txt";
string outputFilePath="transmitted_message.txt";
/*
 generator function description: it simulates the sending process   
 
 1- reads message and polynomial from input file .
 2- divides the message concatenated with number of zeros equal to the order of polynomial by polynomial . 
 3- concatenates message with reminder then write it in the output file .
*/
void generator() 
{
	string S;
	char *End;
	ifstream infile(inputFilePath);
	if (infile.eof())return;
	getline(infile, S);
	messageSize = S.length();
	message = strtoull(S.data(), &End, 2);
	getline(infile, S);
	polynomialSize = S.length();
	polynomial= strtoull(S.data(), &End, 2);
	transmittedMessage = message << (polynomialSize - 1);
	transmittedMessageSize = messageSize + polynomialSize - 1;
	reminder = transmittedMessage >> (transmittedMessageSize - polynomialSize);
	int flag = ((reminder >> (polynomialSize - 1)) & 1) ? 1 : 0;
	for(int i=polynomialSize;i<transmittedMessageSize;i++)
	{
		reminder ^=(polynomial*flag);
		reminder = ((transmittedMessage >> (transmittedMessageSize - i - 1)) & 1) | (reminder << 1);
		flag = ((reminder >> (polynomialSize - 1)) & 1) ? 1 : 0;
	}
	reminder ^= (polynomial*flag);
	transmittedMessage |= reminder;
	infile.close();
	S = bitset <sizeof(unsigned long long int) * 8>(transmittedMessage).to_string();
	S.erase(0, (sizeof(unsigned long long int) * 8) - transmittedMessageSize);
	ofstream outFile(outputFilePath);
	outFile << S;
	outFile.close();

}



/*    verifier finction   */
 void verifier();

 /*   alter function                     */
 void alter(int v);

string toBinary(int n)
 {
	 string r;
	 while (n != 0) { r = (n % 2 == 0 ? "0" : "1") + r; n /= 2; }
	 return r;
 }


int main()
{
	while (1)
	{
		cout << " -for using    generator < file | verifier              command enter 1 " << endl << " -for using    generator <file | alter arg | verifier   command enter 2" << endl << " -for using                  exit                       command enter 3" << endl;
		int x = 0;
		cin >> x;
		if (x == 1)
		{
			system("CLS");
			cout << " --If the file is  not in the same folder or not with name message_generator.txt" << endl << "   please enter the  file path  or enter 0 to skip   " << endl;
			string q;
			cin >> q;
			if (q != "0")
				inputFilePath = q;

			system("CLS");
			generator();
			const int  m = messageSize;
			const int  p = polynomialSize;
			const int  t = transmittedMessageSize;
			cout << " - using Generator " << endl << endl << " message = " << toBinary(message) << endl << " polynomial =" << toBinary(polynomial) << endl << " reminder = " << toBinary(reminder) << endl << " transmittedMessage = " << toBinary(transmittedMessage) << endl << endl;
			cout << " - using verifier " << endl;
			verifier();

			messageSize, polynomialSize, transmittedMessageSize;
		}
		else if (x == 2)
		{
			system("CLS");
			cout << " --If the file is  not in the same folder or not with name message_generator.txt" << endl << "   please enter the  file path  or enter 0 to skip   " << endl;
			string q;
			cin >> q;
			if (q != "0")
				inputFilePath = q;

			system("CLS");

			generator();

			cout << " - using Generator " << endl << endl << " message = " << toBinary(message) << endl << " polynomial =" << toBinary(polynomial) << endl << " reminder = " << toBinary(reminder) << endl << " transmittedMessage = " << toBinary(transmittedMessage) << endl << endl;
			cout << " enter no. of bit want to invert, the length of message is " << transmittedMessageSize << endl;
			int v;
			cin >> v;
			alter(v);
			cout << " - using Alter " << endl << endl << " transmitted Message = " << toBinary(transmittedMessage) << endl << " received Message = " << toBinary(receivedMessage) << endl << endl;

			cout << " - using verifier " << endl;
			verifier();



		}
		else if (x == 3)
		{
			break;
		}
	}
	return 0;
}