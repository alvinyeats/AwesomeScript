Limited report generation is possible through the following commands:

Note: Removal of the /domain switch will generate a report for the local machine.

 
NET USERS /DOMAIN >USERS.TXT 

This command will return the user accounts from the Primary Domain Controller (PDC) of the current domain, and write them to a file called USER.TXT

 
NET ACCOUNTS /DOMAIN >ACCOUNTS.TXT 

This command will return the account policy information from the PDC of the current domain, and write it to a file called ACCOUNTS.TXT

 
NET CONFIG SERVER >SERVER.TXT 

This command will return the server name, version of Windows, active network adapter information/MAC address, Server hidden status, Maximum Logged On Users, Maximum open files per session, Idle session time, and assign it to a file called SERVER.TXT

 
NET CONFIG WORKSTATION >WKST.TXT 

This command will return the workstation name, user name, version of Windows, network adapter, network adapter information/MAC address, Logon domain, COM Open Timeout, COM Send Count, COM Send Timout, and write it to a file called WKST.TXT.

 
NET GROUP /DOMAIN >DGRP.TXT 

This command will return the global groups on the PDC of the current domain, and write them to a file called GRP.TXT.

 
NET LOCALGROUP >LGRP.TXT 

This command will return the local groups on the local machine, and write them to a file call LGRP.TXT.

 
NET VIEW /DOMAIN:DOMAINNAME >VIEW.TXT 

This command will return the resources in the specified domain, and write them to a file called VIEW.TXT.

Using Resource Kit Tools
 
ADDUSERS \\COMPUTERNAME /D USERINFO.TXT 

This Windows Resource Kit command will return a comma delimited file (for spreadsheets) containing user and group information, and write it to a file called USERINFO.TXT.

 
PERMS COMPUTERNAME\USERNAME C:\*.* /S >PERMS.TXT 

This Windows Resource Kit command will return the username permissions on all files in all subdirectories on the c:\ drive of the computername, and write it to a file called PERMS.TXT

In addition to these user management tools, there are many tools and scripts out there to help in querying, creating, modifying and deleting user objects in the directory. You should explore the Support Tools from the Windows 2000 product CD and the Windows 2000 Resource Kit to get acquainted with some of them at least, since they can really ease your work sometimes. Here is a quick description of some of those tools:

The Windows 2000 Resource Kit offers us some scripts for handling users:

 
ClassifyMembers.vbs

Will list objects in a container/OU or a domain.

 
ListProperties.vbs

This script can be used with the WinNT:// namespace against Windows NT, Member or Workstation machines, or with the LDAP:// namespace for Active Directory Domain Controllers. Pay attention that this script is case-sensitive in its syntax.

As for enumerating group membership, there are several tools, such as:

 
FindGrp.exe  IfMember.exe  ShowGrps.exe

Each gives different results, so you might want to run them and compare the outputs of each tool. ShowGrps.exe, for example, can also query for group membership of computer objects:

 
FindGrp.exe <Domainname>\<MachineName>$ 

If you want to search for users satisfying a given criteria, you can try out

 
chkusers.vbs

This script checks your domain for users that satisfy a certain criteria that you define. For example:

 
Cscript CHKUSERS.VBS /A:WinNT://YourDomainName /P:FullName;Description /C:”((LastLogin:>4/3/01 or LastLogin:<8/4/01) and AccountDisabled:=False)”

This will output the full name and description of all active users whose last login was between 4/3/01 to 8/4/01.

Using LDIFDE
From the support tools we can find LDIFDE.exe, which is a tool for bulk import and export of Active Directory Objects. You can use LDIFDE to import new user records into the directory, or export specific information on specific users into a text file. LDIFDE defaults to export mode (reading From the Directory). When you add the -i option it can be used to write changes into the Directory. Also, if you want to export and extract only specific details, such as the user name, title and login name for all the users in a specific OU (Organizational Unit), you can run the following command:

 
ldifde -f C:\ldif\ExportUsers.ldf –s SERVERNAME -d “OU=YourOUname,dc=YourDomainName,dc=com” -p subtree -r “(objectClass=User)” -l “cn,givenName,Title,SamAccountName”
