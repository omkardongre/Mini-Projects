import sys
import os
import hashlib
import schedule
import time
import smtplib
from email.mime.text import MIMEText
from  urllib import request
from email.mime.multipart import MIMEMultipart
from email.mime.base import MIMEBase
from email import encoders

#function to create checksum using MD5 algorithm
 
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
	
	global T_files
	T_files=0
	
	if not os.path.isabs(path):
		path=os.path.abspath(path)
		
	exist=os.path.isdir(path)

	if exist:
	
		dict1={}
		for Folder,Subfolder,File in os.walk(path):
			
			for fname in File:
				
				T_files+=1				
				path=os.path.join(Folder,fname)
				key=hashfile(path)
				if key in dict1:
					dict1[key].append(path)

				else:
					dict1[key]=[path]
				
	return dict1		
			
# printing the path of duplicate files		
def print_dup(dict1):
	
	f_list=list(filter(lambda x : len(x)>1 , dict1.values()))
	for file_arr in f_list:
		val=0
		for fpath in file_arr:
			val+=1
			if val>1:
				print(fpath)
				print("\n")

		print('*'*80)				


#delete  duplicate files
def delete_dup(dup_file):
	
	f_list=list(filter(lambda x : len(x)>1 , dup_file.values()))
	
	f_list = sorted(f_list, key=len)
	

	count=0
	del_files=[]
	sys.argv[1]
	for i in f_list:
		val=0
		for j in i:
			val+=1
			if val>1:
				del_files.append(j)
				os.remove(j)
				count+=1		

	return del_files



def log_file(del_file):

	
	if not os.path.isdir("Marvellous"):
		os.mkdir("Marvellous")
	direct="Marvellous"
	direct=os.path.abspath(direct)
	path=os.path.join(direct,time.ctime()+".txt")


	fp=open(path,'w')
	for File in del_file:
		fp.write(File+"\n")

	fp.close()
	return path
	
def Relative(fname):
	l=fname.split("/")
	return l[len(l)-1]

def is_connected():
	try:
		request.urlopen('http://216.58.192.142',timeout=1)
		return True
	
	except:
		return False

		
def mailsender(fname,time):
	

	fromaddr="omkardongre5@gmail.com"
	toaddr=sys.argv[3]
	
	msg=MIMEMultipart()
	msg['From']=fromaddr
	msg['To']=toaddr
	
	msg['Subject']="Duplication File Deletion Logs"

	body="Start time %s \n"%time+"Total no of Files scanned %d \n"%T_files +"Total no of Files deleted %d \n"%Del_files
	 
	
		
	msg.attach(MIMEText(body,'plain'))
	attachment=open(fname,"rb")
	p=MIMEBase('appplication','octet-stream')
	p.set_payload((attachment).read())
	attachment.close()
	encoders.encode_base64(p)
	relative_p=Relative(fname)
	p.add_header('Content-Disposition', "attachment; filename= %s" %relative_p)
	msg.attach(p)
	
	
	server=smtplib.SMTP_SSL('smtp.gmail.com',465)	
	
	server.login("omkardongre5@gmail.com","GooGle111598")
	text=msg.as_string()
	
	
	server.sendmail(fromaddr,toaddr,text)
	server.quit()
	
	
	
	
		
	
def operation():

	global Time
	global Del_files
	
	Time=time.ctime()
	dup_file =duplicate_files(sys.argv[1])
	del_file=delete_dup(dup_file)
	Del_files=len(del_file)
	path=log_file(del_file)
	mailsender(path,Time)
	print("Sucessfully Done")

def main(): 
	#list1 = sorted(os.listdir(direct), key=len)
	Time=0
	T_files=0
	Del_files=0
	
	time1=int(sys.argv[2])
	schedule.every(time1).seconds.do(operation)
	
	while True:

		if schedule.run_pending():
			print("omkar")
		

if __name__=="__main__":
	main()
