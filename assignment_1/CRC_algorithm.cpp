#include <iostream>
#include <fstream>
#include <ctgmath>
#include <stdlib.h>
#include <bitset>
using namespace std;

int  messageSize, polynomialSize, transmittedMessageSize;
unsigned long long int message, polynomial, reminder, receivedMessage, transmittedMessage;
string inputFilePath = "message_generator.txt", outputFilePath = "transmitted_message.txt", messageString, polynomialString, transmittedMessageString, receivedMessageString;
/*
 generator function description: it simulates the sending process

 1- reads message and polynomial from input file .
 2- divides the message concatenated with number of zeros equal to the order of polynomial by polynomial .
 3- concatenates message with reminder then write it in the output file .
*/
void generator()
{
	char *End;
	ifstream infile(inputFilePath);
	if (infile.eof())return;
	getline(infile, messageString);
	messageSize = messageString.length();
	message = strtoull(messageString.data(), &End, 2);
	getline(infile, polynomialString);
	polynomialSize = polynomialString.length();
	polynomial = strtoull(polynomialString.data(), &End, 2);
	transmittedMessage = message << (polynomialSize - 1);
	transmittedMessageSize = messageSize + polynomialSize - 1;
	reminder = transmittedMessage >> (transmittedMessageSize - polynomialSize);
	int flag = ((reminder >> (polynomialSize - 1)) & 1) ? 1 : 0;
	for (int i = polynomialSize; i < transmittedMessageSize; i++)
	{
		reminder ^= (polynomial*flag);
		reminder = ((transmittedMessage >> (transmittedMessageSize - i - 1)) & 1) | (reminder << 1);
		flag = ((reminder >> (polynomialSize - 1)) & 1) ? 1 : 0;
	}
	reminder ^= (polynomial*flag);
	transmittedMessage |= reminder;
	infile.close();
	transmittedMessageString = bitset <sizeof(unsigned long long int) * 8>(transmittedMessage).to_string();
	transmittedMessageString.erase(0, (sizeof(unsigned long long int) * 8) - transmittedMessageSize);
	ofstream outFile(outputFilePath);
	outFile << transmittedMessageString;
	outFile.close();

}

/*
  alter function description : it simulates the transimssion process

  argu: i-> number of wrong bit (0 mean no error) .
  1- reads message from generator output file .
  2- modifies message by toggles bit number i ----> in case i not equal 0 .
  3- saves message in receivedMessage global variable .
*/
void alter(int i)
{
	char *End;
	ifstream infile(outputFilePath);
	if (infile.eof())return;
	getline(infile, receivedMessageString);
	receivedMessage = strtoull(receivedMessageString.data(), &End, 2);
	if (i > 0)
	{
		receivedMessage ^= (1 << transmittedMessageSize - i);
		receivedMessageString[i - 1] = (receivedMessage >> (transmittedMessageSize - i) & 1) + 48;
	}
}
/*
	verifier function description : it simulates the receiving process

	1-reads messages from receivedMessage global variable .
	2-divides message by polynomial .
	3-print "message is correct" in case the reminder equal zero and "message is not correct" otherwise .
*/
void verifier()
{
	reminder = receivedMessage >> (transmittedMessageSize - polynomialSize);
	int flag = ((reminder >> (polynomialSize - 1)) & 1) ? 1 : 0;
	for (int i = polynomialSize; i < transmittedMessageSize; i++)
	{
		reminder ^= (polynomial*flag);
		reminder = ((receivedMessage >> (transmittedMessageSize - i - 1)) & 1) | (reminder << 1);
		flag = ((reminder >> (polynomialSize - 1)) & 1) ? 1 : 0;
	}
	reminder ^= (polynomial*flag);
	if (reminder == 0)cout << "   message is correct" << endl << endl;
	else cout << "   message is not correct" << endl << endl;
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
			cout << " - using Generator " << endl << endl << " message = " << messageString << endl << " polynomial =" << polynomialString << endl << " reminder = " << transmittedMessageString.substr(messageSize) << endl << " transmittedMessage = " << transmittedMessageString << endl << endl;
			cout << " - using verifier " << endl;
			alter(0); // 0 mean transmission with no error
			verifier();
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

			cout << " - using Generator " << endl << endl << " message = " << messageString << endl << " polynomial =" << polynomialString << endl << " reminder = " << transmittedMessageString.substr(messageSize) << endl << " transmittedMessage = " << transmittedMessageString << endl << endl;
			cout << " enter no. of bit want to invert, the length of message is " << transmittedMessageSize << endl;
			int v;
			cin >> v;
			alter(v);
			cout << " - using Alter " << endl << endl << " transmitted Message = " << transmittedMessageString << endl << " received Message = " << receivedMessageString << endl << endl;
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