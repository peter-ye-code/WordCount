#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<io.h> 

int getChar(FILE *fp);
int getWord(FILE *fp);
int getLine(FILE *fp);
int getnull_line(FILE *fp);
int getcomplex_data(FILE *fp,int &annotion_line,int &code_line);
int getother_line(FILE *fp);
int handle_file();

int main(int argc,char*argv[]){
	FILE *fp;
	int number;
	char string;

    if(argc==3){
    	
    	//读取文件的内容
		fp = fopen(argv[2],"r");
		
		//打开失败 
		if(fp == NULL){
			printf("错误：无法打开%s\n",argv[2]);
			exit(1);
		} 
			//对参数进行匹配 
		    if(!strcmp(argv[1],"-c")){//统计字符数 
		        number = getChar(fp);
		     	printf("字符数为%d\n",number);
		    }
		
		    else if(!strcmp(argv[1],"-w")){//统计单词数 
			    number = getWord(fp);
			    printf("单词数为%d\n",number); 
		   }
		
		    else if(!strcmp(argv[1],"-l")){//统计行数 
			   number = getLine(fp);
			    printf("行数为%d\n",number);
	        }	
			
			else if(!strcmp(argv[1],"-a")){			
				printf("空行行数为%d\n",getnull_line(fp));
				int a,b;
				getcomplex_data(fp,a,b);
				printf("注释行数为%d\n",a);
				printf("代码行数为%d\n",b);
			}
			else if(!strcmp(argv[1],"-s")){
			     handle_file();
			}
	}
	
	else {
		printf("错误：参数输入错误，请重新输入\n");
	}
	
} 

//统计字符数 
int getChar(FILE *fp){
	char ch;
	int number=0;
	while((ch=fgetc(fp))!=EOF){//当函数fgetc返回的ASCII值不为 EOF，表示还可以继续读 
		if((ch!='\n')&&(ch!=' ')&&(ch!='\t')){
			number++;
		}
	}
	rewind(fp);
	return number;
}

//统计单词数 
int getWord(FILE *fp){
	int flag=0;
	int number=0;
	char ch;
	while(!feof(fp))
    {
        ch = fgetc(fp);
        if((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9'))
            flag = 0;
        else if(flag == 0 && (ch != '-' && ch != '/'&& ch != '\'')) //如果进入这个循环，代表不符合上面的循环，即返回的不是字母或者数字 
        {
            number++;
            flag = 1;
        }
    }
    rewind(fp);
    return number;
}

//统计行数 
int getLine(FILE *fp){
	char ch;
	int number=0;
	while(feof(fp)==0)//等于0表示文件未结束 
	{
		ch = fgetc(fp);//读取一个字符，读取一个字节后，光标位置后移一个字节
		if(ch=='\n'){//如果是换行符 
			number++;
		}
	}
	rewind(fp);
	return number+1;
}

//统计空行
int getnull_line(FILE *fp){
	int null_line = 0;
	int bufsize = 100;
	char buf[bufsize] = {0};
	while(feof(fp)==0){
		//遍历文件的每一行 
		fgets(buf,bufsize,fp); 
		if(buf[0]=='\n'){
			null_line++;
		}		
	}
	rewind(fp);
	return null_line;
} 
 
//统计其他注释行
int getother_line(FILE *fp){
	int line;
	while(feof(fp)==0){
	 	char buf[100] = {0};
	    fgets(buf,100,fp);
	    int len=strlen(buf);  
	    for(int i=0;i<len;i++){
	    	if (buf[i]!='/')
    			{
    				if ((i+2<len) && (buf[i+1]=='/')&&(buf[i+2]=='/')) 
    				line++;	
    			}
		}
	} 
	rewind(fp);
	return line;
} 

//统计注释行和代码行 
int getcomplex_data(FILE *fp,int &annotion_line,int &code_line){
	int sigle_annotion_line = 0;
	int bufsize = 100;
	char buf[bufsize] = {0};
	int flag = 0;
	int multi_annotion_line=0;
	while(feof(fp)==0){ 
    	fgets(buf,bufsize,fp);		
		if(buf[0]=='/'&&buf[1]=='/'){
			sigle_annotion_line++;
		}else if(flag == 0){		
			if(buf[0]=='/'&&buf[1]=='*'){
			    multi_annotion_line++;
			    flag=1;
			}		   
	    }else if(flag == 1){
			if(buf[0]!='*'&&buf[1]!='/'){
				multi_annotion_line++;
			}else if(buf[0]=='*'&&buf[1]=='/'){
				multi_annotion_line++;
				flag=0;
			}
		}
	}
	//在调用一个函数需将文件指针调整会开头 
	rewind(fp);
	int other_annotion_line = getother_line(fp);
	annotion_line = sigle_annotion_line+multi_annotion_line+other_annotion_line;
	int line = getLine(fp);
	int null_line = getnull_line(fp);
	code_line = line - annotion_line - null_line;
}


//处理目录下符合条件的文件
int handle_file(){   
    struct _finddata_t fileinfo;    //文件存储信息结构体 
    long file_handle;   //保存的文件句柄
    file_handle = _findfirst("*.txt",&fileinfo);    //查找当前目录下的txt文件
    FILE *fp;
    int a,b;
    if(file_handle==-1)
        printf( "当前目录下没有txt文件\n");
    else 
        do{//先找第一个再找下一个 
            fp=fopen(fileinfo.name,"r");
            printf("找到文件%s\n",fileinfo.name);
            printf("字符数为%d\n",getChar(fp));
            printf("词数为%d\n",getWord(fp));
            printf("行数为%d\n",getLine(fp));
            printf("空行数为%d\n",getnull_line(fp));
            getcomplex_data(fp,a,b);
            printf("注释行数为%d\n",a);
			printf("代码行数为%d\n\n",b);
        }while(_findnext(file_handle,&fileinfo)==0);//找到下一个 
        _findclose(file_handle);    //关闭文件链接 
}




