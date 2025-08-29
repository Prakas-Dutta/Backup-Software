import tkinter as t
from tkinter.ttk import Combobox
from tkinter.filedialog import askdirectory
import os
from tkinter import messagebox
import subprocess
import time
def run_command(command):
    result = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, shell=True)
    return result.stdout, result.stderr
def work():

    if hour.get()=='' or min.get()=='' or sec.get()=='' or destinationPathEntry.get()=='\n' or sourcePathEntry.get()=='\n':
        messagebox.showinfo('Alert!!!', 'All required field is not filled.')
    else:
        if os.path.exists(sourcePathEntry.get())==False:
            messagebox.showinfo('Alert!!!', 'Source Path is not valid.')
        else:
            if (int(hour.get())>23 or int(hour.get())<0) or (int(min.get())>59 or int(min.get())<0) or (int(sec.get())>59 or int(sec.get())<0):
                messagebox.showinfo('Warning!!!', 'Enter a valid time!!!')
            else:
                f=open("info.txt", 'w')
                if file[1]!='':
                    if(file[1]==sourcePathEntry.get() and file[2]==destinationPathEntry.get()):
                        f.writelines([hour.get()+':'+min.get()+':'+sec.get()+'\n',sourcePathEntry.get()+'\n', destinationPathEntry.get()+'\n'])
                    else:
                        f.writelines([hour.get()+':'+min.get()+':'+sec.get()+'\n', '\n',sourcePathEntry.get()+'\n', destinationPathEntry.get()+'\n'])
                else:
                    f.writelines([hour.get()+':'+min.get()+':'+sec.get()+'\n', '\n',sourcePathEntry.get()+'\n', destinationPathEntry.get()+'\n'])
                f.close()
                messagebox.showinfo('Successful!!!', 'All parameters are set succcessfully!!!')
                window.destroy()
                if run_command('tasklist /FI "IMAGENAME eq copy folder recursively.exe"')[0]!='INFO: No tasks are running which match the specified criteria.\n':
                    os.system('taskkill /F /IM "copy folder recursively.exe"')
                    time.sleep(1)
                    os.system('"copy folder recursively.exe"')
                exit(1)

def fun(str):
    global flag
    if str=='pm':
        flag=True
    else:
        flag=False

def setPr(entry, str):
    entry.delete(0, t.END)
    entry.insert(0, str)

flag=None


if os.path.exists('.\\info.txt')==False:
    f=open("info.txt", 'w')
    f.writelines(['\n', '\n', '\n', '\n'])
    f.close()

f=open(".\\info.txt", 'r')

file=f.readlines()

f.close()

for i in range(len(file)):
    file[i]=file[i].replace('\n', '')

print(file[2])

window=t.Tk()
window.resizable(0, 0)
window.geometry("600x300")

heading=t.Label(window, text='Set Parameters', font=('Times New Roman', 20), fg='navy')
heading.place(x=245, y=0)


sourcePathLabel=t.Label(window, text='Source Path:', font=('Times New Roman', 18), fg='salmon')
sourcePathLabel.place(x=60, y=60)

sourcePathEntry=t.Entry(window , font=('Times New Roman', 10), width=32)
if file[1]=='':
    sourcePathEntry.insert(0, file[2])
else:
    sourcePathEntry.insert(0, file[1])
sourcePathEntry.place(x=240, y=70)

sourcePathButton=t.Button(window, text='Select Source Path',font=('Times New Romand', 10), command=lambda:setPr(sourcePathEntry, askdirectory()))
sourcePathButton.configure(bg='light blue')
sourcePathButton.place(x=450, y=60)

destinationPathLabel=t.Label(window, text='Destination Path:', font=('Times New Roman', 18), fg='salmon')
destinationPathLabel.place(x=60, y=120)

destinationPathEntry=t.Entry(window, font=('Times New Roman', 10), width=32)
if file[1]=='':
    destinationPathEntry.insert(0, file[3])
else:
    destinationPathEntry.insert(0, file[2])
destinationPathEntry.place(x=240, y=130)

destinationPathButton=t.Button(window, text='Select Destination Path',font=('Times New Romand', 10), command=lambda:setPr(destinationPathEntry, askdirectory()))
destinationPathButton.configure(bg='light blue')
destinationPathButton.place(x=450, y=120)

setTimeLabel=t.Label(window, text='Set time:', font=('Times New Roman', 18), fg='salmon')
setTimeLabel.place(x=60, y=180)

temp=None

if file[0]!='\n':temp=file[0].split(':')

hour=Combobox(window, values=[i for i in range(24)], font=('Times New Roman', 8), textvariable='ok')

hour.place(x=60, y=220)

min=Combobox(window, values=[i for i in range(60)], font=('Times New Roman', 8))
min.place(x=205, y=220)

sec=Combobox(window, values=[i for i in range(60)], font=('Times New Roman', 8))
sec.place(x=350, y=220)

if file[0]!='\n' and temp!=None:
    setPr(hour, temp[0])
    if len(temp)>1:
        setPr(min, temp[1])
    if len(temp)>2:
        setPr(sec, temp[2])


setButton=t.Button(window, text='Set',font=('Times New Romand', 10))
setButton.configure(padx=20, pady=5, bg='light green', command=lambda:work())
setButton.place(x=500, y=250)


window.mainloop()
