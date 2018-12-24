//
// unix.cpp
//
// Tarun Salh
// 05/09/17
// This is a client program that demonstrates the FILE_SYSTEM class.
//

#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include "FILE_SYSTEM.h"	// File system class
using namespace std;

const int MAX = 80;

int main( int argc, char *argv[] )
{
	if ( argc < 3 )
	{
		cout << "Invalid arguments!" << endl;
		return -1;
	}
	ifstream  fin( argv[1] );	// Input file
	ofstream fout( argv[2] );	// Output file
	if ( !fin || !fout )
	{
		cout << "Problem with files!" << endl;
		return -2;
	}
	char line[MAX] = "";
	bool again = true;
	FILE_SYSTEM System;
	while ( fin.getline( line, MAX, '\n' ) && again )
	{
		fout << "Command:\t" << line << '*' << endl;
		string command = strtok( line, " ,;:\n\t\r'" );
		// If the command is changing, making, or removing a directory or file
		if ( command.find( "dir", 0, 3 ) != string::npos || 
			 command.find( "file", 0, 4 ) != string::npos )
		{
			fout << "Before:\t";
			System.ListDirectory( fout );	// Show directory before command
			char *name = strtok( NULL, " ,;:\n\t\r'" );//Get file/directory name
			if ( command == "mkdir" )	// Make directory
			{
				System.MakeDirectory( name, fout );
			}
			else if ( command == "mkfile" )	// Make file
			{
				System.MakeFile( name, fout );
			}
			else if ( command == "rmdir" )	// Remove directory
			{
				System.RemoveDirectory( name, fout );
			}
			else if ( command == "rmfile" )	// Remove file
			{
				System.RemoveFile( name, fout );
			}
			else if ( command == "chdir" )	// Change directory
			{
				System.ChangeDirectory( name, fout );
			}
			fout << "After:\t";
			System.ListDirectory( fout );	// Show directory after command
		}
		else if ( command == "pwd" )	// Print working directory
		{
			System.PrintWorkingDirectory( fout );
		}
		else if ( command == "ls" )	// List working directory
		{
			System.ListDirectory( fout );
		}
		else if ( command == "lsr" )	// List all files in all subdirectories
		{
			System.ListAllFiles( fout );
		}
		else if ( command == "Q" )	// Quit
		{
			System.Exit();
			System.PrintWorkingDirectory( fout );
			System.ListAllFiles( fout );
			again = false;
		}
		else
		{
			fout << "Not a command!" << endl;
		}
	}
	// Close file handles
	fin.close();
	fout.close();
	return 0;
}
