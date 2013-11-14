#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Secret_Input.H>
#include <FL/Fl_Button.H>
#include <FL/fl_ask.H>

#define COL 60
#define ROW 230
#define BTLROW 150
#define BTEROW BTLROW+220
#define filename "userfile.pwd"

Fl_Input *input[2];

void cb(Fl_Widget *ob) {
  printf("Callback for %s '%s'\n",ob->label(),((Fl_Input*)ob)->value());
}

void exitcb(Fl_Widget *, void *) {
  exit(0);
}

/*******************************************************************
**Sample:
**	strSplit("a|b|c|", 0, '|', b); //result=0; b="a";
**	strSplit("a|b|c|", 1, '|', b); //result=1; b="b";
**	strSplit("a|b|c|", 3, '|', b); //result=-1; b="";
**Test:
**	void main()
**	{
**		char a[20];
**		char b[10];
**		int  i = 0;
**		memset(a, 0, sizeof(a));
**		memset(b, 0, sizeof(b));
**		strcpy(a, "01|23|45|67|");
**		while(strSplit(a, i, '|', b)>=0)
**		{
**			printf("b=[%s] ", b);
**			i++;
**		}
**	}
*******************************************************************/
int strSplit(char *S, int I, char CH, char *RET)
{
	int i = 0;
	int pos = 0, bakpos = 0;
	int iLen = 0;
	if (S[0] == 0)
	{
		 return -1;
	}
	if (I < 0)
	{
		return I;
	}
	iLen = strlen(S);
	if (S[iLen - 1] != CH)
	{
		S[iLen] = CH;
		S[iLen + 1] = 0;
	}
	while((pos = (char *)strchr(S + pos, CH) - S + 1) > 0 )
	{
		if (i == I)
		{
			strncpy(RET, S + bakpos, pos - bakpos - 1);
			return 1;
		}
		else
		{
			memset(RET, 0, sizeof(RET));
		}
		iLen=strlen(S);
		if (pos>=iLen) break;
		i++;
		bakpos = pos;
	}
	return -1;
}

/*******************************************************************
**FUNCTION: rtrim()
**
**PURPOSE: right trim
**
**COMMENTS:
**
*******************************************************************/
char *rtrim(char *src)
{
	int i = 0;
	if(!strstr(src, " ")) return src;
	
	for(i=strlen(src); i>0; i--)
	{
		if( *(src+i-1) != ' ')
		{
			*(src+i) = '\0';
			break;
		}
	}
	if(i == 0)
	{
		*src =	'\0';
	}
	return src;
}

int fCheck(const char *uname,const char * upwd)
{
	char vName[20];
	char vPwd[10];
	FILE *fp;
	char buffer[80];
	char vBuff[10];
	char vUsr[2][10];
	int  iflag = 1;
	char ch='|';
	
	memset(buffer,0,sizeof(buffer));
	memset(vBuff,0,sizeof(vBuff));
	memset(vUsr,0,sizeof(vUsr));
	
	memcpy(vName,uname,sizeof(vName));
	memcpy(vPwd,upwd,sizeof(vPwd));
	
	if( strlen(vName) == 0 || strlen(vPwd) == 0 )
		return 5; // userName and userPwd is not Null
	
	//if (_access(filename, 0) == -1)
	//	return 3;
		
	if( (fp = fopen(filename,"r")) == NULL )
		return 2; // open file err
	else
	{
		while ( fgets(buffer,sizeof(buffer),fp) != NULL ) 
		{
			int i = 0;
			while(strSplit(buffer, i, ch, vBuff) >= 0 && i < 2)
			{
				strcpy(vUsr[i],vBuff);
				memset(vBuff,0,sizeof(vBuff));
				i++;
			}
			/*check user,pwd*/
			if( strcmp(vName,vUsr[0]) == 0 ) /*user exist*/
			{
				if( strcmp(vPwd,vUsr[1]) == 0 )
				{
					fclose(fp);
					return 0;
				}
				else
				{
					fclose(fp);
					return 1;
				}
			}
			else /*user not exist*/
			{
				iflag =0;
			}
		}
		if( iflag == 0 || iflag == 1 )
		{
			fclose(fp);
			return 3;
		}
	}
	fclose(fp);
}

/*******************************************************************
**FUNCTION: createUsr()
**
**PURPOSE: create user.
**
**COMMENTS:
**
*******************************************************************/
int createUsr(const char *uname,const char *pwd)
{
	FILE *fp;
	char vbuff[50];
	
	sprintf(vbuff,"%s|%s|",uname,pwd);
	
	if( (fp=fopen(filename,"a")) == NULL )
	{
		fl_alert("open file err");
		return -1;
	}
	fprintf(fp,"%s\n",rtrim(vbuff));
	fclose(fp);
	
	return 1;
}

void process(const char *uname,const char * upwd)
{
	int flag = 0;
	flag = fCheck(uname,upwd);
	printf("flag=%d",flag);
	switch(flag)
	{
		case 0:
			fl_alert("login successful");
			break;
		case 1:
			fl_alert("user or pwd input err");
			break;
		case 2:
			fl_alert("open file err");
			break;
		case 3:
			if(fl_ask("did you need create user's account","continue"))
			{
				/**
				if(createUsr(uname,upwd)>0)
					fl_message("user has created");
				else
					fl_message("created err"); **/
				createUsr(uname,upwd);	
			}
			break;
		case 5:
			fl_alert("userName and userPwd is not Null");
			break;
		default:
			fl_alert("system err");
	}
}

void flogin(Fl_Widget *, void *) {
	const char *uname,*upwd;
	uname = input[0]->value();
	upwd	= input[1]->value();
	process(uname,upwd);
}

int main(int argc, char **argv) {
	Fl_Window *window = new Fl_Window(800,300);
	
	int y = COL;
	input[0] = new Fl_Input(ROW,y,200,25,"UserName:"); y += 40;
	input[1] = new Fl_Secret_Input(ROW,y,200,25,"UserPwd:"); y += 60;
	
	for (int i = 0; i < 2; i++) {
		input[i]->when(0); input[i]->callback(cb);
	}
	
	Fl_Button *bL,*bE;
	bL = new Fl_Button(BTLROW,y,80,25,"Login");
	bL->callback(flogin,0);
	
	bE = new Fl_Button(BTEROW,y,80,25,"Exit");
	bE->callback(exitcb,0);
	
	window->end();
	window->show(argc, argv);
	return Fl::run();
}