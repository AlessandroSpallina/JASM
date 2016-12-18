/****************************************************************************
* Copyright © 2015 Alessandro Spallina
* email: alessandrospallina1@gmail.com
* github: https://github.com/AlessandroSpallina
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
****************************************************************************/
#ifndef _GETTER_H
#define _GETTER_H

#define MAX_CORE 16
#define CPU_FILE_SIZE 65536 //dimensione massima del file cpuinfo
#define NGETTER 37

extern char getterName[NGETTER][BUFSIZ];
extern void (*getterFunction[NGETTER]) (int);

extern void getVersion (int fd); //return jasm version
extern void getCopyright (int fd); //return copyng license
extern void getHostname (int fd);
extern void getKernelName (int fd);
extern void getKernelRelease (int fd);
extern void getKernelVersion (int fd);
extern void getPosixVersion (int fd);
extern void getMachine (int fd);
extern void getGetter (int fd);
extern void getCpuName (int fd);
extern void getCacheSize (int fd);
extern void getCoreNum (int fd);
extern void getCoreSpeeds (int fd);
extern void getAddressSizes (int fd);
extern void getCreatedProcNum (int fd);
extern void getIfSwap (int fd);
extern void getFileHandlesNum (int fd);
extern void getCpuProcessor (int fd);
extern void getCpuNumber (int fd);
extern void getUpTime (int fd);
extern void getTotalRAM (int fd);
extern void getFreeRAM (int fd);
extern void getProcesses (int fd);
extern void getSchedulerVersion (int fd);
extern void getSchedulerInfo (int fd);
extern void getCHRDevices (int fd);
extern void getBLKDevices (int fd);
extern void getEmulatedFSDevices (int fd);
extern void getRealFSDevices (int fd);
extern void getMeminfo (int fd);
extern void getModules (int fd);
extern void getBuddyinfo (int fd);
extern void getDma (int fd);
extern void getIOmem (int fd);
extern void getKeyUsr (int fd);
extern void getMtrr (int fd);
extern void getMisc (int fd);
extern void getInterrupts (int fd);


#endif
