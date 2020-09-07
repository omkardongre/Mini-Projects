'''

Python Automation Script for duplicate file removal

This is simple python script for removal of duplcation files from any directory provided.

The script uses MD5 for creating hashing.

Hash values of files are same if files has exactly same data.

Almost all files format are supported (except fifo )

Modules- sys,os,hashlib

Inputs - command line arguement for absolute/relative path

Output - All duplicate files in a given directory are deleted.


'''


import sys
import os
import hashlib

#function to create checksum
def hashfile(path,blocksize=1024):
	fd=open(path,'rb')
	hasher=hashlib.md5()
	buf=fd.read(blocksize)

	while len(buf)>0:
		hasher.update(buf)
		buf=fd.read(blocksize)
	
	fd.close()
	return hasher.hexdigest()



#function to go through folder and return dictionary having key as hash and value as absoultue path   

def duplicate_files(path):

	if not os.path.isabs(path):
		path=os.path.abspath(path)	
	
	exist=os.path.isdir(path)
	if exist:
		dict1={}
		for Folder,Subfolder,File in os.walk(path):
			
			for fname in File:
				
				path=os.path.join(Folder,fname)
				key=hashfile(path)
				if key in dict1:
					dict1[key].append(path)

				else:
					dict1[key]=[path]			
	return dict1		
	


#to print duplicates path
def print_dup(dict1):
	
	f_list=list(filter(lambda x : len(x)>1,dict1.values()))
	for file_arr in f_list:
		val=0
		for fpath in file_arr:
			val+=1
			if val>1:
				print(fpath)
				print("\n")

						

# delete duplicates directory 	
def delete_dup(dict1):
	
	#f_list stores names  of the path whose count is greater than 1 
	f_list=list(filter(lambda x : len(x)>1 , dict1.values()))
	count=0
	for i in f_list:
		val=0
		for j in i:
			val+=1
			if val>1:
				os.remove(j)
				count+=1		
	print("Total files deleted :",count)	

def main(): 

	#command line argument
	print("Welcome to Duplicate file deleion script ",sys.argv[0])
	
	#function return hash as key and list of file name as value  
	dict1 =duplicate_files(sys.argv[1])
	
	#displaying all key,value pair, for keys having duplicate files
	print_dup(dict1)

	#remove duplicate files 
	delete_dup(dict1)


if __name__=="__main__":
	main()


