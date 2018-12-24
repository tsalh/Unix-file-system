//
// FILE_SYSTEM.cpp
//
// Tarun Salh
// This is an implementation file for the class FILE_SYSTEM.
//

#include "FILE_SYSTEM.h"	// Class header file
#include <fstream>
#include <iostream>
#include <cstring>
using namespace std;
// File types
#define DIRECTORY 'D'
#define _FILE	  'F'
// Current working directory traversals
#define PARENT	  ".."	// Traverse to the parent of CWD
#define ITSELF	  "."	// Stay in the CWD
#define ROOT 	  '/'	// Go to root

// Default Constructor
FILE_SYSTEM::FILE_SYSTEM()
{
	root         = new File();	// File system starts off with a root directory
	cwd          = root;		// Current working directory starts at root
	root->name   = "/";
	root->type   = DIRECTORY;	// Root is a file type of directory
	root->parent = root;		// Thread points to itself
	root->child  = NULL;		// No files initially in the directory
	root->next   = NULL;		// There are no other directories at root level
}

// Destructor
FILE_SYSTEM::~FILE_SYSTEM()
{
	if ( !root )	// If there are files on the system
	{
		RemoveHelper( &root );	// Remove the root
	}
	root = NULL;
}

// Search for a file by name and return it
FILE_SYSTEM::File *FILE_SYSTEM::Search( char n[] )
{
	string name = n;
	// Inchworm through thr CWD looking for the file name
	File *trav = cwd->child;
	while ( trav && trav->name != name )
	{
		trav = trav->next;
	}
	return trav;
}


// Wrapped methid to make a file or directory
void FILE_SYSTEM::MakeFile( char name[], char type, ostream &fout )
{
	File *p = NULL;
	if ( !Search( name ) )	// If there is not a file with the same name
	{
		// Make a directory
		p 		  = new File();
		p->name   = name;			// set the name
		p->type   = type;			// file type of directory
		p->parent = cwd;			// thread points to CWD
		p->child  = NULL;
		p->next   = NULL;
		Add( p );					// Add the directory into the cwd
		return;
	}
	fout << name << ": File exists!" << endl;
	return;
}

// Make a directory
void FILE_SYSTEM::MakeDirectory( char name[], ostream &fout )
{
	return MakeFile( name, DIRECTORY, fout );
}

// Make a File
void FILE_SYSTEM::MakeFile( char name[], ostream &fout )
{
	return MakeFile( name, _FILE, fout );
}

// Add a file or directory into the CWD
void FILE_SYSTEM::Add( File *z )
{
	File *p = cwd->child, *q = NULL;
	while ( p && ( z->name > p->name ) )
	{
		q = p;
		p = p->next;
	}
	if ( !q )	// If file needs to be added at the front of the list
	{
		z->next    = p;		// Attach z in front of p since it's at the 1st node
		cwd->child = z;		// adjust the head to z the new front node
	}
	else	// Add at the end or inbetween two files
	{
		z->next = p;
		q->next = z;
	}
	return;
}

// Recursive helper to remove a file File
void FILE_SYSTEM::RemoveHelper( File **p )
{
	while ( (*p) )	// While p != NULL
	{
		RemoveHelper( &((*p)->next) );	// Traverse through list
		RemoveHelper( &((*p)->child) );	// Traverse into subdirectory
		delete *p;					// Delete a file
		*p = NULL;					// Avoid pointing to deallocated memmory
	}
	return;
}

// Remove a file or a directory, if its a directory remove all the files
// within that directory
void FILE_SYSTEM::Remove( char n[], char type, ostream &fout )
{
	string name = n;
	File *p = cwd->child, *q = NULL;
	while ( p && (p->name != name) )
	{
		q = p;
		p = p->next;
	}
	if ( !p )	// If there is no file with the name
	{
		fout << name << ": No such file or directory!" << endl;
		return;
	}
	// If trying to remove a directory that is a file or trying to remove a
	// file that is a directory.
	else if ( p->type != (( type == DIRECTORY ) ? (DIRECTORY) : (_FILE)) )
	{
		fout << name << ": Not a " 
			 << ((type == DIRECTORY) ? ("directory") : ("file")) << '!' << endl;
		return;
	}
	else if ( cwd->child == p )	// directory/file at the head of the list
	{
		cwd->child = p->next;	// Move head of the list
		p->next    = NULL;		// Detach directory/file
		p->parent  = NULL;
	}
	else	// Anywhere else in the CWD
	{
		q->next    = p->next;		// Maintain linked-list
		p->next    = NULL;			// Detach directory/file
		p->parent  = NULL;
	}
	return RemoveHelper( &p );  // Call recursive helper to remove directory/file
}

// Remove a directory by name
void FILE_SYSTEM::RemoveDirectory( char name[], ostream &fout )
{
	// Call wrapped function with case remove a Directory
	return Remove( name, DIRECTORY, fout );
}

// Remove a File by name
void FILE_SYSTEM::RemoveFile( char name[], ostream &fout )
{
	// Call wrapped function with case remove a File
	return Remove( name, _FILE, fout );
}

// Traverse through the file system only by changing directorys
void FILE_SYSTEM::ChangeDirectory( char path[], ostream &fout )
{
	File *save = cwd;	// Save off the address of the CWD if the path fails
	// Get the first directory name from the given path
	if ( path[0] == ROOT )
	{
		cwd = root;
	}
	char *directoryName = strtok( path, " \\/\n\t\0\r" );
	while ( directoryName != NULL )	
	{
		// If directoryName is path to the Parent directory
		if ( !strcmp( directoryName, PARENT ) )
		{
			cwd = cwd->parent;	// Traverse to parent directory
		}
		// If directoryName is path to itself
		else if ( !strcmp( directoryName, ITSELF ) )
		{
			// Stay in current directory
		}
		else	// general cases for directories
		{
			// Search for the directory name and traverse to it with CWD pointer
			cwd = Search( directoryName );
			if ( !cwd )	// If the directory does not exit
			{
				// Return the CWD pointer to its original directory before the
				// traversal took place
				cwd = save;
				fout << directoryName << ": No such file or directory!" << endl;
				return;
			}
			else if ( cwd->type != DIRECTORY )	// If the file isn't a directory
			{
				cwd = save;	// Return the CWD pointer to its original directory
				fout << directoryName << ": Not a directory!" << endl;
				return;
			}
		}
		directoryName = strtok( NULL, " \\/\n\t\0\r" );
	}
	return;
}

// Print out the full path from the root to the current working directory
void FILE_SYSTEM::PrintWorkingDirectory( ostream &fout )
{
	PWDHelper( fout, cwd );	// Call recursive helper
	fout << ((cwd == root) ? ('/'):('\0')) << endl; // if CWD is root print root
	return;
}

// Recursive helper to print out the full path from the root to the CWD
void FILE_SYSTEM::PWDHelper( ostream &fout, File *dir )
{
	// Print all the directories up until the parent directory is the root
	if ( dir != root )
	{
		PWDHelper( fout, dir->parent );	// Traverse to the parent directory
		fout << dir->name << '/' << flush;	// Print directory name
	}
	return;
}

// List all the files in the CWD
void FILE_SYSTEM::ListDirectory( ostream &fout )
{
	File *p = cwd->child;	// Start at the beginning file in the directory
	while ( p )
	{
		// If file is a directory put a '/' after the file name
		// Print out file names followed by a tab if there is another file
		fout << p->name << (( p->type == DIRECTORY ) ? ('/'):('\0'))
			 << (( p->next ) ? ('\t') : ('\0')) << flush;
		p = p->next;
	}
	fout << endl;
	return;
}

// List all files in the current working directory and in all subdirectories
void FILE_SYSTEM::ListAllFiles( ostream &fout )
{
	return ListAllFilesHelper( fout, cwd->child, 0 );	// Call recursive helper
}

// Recursive helper to list all the files in the CWD and subdirectories
void FILE_SYSTEM::ListAllFilesHelper( ostream &fout, File *file, short level )
{
	if ( file )	// file != NULL
	{
		// If file is a directory put a '/' after the file name
		for ( short i = 0; i < level; i++ )
		{
			fout << "   " << flush;
		}
		fout << file->name << ((file->type == DIRECTORY) ? ('/'):('\0')) 
			 << endl;
		ListAllFilesHelper( fout, file->child, level+1 );// Trav into subdir
		ListAllFilesHelper( fout, file->next, level );// Trav current directory

	}
	return;
}

// Exit the file system
void FILE_SYSTEM::Exit()
{
	cwd = root;	// Exiting the file system results in setting CWD to the root
	return;
}
