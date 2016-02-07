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
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <errno.h>
#include <fcntl.h>


#include "getter.h"
#include "miscellaneous.h"
#include "logger.h"

char error[BUFSIZ];

//TODO: checks + errno

void getVersion (int fd);
void getCopyright (int fd);
void getHostname (int fd);
void getKernelName (int fd);
void getKernelRelease (int fd);
void getKernelVersion (int fd);
void getPosixVersion (int fd);
void getMachine (int fd);
void getGetter (int fd);
void getCpuName (int fd);
void getCpuProcessor (int fd);
void getUpTime (int fd);
void getTotalRAM (int fd);
void getFreeRAM (int fd);
void getProcesses (int fd);


char getterName[NGETTER][BUFSIZ] = {"Version", "Copyright", "Hostname", "KernelName",
                                    "KernelRelease", "KernelVersion","PosixVersion", "Machine",
                                    "CpuName","CpuProcessor","UpTime","TotalRAM","FreeRAM","Processes"
                                   };

void (*getterFunction[NGETTER]) (int) = {getVersion, getCopyright, getHostname,
                                         getKernelName, getKernelRelease, getKernelVersion,getPosixVersion, getMachine,
                                         getCpuName,getCpuProcessor, getUpTime, getTotalRAM, getFreeRAM, getProcesses 
                                        };

/*
 *  scrive su fd int numero getter e N stringhe nomiGetter
 */
void getGetter (int fd)
{
        int i;
        int n_getter = NGETTER;
        int count = 0;
        //start from an error condition, to make sure we do not miss anything
        ssize_t ret_val = -1;

        ret_val = write (fd, &n_getter, sizeof (n_getter) );

        if (ret_val == 0 || ret_val == -1)
        {
                log_error ("[JASM-DAEMON][getGetter][write()] Error! ret_val is 0 or -1");
                log_error (error);
        }

        for (i = 0; i < NGETTER; i++)
        {
                count = strlen (getterName[i]);
                ret_val = write (fd, &count, sizeof (count) );

                if (ret_val == 0 || ret_val == -1)
                {
                        log_error ("[JASM-DAEMON][getGetter][write()] Error! ret_val is 0 or -1");
                        log_error (error);
                }
                ret_val = write (fd, getterName[i], count);

                if (ret_val == 0 || ret_val == -1)
                {
                        log_error ("[JASM-DAEMON][getGetter][write()] Error! ret_val is 0 or -1");
                        log_error (error);
                }
        }
}

void getVersion (int fd)
{
        int n;

        n = write (fd, VERSION, strlen (VERSION) );
        if (n < 0)
        {
                sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno) );
                log_error ("[JASM-DAEMON][getVersion][write()] Error!");
                log_error (error);
        }
        else
        {
                if (n < strlen (VERSION) )
                {
                        sprintf (error, "[JASM-DAEMON][getVersion][write()] sent %d byte, correct num byte is %zu", n, strlen (VERSION) );
                        log_error (error);
                }
                else
                {
                        sprintf (error, "[JASM-DAEMON][getVersion][write()] sent %d byte", n);
                        log_string (error);
                }
        }
}

void getCopyright (int fd)
{
        int n;

        n = write (fd, COPYRIGHT, strlen (COPYRIGHT) );
        if (n < 0)
        {
                sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno) );
                log_error ("[JASM-DAEMON][getCopyright][write()] Error!");
                log_error (error);
        }
        else
        {
                if (n < strlen (COPYRIGHT) )
                {
                        sprintf (error, "[JASM-DAEMON][getCopyright][write()] sent %d byte, correct num byte is %zu", n, strlen (COPYRIGHT) );
                        log_error (error);
                }
                else
                {
                        sprintf (error, "[JASM-DAEMON][getCopyright][write()] sent %d byte", n);
                        log_string (error);
                }
        }
}

void getHostname (int fd)
{
        struct utsname info;
        char buf[BUFSIZ];

        if (uname (&info) == -1)
        {
                log_error ("getHostname() [uname] failed");
                return;
        }
        else
        {
                strcpy (buf, info.nodename);
                int n = write (fd, buf, strlen (buf) );
                if (n < 0)
                {
                        sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno) );
                        log_error ("[JASM-DAEMON][getHostname][write()] Error!");
                        log_error (error);
                }
                else
                {
                        if (n < strlen (buf) )
                        {
                                sprintf (error, "[JASM-DAEMON][getHostname][write()] sent %d byte, correct num byte is %zu", n, strlen (buf) );
                                log_error (error);
                        }
                        else
                        {
                                sprintf (error, "[JASM-DAEMON][getHostname][write()] sent %d byte", n);
                                log_string (error);
                        }
                }
        }
}

void getKernelName (int fd)
{
        struct utsname info;
        char buf[BUFSIZ];

        if (uname (&info) == -1)
        {
                log_error ("getKernelName() [uname] failed");
                return;
        }
        else
        {
                strcpy (buf, info.sysname);
                int n = write (fd, buf, strlen (buf) );
                if (n < 0)
                {
                        sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno) );
                        log_error ("[JASM-DAEMON][getKernelName][write()] Error!");
                        log_error (error);
                }
                else
                {
                        if (n < strlen (buf) )
                        {
                                sprintf (error, "[JASM-DAEMON][getKernelName][write()] sent %d byte, correct num byte is %zu", n, strlen (buf) );
                                log_error (error);
                        }
                        else
                        {
                                sprintf (error, "[JASM-DAEMON][getKernelName][write()] sent %d byte", n);
                                log_string (error);
                        }
                }
        }
}

void getKernelRelease (int fd)
{
        struct utsname info;
        char buf[BUFSIZ];

        if (uname (&info) == -1)
        {
                log_error ("getKernelRelease() [uname] failed");
                return;
        }
        else
        {
                strcpy (buf, info.release);
                int n = write (fd, buf, strlen (buf) );
                if (n < 0)
                {
                        sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno) );
                        log_error ("[JASM-DAEMON][getKernelRelease][write()] Error!");
                        log_error (error);
                }
                else
                {
                        if (n < strlen (buf) )
                        {
                                sprintf (error, "[JASM-DAEMON][getKernelRelease][write()] sent %d byte, correct num byte is %zu", n, strlen (buf) );
                                log_error (error);
                        }
                        else
                        {
                                sprintf (error, "[JASM-DAEMON][getKernelRelease][write()] sent %d byte", n);
                                log_string (error);
                        }
                }
        }
}

void getKernelVersion (int fd)
{
        struct utsname info;
        char buf[BUFSIZ];

        if (uname (&info) == -1)
        {
                log_error ("getKernelVersion() [uname] failed");
                return;
        }
        else
        {
                strcpy (buf, info.version);
                int n = write (fd, buf, strlen (buf) );
                if (n < 0)
                {
                        sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno) );
                        log_error ("[JASM-DAEMON][getKernelVersion][write()] Error!");
                        log_error (error);
                }
                else
                {
                        if (n < strlen (buf) )
                        {
                                sprintf (error, "[JASM-DAEMON][getKernelVersion][write()] sent %d byte, correct num byte is %zu", n, strlen (buf) );
                                log_error (error);
                        }
                        else
                        {
                                sprintf (error, "[JASM-DAEMON][getKernelVersion][write()] sent %d byte", n);
                                log_string (error);
                        }
                }
        }
}

void getMachine (int fd)
{
        struct utsname info;
        char buf[BUFSIZ];
        if (uname (&info) == -1)
        {
                log_error ("getMachine() [uname]  failed");
                return;
        }
        else
        {
                strcpy (buf, info.machine);
                int n = write (fd, buf, strlen (buf) );
                if (n < 0)
                {
                        sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno) );
                        log_error ("[JASM-DAEMON][getMachine][write()] Error!");
                        log_error (error);
                }
                else
                {
                        if (n < strlen (buf) )
                        {
                                sprintf (error, "[JASM-DAEMON][getMachine][write()] sent %d byte, correct num byte is %zu", n, strlen (buf) );
                                log_error (error);
                        }
                        else
                        {
                                sprintf (error, "[JASM-DAEMON][getMachine][write()] sent %d byte", n);
                                log_string (error);
                        }
                }
        }
}

void getCpuName (int fd)
{
		int cpu_fd;
		char buf[BUFSIZ];
		char *string;
		int i = 0;
		int n;
		
		cpu_fd = open("/proc/cpuinfo", O_RDONLY);
		read(cpu_fd, buf, 200); //TODO controllo errore
		string = &strstr(buf, "model name	: ")[strlen("model name	: ")];
		while(string[i] != '\n') ++i; //conta i caratteri della descrizione della cpu
		sprintf(buf, "%.*s", i, string); //ora buf contiene il modello del processore
		n = write (fd, buf, strlen(buf));
		close(cpu_fd);
		if (n < 0)
    {
        sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno) );
        log_error ("[JASM-DAEMON][getCpuName][write()] Error!");
        log_error (error);
    }
    else
    {
        if (n < strlen (buf) )
        {
            sprintf (error, "[JASM-DAEMON][getCpuName][write()] sent %d byte, correct num byte is %zu", n, strlen (buf) );
            log_error (error);
        }
        else
        {
            sprintf (error, "[JASM-DAEMON][getCpuName][write()] sent %d byte", n);
            log_string (error);
        }
    }
}

void getUpTime (int fd)
{
      char buf[BUFSIZ];
      struct sysinfo sys_info;
      int days,hours,min,sec;
      int n;
      
      if( sysinfo (&sys_info) != 0)
      {
            log_error ("getUpTime() Failed");
            return;
      }
      else{
            days = sys_info.uptime / 86400;
            hours = (sys_info.uptime / 3600) - (days * 24);
            min = (sys_info.uptime / 60) - (hours * 60) - (days * 1440);
            sec = (sys_info.uptime) - (min * 60) - (hours * 3600) - (days * 86400);
            sprintf(buf,"%02d:%02d:%02d:%02d",days,hours,min,sec);
      }
      n = write (fd, buf, strlen (buf) );
        if (n < 0)
        {
            sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno) );
            log_error ("[JASM-DAEMON][getUptime][write()] Error!");
            log_error (error);
        }
        else
        {
            if (n < strlen (buf) )
            {
                sprintf (error, "[JASM-DAEMON][getUptime][write()] sent %d byte, correct num byte is %zu", n, strlen (buf) );
                log_error (error);
            }
            else
            {
                sprintf (error, "[JASM-DAEMON][getUptime][write()] sent %d byte", n);
                log_string (error);
            }
        }
}

void getTotalRAM (int fd)
{
      int n;
      struct sysinfo sys_info;
      char buf[BUFSIZ];
      const unsigned long megabyte = 1024*1024;
      
      if( sysinfo (&sys_info) != 0)
      {
            log_error ("getTotalRAM() Failed");
            return;
      }
      else
      {
            sprintf (buf,"%lu MB",sys_info.totalram/megabyte);
            n = write (fd, buf, strlen (buf) );
            if (n < 0)
            {
                  sprintf (error, "[JASM-DEAMON][errno] %s", strerror (errno) );
                  log_error ("[JASM-DEAMON][getTotalRAM][write()] Error!");
                  log_error (error);
            }
            if (n < strlen(buf))
            {
                  sprintf (error, "[JASM-DAEMON][getTotalRAM][write()] sent %d byte, correct num byte is %zu", n, strlen (buf) );
                  log_error (error);  
            }
            else
            {
                  sprintf (error, "[JASM-DAEMON][getTotalRAM][write()] sent %d byte", n);
                  log_string (error);
            }
      }

}

void getFreeRAM (int fd)
{
      int n;
      struct sysinfo sys_info;
      char buf[BUFSIZ];
      const unsigned long megabyte = 1024*1024;
      
      if( sysinfo (&sys_info) != 0)
      {
            log_error ("getFreeRAM() Failed");
            return;
      }
      else
      {
            sprintf (buf,"%lu MB",sys_info.freeram/megabyte);
            n = write (fd, buf, strlen (buf) );
            if (n < 0)
            {
                  sprintf (error, "[JASM-DEAMON][errno] %s", strerror (errno) );
                  log_error ("[JASM-DEAMON][getFreeRAM][write()] Error!");
                  log_error (error);
            }
            if (n < strlen(buf))
            {
                  sprintf (error, "[JASM-DAEMON][getFreeRAM][write()] sent %d byte, correct num byte is %zu", n, strlen (buf) );
                  log_error (error);  
            }
            else
            {
                  sprintf (error, "[JASM-DAEMON][getFreeRAM][write()] sent %d byte", n);
                  log_string (error);
            }
      }

}

void getProcesses (int fd)
{
      int n;
      char buf[BUFSIZ];
      struct sysinfo sys_info;
      if( sysinfo(&sys_info) != 0)
      {
            log_error("getProcesses() Failed");
            return;
      }
      else
      {
            sprintf(buf,"%u",sys_info.procs);
            n = write(fd,buf,strlen(buf));
            if (n < 0)
            {
                  sprintf (error, "[JASM-DEAMON][errno] %s", strerror (errno) );
                  log_error ("[JASM-DEAMON][getProceses][write()] Error!");
                  log_error (error);
            }
            if (n < strlen(buf) )
            {
                  sprintf (error, "[JASM-DAEMON][getProcesses][write()] sent %d byte, correct num byte is %zu", n, strlen (buf) );
                  log_error (error);  
            }
            else
            {
                  sprintf (error, "[JASM-DAEMON][getProcesses][write()] sent %d byte", n);
                  log_string (error);
            }
      }
}

void getCpuProcessor (int fd)
{
      int n;
      int numCPU,onCPU;
      char buf[BUFSIZ];
      //TODO error checking
      if( (numCPU=sysconf(_SC_NPROCESSORS_CONF))==-1 || (onCPU=sysconf(_SC_NPROCESSORS_ONLN))==-1)
      {
            log_error("getCpuProcessor() Failed");
            return;
      }
      sprintf(buf,"Number of CPU configured: %d Online: %d",numCPU,onCPU);
      n = write(fd,buf,strlen(buf));
      if (n < 0)
      {
            sprintf (error, "[JASM-DEAMON][errno] %s", strerror (errno) );
            log_error ("[JASM-DEAMON][getCpuProcessor][write()] Error!");
            log_error (error);
      }
      if (n < strlen(buf) )
      {
            sprintf (error, "[JASM-DAEMON][getCpuProcessor][write()] sent %d byte, correct num byte is %zu", n, strlen (buf) );
            log_error (error);  
      }
      else
      {
            sprintf (error, "[JASM-DAEMON][getCpuProcessor][write()] sent %d byte", n);
            log_string (error);
      }
}

void getPosixVersion (int fd)
{
      int n;
      long version;
      char buf[BUFSIZ];
      if( (version=sysconf(_SC_2_VERSION))==-1)
      {
            log_error("getPosixVersion() Failed");
            return;
      }
      else
      {
            sprintf(buf,"Version: %d (YYYYMML)",version);
            n = write(fd, buf, strlen(buf));
            if(n < 0)
            {
                sprintf (error, "[JASM-DEAMON][errno] %s", strerror (errno) );
                  log_error ("[JASM-DEAMON][getPosixVersion][write()] Error!");
                  log_error (error);  
            }
            if (n < strlen(buf) )
            {
                  sprintf (error, "[JASM-DAEMON][getPosixVersion][write()] sent %d byte, correct num byte is %zu",n,strlen(buf));
                  log_error (error);  
            }
            else
            {
                  sprintf (error, "[JASM-DAEMON][getPosixVersion][write()] sent %d byte", n);
                  log_string (error);
            }
      }
}     
