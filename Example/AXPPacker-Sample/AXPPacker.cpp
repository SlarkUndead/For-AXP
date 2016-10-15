#include <stdio.h>
#include <string>
#include <windows.h>
#include <shlobj.h>

using namespace std;

int do16bit(char t)
{
 switch(t)
 {
 case '0':
   return 0;
 case '1':
   return 1;
 case '2':
   return 2;
 case '3':
   return 3;
 case '4':
   return 4;
 case '5':
   return 5;
 case '6':
   return 6;
 case '7':
   return 7;
 case '8':
   return 8;
 case '9':
   return 9;
 case 'A':
   return 10;
 case 'B':
   return 11;
 case 'C':
   return 12;
 case 'D':
   return 13;
 case 'E':
   return 14;
 case 'F':
   return 15;
 default:
   return 0;
 }
}

struct fileinfo
{
 std::string name;
 int      off;
 int      size;
};

void mkdir(const char* path)
{
 char buff[256];
 GetCurrentDirectory(256,buff);
 strcat(buff,"\\");
 strcat(buff,path);
 SHCreateDirectoryEx(NULL,buff,NULL);
}

void domaterial()
{
 FILE *fp = fopen( "Material.axp", "rb" );
 if( fp == NULL )
 {
   return;
 }

 int nCount = 0;
 fseek( fp, 0, SEEK_END );
 int nSize = ftell( fp );
 fseek( fp, 0x0b7b5d30, 0 );

 int msize = nSize - 0x0b7b5d30;
 char *pData = new char[msize];
 fread( pData, 1, msize, fp );

 std::string mfile = pData;
 int star = 1;
 int off = mfile.find("\n",star);
 std::string temp = mfile.substr(star,off-star);

 int mfnum = atoi(temp.c_str());

 fileinfo *mfulelist = new fileinfo[mfnum];

 mkdir("Material");
 FILE *fout = fopen("Material\\Material.txt","wb");

 for(int i=0; i<mfnum;i++)
 {
   star = off+1;
   off = mfile.find("\n",star);
   temp = mfile.substr(star,off-star);

   int off1 = temp.find("&#124;",0);
   int off2 = temp.find("&#124;",off1+1);

   mfulelist[i].name = temp.substr(0,off1);

   std::string fsize = temp.substr(off1+1,off2-off1-1);

   mfulelist[i].size = ((((((
     do16bit(fsize[0])*16+
     do16bit(fsize[1]))*16+
     do16bit(fsize[2]))*16+
     do16bit(fsize[3]))*16+
     do16bit(fsize[4]))*16+
     do16bit(fsize[5]))*16+
     do16bit(fsize[6]))*16+
     do16bit(fsize[7]);

   if(mfulelist[i].name == string("Icons/MenpaiLiveSkill2.jpg"))
   {
     mfulelist[i].size = 97604;
   }
   if(mfulelist[i].name == string("Icons/TaskTools4.jpg"))
   {
     mfulelist[i].size = 77308;
   }

   if(i==0)
   {
     mfulelist[i].off = 0x00160028;
   }
   else
   {
     mfulelist[i].off = mfulelist[i-1].off + mfulelist[i-1].size;

     while(1)
     {
       if(mfulelist[i].off%8 == 0)
       {
         fseek( fp,mfulelist[i].off , 0 );
         fread( pData, 1, 8, fp );

         if(!(pData[0] == '\0'&&pData[1] == '\0'&&pData[2] == '\0'&&pData[3] == '\0'
           &&pData[4] == '\0'&&pData[5] == '\0'&&pData[6] == '\0'&&pData[7] == '\0'))
         {
           break;
         }
       }
       ++mfulelist[i].off;
     }
   }

   char *nfilestr = new char[256];
   sprintf(nfilestr,"%d\t%s\t%d\n",i,temp.c_str(),mfulelist[i].off);
   std::string mfilestr = nfilestr;
   fwrite((void*)mfilestr.c_str(),1,mfilestr.size(),fout);
 }

 fclose(fout);

 for(int i=0; i<mfnum;i++)
 {
   fseek( fp, mfulelist[i].off, 0 );

   char *filedata = new char[mfulelist[i].size];

   fread( filedata, 1, mfulelist[i].size, fp );

   if(i == 654)
   {
   }
   int mbdir = 0;
   while(1)
   {
     mbdir = mfulelist[i].name.find("/",mbdir+1);
     if(mbdir>0)
     {
       std::string mdir = mfulelist[i].name.substr(0,mbdir);
       mkdir((std::string("Material\\")+mdir).c_str());
     }
     else
     {
       break;
     }
   }

   FILE *fout = fopen((std::string("Material\\")+mfulelist[i].name).c_str(),"wb");
   fwrite((const void*)filedata,1,mfulelist[i].size,fout);
   fclose(fout);
 }
 fclose(fp);
}

void domodel()
{
 FILE *fp = fopen( "model.axp", "rb" );
 if( fp == NULL )
   return;

 int nCount = 0;
 fseek( fp, 0, SEEK_END );
 int nSize = ftell( fp );
 fseek( fp, 0x0c628730, 0 );

 int msize = nSize - 0x0c628730;
 char *pData = new char[msize];
 fread( pData, 1, msize, fp );

 std::string mfile = pData;
 int star = 1;
 int off = mfile.find("\n",star);
 std::string temp = mfile.substr(star,off-star);

 int mfnum = atoi(temp.c_str());

 fileinfo *mfulelist = new fileinfo[mfnum];

 mkdir("Model");
 FILE *fout = fopen("Model\\Model.txt","wb");

 for(int i=0; i<mfnum;i++)
 {
   star = off+1;
   off = mfile.find("\n",star);
   temp = mfile.substr(star,off-star);

   int off1 = temp.find("&#124;",0);
   int off2 = temp.find("&#124;",off1+1);

   mfulelist[i].name = temp.substr(0,off1);

   std::string fsize = temp.substr(off1+1,off2-off1-1);

   mfulelist[i].size = ((((((
     do16bit(fsize[0])*16+
     do16bit(fsize[1]))*16+
     do16bit(fsize[2]))*16+
     do16bit(fsize[3]))*16+
     do16bit(fsize[4]))*16+
     do16bit(fsize[5]))*16+
     do16bit(fsize[6]))*16+
     do16bit(fsize[7]);

   if(i==0)
   {
     mfulelist[i].off = 0x00160028;
   }
   else
   {
     mfulelist[i].off = mfulelist[i-1].off + mfulelist[i-1].size;

     while(1)
     {
       if(mfulelist[i].off%8 == 0)
       {
         fseek( fp,mfulelist[i].off , 0 );
         fread( pData, 1, 8, fp );

         if(!(pData[0] == '\0'&&pData[1] == '\0'&&pData[2] == '\0'&&pData[3] == '\0'
           &&pData[4] == '\0'&&pData[5] == '\0'&&pData[6] == '\0'&&pData[7] == '\0'))
         {
           break;
         }
       }
       ++mfulelist[i].off;
     }
   }

   char *nfilestr = new char[256];
   sprintf(nfilestr,"%d\t%s\t%d\n",i,temp.c_str(),mfulelist[i].off);
   std::string mfilestr = nfilestr;
   fwrite((void*)mfilestr.c_str(),1,mfilestr.size(),fout);
 }

 fclose(fout);

 for(int i=0; i<mfnum;i++)
 {
   fseek( fp, mfulelist[i].off, 0 );

   char *filedata = new char[mfulelist[i].size];

   fread( filedata, 1, mfulelist[i].size, fp );

   if(i == 654)
   {
   }
   int mbdir = 0;
   while(1)
   {
     mbdir = mfulelist[i].name.find("/",mbdir+1);
     if(mbdir>0)
     {
       std::string mdir = mfulelist[i].name.substr(0,mbdir);
       mkdir((std::string("Model\\")+mdir).c_str());
     }
     else
     {
       break;
     }
   }

   FILE *fout = fopen((std::string("Model\\")+mfulelist[i].name).c_str(),"wb");
   fwrite((const void*)filedata,1,mfulelist[i].size,fout);
   fclose(fout);
 }
 fclose(fp);
}

void doBrushes()
{
 FILE *fp = fopen( "brushes.axp", "rb" );
 if( fp == NULL )
   return;

 int nCount = 0;
 fseek( fp, 0, SEEK_END );
 int nSize = ftell( fp );
 fseek( fp, 0x01e97e30, 0 );

 int msize = nSize - 0x01e97e30;
 char *pData = new char[msize];
 fread( pData, 1, msize, fp );

 std::string mfile = pData;
 int star = 1;
 int off = mfile.find("\n",star);
 std::string temp = mfile.substr(star,off-star);

 int mfnum = atoi(temp.c_str());

 fileinfo *mfulelist = new fileinfo[mfnum];

 mkdir("Brushes");
 FILE *fout = fopen("Brushes\\Brushes.txt","wb");

 for(int i=0; i<mfnum;i++)
 {
   star = off+1;
   off = mfile.find("\n",star);
   temp = mfile.substr(star,off-star);

   int off1 = temp.find("&#124;",0);
   int off2 = temp.find("&#124;",off1+1);

   mfulelist[i].name = temp.substr(0,off1);

   std::string fsize = temp.substr(off1+1,off2-off1-1);

   mfulelist[i].size = ((((((
     do16bit(fsize[0])*16+
     do16bit(fsize[1]))*16+
     do16bit(fsize[2]))*16+
     do16bit(fsize[3]))*16+
     do16bit(fsize[4]))*16+
     do16bit(fsize[5]))*16+
     do16bit(fsize[6]))*16+
     do16bit(fsize[7]);

   if(i==0)
   {
     mfulelist[i].off = 0x00160028;
   }
   else
   {
     mfulelist[i].off = mfulelist[i-1].off + mfulelist[i-1].size;

     while(1)
     {
       if(mfulelist[i].off%8 == 0)
       {
         fseek( fp,mfulelist[i].off , 0 );
         fread( pData, 1, 8, fp );

         if(!(pData[0] == '\0'&&pData[1] == '\0'&&pData[2] == '\0'&&pData[3] == '\0'
           &&pData[4] == '\0'&&pData[5] == '\0'&&pData[6] == '\0'&&pData[7] == '\0'))
         {
           break;
         }
       }
       ++mfulelist[i].off;
     }
   }

   char *nfilestr = new char[256];
   sprintf(nfilestr,"%d\t%s\t%d\n",i,temp.c_str(),mfulelist[i].off);
   std::string mfilestr = nfilestr;
   fwrite((void*)mfilestr.c_str(),1,mfilestr.size(),fout);
 }

 fclose(fout);

 for(int i=0; i<mfnum;i++)
 {
   fseek( fp, mfulelist[i].off, 0 );

   char *filedata = new char[mfulelist[i].size];

   fread( filedata, 1, mfulelist[i].size, fp );

   if(i == 654)
   {
   }
   int mbdir = 0;
   while(1)
   {
     mbdir = mfulelist[i].name.find("/",mbdir+1);
     if(mbdir>0)
     {
       std::string mdir = mfulelist[i].name.substr(0,mbdir);
       mkdir((std::string("Brushes\\")+mdir).c_str());
     }
     else
     {
       break;
     }
   }

   FILE *fout = fopen((std::string("Brushes\\")+mfulelist[i].name).c_str(),"wb");
   fwrite((const void*)filedata,1,mfulelist[i].size,fout);
   fclose(fout);
 }
 fclose(fp);
}

void doEffect()
{
 FILE *fp = fopen( "effect.axp", "rb" );
 if( fp == NULL )
   return;

 int nCount = 0;
 fseek( fp, 0, SEEK_END );
 int nSize = ftell( fp );
 fseek( fp, 0x00746730, 0 );

 int msize = nSize - 0x00746730;
 char *pData = new char[msize];
 fread( pData, 1, msize, fp );

 std::string mfile = pData;
 int star = 1;
 int off = mfile.find("\n",star);
 std::string temp = mfile.substr(star,off-star);

 int mfnum = atoi(temp.c_str());

 fileinfo *mfulelist = new fileinfo[mfnum];

 mkdir("Effect");
 FILE *fout = fopen("Effect\\Effect.txt","wb");

 for(int i=0; i<mfnum;i++)
 {
   star = off+1;
   off = mfile.find("\n",star);
   temp = mfile.substr(star,off-star);

   int off1 = temp.find("&#124;",0);
   int off2 = temp.find("&#124;",off1+1);

   mfulelist[i].name = temp.substr(0,off1);

   std::string fsize = temp.substr(off1+1,off2-off1-1);

   mfulelist[i].size = ((((((
     do16bit(fsize[0])*16+
     do16bit(fsize[1]))*16+
     do16bit(fsize[2]))*16+
     do16bit(fsize[3]))*16+
     do16bit(fsize[4]))*16+
     do16bit(fsize[5]))*16+
     do16bit(fsize[6]))*16+
     do16bit(fsize[7]);

   if(i==0)
   {
     mfulelist[i].off = 0x00160028;
   }
   else
   {
     mfulelist[i].off = mfulelist[i-1].off + mfulelist[i-1].size;

     while(1)
     {
       if(mfulelist[i].off%8 == 0)
       {
         fseek( fp,mfulelist[i].off , 0 );
         fread( pData, 1, 8, fp );

         if(!(pData[0] == '\0'&&pData[1] == '\0'&&pData[2] == '\0'&&pData[3] == '\0'
           &&pData[4] == '\0'&&pData[5] == '\0'&&pData[6] == '\0'&&pData[7] == '\0'))
         {
           break;
         }
       }
       ++mfulelist[i].off;
     }
   }

   char *nfilestr = new char[256];
   sprintf(nfilestr,"%d\t%s\t%d\n",i,temp.c_str(),mfulelist[i].off);
   std::string mfilestr = nfilestr;
   fwrite((void*)mfilestr.c_str(),1,mfilestr.size(),fout);
 }

 fclose(fout);

 for(int i=0; i<mfnum;i++)
 {
   fseek( fp, mfulelist[i].off, 0 );

   char *filedata = new char[mfulelist[i].size];

   fread( filedata, 1, mfulelist[i].size, fp );

   if(i == 654)
   {
   }
   int mbdir = 0;
   while(1)
   {
     mbdir = mfulelist[i].name.find("/",mbdir+1);
     if(mbdir>0)
     {
       std::string mdir = mfulelist[i].name.substr(0,mbdir);
       mkdir((std::string("Effect\\")+mdir).c_str());
     }
     else
     {
       break;
     }
   }

   FILE *fout = fopen((std::string("Effect\\")+mfulelist[i].name).c_str(),"wb");
   fwrite((const void*)filedata,1,mfulelist[i].size,fout);
   fclose(fout);
 }
 fclose(fp);
}

void doInterface()
{
 FILE *fp = fopen( "interface.axp", "rb" );
 if( fp == NULL )
   return;

 int nCount = 0;
 fseek( fp, 0, SEEK_END );
 int nSize = ftell( fp );
 fseek( fp, 0x012ee230, 0 );

 int msize = nSize - 0x012ee230;
 char *pData = new char[msize];
 fread( pData, 1, msize, fp );

 std::string mfile = pData;
 int star = 1;
 int off = mfile.find("\n",star);
 std::string temp = mfile.substr(star,off-star);

 int mfnum = atoi(temp.c_str());

 fileinfo *mfulelist = new fileinfo[mfnum];

 mkdir("Interface");
 FILE *fout = fopen("Interface\\Interface.txt","wb");

 for(int i=0; i<mfnum;i++)
 {
   star = off+1;
   off = mfile.find("\n",star);
   temp = mfile.substr(star,off-star);

   int off1 = temp.find("&#124;",0);
   int off2 = temp.find("&#124;",off1+1);

   mfulelist[i].name = temp.substr(0,off1);

   std::string fsize = temp.substr(off1+1,off2-off1-1);

   mfulelist[i].size = ((((((
     do16bit(fsize[0])*16+
     do16bit(fsize[1]))*16+
     do16bit(fsize[2]))*16+
     do16bit(fsize[3]))*16+
     do16bit(fsize[4]))*16+
     do16bit(fsize[5]))*16+
     do16bit(fsize[6]))*16+
     do16bit(fsize[7]);

   if(i==0)
   {
     mfulelist[i].off = 0x00160028;
   }
   else
   {
     mfulelist[i].off = mfulelist[i-1].off + mfulelist[i-1].size;

     while(1)
     {
       if(mfulelist[i].off%8 == 0)
       {
         fseek( fp,mfulelist[i].off , 0 );
         fread( pData, 1, 8, fp );

         if(!(pData[0] == '\0'&&pData[1] == '\0'&&pData[2] == '\0'&&pData[3] == '\0'
           &&pData[4] == '\0'&&pData[5] == '\0'&&pData[6] == '\0'&&pData[7] == '\0'))
         {
           break;
         }
       }
       ++mfulelist[i].off;
     }
   }

   char *nfilestr = new char[256];
   sprintf(nfilestr,"%d\t%s\t%d\n",i,temp.c_str(),mfulelist[i].off);
   std::string mfilestr = nfilestr;
   fwrite((void*)mfilestr.c_str(),1,mfilestr.size(),fout);
 }

 fclose(fout);

 for(int i=0; i<mfnum;i++)
 {
   fseek( fp, mfulelist[i].off, 0 );

   char *filedata = new char[mfulelist[i].size];

   fread( filedata, 1, mfulelist[i].size, fp );

   if(i == 654)
   {
   }
   int mbdir = 0;
   while(1)
   {
     mbdir = mfulelist[i].name.find("/",mbdir+1);
     if(mbdir>0)
     {
       std::string mdir = mfulelist[i].name.substr(0,mbdir);
       mkdir((std::string("Interface\\")+mdir).c_str());
     }
     else
     {
       break;
     }
   }

   FILE *fout = fopen((std::string("Interface\\")+mfulelist[i].name).c_str(),"wb");
   fwrite((const void*)filedata,1,mfulelist[i].size,fout);
   fclose(fout);
 }
 fclose(fp);
}

void doSound()
{
 FILE *fp = fopen( "sound.axp", "rb" );
 if( fp == NULL )
   return;

 int nCount = 0;
 fseek( fp, 0, SEEK_END );
 int nSize = ftell( fp );
 fseek( fp, 0x07ce2930, 0 );

 int msize = nSize - 0x07ce2930;
 char *pData = new char[msize];
 fread( pData, 1, msize, fp );

 std::string mfile = pData;
 int star = 1;
 int off = mfile.find("\n",star);
 std::string temp = mfile.substr(star,off-star);

 int mfnum = atoi(temp.c_str());

 fileinfo *mfulelist = new fileinfo[mfnum];

 mkdir("Sound");
 FILE *fout = fopen("Sound\\Sound.txt","wb");

 for(int i=0; i<mfnum;i++)
 {
   star = off+1;
   off = mfile.find("\n",star);
   temp = mfile.substr(star,off-star);

   int off1 = temp.find("&#124;",0);
   int off2 = temp.find("&#124;",off1+1);

   mfulelist[i].name = temp.substr(0,off1);

   std::string fsize = temp.substr(off1+1,off2-off1-1);

   mfulelist[i].size = ((((((
     do16bit(fsize[0])*16+
     do16bit(fsize[1]))*16+
     do16bit(fsize[2]))*16+
     do16bit(fsize[3]))*16+
     do16bit(fsize[4]))*16+
     do16bit(fsize[5]))*16+
     do16bit(fsize[6]))*16+
     do16bit(fsize[7]);

   if(i==0)
   {
     mfulelist[i].off = 0x00160028;
   }
   else
   {
     mfulelist[i].off = mfulelist[i-1].off + mfulelist[i-1].size;

     while(1)
     {
       if(mfulelist[i].off%8 == 0)
       {
         fseek( fp,mfulelist[i].off , 0 );
         fread( pData, 1, 8, fp );

         if(!(pData[0] == '\0'&&pData[1] == '\0'&&pData[2] == '\0'&&pData[3] == '\0'
           &&pData[4] == '\0'&&pData[5] == '\0'&&pData[6] == '\0'&&pData[7] == '\0'))
         {
           break;
         }
       }
       ++mfulelist[i].off;
     }
   }

   char *nfilestr = new char[256];
   sprintf(nfilestr,"%d\t%s\t%d\n",i,temp.c_str(),mfulelist[i].off);
   std::string mfilestr = nfilestr;
   fwrite((void*)mfilestr.c_str(),1,mfilestr.size(),fout);
 }

 fclose(fout);

 for(int i=0; i<mfnum;i++)
 {
   fseek( fp, mfulelist[i].off, 0 );

   char *filedata = new char[mfulelist[i].size];

   fread( filedata, 1, mfulelist[i].size, fp );

   if(i == 654)
   {
   }
   int mbdir = 0;
   while(1)
   {
     mbdir = mfulelist[i].name.find("/",mbdir+1);
     if(mbdir>0)
     {
       std::string mdir = mfulelist[i].name.substr(0,mbdir);
       mkdir((std::string("Sound\\")+mdir).c_str());
     }
     else
     {
       break;
     }
   }

   FILE *fout = fopen((std::string("Sound\\")+mfulelist[i].name).c_str(),"wb");
   fwrite((const void*)filedata,1,mfulelist[i].size,fout);
   fclose(fout);
 }
 fclose(fp);
}

void doScene()
{
 FILE *fp = fopen( "scene.axp", "rb" );
 if( fp == NULL )
 {
   return;
 }

 int nCount = 0;
 fseek( fp, 0, SEEK_END );
 int nSize = ftell( fp );
 fseek( fp, 0x159b8b30, 0 );

 int msize = nSize - 0x159b8b30;
 char *pData = new char[msize];
 fread( pData, 1, msize, fp );

 std::string mfile = pData;
 int star = 1;
 int off = mfile.find("\n",star);
 std::string temp = mfile.substr(star,off-star);

 int mfnum = atoi(temp.c_str());

 fileinfo *mfulelist = new fileinfo[mfnum];

 mkdir("Scene");
 FILE *fout = fopen("Scene\\Scene.txt","wb");

 for(int i=0; i<mfnum;i++)
 {
   star = off+1;
   off = mfile.find("\n",star);
   temp = mfile.substr(star,off-star);

   int off1 = temp.find("&#124;",0);
   int off2 = temp.find("&#124;",off1+1);

   mfulelist[i].name = temp.substr(0,off1);

   std::string fsize = temp.substr(off1+1,off2-off1-1);

   mfulelist[i].size = ((((((
     do16bit(fsize[0])*16+
     do16bit(fsize[1]))*16+
     do16bit(fsize[2]))*16+
     do16bit(fsize[3]))*16+
     do16bit(fsize[4]))*16+
     do16bit(fsize[5]))*16+
     do16bit(fsize[6]))*16+
     do16bit(fsize[7]);

   if(i==0)
   {
     mfulelist[i].off = 0x00160028;
   }
   else
   {
     mfulelist[i].off = mfulelist[i-1].off + mfulelist[i-1].size;

     while(1)
     {
       if(mfulelist[i].off%8 == 0)
       {
         fseek( fp,mfulelist[i].off , 0 );
         fread( pData, 1, 8, fp );

         if(!(pData[0] == '\0'&&pData[1] == '\0'&&pData[2] == '\0'&&pData[3] == '\0'
           &&pData[4] == '\0'&&pData[5] == '\0'&&pData[6] == '\0'&&pData[7] == '\0'))
         {
           break;
         }
       }
       ++mfulelist[i].off;
     }
   }

   char *nfilestr = new char[256];
   sprintf(nfilestr,"%d\t%s\t%d\n",i,temp.c_str(),mfulelist[i].off);
   std::string mfilestr = nfilestr;
   fwrite((void*)mfilestr.c_str(),1,mfilestr.size(),fout);
 }

 fclose(fout);

 for(int i=0; i<mfnum;i++)
 {
   fseek( fp, mfulelist[i].off, 0 );

   char *filedata = new char[mfulelist[i].size];

   fread( filedata, 1, mfulelist[i].size, fp );

   if(i == 654)
   {
   }
   int mbdir = 0;
   while(1)
   {
     mbdir = mfulelist[i].name.find("/",mbdir+1);
     if(mbdir>0)
     {
       std::string mdir = mfulelist[i].name.substr(0,mbdir);
       mkdir((std::string("Scene\\")+mdir).c_str());
     }
     else
     {
       break;
     }
   }

   FILE *fout = fopen((std::string("Scene\\")+mfulelist[i].name).c_str(),"wb");
   fwrite((const void*)filedata,1,mfulelist[i].size,fout);
   fclose(fout);
 }
 fclose(fp);
}

int main()
{
 domaterial();
 domodel();
 doBrushes();
 doEffect();
 doInterface();
 doSound();
 doScene();
}