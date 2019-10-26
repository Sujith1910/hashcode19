import serial
import string
import csv

ser = serial.Serial('/dev/ttyUSB0',9600)
s = [0]

vsl = ser.readline()
#val="Hello"
f = open('serial.csv', 'a', newline='')
filewriter = csv.writer(f, delimiter=',', quotechar=' ', quoting=csv.QUOTE_MINIMAL)
filewriter.writerow(['Time', 'Humidity', "Temperature", "Oxygen", "Carbon Di Oxide"])
f.close()
while True:
   val = ser.readline()
   val = str(val).strip("b'").strip("\\n").strip("\\r")
   if(val):
      s.append(val)
   if(len(s)==5):
      print(s,sep="\t")
      print(s[0])
      print("\n")
      f = open('serial.csv', 'a', newline='')
      filewriter = csv.writer(f, delimiter=',', quotechar=' ', quoting=csv.QUOTE_MINIMAL)
      filewriter.writerow([s[0], s[1], s[2], s[3], s[4]])
      f.close()
      """
      with open('6338_Task1.2.csv', 'a', newline='') as csvfile:
         filewriter = csv.writer(csvfile, delimiter=',',
                                 quotechar=' ', quoting=csv.QUOTE_MINIMAL)
         filewriter.writerow([s[0],s[1],s[2],s[3],s[4]])"""
      s=[]

