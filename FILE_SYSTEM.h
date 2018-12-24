//
// FILE_SYSTEM.h
//
// Tarun Salh
// 05/01/17
// This is a header for the class FILE_SYSTEM that implements a graph to
// represent a UNIX-like file system.
//

#ifndef FILE_SYSTEM_H_INCLUDED
#define FILE_SYSTEM_H_INCLUDED

#include <fstream>
#include <string>
using namespace std;



class FILE_SYSTEM
{
private:
	// There are two types of files for this class, directories and regular
	// files. Each file will have a name for itself and a link back to its
	// parent. The parent will always be a directory, since the files in the
	// directory will be kept together in a linked-list. So a directories' child
	// will be the head of the linked-list. A type "file" will not use the child
	// pointer.
	struct File	// File for the class
	{
		string name;	// Name of the file
		char type;		// file type
		File *parent;	// thread
		File *child;	// only used by a directory
		File *next;		// pointer to next File in a single directory
	};
	File * root;	// root of the graph
	File * cwd;	// pointer to the current working directory
	FILE_SYSTEM::File * Search( char [] );		// Find a file/directory
	void Add( File * );							// Add file or directory
	void Remove( char [], char, ostream & );	// Remove directory/file
	void RemoveHelper( File ** );				// Remove helper
	void PWDHelper( ostream &, File * );		// PWD recursive helper
	void ListAllFilesHelper( ostream &, File *, short );// List all files helper
	void MakeFile( char [], char, ostream & );	// Wrapped function to make file
public:
	FILE_SYSTEM();								// Constructor
	~FILE_SYSTEM();								// Destructor
	void MakeDirectory( char [], ostream & );	// Wrapper around make directory
	// Wrapper around Remove for a directory
	void RemoveDirectory( char [], ostream & );
	void ChangeDirectory( char [], ostream & );	// Traverse through directory
	void MakeFile( char [], ostream & );		// Wrapper around make file
	// Wrapper around Remove for a file
	void RemoveFile( char [], ostream & );
	void PrintWorkingDirectory( ostream & );	// Print directory name
	void ListDirectory( ostream & );			// List files in directory
	void ListAllFiles( ostream & );				// List all files in directories
	void Exit();								// Exit file system
};

#endif	// FILE_SYSTEM_H_INCLUDED
